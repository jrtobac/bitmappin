#include "bitmappin.h"
#include "print.h"

void print_bitmap_info(struct transform *tr)
{
	printf("--- File:%s ---\n", tr->infile);
	printf("Magic:%x\n", tr->bm.fh.file_type);
	printf("Size:%d\n", tr->bm.fh.file_size);
	printf("Reserved1:%d\n", tr->bm.fh.res1);
	printf("Reserved2:%d\n", tr->bm.fh.res2);
	printf("Bitmap Offset:%d\n", tr->bm.fh.bitmap_offset);
	printf("\n");
	printf("Bitmap Header Size:%d\n", tr->bm.bh.b_hdr_size);
	printf("Width:%d\n", tr->bm.bh.width);
	printf("Height:%d\n", tr->bm.bh.height);
	printf("Planes:%d\n", tr->bm.bh.planes);
	printf("Bits Per Pixel:%d\n", tr->bm.bh.bpp);
	printf("Compression:%d\n", tr->bm.bh.compression);
	printf("Size of Bitmap:%d\n", tr->bm.bh.size_bitmap);
	printf("Horizontal Resolution:%d\n", tr->bm.bh.hor_res);
	printf("Vertical Resolution:%d\n", tr->bm.bh.vert_res);
	printf("Colors Used:%d\n", tr->bm.bh.cols_used);
	printf("Important Colors:%d\n", tr->bm.bh.cols_imp);
}

void print_bitmap_data(struct transform *tr)
{
	int y;
	int x;

	for (y = 0; y < tr->bm.bh.height; y++) {
		for (x = 0; x < tr->bm.bh.width; x++) {
			printf("data[%d][%d]:blue:%x green:%x red:%x\n", x, y, tr->bm.data[tr->bm.bh.height * y + x].blue, tr->bm.data[tr->bm.bh.height * y + x].green, tr->bm.data[tr->bm.bh.height * y + x].red);
		}
	}
}
