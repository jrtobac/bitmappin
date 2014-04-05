#ifndef _BITMAPPIN_H_
#define _BITMAPPIN_H_

#include "bitmappin-structs.h"

int parse_input(int argc, char *argv[], struct transform *tr);
int read_in_file(struct transform *tr);
int write_out_file(struct transform *tr);
void init_transform(struct transform *tr);
	
#endif //_BITMAPPIN_H_
