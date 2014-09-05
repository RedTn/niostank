/*
 * audio.h
 *
 *  Created on: 2013-01-28
 *      Author: kidax
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include <altera_up_sd_card_avalon_interface.h>
#include <string.h>
#include <math.h>
#include "global.h"

#define MAXFILES 20
#define AV_CONFIG "/dev/audio_and_video_config"
#define AUDIO_DEV "/dev/audio"


void av_config_setup();
int init_sd(char * filenames[]);
void play_wav(unsigned int **arr);
void init_wav(unsigned int *arr[]);
void clean_audio(unsigned int *arr[]);
int time_audio(unsigned int **arr);

#endif /* AUDIO_H_ */
