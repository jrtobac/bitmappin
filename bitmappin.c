#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* global variables for input and output file names */
char *g_infile, *g_outfile;

/* used to parse the command line input */
int parse_input(int argc, char *argv[])
{
#define FILE_EXTENSION ".bmp"
	extern char *optarg;
	int c, err = 0;
	char iflag = 0, oflag = 0;
	char *in_extension, *out_extension;
	static char usage[] = "usage: %s -i infile.bmp -o outfile.bmp\n";

	/* parse the command line arguments -i for input -o for output */
	while((c = getopt(argc, argv, "i:o:")) != -1){
		if( c == 'i'){
			g_infile = optarg;
			iflag = 1;
		}
		else if(c == 'o'){
			g_outfile = optarg;
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

int read_in_file()
{


	return 0;
}

int main(int argc, char *argv[])
{

	parse_input(argc, argv);
	/* print input and output file names */
	printf("Input: %s\nOutput: %s\n", g_infile, g_outfile);
	read_in_file();
	return 0;
}
