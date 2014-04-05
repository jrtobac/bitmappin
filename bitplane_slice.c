#include "operations.h"

/**@brief bitplane_slice slices out a bitplane specified in
 *        tr->bitplane_slice_num.  
 *
 * This function slices out the bitplane for red, green, and blue,
 * so if bitplace_slice_num == 1, the second least significant bitplane
 * for every color will be passed through to the output.
 *
 * @arg tr is the struct with info about which bitplane to slice.
 * @ret returns 0
 */
int bitplane_slice(struct transform *tr)
{
	int i;
	int num_pixels = tr->bm.bh.height * tr->bm.bh.width;
	char slice_mask = 1 << tr->bitplane_slice_num;

	for (i = 0; i < num_pixels; i++) {
		tr->bm.data[i].blue &= slice_mask;
		tr->bm.data[i].green &= slice_mask;
		tr->bm.data[i].red &= slice_mask;
	}
	
	return 0;
}
