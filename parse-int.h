#ifndef _PARSE_INT_H_
#define _PARSE_INT_H_

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

#include "help.h"
#include "operations.h"

int parse_bps_color(unsigned char *mask, char *optarg);
int parse_bitplane_slice(struct transform *tr, char *optarg);

#endif //_PARSE_INT_H_
