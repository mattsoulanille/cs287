SHELL=/bin/sh

.SUFFIXES:
.SUFFIXES: .c
CC = gcc
CFLAGS = -c -g -fopenmp -lm

matrixModule: matrixModule.c
	$(CC) $< -o $@.o ${CFLAGS}

clean:
	rm matrixModule.o
