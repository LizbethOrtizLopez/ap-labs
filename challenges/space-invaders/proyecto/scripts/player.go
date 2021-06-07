package scripts

import (
	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/ebitenutil"
)

var canonDE *ebiten.DrawImageOptions

// Snake : Object which the player controls
type Canon struct {
	game             *Game
	lastDir          string
	canon    ebiten.Image	
	bodyParts        []float64
	pointsWaiting    int
	points           int
	channelMovements chan int
	collision        bool
	isShooting       bool
	lives			 int
}

//Genera un canon
func CreatePlayer(g *Game) *Canon {
	s := Canon{
		game:          g,
		lastDir:       "center",
		pointsWaiting: 0,
		collision:     false,
		isShooting:	   false,
		lives:		   10,
	}
	s.channelMovements = make(chan int)
	
	s.bodyParts = []float64{520, 650}
	canon, _, _ := ebitenutil.NewImageFromFile("images/Canon.png", ebiten.FilterDefault)
	s.canon = *canon
	return &s
}

func (s *Canon) ChannelPipe() error {
	dotTime := <-s.channelMovements
	for {
		s.Direction(dotTime)
		dotTime = <-s.channelMovements
	}
}

// Direction Logical update of the snake
func (s *Canon) Direction(dotTime int) error {
	xPos := s.bodyParts[0]
	
	if ebiten.IsKeyPressed(ebiten.KeyUp) {
		if !s.isShooting{
			s.isShooting=true
		}
	} 

	if ebiten.IsKeyPressed(ebiten.KeyRight) && s.lastDir != "right" && xPos<1051{
		s.lastDir = "right"
		return nil
	}else if ebiten.IsKeyPressed(ebiten.KeyLeft) && s.lastDir != "left" && xPos>20 {
		s.lastDir = "left"
		return nil
	}else{
		s.lastDir = "center"
		return nil
	}
	
	return nil
}

// dibujar el canon
func (s *Canon) Draw(screen *ebiten.Image, dotTime int) error {
	if s.game.playing {
		s.Move(dotTime)
	}
	if(s.lives <= 0){
		s.game.End()
	}
	canonDE = &ebiten.DrawImageOptions{}
	xPos, yPos := s.GetPos()
	canonDE.GeoM.Translate(xPos, yPos)	
	screen.DrawImage(&s.canon, canonDE)
	return nil
}

// MoveSnake position values for the snake head
func (s *Canon) Move(dotTime int) {
	if dotTime == 1 {
		//cambiar a vidas
		if s.pointsWaiting > 0 {
			
			s.pointsWaiting--
		}
		switch s.lastDir { 
		case "center":
			s.Mover(0,0)
		case "right":
			s.Mover(40, 0)
		case "left":
			s.Mover(-40, 0)
		}

	}
}

// Returns player's position 
func (s *Canon) GetPos() (float64, float64) {
	return s.bodyParts[0], s.bodyParts[1]
}


// ChangeBody adds a body to serpent
func (s *Canon) ChangeBody(newX, newY float64) {
	s.bodyParts = []float64{newX, newY}
	
}

// mueve al canon
func (s *Canon) Mover(newXPos, newYPos float64) {
	newX := s.bodyParts[0] + newXPos
	newY := s.bodyParts[1] + newYPos
	s.ChangeBody(newX, newY)
}

// AddPoint controls game's points
func (s *Canon) AddPoint() {
	s.points++
	s.pointsWaiting++
}
