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

	if(tr->log_level > 0){
		printf("\nPerforming bitplance slicing\n");
	}
	if(tr->log_level == 2){
		printf("Slicing blue with mask %d\n", tr->b_bps_mask);
		printf("Slicing green with mask %d\n", tr->g_bps_mask);
		printf("Slicing red with mask %d\n", tr->r_bps_mask);
	}

	for (i = 0; i < num_pixels; i++) {
		tr->bm.data[i].blue &= tr->b_bps_mask;
		tr->bm.data[i].green &= tr->g_bps_mask;
		tr->bm.data[i].red &= tr->r_bps_mask;
	}
	
	return 0;
}
