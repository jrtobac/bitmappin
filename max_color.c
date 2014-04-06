#include "operations.h"
#include "max_color.h"

/**@brief max_color maxes out specified colors in the input image.
 *
 * Depending on which values were specified at the command line, 
 * this function will max some combination of red, green, and blue.
 * This can tint the image or even turn it completely white.
 *
 * @arg tr is the structure containing all the information needed
 *      to perform the transformation.
 * @ret Will always return 0;
 */
int max_color(struct transform *tr)
{
	if (tr->w_flag) {
		color_white(&tr->bm);
		return 0;
	}

	if (tr->r_flag) max_red(&tr->bm);
	if (tr->b_flag) max_blue(&tr->bm);
	if (tr->g_flag) max_green(&tr->bm);
	
	return 0;
}

/* max out green pixel values */
void max_green(struct bitmap *bm)
{
	int x, y;
  
	for ( y = 0; y < bm->bh.height; y++){
		for ( x = 0; x < bm->bh.width; x++){
			bm->data[bm->bh.height * y + x].green =  MAX_PIXEL_VALUE;
		}
	}
}


/* max out blue pixel values */
void max_blue(struct bitmap *bm)
{
	int x, y;

	for ( y = 0; y < bm->bh.height; y++){
		for ( x = 0; x < bm->bh.width; x++){
			bm->data[bm->bh.height * y + x].blue =  MAX_PIXEL_VALUE;
		}
	}

}

/* max out red pixel values */
void max_red(struct bitmap *bm)
{
	int x, y;

	for ( y = 0; y < bm->bh.height; y++){
		for ( x = 0; x < bm->bh.width; x++){
			bm->data[bm->bh.height * y + x].red =  MAX_PIXEL_VALUE;
		}
	}
}

/* max out all rgb pixel values */
void color_white(struct bitmap *bm)
{
	max_green(bm);
	max_blue(bm);
	max_red(bm);
}
