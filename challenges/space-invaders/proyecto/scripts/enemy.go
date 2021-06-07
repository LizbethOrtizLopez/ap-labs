package scripts

import (
	"math/rand"
	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/ebitenutil"
)

// Enemy object
type Enemy struct {
	game             *Game
	numParts         int
	lastDir          string
	enemy    ebiten.Image
	bodyParts        []float64
	seed             rand.Source
	pointsWaiting    int
	points           int
	channelMovements chan int
	collision        bool
	alive 			 bool
}

// CreateEnemy initialize enemy
func CreateEnemy(g *Game, x float64, y float64) *Enemy {
	e := Enemy{
		game:          g,
		numParts:      0,
		lastDir:       "right",
		pointsWaiting: 0,
		collision:     false,
		alive:		   true,
	}

	e.channelMovements = make(chan int)
	

	e.bodyParts = []float64{x,y}

	enemy, _, _ := ebitenutil.NewImageFromFile("images/Alien.png", ebiten.FilterDefault)
	e.enemy = *enemy
	
	return &e
}

//ChannelPipe Pipe movements enemy
func (s *Enemy) ChannelPipe() error {
	for {
		dotTime := <-s.channelMovements
		s.Direction(dotTime)
	}
}

// Direction Logical update of the enemy
func (s *Enemy) Direction(dotTime int) error {
	
	if dotTime == 1 { 
		//checks collision between enemy and player
		xPos, yPos := s.game.canon.GetPos()
		if s.CollisionWithPlayer(xPos, yPos) {
			s.game.canon.collision = true
			s.game.End()
		}
	}
	return nil
}

// Draw the snake
func (s *Enemy) Draw(screen *ebiten.Image, dotTime int) error {
	if s.game.playing {
		s.UpdatePos(dotTime)
	}
	enemyDO := &ebiten.DrawImageOptions{}
	xPos, yPos := s.GetPos()
	enemyDO.GeoM.Translate(xPos, yPos)

	screen.DrawImage(&s.enemy, enemyDO)

	return nil
}

// UpdatePos changes position values for the snake head
func (s *Enemy) UpdatePos(dotTime int) {
	if dotTime == 1 {
		if s.alive{
			if s.pointsWaiting > 0 {
				s.pointsWaiting--
			}
			switch s.lastDir {
			case "down":
				s.TranslatePos(0, +20)
			case "right":
				s.TranslatePos(20, 0)
			case "left":
				s.TranslatePos(-20, 0)
			}
		}
	}
}

// Returns enemy's position
func (s *Enemy) GetPos() (float64, float64) {
	return s.bodyParts[0], s.bodyParts[1]
}

// CollisionWithPlayer evaluates the collision Delete this
func (s *Enemy) CollisionWithPlayer(xPos, yPos float64) bool {
	if xPos == s.bodyParts[0] && yPos == s.bodyParts[1] {
		return true
	}
	return false
}

// TranslatePos changes enemy's position 
func (s *Enemy) TranslatePos(newXPos, newYPos float64) {
	newX := s.bodyParts[0] + newXPos
	newY := s.bodyParts[1] + newYPos
	s.bodyParts = []float64{newX,newY}
}

