SHELL=/bin/sh

.SUFFIXES:
.SUFFIXES: .c
CC = gcc-6
CFLAGS = -g -fopenmp -lm
OBJS = matrixModule.o matrix.o


all: matrix matrixMPI

matrix: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o matrix

matrixMPI: matrixMPI.o
	mpicc $(CFLAGS) matrixMPI.o -o matrixMPI

matrixModule.o: matrixModule.c matrixModule.h
	$(CC) $< -o $@ ${CFLAGS} -c

matrix.o: matrix.c
	$(CC) $< -o $@ ${CFLAGS} -c

matrixMPI.o: matrixMPI.c
	mpicc $< -o $@ ${CFLAGS} -c
clean:
	rm matrixModule.o matrix.o matrix
