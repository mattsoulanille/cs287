//Mathew Soulanille

#include "matrixModule.h"
#include <stdio.h>
#include <stdlib.h>

double dot(double* v1, double* v2, int len) {
  // Calculate dot product of vectors
  //int len = (int) (sizeof(v1) / sizeof(v1[0]));

  double result = 0;
  int i;
  for (i = 0; i < len; i++) {
    result += v1[i] * v2[i];
  }

  return result;
}


double* transpose(double* m, int dim, double* result) {
  // Turns the representation from rows to columns. (transposes matrix)
  // dim[2] is {height, width} of m
  //
  //[[1,2,3],      [[1,4,7],
  // [4,5,6],  ->   [2,5,8],
  // [7,8,9]]       [3,6,9]]
  
  int h; // height
  int w; // width
  
  for (w = 0; w < dim; w++) {
    for (h = 0; h < dim; h++) {
      // result[h][w] = m[w][h];
      *(result + h*dim + w) = *(m + w*dim + h);
    }
  }
  return result;
}

double* multiply(double* m1, double* m2, int dim, double* result) {
  // elements of m1 (and m2) are arrays of the rows of the matrix
  // Check dimensions for sanity:
  //  double m2_transpose[m2_size[1]][m2_size[0]];

  // Construct memory for resultant transpose matrix


  //  double *m2_transpose = calloc(sizeof(double), dim*dim);

  //  transpose(m2, dim, m2_transpose);


  int h; // height
  int w; // width
  int d; // dot product index
  // height of first matrix
  // width of second
  for (h = 0; h < dim; h++) {
    for (w = 0; w < dim; w++) {
      for (d = 0; d < dim; d++) {
	// result[h][w] = dot(m1[h], m2_transpose[w], m1_size[1]);
	*(result + dim*h + w) += *(m1 + h*dim + d) * *(m2 + d*dim + w);
      }
    }
  }


  return result;

}

double* multiply_parallel(double* m1, double* m2, int dim, double* result, int threads) {
  // elements of m1 (and m2) are arrays of the rows of the matrix
  // Check dimensions for sanity:
  //  double m2_transpose[m2_size[1]][m2_size[0]];

  // Construct memory for resultant transpose matrix

  
  //  double *m2_transpose = calloc(sizeof(double), dim*dim);

  //  transpose(m2, dim, m2_transpose);


  int h; // height
  int w; // width
  int d; // dot product index
  // height of first matrix
  // width of second
  //printf("test\n");
#pragma omp parallel for collapse(2) num_threads(1) shared(result)
  for (h = 0; h < dim; h++) {
    for (w = 0; w < dim; w++) {

      for (d = 0; d < dim; d++) {
	// result[h][w] = dot(m1[h], m2_transpose[w], m1_size[1]);
	*(result + dim*h + w) += *(m1 + h*dim + d) * *(m2 + d*dim + w);
      }

    }
  }


  return result;

}
