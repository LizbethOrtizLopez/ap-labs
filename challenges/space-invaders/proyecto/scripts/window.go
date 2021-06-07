package scripts

import (
	"image/color"
	"strconv"

	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/text"
	"golang.org/x/image/font/basicfont"
)

// Window GUI
type Window struct {
	game        *Game
	points      int
	win			bool
}

// CreateWindow initialices window
func CreateWindow(g *Game) *Window {
	w := Window{
		game:        g,
		points:      0,
		win:		 false,
	}

	return &w
}

func (w *Window) AddPoint() {
	w.points++
}

//TextFormat gives format to text
func TextFormat(text string) (w int, h int) {
	return 10 * len(text), 16
}

// EndGame show results and detect ends
func (w *Window) EndGame(screen *ebiten.Image) {

	if  w.win { //Player killed enemies
		goText := "GREAT YOU WIN!"
		textW, textH := TextFormat(goText)
		screenW := screen.Bounds().Dx()
		screenH := screen.Bounds().Dy()
		text.Draw(screen, goText, basicfont.Face7x13, screenW/2-textW/2, screenH/2+textH/2, color.White)
		text.Draw(screen, "Score: "+strconv.Itoa(w.points), basicfont.Face7x13, 30, 30, color.White)
	} else {
		//enemies killed you loser
		goText := "GAME OVER -> YOU DIED"
		textW, textH := TextFormat(goText)
		screenW := screen.Bounds().Dx()
		screenH := screen.Bounds().Dy()
		text.Draw(screen, goText, basicfont.Face7x13, screenW/2-textW/2, screenH/2+textH/2, color.White)
		text.Draw(screen, "Score: "+strconv.Itoa(w.points), basicfont.Face7x13, 30, 30, color.White)
	}
}

// Draw text points
func (w *Window) Draw(screen *ebiten.Image) error {
	//TODO que siempre se muestren los textos
	if !w.game.playing {
		w.EndGame(screen)
	}

	return nil
}

//EndAux calls end principal and prepares function
//No sirve de nada
func (w *Window) EndAux(screen *ebiten.Image) {
	w.EndGame(screen)
}
