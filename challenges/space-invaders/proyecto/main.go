package main

import (
	"fmt"
	"log"
	"math/rand"
	"os"
	"strconv"
	"time"
	"github.com/arath11/SpaceInvadersGo/scripts"
	"github.com/hajimehoshi/ebiten"	
)

var gm scripts.Game

var enemiesN int

// init function default runs at start
func init() {
	rand.Seed(time.Now().UnixNano())
	if len(os.Args) != 2 {
		fmt.Println("Wrong number of parameters")
		os.Exit(3)
	}

	var err error

	enemiesN, err = strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("Enemies must be number")
		os.Exit(3)
	}

	gm = scripts.NewGame(enemiesN)
}

type World struct {
	area   []bool
	width  int
	height int
}

type Game struct {
	world  *World
	pixels []byte
}

// NewWorld creates a new world.
func NewWorld(width, height int, maxInitLiveCells int) *World {
	w := &World{
		area:   make([]bool, width*height),
		width:  width,
		height: height,
	}
	w.Init(maxInitLiveCells)
	return w
}

//todo: hacer un algorimto para llenado 
func (w *World) Init(maxLiveCells int) {
	for i := 0; i < maxLiveCells; i++ {
		x := rand.Intn(w.width)
		y := rand.Intn(w.height)
		w.area[y*w.width+x] = true
	}
}

func (g *Game) Update(screen *ebiten.Image) error {

	g.world.Update()
	if err := gm.Update(); err != nil {
		return err
	}
	return nil
}

//Actualizacion de juego
func (w *World) Update() {
	width := w.width
	height := w.height
	next := make([]bool, width*height)

	w.area = next
}

func (w *World) Draw(pix []byte) {
	for i := range w.area {
		pix[4*i] = 0
		pix[4*i+1] = 0
		pix[4*i+2] = 0
		pix[4*i+3] = 0
	}
}

const (
	screenWidth  = 1080
	screenHeight = 720
)


//metodo requerido por ebiten
func (g *Game) Draw(screen *ebiten.Image) {
	if g.pixels == nil {
		g.pixels = make([]byte, screenWidth*screenHeight*4)
	}
	g.world.Draw(g.pixels)
	screen.ReplacePixels(g.pixels)

	if err := gm.Draw(screen); err != nil {
		fmt.Println(err)
	}
}


//metodo requerido por ebiten
func (g *Game) Layout(outsideWidth, outsideHeight int) (screenWidth, screenHeight int) {
	return 1080, 720
}


func main() {
	game := &Game{}

	game.world = NewWorld(screenWidth, screenHeight, int((screenWidth*screenHeight)/20))

	ebiten.SetWindowSize(1080, 720)
	ebiten.SetWindowTitle("SPACE INVADERS :)")

	if err := ebiten.RunGame(game); err != nil {
		log.Fatal(err)
	}

}
