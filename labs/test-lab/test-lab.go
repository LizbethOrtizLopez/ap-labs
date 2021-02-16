package main

import (
	"fmt"
	"os"
)

func main() {
	
	if (len(os.Args)==1){
	  fmt.Println("Error")
	}else if(len(os.Args)>=2){
	  fmt.Printf("Hello ")
	  for i:=1;i<len(os.Args)-1;i++ {
	    fmt.Printf(os.Args[i]+ " ")
	  }
	  fmt.Printf(os.Args[len(os.Args)-1]+", Welcome to the jungle")
	  fmt.Println("")
	}
}
