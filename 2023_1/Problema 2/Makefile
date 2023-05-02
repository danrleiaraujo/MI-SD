Q ?= @

DEBUG = -O3
CC = gcc
INCLUDE = -I/usr/local/include
CFLAGS = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LDLIBS = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

SRC = main.c
OBJ = $(SRC: .c=.o)
BINS = $(SRC: .c=)

main: main.o
	$Q $(CC) -o $@ main.o $(LDFLAGS) $(LDLIBS)

clean:
	rm *.o