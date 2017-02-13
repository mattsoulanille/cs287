SHELL=/bin/sh

.SUFFIXES:
.SUFFIXES: .c
CC = /usr/local/bin/gcc-6
CFLAGS = -g -fopenmp

darts: darts_cli.c
	$(CC) $< -o $@ ${CFLAGS}

clean:
	rm darts
