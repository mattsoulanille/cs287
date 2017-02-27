//Mathew Soulanille

#include "matrixModule.h"
#include <stdio.h>
#include <stdlib.h>

double dot(double* v1, double* v2, int len) {
  // Calculate dot product of vectors
  //int len = (int) (sizeof(v1) / sizeof(v1[0]));

  //if ((int) (sizeof(v2) / sizeof(v2[0])) != len) {
  //    return -1000000000000.0; // this is terrible. I don't know how to do exceptions
    //  }


  double result = 0;
  int i;
  for (i = 0; i < len; i++) {
    result += v1[i] * v2[i];
  }

  return result;
}


double** transpose(double** m, int size[2], double** result) {
  // Turns the representation from rows to columns. (transposes matrix)
  // size[2] is {height, width} of m
  //
  //[[1,2,3],      [[1,4,7],
  // [4,5,6],  ->   [2,5,8],
  // [7,8,9]]       [3,6,9]]
  
  int h; // height
  int w; // width

  for (w = 0; w < size[1]; w++) {
    for (h = 0; h < size[0]; h++) {
      result[h][w] = m[w][h];
    }
  }
  return result;
}

double** multiply(double** m1, int m1_size[2],\
		  double** m2, int m2_size[2], double** result) {
  // elements of m1 (and m2) are arrays of the rows of the matrix
  // Check dimensions for sanity:
  // mn_size = {height, width}
  if (m1_size[1] != m2_size[0]) {
    //   return NULL;
    //There is a problem
    printf("Bad Matrix Dimensions");
  }

  //  double m2_transpose[m2_size[1]][m2_size[0]];

  // Construct memory for resultant transpose matrix

  
  double **m2_transpose = malloc(m2_size[1] * sizeof(double*));

  if (m2_transpose) {
    int i;
    for (i=0; i<m2_size[0]; i++) {

      m2_transpose[i] = malloc(m2_size[0] * sizeof(double));
      if (m2_transpose[i] == NULL) {
	printf("Malloc error");
	return NULL;
      }

    }
  }
  else {
    printf("Malloc error");
    return NULL;
  }
  
  transpose(m2, m2_size, m2_transpose);


  int h; // height
  int w; // width
  // height of first matrix
  // width of second
  for (h = 0; h < m1_size[0]; h++) {
    for (w = 0; w < m2_size[1]; w++) {
      result[h][w] = dot(m1[h], m2_transpose[w], m1_size[1]);
    }
  }


  return result;

}
