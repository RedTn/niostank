
/*
 * projectile.c
 *
 *  Created on: Jan 23, 2013
 *      Author: Gary
 */

#include "projectiles.h"
#include <stdio.h>
void shoot_Missile(int angle, int velocity, int X0, int Y0, int** collisionMatrix, terrain background)
{

	float ts = 0;

	//1 is arbitrary number as initial value does not matter as long as within bounds of while loop
	int Xtemp = 1;
	int Ytemp = 1;

	int ii = 0;

	int Vx0 = calc_XVelocity(angle, velocity);
	int Vy0 = calc_YVelocity(angle, velocity);

	//projectile smoke trail uses queue and shifting
	//The last element will be erased and first element is the newest coordinate
	//of the missile
	const int trail_Length = 35;
	int trailX[trail_Length];
	int trailY[trail_Length];

	//Initial to out of bound coordinates so it won't erase
	//until 35 pixels have been drawn
	for(ii = trail_Length; ii > 0; ii--)
	{
		trailX[ii] = -1;
		trailY[ii] = -1;
	}

	//loop continues until out of bounds or collision
	do
	{
		Xtemp = calc_Xpos(Vx0, X0, ts);
		Ytemp = calc_Ypos(Vy0, Y0, ts);

		Ytemp = YMAXSCREEN_RESOLUTION - Ytemp - 1; // have to flip Y values since 0, 0 coordinate is top left
											  // Using bottom left as 0, 0 reference

		trailX[0] =  Xtemp; //store newest coordinate in first element
		trailY[0] =  Ytemp;

		//shift array right
		for(ii = trail_Length - 1; ii > 0; ii--)
		{
			trailX[ii] = trailX[ii - 1];
			trailY[ii] = trailY[ii - 1];
		}

		ts += DELTA_TIME;


		// only draw new pixel for missile for if in screen and no collision
		if(isWithinBounds(Xtemp, Ytemp) == 1 && Ytemp >= 0) {
			if(check_CollisionAt(collisionMatrix, Xtemp, Ytemp) == 0)
			{
				//alt_up_pixel_buffer_dma_draw(pixel_buffer, 0xFFFF, Xtemp, Ytemp);
				alt_up_pixel_buffer_dma_draw_line(pixel_buffer, Xtemp, Ytemp, Xtemp, Ytemp, 0XFFFF, 0);
				usleep(3500);

			}
			else break; //collision so end loop
		}

		//only erase old pixel if it is in screen
		if(isWithinBounds(trailX[trail_Length - 1], trailY[trail_Length - 1]) == 1 && trailY[trail_Length - 1] >= 0)
		{
			//alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, trailX[trail_Length - 1], trailY[trail_Length - 1]);
			alt_up_pixel_buffer_dma_draw_line(pixel_buffer, trailX[trail_Length - 1], trailY[trail_Length - 1], trailX[trail_Length - 1], trailY[trail_Length - 1], background[320 * (239 - trailY[trail_Length - 1]) + trailX[trail_Length - 1]], 0);
			alt_up_pixel_buffer_dma_draw_line(pixel_buffer, trailX[trail_Length - 1], trailY[trail_Length - 1], trailX[trail_Length - 1], trailY[trail_Length - 1], background[320 * (239 - trailY[trail_Length - 1]) + trailX[trail_Length - 1]], 1);

		}
		//updateVGA();
		//updateVGA();

	}while(isWithinBounds(Xtemp, Ytemp) == 1); //&& check_CollisionAt(collisionMatrix, Xtemp, Ytemp) == 0);

	//remove rest of trail once missile stops
	for(ii = 0; ii < trail_Length; ii++){
		if(isWithinBounds(trailX[ii], trailY[ii]) == 1 && trailY[ii] >= 0)
		{
			alt_up_pixel_buffer_dma_draw_line(pixel_buffer, trailX[ii], trailY[ii], trailX[ii], trailY[ii], background[320*(239 - trailY[ii]) + trailX[ii]], 0);
			alt_up_pixel_buffer_dma_draw_line(pixel_buffer, trailX[ii], trailY[ii], trailX[ii], trailY[ii], background[320*(239 - trailY[ii]) + trailX[ii]], 1);
			//alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x0000, trailX[ii], trailY[ii]);
		}
	}
}



//-------------Helper Functions


int calc_Ypos(int Vy0, int Y0, float ts)
{
	//vt + y0 - 0.5gt^2
	return (int)(((Vy0 * ts) + Y0) - ((0.5) * GRAVITY_ACC * ts * ts));

}

int calc_Xpos(int Vx0, int X0, float ts)
{

	return (int)(X0 + Vx0 * ts);

}

int calc_XVelocity(int angle, int velocity)
{

	return (int)(cos(deg_to_Rad(angle)) * velocity);

}

int calc_YVelocity(int angle, int velocity)
{
	return (int)(sin(deg_to_Rad(angle)) * velocity);

}

char isWithinBounds(int x, int y)
{
	//Can be above screen
	if ( x <= XMAXSCREEN_RESOLUTION && x >= 0 && y <= YMAXSCREEN_RESOLUTION)
		return 1;
	else
		return 0;
}

float deg_to_Rad(int angle)
{
	return angle * PI_ / 180;
}

int rad_to_Deg(float angle)
{
	return angle * 180 / PI_;
}
