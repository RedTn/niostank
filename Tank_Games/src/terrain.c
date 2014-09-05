/*
 * terrain.c
 *
 *  Created on: 2013-01-26
 *      Author: Jae-Hwan Jung
 */



#include "terrain.h"

static int height;
static int width;

void initTerrain(short int fd, terrain* t)
{
	decode_Bmp(fd, t, &width, &height);
}

void drawBackground(terrain t)
{
	int i,j;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			unsigned short color = t[width*i+j];
			alt_up_pixel_buffer_dma_draw(pixel_buffer, color, j,  239-i);
		}
	}

}

void drawTerrain(terrain t)
{
	int i,j;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			unsigned short color = t[width*i+j];
			if (color < 0xCCCC)
				alt_up_pixel_buffer_dma_draw(pixel_buffer, color, j,  239-i);
		}
	}
}
