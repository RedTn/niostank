

#include "bmp_decoder.h"

static short int fd;

#define fix_endian(x)  (fix_endian_func(&(x), sizeof(x)))

//Requires: nothing
//Modifies: fd
//Effects : sets the file descriptor of the bmp file to be read
static void init_Bmp(short int bmp_file_descriptor)
{
	fd = bmp_file_descriptor;
}

//Requires: file has been read and file descriptor has been assigned
//Modifies: buffer
//Effects : Reads up to size bytes into the buffer, returning number of bytes read
static int read_File(void* buffer, int size)
{
	if (size == 1)
	{
		unsigned char LSB = 0;
		 LSB = alt_up_sd_card_read(fd);
		 *(unsigned char*)buffer = LSB;
	}
	else if (size == 2)
	{
		unsigned short LSB = 0;
		LSB = alt_up_sd_card_read(fd);
		LSB = LSB;
		unsigned short MSB = 0;
		MSB = alt_up_sd_card_read(fd);
		MSB = MSB << 8;
		unsigned int result = LSB | MSB;
		*(unsigned short*)buffer = result;
	}
	else if (size == 4)
	{
		unsigned long LSB = 0;
		LSB = alt_up_sd_card_read(fd);
		LSB = LSB;
		unsigned long LSB2 = 0;
		LSB2 = alt_up_sd_card_read(fd);
		LSB2 = LSB2 << 8;
		unsigned long MSB2 = 0;
		MSB2 = alt_up_sd_card_read(fd);
		MSB2 = MSB2 << 16;
		unsigned long MSB = 0;
		MSB = alt_up_sd_card_read(fd);
		MSB = MSB << 24;
		*(unsigned long*)buffer = LSB | LSB2 | MSB2 | MSB;
	}
	else
		return 0;
	return size;
}

static void fix_endian_func(void* data, int size) {
    int endian_test = 1;
    unsigned char* endian_test_bytes = (unsigned char *)&endian_test;
    if (endian_test_bytes[0] == '\0') {
        unsigned char* cdata = data;
        int i;
        for (i=0; i<size/2; i++) {
            unsigned char temp = cdata[i];
            cdata[i] = cdata[size-1 - i];
            cdata[size-1 - i] = temp;
        }
    }
}

static unsigned char read_u8(read_func read) {
    unsigned char result;
    if (read(&result, 1) < 1) {
        printf("Unexpected end of file");
        exit(-1);
    }
    return result;
}
static unsigned short read_u16(read_func read) {
    unsigned short result = 0;
    if (read(&result, 2) < 2) {
        printf("Unexpected end of file");
        exit(-1);
    }
    fix_endian(result);
    return result;
}

static unsigned long read_u32(read_func read) {
    unsigned long result = 0;
    if (read(&result, 4) < 4) {
        printf("Unexpected end of file");
        exit(-1);
    }
    fix_endian(result);
    return result;
}
static signed long read_s32(read_func read) {
    unsigned long result = 0;
    if (read(&result, 4) < 4) {
        printf("Unexpected end of file");
        exit(-1);
    }
    fix_endian(result);
    if ((result >> 31) & 1) { /* If it's negative... */
        result |= ((unsigned long)(-1)) << 31;
    }
    return (long)result;
}
static void read_bitmap_header(read_func in, struct bmp_file_header* header) {
	header->type                  = 0;
	header->size                  = 0;
    header->reserved              = 0;
	header->bitmap_offset         = 0;
	header->header_size           = 0;
	header->width                 = 0;
    header->height                = 0;
    header->planes                = 0;
    header->bits_per_pixel        = 0;
    header->compression           = 0;
    header->bitmap_size           = 0;
    header->horizontal_resolution = 0;
    header->vertical_resolution   = 0;
    header->num_colors            = 0;
    header->num_important_colors  = 0;

    //the header field used to identify the BMP & DIB file
    //big endian. not little-endian.
    header->type                  = read_u16(in);
    //the size of the BMP file in bytes
    header->size                  = read_u32(in);
    //reserved => application dependent. ignore.
    header->reserved              = read_u32(in);
    /*the offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.*/
    header->bitmap_offset         = read_u32(in);
    /*the size of this header (40 bytes)*/
    header->header_size           = read_u32(in);
    /*the bitmap width in pixels (signed integer).*/
    header->width                 = read_s32(in);
    /*the bitmap height in pixels (signed integer).*/
    header->height                = read_s32(in);
    /*the number of color planes being used. Must be set to 1.*/
    header->planes                = read_u16(in);
    /*the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.*/
    header->bits_per_pixel        = read_u16(in);
    /*the compression method being used. See the next table for a list of possible values.*/
    header->compression           = read_u32(in);
    /*the image size. This is the size of the raw bitmap data (see below), and should not be confused with the file size.*/
    header->bitmap_size           = read_u32(in);
    /*the horizontal resolution of the image. (pixel per meter, signed integer)*/
    header->horizontal_resolution = read_s32(in);
    /*the vertical resolution of the image. (pixel per meter, signed integer)*/
    header->vertical_resolution   = read_u32(in);
    /*the number of colors in the color palette, or 0 to default to 2n.*/
    header->num_colors            = read_u32(in);
    /*the number of important colors used, or 0 when every color is important; generally ignored.*/
    header->num_important_colors  = read_u32(in);
}

//Effects: returns the number of palette entries by simply dividing the size by the size of a palette entry
static int read_palette(read_func in, struct bmp_file_header* header, palette palett)
{
	//color table not used when bpp is 16-bit or higher
    int palette_size = header->bitmap_offset - HEADER_SIZE;
    if (palette_size != 0)
    	printf("unexpected color palette");
    /*
    if (in(&palette[0][0], palette_size) < palette_size) {
        printf("Unexpected end of file");
        exit(-1);
    }
    */
    return palette_size/PALETTE_ENTRY_SIZE;
}

static void convert_pixel_data(read_func in, struct bmp_file_header* header, raw_pixels* output)
{
	//The bits representing the bitmap pixels are packed in rows.
	//The size of each row is rounded up to a multiple of 4 bytes (a 32-bit DWORD) by padding.
	//For images with height > 1, multiple padded rows are stored consecutively, forming a Pixel Array.
	//The total number of bytes necessary to store one row of pixels can be calculated as:
	//RowSize = rounddown((BitsPerPixel*ImageWidth+31)/32)*4
	//PixelArraySize = RowSize*abs(ImageHeight)
	int RowSize = ((header->bits_per_pixel*header->width+31)/32)*4;
	int PixelArraySize = RowSize*abs(header->height);
    *output = (unsigned short*)malloc(PixelArraySize);

    //each pixel array is a block of 32-bit DWORD
    //normally pixels are stored "upside-down"
    //Padding bytes must be appended to the end of the rows in order to bring up the length of the rows to a multiple of four bytes
    int numOfPixels = 0;
    numOfPixels = header->width*header->height;
    int i;

    for(i=0;i<numOfPixels;i++)
    {
    	//NOTE: for some reason Red and Green bits need to be shift to the left by 1 bit. Blue stays the stays the same.
    	unsigned short pixel_color;
    	pixel_color = read_u16(in);
    	unsigned short rg = 0;
    	rg = pixel_color & (unsigned short)0xFFE0;
    	unsigned short b = 0;
    	b = pixel_color & (unsigned short)0x1F;
    	rg = rg << 1;
    	pixel_color = rg | b;
    	(*output)[i] = pixel_color;

    }

}

//Requires: nothing
//Modifies: fd
//Effects: decode a bmp file and returns its array
void decode_Bmp(short int bmp_File_Descriptor, raw_pixels* output, int* width, int* height)
{
	//File descriptor of the opened file

	struct bmp_file_header header;
	palette palett;

	init_Bmp(bmp_File_Descriptor);

	read_bitmap_header(read_File, &header);
	read_palette(read_File, &header, palett);
	convert_pixel_data(read_File, &header, output);
	*width = header.width;
	*height = header.height;
}


