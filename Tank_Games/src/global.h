/*
 * global.h
 *
 *  Created on: Jan 26, 2013
 *      Author: Gary
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_


#define XMAXSCREEN_RESOLUTION 320
#define YMAXSCREEN_RESOLUTION 240
#define PI_ 3.14159
#define EMPTY -2
#define TERRAIN_COLOR 0x7E0

#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"

unsigned int **arr;
static alt_alarm alarm;
static int axis;
int time_of_sound;
int audio_isr_k;
extern alt_up_audio_dev * audio;


#endif /* GLOBAL_H_ */
