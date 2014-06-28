#include "parse-int.h"
#include "bitmappin-structs.h"
#include <ctype.h>
#include <stdio.h>

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

/**@brief Set the numbers with their corresponding colors
 *
 *@arg struct transform *tr: Stores the numbers with their corresponding colors for the rest of the program
 *@arg char **optarg: Holds the numbers to be parsed from the command line
 *
 *@ret Returns 0 on success, -1 on error
 */
int parse_numbers(struct transform *tr, char **optarg){
 
	int temp;
	int start;
	int end;
       
	start = strtol(*optarg, optarg, 10);

	if(start > MAX_PIXEL_VALUE + 1 || start < MIN_PIXEL_VALUE - 1){
		fprintf(stderr, "Color Value must be from -1-256\n");
		return -1;
	}
	if(**optarg == '-'){
		if(++*optarg != NULL){
			if(isdigit(**optarg)){
				end = strtol(*optarg, optarg, 10);

				if(end > MAX_PIXEL_VALUE + 1 || end < MIN_PIXEL_VALUE - 1){
					fprintf(stderr, "Color Value must be from -1-256\n");
					return -1;
				}		
			}
			else{
				fprintf(stderr, "No ending number specified after '-'\n");
				return -1;
			}
		}
		else{
			fprintf(stderr, "Must specify an ending number after '-'\n");
			return -1;
		}
	}
	else{
		end = start;
	}
	
	if(start > end){
		temp = start;
		start = end;
		end = temp;
	}
	
	if(tr->r_to_set == true){
		if(tr->r_set){
			fprintf(stderr, "Red value already specified\n");
			return -1;
		}
		else{
			tr->r_start = start;
			tr->r_end = end;
			tr->r_set = true;
			tr->r_to_set = false;
		}
	}
	if(tr->g_to_set == true){
		if(tr->g_set){
			fprintf(stderr, "Green value already specified\n");
			return -1;
		}
		else{
			tr->g_start = start;
			tr->g_end = end;
			tr->g_set = true;
			tr->g_to_set = false;
		}
	}
	if(tr->b_to_set == true){
		if(tr->b_set){
			fprintf(stderr, "Blue value already specified\n");
			return -1;
		}
		else{
			tr->b_start = start;
			tr->b_end = end;
			tr->b_set = true;
			tr->b_to_set = false;
		}
	}

	return 0;

}

/**@brief Parse the colors to be set 
 *
 * Parses **optarg to let the program know what colors the following numbers correspond to 
 *
 * @arg struct transform *tr: Stores the data of the colors that are to have their numbers set
 * @arg char **optarg: The flag from the command line that is to be parsed
 *
 *@ret Returs 0 on success, -1 on error
*/
int parse_colors(struct transform *tr, char **optarg){
		
	if(isdigit(**optarg)){
		tr->r_to_set = true;
		tr->g_to_set = true;
		tr->b_to_set = true;
	       
		return 0;
	}
	else{
		while(**optarg == 'r' || **optarg == 'R' || **optarg == 'g' || **optarg == 'G' || **optarg == 'b' || **optarg == 'B'){
			if(**optarg == 'r' || **optarg == 'R'){
				tr->r_to_set = true;
			}
			else if(**optarg == 'g' || **optarg == 'G'){
				tr->g_to_set = true;
			}
			else if(**optarg == 'b' || **optarg == 'B'){
				tr->b_to_set = true;

			}		
			++*optarg;
		}
		if(isdigit(**optarg)){
			return 0;
		}
		else{
			fprintf(stderr, "No number was given after the color(s)\n");
			return -1;
		}
	}
}

/**@brief Parse the colors and numbers from the command prompt
 *
 * Store the RGB values that were specified from the command prompt with their corresponding numbers
 *
 *@arg struct transform *tr: Allows the parsed data to be stored for the rest of the program to access
 *@arg char *optarg: Holds the data from the command line that is to be parsed
 *
 *@ret Returns 0 on success and -1 on error
*/
int parse_colors_and_numbers(struct transform *tr, char *optarg){

	while(*optarg){
		if(parse_colors(tr, &optarg)){
			fprintf(stderr, "Failed to parse color(s)\n");
			return -1;
		}

		if(parse_numbers(tr, &optarg)){
			fprintf(stderr, "Failed to parse numbers\n");
			return -1;
			}
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

	static char usage[] = "usage: %s -i infile.bmp [OPTIONS] -o outfile.bmp\n\nOPTIONS:\nw,r,g,b,f,s,n,t\n";

	/* parse the command line arguments -i for input -o for output */
	while((c = getopt(argc, argv, "t:wfrhgbns:i:o:")) != -1){
				
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
		else if (c == 't'){
			if(parse_colors_and_numbers(tr, optarg)){
				return -1;
			}
			else{
				tr->op = thresholding;
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
