SHELL=/bin/sh

.SUFFIXES:
.SUFFIXES: .c
CC = gcc
CFLAGS = -g -fopenmp -lm

darts: darts_cli.c
	$(CC) $< -o $@ ${CFLAGS}

clean:
	rm darts
