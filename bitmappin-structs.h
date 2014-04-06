#ifndef _BITMAPPIN_STRUCTS_H_
#define _BITMAPPIN_STRUCTS_H_

#define FILE_HDR_SIZE 14
#define BITMAP_HDR_SIZE 40
#define FILE_AND_BITMAP_HDR_SIZE FILE_HDR_SIZE + BITMAP_HDR_SIZE
#define FILE_EXTENSION ".bmp"
#define MAX_PIXEL_VALUE 255

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
        int (*op)(struct transform *); //Operation to be performed
	
	/* Other arguments should go below here */
	char i_flag; //Was an input file provided?
	char o_flag; //Was an output file provided?
	char r_flag; //Max red in image
	char g_flag; //Max green in image
	char b_flag; //Max blue in image
	char w_flag; //Turn image white
	char bitplane_slice_num; //Which bitplane to slice
};

#endif //_BITMAPPIN_STRUCTS_H_
