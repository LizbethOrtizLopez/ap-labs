package scripts

import (
	"github.com/hajimehoshi/ebiten"
	"github.com/hajimehoshi/ebiten/ebitenutil"
)

type Bullet struct {
	xLimit int
	yLimit int
	xPos   float64
	yPos   float64
	shoot  bool
	mode   bool
	game   *Game
	bullet ebiten.Image
}

// Generates a Bullet at a given position
func CreateBullet(g *Game,x float64, y float64, modeBullet bool) *Bullet {
	c := Bullet{
		game:   g,
		xLimit: -20,
		yLimit: -20,
		shoot:  false,
		mode:   modeBullet,
	}

	bullet, _, _ := ebitenutil.NewImageFromFile("images/BulletPlayer.png", ebiten.FilterDefault)
	c.bullet = *bullet

	

	c.xPos = float64(x)
	c.yPos = float64(y)
	return &c
}

// Update the state of a bullet to delete
func (c *Bullet) Update(dotTime int) error {
	return nil
}

// Draw the bullet
func (c *Bullet) Draw(screen *ebiten.Image, dotTime int) error {
	if c.game.playing{
		if c.shoot{
			c.MoveBullet(dotTime)
		}
	}
	
	canonDE = &ebiten.DrawImageOptions{}
	canonDE.GeoM.Translate(c.xPos, c.yPos)
	screen.DrawImage(&c.bullet, canonDE)

	//CHECAR SI SE SALIO DE LA PANTALLA 
	if c.mode{
		//bala enemigo
		if c.yPos > 720{
			c.shoot=false
			c.xPos=-20
			c.yPos=230
		}
	}else{
		//bala amiga
		if c.yPos < 0{
			c.shoot=false
			c.xPos=-20
			c.yPos=630

		}
	}

	return nil
}

func (c *Bullet)Disparar(newXPos float64, newYPos float64){
	if !c.shoot{
		c.shoot=true
		c.xPos=newXPos
		c.yPos=newYPos
	}
}

func (c *Bullet) MoveBullet(dotTime int){
	if dotTime == 1{
		if c.mode{
			c.Mover(0,20)
		}else{
			c.Mover(0,-20)
		}
	}
}


// mueve al canon
func (c *Bullet) Mover(newXPos float64, newYPos float64) {
	c.xPos = c.xPos + newXPos
	c.yPos = c.yPos + newYPos
}

