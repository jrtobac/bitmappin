#include "operations.h"
#include "thresholding.h"


/**@brief Thresholding performs thresholding on only the specified pixels at their corresponding values
 *
 * This function looks at the image pixel by pixel to see if the red, green, or blue has been set for thresholding
 * If the input pixel values are greater than their specified end values, the pixel is set to 255
 * If the input pixel values are less than their specified start values, tha pixel is to zero
 * All pixels within the specified range and all non-set colors are not changed
 *
 *@args tr is a pointer to the struct transform which hold all the information for the function
 *
 *@ret returns zero if it has completed successfully
 */
int thresholding(struct transform *tr){
	
	int x, y;
	
	for( y = 0; y < tr->bm.bh.height; y++){
		for( x = 0; x < tr->bm.bh.width; x++){
			if(tr->r_set){
				if(tr->bm.data[y * tr->bm.bh.width + x].red < tr->r_start){
					tr->bm.data[y * tr->bm.bh.width + x].red = MIN_PIXEL_VALUE;
				}
				else if(tr->bm.data[y * tr->bm.bh.width + x].red > tr->r_end){
					tr->bm.data[y * tr->bm.bh.width + x].red = MAX_PIXEL_VALUE;
				}
			}

			if(tr->g_set){
				if(tr->bm.data[y * tr->bm.bh.width + x].green < tr->g_start){
					tr->bm.data[y * tr->bm.bh.width + x].green = MIN_PIXEL_VALUE;
				}
				else if(tr->bm.data[y * tr->bm.bh.width + x].green > tr->g_end){
					tr->bm.data[y * tr->bm.bh.width + x].green = MAX_PIXEL_VALUE;
				}
			}

			if(tr->b_set){
				if(tr->bm.data[y * tr->bm.bh.width + x].blue < tr->b_start){
					tr->bm.data[y * tr->bm.bh.width + x].blue = MIN_PIXEL_VALUE;
				}
				else if(tr->bm.data[y * tr->bm.bh.width + x].blue > tr->b_end){
					tr->bm.data[y * tr->bm.bh.width + x].blue = MAX_PIXEL_VALUE;
				}
			}
		}
	}
	
	if(tr->log_level > 0){
		printf("\nperforming thresholding\n");
	}
	if(tr->log_level == 2){
		if(tr->r_set){
			printf("thresholding red from %d - %d\n", tr->r_start, tr->r_end);
		}
		if(tr->g_set){
			printf("thresholding green from %d - %d\n", tr->g_start, tr->g_end);
		}
		if(tr->r_set){
			printf("thresholding blue from %d - %d\n", tr->b_start, tr->b_end);
		}
	}

	
	return 0;
}
