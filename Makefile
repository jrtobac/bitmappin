CC=gcc
CFLAGS=-Wall -Wextra

EXEC=bitmappin
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

$(EXEC): $(OBJECTS)
all: bitmappin.c
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
