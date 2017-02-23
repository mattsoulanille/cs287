//Mathew Soulanille

#include "matrixModule.h"

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




/* /\* int* getMatrixSize(double** m, int* result) { *\/ */
/* /\*   // height *\/ */
/* /\*   int mh = (int) (sizeof(m) / sizeof(m[0])); *\/ */

/* /\*   //width *\/ */
/* /\*   int mw = (int) (sizeof(m[0]) / sizeof(m[0][0])); *\/ */

/* /\*   result[0] = mh; *\/ */
/* /\*   result[1] = mw; *\/ */
/* /\*   // height by width since height is first *\/ */
/* /\*   // in my matrix representation *\/ */
/* /\*   return result; *\/ */
  
/* /\* } *\/ */

/* double** transpose(double** m, double** result) { */
/*   // Turns the representation from rows to columns. (transposes matrix) */
/*   // */
/*   //[[1,2,3],      [[1,4,7], */
/*   // [4,5,6],  ->   [2,5,8], */
/*   // [7,8,9]]       [3,6,9]] */

/*   int size[2]; */
/*   getMatrixSize(m, size); */
  
/*   int h; // height */
/*   int w; // width */

/*   for (w = 0; w < size[1]; w++) { */
/*     for (h = 0; h < size[0]; h++) { */
/*       result[h][w] = m[w][h]; */
/*     } */
/*   } */
/*   return result; */
  

/* } */

/* double** multiply(double** m1, double** m2, double** result) { */
/*   // elements of m1 (and m2) are arrays of the rows of the matrix */
/*   // Check dimensions for sanity: */
/*   int m1_size[2]; */
/*   getMatrixSize(m1, m1_size); */

/*   int m2_size[2]; */
/*   getMatrixSize(m1, m2_size); */

/*   double m2_transpose[5][5]; */
/*   //double m2_transpose[m2_size[1]][m2_size[0]]; */

/*   transpose(m2, m2_transpose); */
  


/*   int h; // height */
/*   int w; // width */
/*   // height of first matrix */
/*   // width of second */
/*   for (h = 0; h < m1_size[0]; h++) { */
/*     for (w = 0; w < m2_size[1]; w++) { */
/*       result[h][w] = dot(m1[h], m2_transpose[w]); */
/*     } */
/*   } */

/*   return result; */

/* } */
