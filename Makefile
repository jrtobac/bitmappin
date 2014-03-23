CC=gcc
CFLAGS=

all: bitmappin.c
	$(CC) $(CFLAGS) bitmappin.c -o bitmappin
