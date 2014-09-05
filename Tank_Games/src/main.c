/*
 *
 *  Created on: 2013-01-22
 *      Author: Gary
 */
#include <stdio.h>
#include "altera_up_sd_card_avalon_interface.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "graphics.h"
#include "tank.h"
#include "bmp_decoder.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "terrain.h"
#include "audio.h"
#include "projectiles.h"
#include "VGA.h"
#include "collision.h"
#include "ps_controller.h"
#include "tank_controls.h"
#include "global.h"
#include "timing.h"


int main()
{
	arr = (unsigned int**)malloc(4377 * sizeof(unsigned int));
	psController ps;
	int velocity = 0;
	int angle = 0;
	long ii = 0;
	int** collisionMatrix;
	int ee;
	int numfiles;
	char * filenames[MAXFILES];

	//init_VGA_Pixel();
	initVGA(); //pixel
	init_VGA_Char();
	av_config_setup();
	numfiles = init_sd(filenames);
	init_wav(arr);
	printf("timing audio\n");
	//time_of_sound = time_audio(arr);
	printf("looping audio\n");

	audio_isr_k = 0;
	alt_irq_register(AUDIO_IRQ, NULL, &audio_isr);
	alt_up_audio_enable_write_interrupt(audio);


	//====INITIALIZE AND DRAW TERRAIN USING BMP ===========================//
		short int fd;

		terrain background;
		fd = alt_up_sd_card_fopen("bg.bmp", false);
		initTerrain(fd, &background);
		drawBackground(background);
		alt_up_sd_card_fclose(fd);


		terrain terrain;
		fd = alt_up_sd_card_fopen("tr.bmp", false);
		initTerrain(fd, &terrain);
		drawTerrain(terrain);
		alt_up_sd_card_fclose(fd);

	//====INITIALIZE AND DRAW USER TANK USING BMP ===========================//
		fd = alt_up_sd_card_fopen("tank.bmp", false);
		tank user;
		initUserTank(&user, fd);
		drawTank(&user, background);
		alt_up_sd_card_fclose(fd);
	//====INITIALIZE AND DRAW ENEMY TANK USING BMP ===========================//
		tank enemy;
		fd = alt_up_sd_card_fopen("tk.bmp", false);
		initEnemyTank1(&enemy, fd);
		drawTank(&enemy,background);
		alt_up_sd_card_fclose(fd);
	//====SWAP BUFFERS =======================================================//
		updateVGA();
		drawBackground(background);
		drawTerrain(terrain);
		drawTank(&enemy, background);
		for(ee=0;ee < 20;ee++)
		{
			moveTank(&user, RIGHT, 1);
			drawTank(&user,background);
			updateVGA();
			drawTank(&user,background);
			//delay();
		}
	//for(ii = 0; ii < 300000; ii++);

	collisionMatrix = init_CollisionMatrix(XMAXSCREEN_RESOLUTION + 1, XMAXSCREEN_RESOLUTION + 1);

	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 120, 340, 240, 0x7E0 , 0); //draw terrain

	set_CollisionAtRectArea(collisionMatrix, 0 , 0, 340, 240, EMPTY);
	set_CollisionAtRectArea(collisionMatrix, 0 , 161, 340, 240, TERRAIN_COLOR);//set the rectangular area drawn by terrain to filled with terrain
	while(1){
		//angle = 0;
		velocity = 0;
		set_UserTank(&angle, &velocity, ps, &user, &enemy, background); //reads controller to set angle, power, and shoot
		shoot_Missile(angle, velocity,( user.x + user.t_VGA.width) - 1 , (240 - user.y + user.t_VGA.height) - 7, collisionMatrix, background);

	}
	clean_audio(arr);
	return 0;
}



