#ifndef _BITMAPPIN_STRUCTS_H_
#define _BITMAPPIN_STRUCTS_H_

#include <stdio.h>

#define FILE_HDR_SIZE 14
#define BITMAP_HDR_SIZE 40
#define FILE_AND_BITMAP_HDR_SIZE FILE_HDR_SIZE + BITMAP_HDR_SIZE
#define FILE_EXTENSION ".bmp"
#define MAX_PIXEL_VALUE 255
#define MIN_PIXEL_VALUE 0

typedef int bool;
#define false 0
#define true 1

struct file_header {
	unsigned short padding;
	unsigned short file_type;
	unsigned int file_size;
	unsigned short res1;
	unsigned short res2;
	unsigned int bitmap_offset;
};

struct bitmap_header {
	unsigned int b_hdr_size;
	int width;
	int height;
	unsigned short planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int size_bitmap;
	unsigned int hor_res;
	unsigned int vert_res;
	unsigned int cols_used;
	unsigned int cols_imp;
};

struct pixel {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

/* struct bitmap holds the headers and data read in from a bitmap */
struct bitmap {
	struct file_header fh;
	struct bitmap_header bh;
  	struct pixel *data;
};

/* struct transform is a big meta structure for keeping track of
 * a single run of the bitmappin program. */
struct transform {
	struct bitmap bm; //Working image
	char *infile;
	char *outfile;
	char *helpfile;
        int (*op)(struct transform *); //Operation to be performed
	
	/* Other arguments should go below here */
	char i_flag; //Was an input file provided?
	char o_flag; //Was an output file provided?
	char r_flag; //Max red in image
	char g_flag; //Max green in image
	char b_flag; //Max blue in image
	char w_flag; //Turn image white
	unsigned char r_bps_mask;
	unsigned char g_bps_mask;
	unsigned char b_bps_mask;
	char bitplane_slice_num; //Which bitplane to slice
	int thresholding_num; //Number to perform thresholding on

	bool r_to_set;
	bool g_to_set;
	bool b_to_set;
	bool r_set;
	bool g_set;
	bool b_set;

	unsigned int r_start;
	unsigned int g_start;
	unsigned int b_start;
	unsigned int r_end;
	unsigned int g_end;
	unsigned int b_end;
	
	unsigned char log_level;
};

#endif //_BITMAPPIN_STRUCTS_H_
