SHELL=/bin/sh

.SUFFIXES:
.SUFFIXES: .c
CC = gcc-6
CFLAGS = -g -fopenmp -lm
OBJS = matrixModule.o matrix.o


all: matrix

matrix: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o matrix

matrixModule.o: matrixModule.c matrixModule.h
	$(CC) $< -o $@ ${CFLAGS} -c

matrix.o: matrix.c
	$(CC) $< -o $@ ${CFLAGS} -c

clean:
	rm matrixModule.o matrix.o
