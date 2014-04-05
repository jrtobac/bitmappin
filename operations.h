#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include "bitmappin-structs.h"

int do_nothing(struct transform *tr);
int max_color(struct transform *tr);
void max_green(struct bitmap *bm);
void max_blue(struct bitmap *bm);
void max_red(struct bitmap *bm);
void color_white(struct bitmap *bm);


#endif //_OPERATIONS_H_
