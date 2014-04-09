#include "operations.h"
#include "flip.h"
#define DIVIDE_BY_TWO 1

/**@brief flip flips the picture upside-down
 *
 *@arg tr is the structure containing all the information needed 
 *     to perform the transformation
 *@ret will always return 0
 **/
int flip(struct transform *tr)
{
	int x, y;
	int temp;

	for(y = 0; y < tr->bm.bh.height >> DIVIDE_BY_TWO; y++){
		for(x = 0; x < tr->bm.bh.width; x++){
			/* flip green */
			temp = tr->bm.data[tr->bm.bh.height * y + x].green;
			tr->bm.data[tr->bm.bh.height * y + x].green = tr->bm.data[(tr->bm.bh.height * tr->bm.bh.width - tr->bm.bh.width) - (tr->bm.bh.height * y - x)].green;	       
			tr->bm.data[(tr->bm.bh.height * tr->bm.bh.width - tr->bm.bh.width) - (tr->bm.bh.height * y - x)].green = temp;
			
			/* flip blue */
			temp = tr->bm.data[tr->bm.bh.height * y + x].blue;
			tr->bm.data[tr->bm.bh.height * y + x].blue = tr->bm.data[(tr->bm.bh.height * tr->bm.bh.width - tr->bm.bh.width) - (tr->bm.bh.height * y - x)].blue;	       
			tr->bm.data[(tr->bm.bh.height * tr->bm.bh.width - tr->bm.bh.width) - (tr->bm.bh.height * y - x)].blue = temp;
			
			/* flip red */
			temp = tr->bm.data[tr->bm.bh.height * y + x].red;
			tr->bm.data[tr->bm.bh.height * y + x].red = tr->bm.data[(tr->bm.bh.height * tr->bm.bh.width - tr->bm.bh.width) - (tr->bm.bh.height * y - x)].red;	       
			tr->bm.data[(tr->bm.bh.height * tr->bm.bh.width - tr->bm.bh.width) - (tr->bm.bh.height * y - x)].red = temp;
		}
	}
	return 0;
}
			
