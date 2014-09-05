/*
 * tank_controls.c
 *
 *  Created on: 2013-01-24
 *      Author: Gary
 */

#include "tank_controls.h"

const int x0_chargeBar = 70;
const char y0_chargeBar = 3;
const char chargeBar_Width = 5;

void set_UserTank(int* angle, int* velocity, psController ps, tank* user, tank* enemy, terrain tr)
{
	char done_Charging = 0;
	char hasxBeenPressed = 0;

	//Draw background of power bar
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0_chargeBar, y0_chargeBar, (MAX_VELOCITY) + x0_chargeBar, chargeBar_Width +  y0_chargeBar, 0x7BEF, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0_chargeBar, y0_chargeBar, (MAX_VELOCITY) + x0_chargeBar, chargeBar_Width +  y0_chargeBar, 0x7BEF, 1);
	//loop continues until x is pressed and released, a collision occurs, or velocity maxed out
	while(done_Charging == 0)
	{
		update_Controller_Keys(&ps);
		set_Angle( angle, ps);
		done_Charging = set_Velocity( velocity, ps);

		if(ps.b.x == 1)
			hasxBeenPressed = 1;

		//X has been released
		if(ps.b.x == 0 && hasxBeenPressed == 1)
			done_Charging = 1;

		if(ps.b.right == 1)
		{
			moveTank(user, 2, 1); //go right
			drawTank(user, tr);
			drawTank(enemy, tr);
			updateVGA();
		}
		else if(ps.b.left)
		{
			moveTank(user, 1, 1); // go left
			drawTank(user, tr);
			drawTank(enemy, tr);
			updateVGA();
		}
		display_Input(*angle, *velocity);
		delay();
	}


}


void set_Angle(int* angle, const psController ps)
{
	//increase (move left) if l1
	if(ps.b.l1 == 1)
	{
		if(*angle < 180)
			(*angle)++;
	}

	//decrease(move right) if r1
	else if(ps.b.r1 == 1)
	{
		if(*angle > 0)
			(*angle)--;
	}
}
char set_Velocity(int* velocity, const psController ps)
{
	//if x is pressed and power below max, charge it up
	if(ps.b.x == 1)
	{
		if(*velocity == MAX_VELOCITY)
			return 1; //max power so can't add anymore

		else if(*velocity < 100)
			(*velocity)++;
	}

	return 0;
}

void delay()
{
	usleep(10000);
}

void display_Input(int angle, int velocity )
{

	char buffer[11]; //stores string to print
	const float multipler_to_make_out_of_100 = 1.34;
	alt_up_char_buffer_string(char_buffer, "Angle: ", 0, 1);
	sprintf(buffer, "%d", angle); //convert int to string

	//To display 3 digits all the time, we need to add '0's in front if value is less than 3 digits
	if (angle < 10)
	{

		buffer[2] = buffer[0];
		buffer[1] = '0';
		buffer[0] = '0';

	}
	else if(angle < 100)
	{
		buffer[2] = buffer[1];
		buffer[1] = buffer[0];
		buffer[0] = '0';
	}

	buffer[3] ='\0';

	alt_up_char_buffer_string(char_buffer, buffer, 6, 1);
	sprintf(buffer, "%d", (int)(velocity * multipler_to_make_out_of_100));


	if (velocity * multipler_to_make_out_of_100 < 10)
	{

		buffer[2] = buffer[0];
		buffer[1] = '0';
		buffer[0] = '0';

	}
	else if(velocity * multipler_to_make_out_of_100 < 100)
	{
		buffer[2] = buffer[1];
		buffer[1] = buffer[0];
		buffer[0] = '0';
	}

	buffer[3] ='\0';

	alt_up_char_buffer_string(char_buffer, "Power: ", 11, 1);
	alt_up_char_buffer_string(char_buffer, buffer, 21, 1);

	//charge bar: top left is fixed, while bottom right is dependent on velocity
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0_chargeBar, y0_chargeBar, (velocity) + x0_chargeBar, chargeBar_Width +  y0_chargeBar, 0xF800, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x0_chargeBar, y0_chargeBar, (velocity) + x0_chargeBar, chargeBar_Width +  y0_chargeBar, 0xF800, 1);

}
