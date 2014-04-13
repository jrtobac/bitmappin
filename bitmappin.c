#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "bitmappin.h"
#include "operations.h"
#include "print.h"
#include "help.h"
#include "bitmappin-structs.h"
#include "parse.h"

/**@brief read_in_file reads in a bitmap file specified by tr->infile and 
 * populates the rest of the tr->bm structure.
 *
 * @arg tr is the struct that contains both the input filename and the
 *      tr->bm bitmap struct that will be populated.
 * @ret returns 0 on success and a negative number on error
 */
int read_in_file(struct transform *tr)
{
	FILE *in_file = NULL;
	size_t ret;
	int r;
	int i;

	in_file = fopen(tr->infile, "r");
	if (!in_file) {
		fprintf(stderr, "Error opening input file: %s\n", tr->infile);
		return -1;
	}

	/* Read in the headers of the file */
	ret = fread(&tr->bm.fh.file_type, FILE_AND_BITMAP_HDR_SIZE, 1, in_file);
	if (ret != 1) {
		fprintf(stderr, "Failed header read\n");
		goto out;
	}

	r = fseek(in_file, tr->bm.fh.bitmap_offset, SEEK_SET);
	if (r) {
		fprintf(stderr, "Failed fseek\n");
		goto out;
	}

	/* malloc bitmap data */
	tr->bm.data = (struct pixel *) malloc(sizeof(struct pixel) * tr->bm.bh.height * tr->bm.bh.width);
	if(tr->bm.data == NULL){
		fprintf(stderr, "Failed to malloc bitmap data\n");
		goto out;
	}
	
	/* read in bitmap data */
	for(i = 0; i < tr->bm.bh.height * tr->bm.bh.width; i+=tr->bm.bh.width){
		ret = fread(&tr->bm.data[i], sizeof(struct pixel), tr->bm.bh.width, in_file);
		if(ret != (size_t)tr->bm.bh.width){
			fprintf(stderr, "Failed to read bitmap data\n");
			goto out;
		}
	}
	
	fclose(in_file);
	return 0;

  out:
	if (in_file) fclose(in_file);
	if (tr->bm.data) {
		free(tr->bm.data);
		tr->bm.data = NULL;
	}
	return -1;
}


/**@brief write_out_file writes the contents of the bitmap structure
 *        in tr->bm to the file specified by tr->outfile.
 *
 * @arg tr is the structure containing the bitmap and the outfile name
 * @return returns 0 on success and a negative number on error
 */
int write_out_file(struct transform *tr)
{
	FILE *out_file;
	size_t ret;
	int i;
	
	/* open file and write header */
	out_file = fopen(tr->outfile, "w");
	if (!out_file) {
		fprintf(stderr, "Failed to open outfile:%s\n", tr->outfile);
		goto out;
	}

	ret = fwrite(&tr->bm.fh.file_type, FILE_AND_BITMAP_HDR_SIZE, 1, out_file);
	if (ret != 1) {
		fprintf(stderr, "Failed to write file headers\n");
		goto out;
	}

	/* write the bitmap data */
	for (i = 0; i < tr->bm.bh.height * tr->bm.bh.width; i+=tr->bm.bh.width) {
		ret = fwrite(&tr->bm.data[i], sizeof(struct pixel), tr->bm.bh.width, out_file);
		if (ret != (size_t)tr->bm.bh.width) {
			fprintf(stderr, "Failed writing data to file\n");
			goto out;
		}
	}

	fclose(out_file);
	free(tr->bm.data);
	tr->bm.data = NULL;
	return 0;

  out:
	fclose(out_file);
	free(tr->bm.data);
	tr->bm.data = NULL;
	return -1;
}

/**@brief Initialize the members of tr to be NULL or 0 where applicable.
 *
 * @arg tr is the struct we are initializing
 * @ret return type is void.
 */
void init_transform(struct transform *tr)
{
	tr->bm.data = NULL;
	tr->infile = NULL;
	tr->outfile = NULL;
	tr->op = do_nothing;
	tr->i_flag = 0;
	tr->o_flag = 0;
	tr->r_flag = 0;
	tr->g_flag = 0;
	tr->b_flag = 0;
	tr->w_flag = 0;
	tr->r_bps_mask = 0;
	tr->g_bps_mask = 0;
	tr->b_bps_mask = 0;
}


int main(int argc, char *argv[])
{
	struct transform tr;
	int ret;
	
	init_transform(&tr);

	ret = parse_input(argc, argv, &tr);
	if (ret < 0) return -1;

	ret = read_in_file(&tr);
	if (ret < 0) return -1;
		

	ret = tr.op(&tr);
	if (ret < 0) {
		fprintf(stderr, "Transformation failed, no output generated.");
		return -1;
	}

	ret = write_out_file(&tr);
	if (ret < 0) return -1;

	return 0;
}
