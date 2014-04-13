#include <stdio.h>
#include <stdlib.h>

/**@brief Display the help file for the user and exit the program
 *
 *@ret return type is void
 */
void help_menu()
{
	printf(
"This program is used to perform different image processing techniques on bmp files.  Currently, the program is being developed to support Microsoft Windows 3.x bmp files.\n\
\n\
usage: bitmappin -i <input_file.bmp> [options...] -o <output_file.bmp>\n\
\n\
OPTIONS\n\
-h\n\
	An optional flag to list the help file of the program\n\
\n\
-i intput_file.bmp\n\
	A required flag and argument to specify the input file that to process\n\
\n\
-o output_file.bmp\n\
	A requited flag and argument to specify the output bmp file\n\
\n\
-w\n\
	Max out the red, green and blue values of the bmp file\n\
\n\
-r\n\
	Max out the red values of the bmp file\n\
\n\
-g\n\
	Max out the green values of the bmp file\n\
\n\
-b\n\
	Max out the blue values of the bmp file\n\
\n\
-n\n\
	Create a negative of the input image\n\
\n\
-s <n>\n\
	Display only the n numbered bitplane and slice away the rest of the bitplanes setting them to zero\n\
\n\
-f\n\
	Flip the image upside down\n\
\n\
NOTE:\n\
	If only the required fields are specified, the input file is simply copied into the output file\n\
\n\
AUTHORS\n\
	Jeremy Tobac\n\
	Evan G. Tobac\n");
}
