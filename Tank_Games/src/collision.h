/*
 * collision.h
 *
 *  Created on: Jan 26, 2013
 *      Author: Gary
 */

//Overview: Creates, reads, and modifies a matrix to contain the pixel position of the objects on the screen to detect
//			any collisions

//@param plan to make the variable value to contain some special number to determine what object is
#ifndef COLLISION_H_
#define COLLISION_H_

#include "global.h"
#include <stdlib.h>

//Ensures: Dynamically creates a 2d array with the given row and column sizes
int** init_CollisionMatrix(int row, int col);


//Req: 		collisionMatrix has already been initialized, and x && y are within bounds
//Modifies: collisionMatrix
//Ensures: Sets the element at [y][x] to value
void set_CollisionAt(int** collisionMatrix, int x, int y, int value);


//Req: 		collisionMatrix has already been initialized, and x && y are within bounds
//Modifies: collisionMatrix
//Ensures: Sets a rectangular area of elements from [y0][x0] (top left corner) to [y1][x1] (bottom right)
//		   to value
void set_CollisionAtRectArea(int** collisionMatrix, int x0, int y0, int x1, int y1, int value);


//Req: 		collisionMatrix has already been initialized, and x && y are within bounds
//Ensures:  Returns false(0) if the element at [y][x] is not occupied. Otherwise return 1.
char check_CollisionAt(int** collisionMatrix, int x, int y);


//Req: 		collisionMatrix has already been initialized, and x && y are within bounds
//Ensures:  Returns false (0) if the area of elements from [y0][x0] (top left corner) to [y1][x1] (bottom right)
//			if none are occupied. Else return 1.
char check_CollisionAtRectArea(int** collisionMatrix, int x0, int y0, int x1, int y1, int value);



#endif /* COLLISION_H_ */
