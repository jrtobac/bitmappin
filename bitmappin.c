#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmappin.h"

#define FILE_HDR_SIZE 14
#define BITMAP_HDR_SIZE 40
#define FILE_AND_BITMAP_HDR_SIZE FILE_HDR_SIZE + BITMAP_HDR_SIZE
#define FILE_EXTENSION ".bmp"

/* used to parse the command line input */
int parse_input(int argc, char *argv[], struct transform *tr)
{
	extern char *optarg;
	int c, err = 0;
	char iflag = 0, oflag = 0;
	char *in_extension, *out_extension;
	static char usage[] = "usage: %s -i infile.bmp -o outfile.bmp\n";
	/* parse the command line arguments -i for input -o for output */
	while((c = getopt(argc, argv, "i:o:")) != -1){
		if( c == 'i'){
			tr->infile = optarg;
			iflag = 1;
		}
		else if(c == 'o'){
			tr->outfile = optarg;
			oflag = 1;
		}
		else if(c == '?'){
			err = 1;
		}	
	}
	
	/* check for command line errors */
	if(argc < 5){
		fprintf(stderr, "%s too few arguments\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	else if(iflag == 0){
		fprintf(stderr, "%s: missing -i option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if(oflag == 0){
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if ((in_extension = strchr(g_infile, '.')) == NULL || (strcmp(in_extension, FILE_EXTENSION) != 0 )){
		fprintf(stderr, "%s: incorrect input file extension\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if ((out_extension = strchr(g_outfile, '.')) == NULL || (strcmp(out_extension, FILE_EXTENSION) != 0)){
		fprintf(stderr, "%s: incorrect output file extension\n", argv[0]); 	
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if(err){
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
			
	return 0;
}

void print_bitmap_info(struct bitmap *bm)
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
}

void print_bitmap_data(struct bitmap *bm)
{
	int i;
	int j;

	for (i = 0; i < bm->bh.height; i++) {
		for (j = 0; j < bm->bh.width; j++) {
			printf("data[%d][%d]:blue:%x green:%x red:%x\n", i, j, bm->data[i][j].blue, bm->data[i][j].green, bm->data[i][j].red);
		}
	}
}

int read_in_file(struct bitmap *bm)
{
	FILE *in_file;
	size_t ret;
	int i;

	in_file = fopen(g_infile, "r");
	if (!in_file) {
		fprintf(stderr, "Error opening input file: %s\n", g_infile);
		exit(-1);
	}

	/* Read in the headers of the file */
	ret = fread(&bm->fh.file_type, FILE_AND_BITMAP_HDR_SIZE, 1, in_file);
	if (ret != 1) {
		fprintf(stderr, "Failed header read\n");
		exit(-2);
	}

	print_bitmap_info(bm);

	//TODO SEEK to start of data

	bm->data = (struct pixel **)
		malloc(sizeof(struct pixel *) * bm->bh.height);
	if (bm->data == NULL) {
		fprintf(stderr, "Failed to malloc rows\n");
		exit(-3);
	}
	
	for (i = 0; i < bm->bh.height; i++) {
		bm->data[i] = (struct pixel *)
			malloc(sizeof(struct pixel) * bm->bh.width);
		if (bm->data[i] == NULL) {
			fprintf(stderr, "Failed to malloc col\n");
			exit(-4);
		}
	}

	for (i = 0; i < bm->bh.height; i++) {
		ret = fread(bm->data[i], sizeof(struct pixel), 
			    bm->bh.width, in_file);
		if (ret != bm->bh.width) {
			fprintf(stderr, "Failed to read bitmap data\n");
			exit(-5);
		}
	}

//	print_bitmap_data(bm);

	return 0;
}

int write_out_file(struct bitmap *bm)
{
	FILE *out_file;
	size_t ret;
	int i;

	out_file = fopen(g_outfile, "w");
	if (!out_file) {
		fprintf(stderr, "Failed to open outfile:%s\n", g_outfile);
		exit(-6);
	}

	ret = fwrite(&bm->fh.file_type, FILE_AND_BITMAP_HDR_SIZE, 1, out_file);
	if (ret != 1) {
		fprintf(stderr, "Failed to write file headers\n");
		exit(-7);
	}
	
	for (i = 0; i < bm->bh.height; i++) {
		ret = fwrite(bm->data[i], sizeof(struct pixel), 
			     bm->bh.width, out_file);
		if (ret != bm->bh.width) {
			fprintf(stderr, "Failed writing data to file\n");
			exit(-8);
		}
	}
	return 0;
	//TODO Close output file and input file and free malloc'ed stuff
}

int main(int argc, char *argv[])
{
	struct transform tr;
	parse_input(argc, argv, &tr);
	/* print input and output file names */
	printf("Input: %s\nOutput: %s\n", g_infile, g_outfile);
	read_in_file(&bm);

	tr->op(tr);


	op = max_something;

	write_out_file(&bm);
	return 0;
}
