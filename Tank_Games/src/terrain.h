/*
 * terrain.h
 *
 *  Created on: 2013-01-26
 *      Author: Jae-Hwan Jung
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "bmp_decoder.h"
#include "graphics.h"

typedef raw_pixels terrain;

void initTerrain(short int fd, terrain* t);

void drawBackground(terrain t);

void drawTerrain(terrain t);

#endif /* TERRAIN_H_ */
