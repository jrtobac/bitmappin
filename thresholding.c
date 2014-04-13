#include "operations.h"
#include "thresholding.h"


/**@brief thresholding performs thresholding at a specified pixel for red green and blue
 *
 * This function looks at the image pixel by pixel to see if the red, green, or blue values are greater than or less than the threshold.
 * if they are less than the threshold, that pixel and color value are set to zero, otherwise they are set to 255
 *
 *@args tr is a pointer to the struct transform which hold all the information for the function
 *
 *@ret returns zero if it has completed successfully
 */
int thresholding(struct transform *tr){
	
	int x, y;
	
	for( y = 0; y < tr->bm.bh.height; y++){
		for( x = 0; x < tr->bm.bh.width; x++){
			
			if( tr->bm.data[y*tr->bm.bh.width + x].green < tr->thresholding_num){
				tr->bm.data[y*tr->bm.bh.width + x].green = MIN_PIXEL_VALUE;
			}
			else{
				tr->bm.data[y*tr->bm.bh.width + x].green = MAX_PIXEL_VALUE;
			}
			
			if( tr->bm.data[y*tr->bm.bh.width + x].blue < tr->thresholding_num){
				tr->bm.data[y*tr->bm.bh.width + x].blue = MIN_PIXEL_VALUE;
			}
			else{
				tr->bm.data[y*tr->bm.bh.width + x].blue = MAX_PIXEL_VALUE;
			}
			
			if( tr->bm.data[y*tr->bm.bh.width + x].red < tr->thresholding_num){
				tr->bm.data[y*tr->bm.bh.width + x].red = MIN_PIXEL_VALUE;
			}
			else{
				tr->bm.data[y*tr->bm.bh.width + x].red = MAX_PIXEL_VALUE;
			}
			
		}
	}
	
	return 0;
}
