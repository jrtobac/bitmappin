#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include "bitmappin-structs.h"

int do_nothing(struct transform *tr);
int max_color(struct transform *tr);
int image_negative(struct transform *tr);
int flip(struct transform *tr);
int bitplane_slice(struct transform *tr);
int thresholding(struct transform *tr);

#endif //_OPERATIONS_H_
