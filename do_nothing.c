#include <stdio.h>
#include "operations.h"

/**@brief do_nothing does nothing to the input image.
 *
 * This is the default operation to be performed on the input image.  If
 * the user does not specify an operation, the input is simply passed
 * on to the output unaltered.  A message is printed to the output to
 * let the user know nothing has changed.
 *
 * @arg tr is the transform struct that holds the information
 * @return returns 0 always.
 */
int do_nothing(struct transform *tr)
{
	if(tr->log_level > 0){
		printf("\nCopying input to output\n\n");
	}

	fprintf(stdout, "No operation specified. %s copied to %s.\n",
		tr->infile, tr->outfile);
	return 0;
}
	
