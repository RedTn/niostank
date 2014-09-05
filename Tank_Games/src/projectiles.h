/*
 * projectile.h
 *
 *  Created on: Jan 23, 2013
 *      Author: Gary
 */
//Overview: Given the angle and velocity, a chosen projectile's path is calculated and drawn on the screen

#ifndef PROJECTILES_H_
#define PROJECTILES_H_

//-========Define constants
#define NUMBER_OF_TANKS 1
#define GRAVITY_ACC 10
#define DELTA_TIME 0.02 //time calculation increment for projectiles


//---======Libraries and Headers---
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "collision.h"
#include "global.h"
#include "graphics.h"
#include "bmp_decoder.h"
#include "terrain.h"
#include "VGA.h"
#include <math.h>
#include "sys/alt_alarm.h"

//==========Method headers ==================================================

//---------Top Functions-----

//Req:		collisionMatrix has been initialized
//Modifies: collisionMatrix
//Ensures: shoot missile from given position with given velocity and angle. Missile moves until collision or
//		   out of screen
void shoot_Missile(int angle, int velocity, int X0, int Y0, int** collisionMatrix, terrain background );



//-------Helper functions---------------------


//Ensures: returns y position of projectile at a given time ts using newton's equations
//@param Vy0 = initial y velocity at t = 0, Y0 = inital position at t = 0
int calc_Ypos(int Vy0, int Y0, float ts);

//Ensures: returns x position of projectile at some time ts using newton's equations
//@param Vy0 = initial y velocity at t = 0, Y0 = inital position at t = 0
int calc_Xpos(int Vx0, int X0, float ts);

//Ensures: returns X component of velocity from angle
int calc_XVelocity(int angle, int velocity);

//Ensures: returns Y component of velocity from angle
int calc_YVelocity(int angle, int velocity);

//Ensures: returns 0 if coordinates are not within bounds and 1 if within.
//In bounds refers to not left of screen, right of screen, or below screen
char isWithinBounds(int x, int y);

float deg_to_Rad(int angle);

int rad_to_Deg(float angle);

//=====================================================================================

#endif /* PROJECTILES_H_ */
