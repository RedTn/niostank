/*
 * tank.c
 *
 *  Created on: 2013-01-22
 *      Author: Jae-Hwan Jung
 */



#include "tank.h"

void drawTank(tank* t, terrain tr)
{
	int i=0,j=0;
	unsigned short color, prev_color;
	for (i=0;i<(t->t_VGA.height);i++)
	{
		for (j=0;j<t->t_VGA.width;j++)
		{
			prev_color = tr[(239-t->prev_y+i)*320 + (t->prev_x+j)];
			alt_up_pixel_buffer_dma_draw(pixel_buffer, prev_color, t->prev_x+j,  t->prev_y-i);
		}
	}



	for (i=0;i<(t->t_VGA.height);i++)
	{
		for (j=0;j<t->t_VGA.width;j++)
		{
			color = t->t_VGA.pixel_Array[t->t_VGA.width*i+j];

			if (color < 0xCCCC)
			{


				alt_up_pixel_buffer_dma_draw(pixel_buffer, color, t->x+j,  t->y-i);
			}
		}
	}
}

//Requires: fs is a bmp file (16-bit RGB, no color palette)
//Modifies: tank
//Effects : initialize the given tank as the user tank using bmp file
void initUserTank(tank* t, short int fd)
{
	//Tanks initial location
	const int X_POSITION = 10;
	const int Y_POSITION = 160;

	//setting initial position of the tank
	t->x = t->prev_x = X_POSITION;
	t->y = t->prev_y = Y_POSITION;

	//decode the bmp file and initialize the tank object
	decode_Bmp(fd, &(t->t_VGA.pixel_Array), &(t->t_VGA.width), &(t->t_VGA.height));

	int PixelArraySize = t->t_VGA.height*t->t_VGA.width*sizeof(short);
	t->t_VGA.prev_Array = (unsigned short*)malloc(PixelArraySize);
}

void initEnemyTank1(tank* t, short int fd)
{
	//Tanks initial location
	const int X_POSITION = 230;
	const int Y_POSITION = 161;

	//setting initial position of the tank
	t->x = X_POSITION;
	t->y = Y_POSITION;

	//decode the bmp file and initialize the tank object
	decode_Bmp(fd, &(t->t_VGA.pixel_Array), &(t->t_VGA.width), &(t->t_VGA.height));

	int PixelArraySize = t->t_VGA.height*t->t_VGA.width*sizeof(short);
	t->t_VGA.prev_Array = (unsigned short*)malloc(PixelArraySize);
}

//Requires:
//Modifies:
//Effects : updates the location of the tank to the given direction and by the given number of pixels
void moveTank(tank* t, direction d, unsigned int x)
{
	t->prev_x = t->x;
	t->prev_y = t->y;

	if (d == LEFT)
	{
		t->x = t->x - x;
	}
	else
	{
		t->x = t->x + x;
	}
}
