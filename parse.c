#include "parse-int.h"
#include "bitmappin-structs.h"

int parse_bps_color(unsigned char *mask, char *optarg)
{
	unsigned char start = 0;
	unsigned char end = 0;

	while ((*optarg >= '0' && *optarg <= '7') || *optarg == '-') {
		if (*optarg == '-') {
			end = (*(optarg + 1)) - '0';
			if (!(end <= 7)) {
				fprintf(stderr, "Bad bitplane slice argument");
				return -1;
			}

			for (; start <= end; start++) {
				*mask |= 1 << start;
			}

		} else if (*(optarg + 1) != '-') {
			(*mask) |= (1 << (*optarg - '0'));
		} else {
			start = *optarg - '0';
		}
		optarg++;
	}
	return 0;
}

int parse_bitplane_slice(struct transform *tr, char *optarg)
{
	int ret = 0;
	tr->op = bitplane_slice;	
	if (!*optarg) {
		fprintf(stderr, "Warning: No bitplanes specified; Output will be white.\n");
		return 0;
	}
	if (*optarg >= '0' && *optarg <= '7') {
		ret = parse_bps_color(&tr->r_bps_mask, optarg);
		ret &= parse_bps_color(&tr->g_bps_mask, optarg);
		ret &= parse_bps_color(&tr->b_bps_mask, optarg);
		return ret;
	}
	
	while (*optarg) {
		if (*optarg == 'r' || *optarg == 'R') {
			ret = parse_bps_color(&tr->r_bps_mask, ++optarg);
		} else if (*optarg == 'g' || *optarg == 'G') {
			ret = parse_bps_color(&tr->g_bps_mask, ++optarg);
		} else if (*optarg == 'b' || *optarg == 'B') {
			ret = parse_bps_color(&tr->b_bps_mask, ++optarg);
		} else if (*optarg != '-' && *optarg < '0' && *optarg > '7') {
			fprintf(stderr, "Invalid bitplane slice argument");
			ret = -1;
		}
		if (ret < 0) {
			return ret;
		}
		optarg++;
	}

	return 0;
}

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

	static char usage[] = "usage: %s -i infile.bmp [OPTIONS] -o outfile.bmp\n\nOPTIONS:\nw,r,g,b,f,s,n\n";

	/* parse the command line arguments -i for input -o for output */
	while((c = getopt(argc, argv, "wfrhgbns:i:o:")) != -1){
				
		if( c == 'i'){
			tr->infile = optarg;
			i_flag = 1;
		}
		else if(c == 'o'){
			tr->outfile = optarg;
			o_flag = 1;
		}
		else if (c == 's') {
			if (parse_bitplane_slice(tr, optarg)) {
				return -1;
			}
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
		else if (c == 'h') {
			help_menu();
			exit(0);
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
