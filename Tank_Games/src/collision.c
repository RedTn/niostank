/*
 * collision.c
 *
 *  Created on: Jan 26, 2013
 *      Author: Gary
 */

#include "collision.h"


int** init_CollisionMatrix(int row, int col)
{
	//initialize 2d array
	int** collisionMatrix = (int**)malloc(row * sizeof(int));
	int ii;


	for(ii = 0; ii < row; ii++)
	{
		collisionMatrix[ii] = (int*)malloc(col * sizeof(int));
	}

	return collisionMatrix;
}

void set_CollisionAt(int** collisionMatrix, int x, int y, int value)
{
	collisionMatrix[y][x] = value;
}
void set_CollisionAtRectArea(int** collisionMatrix, int x0, int y0, int x1, int y1, int value)
{
	int yi = 0;
	int xi = 0;

	for(yi = y0; yi <= y1; yi++)
	{
		for(xi = x0; xi <= x1; xi++)
			collisionMatrix[yi][xi] = value;
	}
}

char check_CollisionAt(int** collisionMatrix, int x, int y)
{
	if(collisionMatrix[y][x] == TERRAIN_COLOR ) // will need to add other colors for other non empty space objects
	{
		return 1;
	}

	return 0;
}

char check_CollisionAtRectArea(int** collisionMatrix, int x0, int y0, int x1, int y1, int value)
{
	int yi = 0;
	int xi = 0;

	//check row by row
	for(yi = y0; yi <= y1; yi++)
	{
		for(xi = x0; xi <= x1; xi++)
		{
			if( check_CollisionAt(collisionMatrix, yi, xi) == 1)
				return 1;
		}

	}

	return 0;
}


