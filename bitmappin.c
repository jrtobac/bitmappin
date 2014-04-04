#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmappin.h"

#define FILE_HDR_SIZE 14
#define BITMAP_HDR_SIZE 40
#define FILE_AND_BITMAP_HDR_SIZE FILE_HDR_SIZE + BITMAP_HDR_SIZE
#define FILE_EXTENSION ".bmp"
#define MAX_PIXEL_VALUE 255

/* global variables for input and output file names */
char g_r_flag = 0, g_g_flag = 0, g_b_flag = 0, g_w_flag = 0;

/* used to parse the command line input */
int parse_input(int argc, char *argv[], struct transform *tr)
{
	extern char *optarg;
	int c, err = 0;
	char i_flag = 0, o_flag = 0;
	char *in_extension, *out_extension;

	static char usage[] = "usage: %s -i infile.bmp [OPTIONS] -o outfile.bmp\n\nOPTIONS:\nw,r,g,b\n";

	/* parse the command line arguments -i for input -o for output */
	while((c = getopt(argc, argv, "wrgbi:o:")) != -1){
		if( c == 'i'){
			tr->infile = optarg;
			i_flag = 1;
		}
		else if(c == 'o'){
			tr->outfile = optarg;
			o_flag = 1;
		}
		else if(c == 'w'){
		  g_w_flag =1;
		}
		else if(c == 'b'){
		  g_b_flag = 1;
		}
		else if(c == 'g'){
		  g_g_flag = 1;
		}
		else if(c == 'r'){
		  g_r_flag = 1;
		}
		else if(c == '?'){
			err = 1;
		}	
		printf("%s\n\n", optarg);
	}
	
	/* check for command line errors */
	if(argc < 5){
		fprintf(stderr, "%s too few arguments\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	else if(i_flag == 0){
		fprintf(stderr, "%s: missing -i option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if(o_flag == 0){
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if ((in_extension = strrchr(tr->infile, '.')) == NULL || (strcmp(in_extension, FILE_EXTENSION) != 0 )){
		fprintf(stderr, "%s: incorrect input file extension\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if ((out_extension = strrchr(tr->outfile, '.')) == NULL || (strcmp(out_extension, FILE_EXTENSION) != 0)){
		fprintf(stderr, "%s: incorrect output file extension\n", argv[0]); 	
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if(err){
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
			
	return 0;
}

/*void print_bitmap_info(struct bitmap *bm)
{
	printf("--- File:%s ---\n", g_infile);
	printf("Magic:%x\n", bm->fh.file_type);
	printf("Size:%d\n", bm->fh.file_size);
	printf("Reserved1:%d\n", bm->fh.res1);
	printf("Reserved2:%d\n", bm->fh.res2);
	printf("Bitmap Offset:%d\n", bm->fh.bitmap_offset);
	printf("\n");
	printf("Bitmap Header Size:%d\n", bm->bh.b_hdr_size);
	printf("Width:%d\n", bm->bh.width);
	printf("Height:%d\n", bm->bh.height);
	printf("Planes:%d\n", bm->bh.planes);
	printf("Bits Per Pixel:%d\n", bm->bh.bpp);
	printf("Compression:%d\n", bm->bh.compression);
	printf("Size of Bitmap:%d\n", bm->bh.size_bitmap);
	printf("Horizontal Resolution:%d\n", bm->bh.hor_res);
	printf("Vertical Resolution:%d\n", bm->bh.vert_res);
	printf("Colors Used:%d\n", bm->bh.cols_used);
	printf("Important Colors:%d\n", bm->bh.cols_imp);
	}*/

void print_bitmap_data(struct bitmap *bm)
{
  int y;
  int x;

	 
  for (y = 0; y < bm->bh.height; y++) {
    for (x = 0; x < bm->bh.width; x++) {
      printf("data[%d][%d]:blue:%x green:%x red:%x\n", x, y, bm->data[bm->bh.height * y + x].blue, bm->data[bm->bh.height * y + x].green, bm->data[bm->bh.height * y + x].red);
    }
  }
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
		if(ret != tr->bm.bh.width){
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
		if (ret != tr->bm.bh.width) {
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

int main(int argc, char *argv[])
{
	struct transform tr;
	int ret;

	tr.bm.data = NULL;

	parse_input(argc, argv, &tr);
	/* print input and output file names */
	read_in_file(&tr);

//	ret = tr.op(&tr);

	/* color picture according to flags */
  	if(g_w_flag == 1){
		color_white(&tr.bm);	  
	}else{ 
		if(g_g_flag == 1){
			max_green(&tr.bm);
		}
		if (g_b_flag == 1){
			max_blue(&tr.bm);
		}
		if (g_r_flag == 1){
			max_red(&tr.bm);
		}
	}

	write_out_file(&tr);

	return 0;
}
