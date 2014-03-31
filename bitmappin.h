#ifndef _BITMAPPIN_H_
#define _BITMAPPIN_H_

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
	unsigned int width;
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

struct bitmap {
	struct file_header fh;
	struct bitmap_header bh;
	struct pixel **data;
};


#endif //_BITMAPPIN_H_
