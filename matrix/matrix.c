#include "matrixModule.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// My dad helped me with some of the text input, but I thought of
// all the matrix multiplication.

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

int main(int argc, char* argv[]) {

  FILE *matrixFile;
  if (argc == 3) {

    matrixFile = fopen(argv[1], "r");
  }
  else {
    printf("Usage: matrix (matrixFile) (dimension)\n");
    return 1;
  }

  int dim = atoi(argv[2]);
  // http://stackoverflow.com/questions/3501338/c-read-file-line-by-line
  //char buf[256];
  char *line = NULL;
  //  line = buf;
  size_t len = 0;
  ssize_t read = getline(&line, &len, matrixFile);

  //double matrix[dim][dim];
  double *matrix;
  matrix = calloc(sizeof(double), dim*dim);
  
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


  //  printm( matrix, dim);

  double *result;
  result = calloc(sizeof(double), dim*dim);
  multiply(matrix, matrix, dim, result);
  //printm( result, dim);
  
}
