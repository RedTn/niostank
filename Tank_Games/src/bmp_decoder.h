/*
 * bmp_decoder.h
 *
 *  Created on: 2013-01-23
 *      Author: Jae-Hwan Jung
 */

#ifndef BMP_DECODER_H_
#define BMP_DECODER_H_

#include <stdio.h>
#include <string.h>

#include "altera_up_sd_card_avalon_interface.h"
#include "math.h"

/* Reads up to size bytes into the buffer, returning number of bytes read */
typedef int (*read_func)(void* buffer, int size);

enum color_component {
    BLUE,
    GREEN,
    RED
};

#define PALETTE_ENTRY_SIZE   4
#define MAX_PALETTE_SIZE     256
#define HEADER_SIZE 54
typedef unsigned char palette[MAX_PALETTE_SIZE][PALETTE_ENTRY_SIZE];
typedef unsigned short* raw_pixels;

enum compression_values {
    UNCOMPRESSED = 0,

};
struct bmp_file_header {
	//file header
    unsigned short   type;
    unsigned long    size;
    unsigned long    reserved;
    unsigned long    bitmap_offset;
    //information header
    unsigned long    header_size;
    signed   long    width;
    signed   long    height;
    unsigned short   planes;
    unsigned short   bits_per_pixel;
    unsigned long    compression;
    unsigned long    bitmap_size;
    signed   long    horizontal_resolution;
    signed   long    vertical_resolution;
    unsigned long    num_colors;
    unsigned long    num_important_colors;
};



//Requires: nothing
//Modifies: fd
//Effects: decode a bmp file and stores the array in output
void decode_Bmp(short int bmp_File_Descriptor, raw_pixels* output, int* width, int* height);


#endif /* BMP_DECODER_H_ */
