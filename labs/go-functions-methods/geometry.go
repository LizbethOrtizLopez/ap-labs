// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point

//Lizbeth Ortiz Lopez.
//A00227346.
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ x, y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

func onSegment(p Point, q Point, r Point) bool{
    if (q.X() <= math.Max(p.X(), r.X()) && (q.X() >= math.Min(p.X(), r.X()) && (q.Y() <= math.Max(p.Y(), r.Y()) && (q.Y() >= math.Min(p.Y(), r.Y()))))) {
		return true	
	}
	return false
}

func orientation(p, q , r Point) float64 {
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    val := (q.Y() - p.Y()) * (r.X() - q.X()) - (q.X() - p.X()) * (r.Y() - q.Y());
    if (val == 0){
		return 0;
	} 
	if (val > 0){
		return 1;
	}
	return 2;
}

func doIntersect(p1, q1, p2, q2 Point) bool{
    // Find the four orientations needed for general and
    // special cases
    o1 := orientation(p1, q1, p2);
    o2 := orientation(p1, q1, q2);
    o3 := orientation(p2, q2, p1);
    o4 := orientation(p2, q2, q1);
  
    // General case
    if (o1 != o2 && o3 != o4){
        return true;
	}
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) {
		return true;
	}
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) {
		return true;
	}
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) {
		 return true;
	}
  
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) {
		 return true;
	}
  
    return false; // Doesn't fall in any of the above cases
}
  
// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			aux := path[i-1].Distance(path[i])
			sum += aux
			fmt.Print(" + ", aux)
		} else if i == 0 {
			aux := path[i].Distance(path[len(path)-1])
			sum += aux
			fmt.Print(aux)
		}
	}
	return sum
}

func (aux Path) intersect() bool {
	var chocan bool
	for i := 0; i < len(aux)-3 && !chocan; i++ {
		chocan = doIntersect(aux[i], aux[i+1], aux[i+2], aux[i+3])
	}
	return chocan
}

func main() {

	aux := Path{}
	if len(os.Args) == 2 {
		sides, _ := strconv.Atoi(os.Args[1])
			if (sides > 2) {
				fmt.Printf("- Generating a [%d] sides figure\n", sides)
				for i := 0; i < sides; i++ {
					initRandom := rand.NewSource(time.Now().UnixNano())
					random := rand.New(initRandom)
					aux = append(aux, Point{random.Float64()*100 - (-100) + (-100), random.Float64()*100 - (-100) + (-100)})
					for aux.intersect() {
						aux[i] = Point{random.Float64()*100 - (-100) + (-100), random.Float64()*100 - (-100) + (-100)}
					}
				}
				fmt.Printf("- Figure's vertices\n")
				for i := range aux {
					fmt.Printf("   ( %.2f, %.2f)\n", aux[i].X(), aux[i].Y())
				}
				fmt.Println("- Figure's Perimeter")
				fmt.Printf("- ")
				perimeter := aux.Distance()
				fmt.Println(" =", perimeter)
			} else {
				fmt.Printf("Error. Wrong number of sides. \n")
			}		
				
	} else {
		fmt.Printf("Error. Wrong number or arguments. \n")
	}
}


