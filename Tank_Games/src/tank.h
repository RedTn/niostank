/*
 * tank.h
 *
 *  Created on: 2013-01-22
 *      Author: Jae-Hwan Jung
 */

#ifndef TANK_H_
#define TANK_H_

#include <math.h>
#include "stdlib.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "bmp_decoder.h"
#include "graphics.h"
#include "terrain.h"

//video graphics array for a tank
typedef struct
{
	int height;
	int width;
	raw_pixels pixel_Array;
	raw_pixels prev_Array;
}tankVGA;

//structure for tanks
typedef struct
{
	int x, y;
	int prev_x, prev_y;
	tankVGA t_VGA;
}tank;

typedef enum { LEFT = 1, RIGHT} direction;

//Requires: the pixel buffer device is already opened with corresponding device structure specified by pixel_buffer
//			two pixel buffers are used
//			both pixel buffer addresses are assigned and set
//Modifies: nothing
//Effects : draws the given tank at (x, y)
void drawTank(tank* t, terrain tr);


//Requires: fs is a bmp file (16-bit RGB, no color palette)
//Modifies: tank
//Effects : initialize the given tank as the user tank using bmp file
void initUserTank(tank* t, short int fd);

void initEnemyTank1(tank* t, short int fd);

//Requires:
//Modifies:
//Effects : updates the location of the tank to the given direction and by the given number of pixels
void moveTank(tank* t, direction d, unsigned int x);

#endif /* TANK_H_ */
