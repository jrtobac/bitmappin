#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "bitmappin.h"
#include "operations.h"
#include "print.h"

/**@brief Parses the command line input and populates tr with the info.
 *
 * @arg argc is the number of command line arguments passed
 * @arg argv is the list of command line arguments passed
 * @arg tr is the structure that will contain all the info necessary
 *      for performing the transformation on the image.
 * @ret Return 0 on success and a negative number on failure.
 */
int parse_input(int argc, char *argv[], struct transform *tr)
{
	extern char *optarg;
	int c, err = 0;
	char i_flag = 0, o_flag = 0;
	char *in_extension, *out_extension;

	static char usage[] = "usage: %s -i infile.bmp [OPTIONS] -o outfile.bmp\n\nOPTIONS:\nw,r,g,b,n\n";

	if(argc < 5){
		fprintf(stderr, "%s too few arguments\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		return -1;
	}

	/* parse the command line arguments -i for input -o for output */
	while((c = getopt(argc, argv, "wfrgbns:i:o:")) != -1){

		if( c == 'i'){
			tr->infile = optarg;
			i_flag = 1;
		}
		else if(c == 'o'){
			tr->outfile = optarg;
			o_flag = 1;
		}
		else if (c == 's') {
			tr->bitplane_slice_num = *optarg - '0';
			if (tr->bitplane_slice_num < 0 ||
			    tr->bitplane_slice_num > 7) {
				fprintf(stderr, 
					"bitplane slice number must be 0-7\n");
				return -1;
			}
			tr->op = bitplane_slice;
		}
		else if(c == 'w'){
			tr->w_flag = 1;
			tr->op = max_color;
		}
		else if(c == 'b') {
			tr->b_flag = 1;
			tr->op = max_color;
		}
		else if(c == 'g'){
			tr->g_flag = 1;
			tr->op = max_color;
		}
		else if(c == 'r'){
			tr->r_flag = 1;
			tr->op = max_color;
		}
		else if(c == 'n'){
			tr->op = image_negative;
		}
		else if(c == 'f'){
			tr->op = flip;
		}
		else if(c == '?'){
			err = 1;
		}	
	}
	
	/* check for command line errors */
	if (i_flag == 0) {
		fprintf(stderr, "%s: missing -i option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		return -1;
	}
	if(o_flag == 0) {
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		return -1;
	} 
	if ((in_extension = strrchr(tr->infile, '.')) == NULL || 
	    (strcmp(in_extension, FILE_EXTENSION) != 0 )) {
		fprintf(stderr, "%s: bad input file extension\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		return -1;
	}
	if ((out_extension = strrchr(tr->outfile, '.')) == NULL ||
	    (strcmp(out_extension, FILE_EXTENSION) != 0)) {
		fprintf(stderr, "%s: bad output file extension\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		return -1;
	}
	if(err) {
		fprintf(stderr, usage, argv[0]);
		return -1;
	}
			
	return 0;
}


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
	tr->bitplane_slice_num = 0;
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
