#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_TAG 1
#define RANGE_TAG 2
#define RESULT_TAG 3

void printm(double *matrix, int dim) {

  int x;
  int y;
  //  fprintf(stderr, "{%lf}\n", *matrix);

  //  printf("%lf\t", *matrix);

  for (y=0; y<dim; y++) {
    for (x=0; x<dim; x++) {
      printf("%lf\t", *(matrix + y*dim + x));
    }
    printf("\n");
  }
}


int readMatrix(FILE *matrixFile, double *matrix, int dim) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read = getline(&line, &len, matrixFile);
  int y;
  for (y=0; y<dim; y++) {
    //printf("%s", line);
    
    char *endpointer = line;
    int x;
    for (x=0; x<dim; x++) {
      double entry = strtod(endpointer, &endpointer);
      //fprintf(stderr, " %lf", entry);
      //matrix[y][x] = entry;
      *(matrix + y*dim + x) = entry;
    }
    read = getline(&line, &len, matrixFile);
  }

}

int main(int argc, char *argv[]) {
  int rank, size, i, dim, workunit_size, print;
  double *buffer;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (argc == 4) {
    dim = atoi(argv[2]);
    print = atoi(argv[3]);
  }
  else {
    if (rank == 0) {
      printf("Usage: matrix (matrixFile) (dimension) (print 0/1)\n");
    }
    return 1;
  }
  
  double *matrix;
  matrix = calloc(sizeof(double), dim*dim);

  if (rank == 0) {
    FILE *matrixFile;
    matrixFile = fopen(argv[1], "r");

    // read the matrix
    readMatrix(matrixFile, matrix, dim);
    // send the matrix to the others
    for (i = 1; i < size; i++) {
      // Maybe use mpi_bcast...
      MPI_Send(matrix, dim*dim, MPI_DOUBLE, i, MATRIX_TAG, MPI_COMM_WORLD);
    }
  }
  else {
    // if not the host machine
    // Get matrix
    MPI_Recv(matrix, dim*dim, MPI_DOUBLE, 0, MATRIX_TAG, MPI_COMM_WORLD, &status);
    //printm(matrix, dim);
  }

  int range[2];
  int blockSize = dim / size;
  range[0] = (size - rank - 1) * blockSize;
  range[1] = (size - rank) * blockSize;

  if (rank == 0) {
    // account for non-divisible matrix sizes.
    range[1] = dim;
  }

  //printf("calculating [%d, %d]\n", range[0], range[1]);

  // Allocate result buffer
  int resultSize = (range[1] - range[0]) * dim;
  double *result = calloc(sizeof(double), resultSize);

  // Iterate over the rows to dot by
  int h; // height
  int w; // width
  //printf("range: [%d, %d]\n", range[0], range[1]);    
  for (h = range[0]; h < range[1]; h++) {

    // iterate over columns
    int dotResult = 0;

    for (w = 0; w < dim; w++) {

      // Dot Product
      for (i = 0; i < dim; i++) {
	
	*(result + dim * (h - range[0]) + w ) += *(matrix + dim * h + i) * *(matrix + dim * i + w);
	//printf("h: %d\tw: %d\ti: %d\n", h, w, i);
	//printf("%lf\t", *(result + dim * (h - range[0]) + w ));
      }


    }
    //printf("\n");    
  }

  // Send results to the main process
  if (rank != 0) {
    // printf("Sending result from process %d\n", rank);
    MPI_Send(result, resultSize, MPI_DOUBLE, 0, RESULT_TAG, MPI_COMM_WORLD);
  }

  // Receive calculated pieces from others
  if (rank == 0) {
    // final result matrix
    double *productMatrix = calloc(sizeof(double), dim*dim);


    // Add this process's results to the matrix
    for (h = range[0]; h < range[1]; h++) {
      for (w = 0; w < dim; w++) {
	*(productMatrix + h*dim + w) = *(result - range[0] + h*dim + w);
      }
    }


    
    int otherRange[2];
    // Get results from all other processes
    for (i = 1; i < size; i++) {
      otherRange[0] = (size - rank - 1) * blockSize;
      otherRange[1] = (size - rank) * blockSize;
      int otherResultSize = (otherRange[1] - otherRange[0]) * dim;
      double *otherResult = calloc(sizeof(double), otherResultSize);
      // printf("Getting result from process %d\n", i);
      MPI_Recv(otherResult, otherResultSize, MPI_DOUBLE, i, RESULT_TAG, MPI_COMM_WORLD, &status);



      // add the other results to the final product
      for (h = otherRange[0]; h < otherRange[1]; h++) {
	for (w = 0; w < dim; w++) {
	  *(productMatrix + h*dim + w) = *(otherResult - otherRange[0] + h*dim + w);
	}
      }

    }
    if (print) {
      printm(matrix, dim);
      printf("\n\n became \n\n");
      printm(productMatrix, dim);
    }
    
  }

  
  MPI_Finalize();
  return 0;
}