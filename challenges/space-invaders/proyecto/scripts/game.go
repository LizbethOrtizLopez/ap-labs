package scripts

import (
	"time"
	"math/rand"
	"github.com/hajimehoshi/ebiten"
)

// Game object that contains everything
type Game struct {
	canon       *Canon
	canonChan   chan int
	window      *Window
	playerBullets []*Bullet
	enemyBullets  []*Bullet
	numBullets int	
	numEnemies  int
	enemies     []*Enemy
	enemiesChan []chan int
	playing     bool
	dotTime     int
}

//creamos un nuevo juego       
func NewGame( enemies int) Game {
	g := Game{
		playing:     true,
		dotTime:     0,
		numBullets: 30,
		numEnemies:  enemies,
	}

	//balas del jugador
	arrayPlayerBullets := make([]*Bullet, g.numBullets)
	//balas de enemigos
	arrayEnemybullets := make([]*Bullet, g.numBullets)
	for i := 0; i < g.numBullets; i++ {
		arrayPlayerBullets[i] = CreateBullet(&g,-20,-20,false)
		arrayEnemybullets[i] = CreateBullet(&g,-20,-20,true)
		time.Sleep(02)
	}
	
	//enemigos
	arrayEnemies := make([]*Enemy, g.numEnemies)
	
	var x float64 = 20
	var y float64 = 20

	for i := 0; i < len(arrayEnemies); i++ {
		if(i%11==0){
			x=20
			y=y+40
		}else{
			x=x+60
		}
		arrayEnemies[i] = CreateEnemy(&g,x,y)
		time.Sleep(02)
	}
	//creamos el canal 
	enemiesChan := make([]chan int, g.numEnemies)


	for i := 0; i < len(enemiesChan); i++ {
		enemiesChan[i] = make(chan int)
		arrayEnemies[i].channelMovements = enemiesChan[i]
		go arrayEnemies[i].ChannelPipe()
		time.Sleep(02)
	}
	//asignamos
	g.enemiesChan = enemiesChan
	g.playerBullets = arrayPlayerBullets
	g.enemyBullets = arrayEnemybullets
	g.enemies = arrayEnemies

	//creamos el canon
	g.canon = CreatePlayer(&g)
	g.canonChan = make(chan int)
	go g.canon.ChannelPipe()

	//creamos nueva ventana con 2 balas
	g.window = CreateWindow(&g)
	return g
}

// End the game
func (g *Game) End() {
	g.playing = false
}


func (g *Game) Update() error {
	if g.playing {
		//vidas y te mostrara la pantalla de derrota
		/*
		if g.numBullets == 0 {
			g.playing = false
		}*/

		//velocidad del juego
		g.dotTime = (g.dotTime + 1) % 10

		//mandar tick
		if err := g.canon.Direction(g.dotTime); err != nil {
			g.canonChan <- g.dotTime
		}

		//mandar tick
		for i := 0; i < len(g.enemiesChan); i++ {
			g.enemiesChan[i] <- g.dotTime
		}
		

		//checar movimiento alien con paredes
		//movimiento de izquierda a detr
		for j := 0; j < len(g.enemies); j++ {
			//cambiar a izquierda
			if g.enemies[j].bodyParts[0] <10{
				for i := 0; i < len(g.enemies); i++ {
					g.enemies[i].lastDir="right"
				}
				break
			}else if g.enemies[j].bodyParts[0]>1030{
				//cambiar a derecha		
				for i := 0; i < len(g.enemies); i++ {
					g.enemies[i].lastDir="left"
				}
				break
			}			
		}
		
		azarbajar:= 750
		

		seed := rand.NewSource(time.Now().UnixNano())
		random := rand.New(seed)
		
		//movimiento a abajo random
		nuevoNum:=random.Intn(azarbajar)
		
		if nuevoNum<1{
			for j := 0; j < len(g.enemies); j++ {
				g.enemies[j].bodyParts[1]=g.enemies[j].bodyParts[1]+35
			}	
		}

		azardisparar:= 50
		nuevoNum = random.Intn(azardisparar)
		if nuevoNum < 1{
			randomEnemy := random.Intn(len(g.enemies))
			if g.enemies[randomEnemy].alive{
				for i:= 0; i < len(g.enemyBullets); i++{
					if !g.enemyBullets[i].shoot{
						g.enemyBullets[i].Disparar(g.enemies[randomEnemy].bodyParts[0],g.enemies[randomEnemy].bodyParts[1]+20)
						g.enemyBullets[i].shoot = true
						break
					}	
				}	
			}
		}
		//checar bala
		if g.canon.isShooting{
			for i:= 0; i < len(g.playerBullets); i++{
				if !g.playerBullets[i].shoot{
					g.playerBullets[i].Disparar(g.canon.bodyParts[0],g.canon.bodyParts[1]-20)
					g.canon.isShooting=false
					g.playerBullets[i].shoot = true
					break
				}	
			}	
		}		

		//colision con enemigos
		for j := 0; j < len(g.enemies); j++ {
			if g.enemies[j].alive{
				xPos, yPos := g.enemies[j].GetPos()
				for i := 0; i < len(g.playerBullets); i++ {
					if xPos < g.playerBullets[i].xPos && xPos+30 >g.playerBullets[i].xPos && yPos < g.playerBullets[i].yPos && yPos+50 >g.playerBullets[i].yPos{
						g.playerBullets[i].yPos = -20	
						g.playerBullets[i].xPos = -20
						g.playerBullets[i].shoot=false
					
						g.enemies[j].alive=false
						g.enemies[j].bodyParts[0]=500
						g.enemies[j].bodyParts[1]=2000

						g.window.AddPoint()
						g.numEnemies=g.numEnemies-1
						break
				}
			}
			}
		}
		xPos, yPos := g.canon.GetPos()
		for i:= 0; i < len(g.enemyBullets); i++{
			if xPos -5 < g.enemyBullets[i].xPos && xPos+25 >g.enemyBullets[i].xPos && yPos < g.enemyBullets[i].yPos && yPos+50 >g.enemyBullets[i].yPos {
				g.canon.lives = g.canon.lives - 1
				g.enemyBullets[i].yPos = -20	
				g.enemyBullets[i].xPos = -20
				g.enemyBullets[i].shoot=false
				//fmt.Println("Heridooo!",g.canon.lives)
			}
		}
	}
		
	for i := 0; i < g.numBullets; i++ {
		if err := g.playerBullets[i].Update(g.dotTime); err != nil {
			return err
		}
	}
	for i := 0; i < g.numBullets; i++ {
		if err := g.enemyBullets[i].Update(g.dotTime); err != nil {
			return err
		}
	}
	return nil
}


//cambiar
func (g *Game) Draw(screen *ebiten.Image) error {
	//canon	
	if err := g.canon.Draw(screen, g.dotTime); err != nil {
		return err
	}
	//enemigos
	for _, enemy := range g.enemies {
		if err := enemy.Draw(screen, g.dotTime); err != nil {
			return err
		}
	}
	//ventana
	if err := g.window.Draw(screen); err != nil {
		return err
	}
	//balas del jugador
	for i := 0; i < len(g.playerBullets); i++ {
		if err := g.playerBullets[i].Draw(screen, g.dotTime); err != nil {
			return err
		}
	}
	//balas de enemigos
	for i := 0; i < len(g.enemyBullets); i++ {
		if err := g.enemyBullets[i].Draw(screen, g.dotTime); err != nil {
			return err
		}
	}
	//vidas
	if g.numEnemies == 0 {
		g.window.win = true
		g.window.EndGame(screen)
	}
	return nil
}
