/*
 * tank_controls.h
 *
 *  Created on: 2013-01-24
 *      Author: Gary
 */


//Overview: Gets user input from controller to set the angle and power and displays on screen

#ifndef TANK_CONTROLS_H_
#define TANK_CONTROLS_H_

#include "tank.h"
#include "VGA.h"
#include "ps_controller.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "terrain.h"
#include <stdio.h>
#include "sys/alt_alarm.h"

#define MAX_VELOCITY 75

//Modifies: angle, velocity by reference
//Ensures: Reads user input from controller to sets the tank's the angle and velocity.
//		   Function ends when X is held and released.
void set_UserTank(int* angle, int* velocity, psController ps, tank* user, tank* enemy,terrain tr);

//delay, probably temporary
void delay();

//Req: ps has already been updated
//Modifies: angle
//Ensures: Increments angle by reference when L1 is pressed and decrements angle when r1 is pressed
//		   180 is left, 0 is right
void set_Angle(int* angle,  psController ps);

//Req: ps has already been updated
//Modifies: angle
//Ensures: increases velocity (power) when x is pressed
char set_Velocity(int* velocity, const psController ps);

//Ensures: displays the angle and velocity(power) on the screen
//@param: x refers to the current bottom right corner coordinate of the charge bar
void display_Input(int angle, int velocity );

#endif /* TANK_CONTROLS_H_ */
