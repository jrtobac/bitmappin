#include "operations.h"
#include "image_negative.h"

/**@brief image negative creates a negative of the image.
 *
 *@arg tr is the structure containing all the information needed to 
 *    perform the transformation.
 *
 *Max value - pixel value  == image negative of pixel
 *
 *@ret will always return 0
 *
 **/

int image_negative(struct transform *tr)
{
	int x, y;

	if(tr->log_level > 0){
		printf("\nTransforming image into negative\n\n");
	}
			
	for(y = 0; y < tr->bm.bh.height; y++){
		for(x=0; x< tr->bm.bh.width; x++){
			tr->bm.data[tr->bm.bh.height * y + x].green = MAX_PIXEL_VALUE - tr->bm.data[tr->bm.bh.height * y + x].green;
			tr->bm.data[tr->bm.bh.height * y + x].blue = MAX_PIXEL_VALUE - tr->bm.data[tr->bm.bh.height * y + x].blue;
			tr->bm.data[tr->bm.bh.height * y + x].red = MAX_PIXEL_VALUE - tr->bm.data[tr->bm.bh.height * y + x].red;

      		}
	}
	return 0;
}
			
