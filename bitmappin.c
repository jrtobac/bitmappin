#include <stdio.h>
#include <stdlib.h>

/* global variables for input and output file names */
char *g_infile, *g_outfile;

/* used to parse the command line input */
int parse_input(int argc, char *argv[])
{
	extern char *optarg;

	int c, err = 0;
	char iflag = 0, oflag = 0;

	static char usage[] = "usage: %s -i infile -o outfile\n";

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
	if(iflag == 0){
		fprintf(stderr, "%s: missing -i option\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}else if(oflag == 0){
		fprintf(stderr, "%s: missing -o option\n", argv[0]);
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
