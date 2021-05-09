// Copyright ©️ 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.


//Lizbeth Ortiz López
//A00227346


package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
	//"time"
)

var usage string = "\nUsage: go run server.go -host <address> -port <port>"

//!+broadcaster

type chatClient struct {
	user       string
	address    string
	channel    chan string //chan<- string
	admin      bool
	connection net.Conn
	kiched     bool
	//timestamp?
}

type messageFormat struct {
	to          string
	message     string
	from        string
	fromChannel chan string
}

type userFormat struct {
	interestedIn string
	fromChannel  chan string
}

type kickFormat struct {
	from        string
	fromChannel chan string
	whoToKick   string
}

var (
	entering     = make(chan *chatClient)
	leaving      = make(chan *chatClient)
	messages     = make(chan string)
	clientAccept = make(chan string)
	commands     = make(chan string)
	usersCmd     = make(chan chan string)
	msgCmd       = make(chan messageFormat)
	userCmd      = make(chan userFormat)
	kickCmd      = make(chan kickFormat)
	arrival      = make(chan string)
)

//var clients = make(map[chatClient]bool)

func broadcaster() {
	clients := make(map[*chatClient]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			for cli := range clients {
				cli.channel <- msg
			}

		case cli := <-entering:
			//check that user doesnt already exist
			isUnique := true
			isEmpty := true

			for c := range clients {
				isEmpty = false
				if c.user == cli.user {
					cmdFormat(cli.channel, "user already in use. Please enter to the chat with a different user.")
					close(cli.channel)
					isUnique = false
					break
				}
			}

			if isUnique {
				clients[cli] = true
				serverWriter("New connected user [" + cli.user + "]")
				cmdFormat(cli.channel, "Welcome to the Simple IRC Server")
				cmdFormat(cli.channel, "Your user ["+cli.user+"] is successfully logged")
				clientAccept <- "yes"
				if isEmpty {
					cli.admin = true
					serverWriter("[" + cli.user + "] was promoted as the channel ADMIN")
					cmdFormat(cli.channel, "Congrats, you were the first user.")
					cmdFormat(cli.channel, "You're the new IRC Server ADMIN")
				}
			} else {
				serverWriter("Client " + cli.address + " failed to connect due to duplicate user.")
				clientAccept <- "no"
			}

		case cli := <-leaving:
			cli.connection.Close()
			delete(clients, cli)
			if cli.kiched == false {

				for cc := range clients {
					cmdFormat(cc.channel, "["+cli.user+"] left channel")
				}

				serverWriter("[" + cli.user + "] left")
			}
			close(cli.channel)

			if cli.admin {
				//make other user admin
				for possAdmin := range clients {
					possAdmin.admin = true
					cmdFormat(possAdmin.channel, "You're the new IRC Server ADMIN")
					serverWriter("[" + possAdmin.user + "] was promoted as the channel ADMIN")
					for nonAdmins := range clients {
						if nonAdmins.user != possAdmin.user {
							cmdFormat(nonAdmins.channel, "["+possAdmin.user+"] was promoted as the channel ADMIN")
						}
					}
					break
				}

			}

		case clientCh := <-usersCmd:
			var connectedUsers string
			for cli := range clients {
				connectedUsers += cli.user + ", "
			}

			cmdFormat(clientCh, connectedUsers)

		case msgC := <-msgCmd:
			var receivingCh chan string = nil

			for cli := range clients {
				if cli.user == msgC.to {
					receivingCh = cli.channel
					break
				}
			}

			if receivingCh != nil {
				userSay(receivingCh, msgC.from, msgC.message)
			} else {
				cmdFormat(msgC.fromChannel, "User does not exist or is not connected.")
			}
		case uCmd := <-userCmd:

			var outputMsg string = ""

			for cli := range clients {
				if cli.user == uCmd.interestedIn {
					outputMsg = "user: " + cli.user + ", IP: " + cli.address
					break
				}
			}

			if outputMsg != "" {
				cmdFormat(uCmd.fromChannel, outputMsg)
			} else {
				cmdFormat(uCmd.fromChannel, "User does not exist or is not connected.")
			}

		case kCmd := <-kickCmd:

			requestingClientadmin := false

			for k := range clients {
				if kCmd.from == k.user {
					requestingClientadmin = k.admin
				}
			}

			if requestingClientadmin {
				var kickChan chan string = nil
				var kickuser string
				var toBeKicked *chatClient

				for cli := range clients {
					if cli.user == kCmd.whoToKick {
						toBeKicked = cli
						kickChan = cli.channel
						kickuser = cli.user
						toBeKicked.kiched = true
						break
					}
				}

				if kickChan != nil {

					cmdFormat(kickChan, "You were kicked from this channel")
					cmdFormat(kickChan, "Bad language is not allowed on this channel")
					toBeKicked.connection.Close()
					serverWriter("[" + kickuser + "] was kicked from the channel for bad language policy violation")
					for cc := range clients {
						cmdFormat(cc.channel, "["+kickuser+"] was kicked from the channel for bad language policy violation")
					}

				} else {
					cmdFormat(kCmd.fromChannel, "User does not exist or is not connected.")
				}
			} else {
				cmdFormat(kCmd.fromChannel, "Deny action")
			}

		case arr := <-arrival:
			for cc := range clients {
				if arr != cc.user {
					cmdFormat(cc.channel, "New connected user ["+arr+"]")
				}
			}
		}
	}
}

//!-broadcaster

//!+handleConn

func handleConn(conn net.Conn) { //handling client connection

	buffer := make([]byte, 512)
	size, err := conn.Read(buffer)

	if err != nil {
		log.Fatal(err)
	}

	user := string(buffer[:size])
	address := conn.RemoteAddr().String()

	ch := make(chan string)
	go clientWriter(conn, ch, user)

	currentChatClient := chatClient{user: user, address: address, channel: ch, admin: false, connection: conn, kiched: false}
	clientPointer := &currentChatClient

	entering <- clientPointer

	accepted := <-clientAccept

	if accepted == "yes" {
		arrival <- user
		input := bufio.NewScanner(conn)
		for input.Scan() {
			body := make([]string, 2)
			trimmed := strings.Trim(input.Text(), " ")
			if trimmed != "" {
				if trimmed[0] == '/' {
					separated := strings.SplitN(trimmed, " ", 2)

					switch separated[0] {
					case "/users": //ignore everything else
						usersCmd <- ch

					case "/msg": //fix error: /msg lol (index out of range)
						if len(separated) == 2 {
							separatedMsg := strings.SplitN(strings.Trim(separated[1], " "), " ", 2)
							if len(separatedMsg) == 2 {
								body[0] = separatedMsg[1]
								body[1] = separatedMsg[0]
								msgCmd <- messageFormat{to: body[1], message: body[0], from: user, fromChannel: ch}
							}

						}
						break
					case "/time": //ignore everything else
						zone, _ := time.Now().Zone()
						body[0] = "Local Time: " + zone + " " + time.Now().Format("15:04:05\n")
						cmdFormat(ch, body[0])
						break

					case "/user": //ignore everything else beyond <user> //fix error: /msg lol (index out of range)
						if len(separated) == 2 {
							separatedUser := strings.SplitN(strings.Trim(separated[1], " "), " ", 2)
							body[1] = separatedUser[0]
							userCmd <- userFormat{fromChannel: ch, interestedIn: body[1]}
						}
						break

					case "/kick": //ignore everything else beyond <user> //fix error: /msg lol (index out of range)
						if user != body[1] {
							if len(separated) == 2 {
								separatedUser := strings.SplitN(strings.Trim(separated[1], " "), " ", 2)
								body[1] = separatedUser[0]
								kickCmd <- kickFormat{fromChannel: ch, whoToKick: body[1], from: user}
							}
						} else {
							cmdFormat(ch, "Erro. You can't kick yourself")
						}
						break
					default: //treat as normal message
						userSay(messages, user, body[0])
						break
					}

				} else {
					userSay(messages, user, input.Text())
				}
				//cmdFormat(ch, "Incomplete command.")
			}
		}
	}
}

//!-handleConn

func clientWriter(conn net.Conn, ch <-chan string, user string) {
	for msg := range ch {
		fmt.Fprintln(conn, ""+msg+"\n"+user+">") // NOTE: ignoring network errors
	}
}

func serverWriter(message string) {
	fmt.Println("irc-server > " + message)
}

func cmdFormat(ch chan string, message string) { //receive chatClient chan
	labeledMsg := "irc-server > " + message
	ch <- labeledMsg
}

func userSay(ch chan string, user string, message string) {
	labeledMsg := user + " > " + message
	ch <- labeledMsg
}

func forcedBroadcast(message string, channel chan string) {
	messages <- message
}

//!+main
func main() {

	configuration := os.Args[1:]

	if len(configuration) != 4 {
		fmt.Println("Invalid number of arguments.", usage)
	} else if configuration[0] != "-host" || configuration[2] != "-port" {
		fmt.Println("Unsupported flags.", usage)
	} else if _, err := strconv.Atoi(configuration[3]); err != nil {
		fmt.Println("Bad port number.")
	} else {

		listener, err := net.Listen("tcp", configuration[1]+":"+configuration[3])
		if err != nil {
			log.Fatal(err)
		}

		go broadcaster() //global!!

		serverWriter("Simple IRC Server started at " + configuration[1] + ":" + configuration[3])
		serverWriter("Ready for receiving new clients")

		for {
			conn, err := listener.Accept()
			if err != nil {
				log.Print(err)
				continue
			}
			go handleConn(conn)
		}
	}

}