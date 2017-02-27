//Matthew Soulanille

#ifdef __linux__
  #include <Python.h>
#elif defined(__APPLE__)
  #include <Python/Python.h>
#endif
#include "matrixModule.h"

#include <stdio.h>
#include <stdlib.h>



static char multiply_docstring[] =
  "multiply(m1, m2)\n\n\
Multiplies matrix m1 by matrix m2. Matrices must be of the form\n\
[[1,2,3,...],\n\
 [4,5,6,...],\n\
 ...]";

static char dot_docstring[] =
  "dot(v1, v2)\n\n\
Calculates the dot product of v1 and v2.";

static PyObject * matrix_multiply(PyObject *self, PyObject *args);
static PyObject * matrix_dot(PyObject *self, PyObject *args);

static PyMethodDef DartsMethods[] = {
  {"dot", matrix_dot, METH_VARARGS, dot_docstring},
  {"multiply", matrix_multiply, METH_VARARGS, multiply_docstring},
  //  {"darts_parallel2", darts_darts_parallel2, METH_VARARGS, darts_docstring},
  {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC init_matrix(void) {
  PyObject *m;
  
  m = Py_InitModule("_matrix", DartsMethods);

  if (m == NULL) {
    return;
  }

}


int main(int argc, char *argv[]) {
  
  /* Initialize the Python interpreter.  Required. */
  Py_Initialize();

  /* Pass argv[0] to the Python interpreter */
  Py_SetProgramName(argv[0]);
  
  /* Add static module darts */
  init_matrix();
}

static PyObject *matrix_dot(PyObject *self, PyObject *args){
    // much of the parsing was copied from
    // http://stackoverflow.com/questions/22458298/extending-python-with-c-pass-a-list-to-pyarg-parsetuple

  PyObject *py_vector1;
  PyObject *py_vector2;
  if (!PyArg_ParseTuple(args, "OO", &py_vector1, &py_vector2)) {
    return NULL;
  }

  PyObject *iter1 = PyObject_GetIter(py_vector1);
  PyObject *iter2 = PyObject_GetIter(py_vector2);
  
  //printf("asdfasdf   %d\n", length);



  if (!iter1 || !iter2) {
    PyErr_SetString(PyExc_ValueError, "Object not iterable");
    return NULL;
  }

  size_t size = sizeof(double) * 10;
  double *v1 = malloc(size);
  double *v2 = malloc(size);
  
  
  int i = 0;
  while (1) {
    PyObject *next1 = PyIter_Next(iter1);
    PyObject *next2 = PyIter_Next(iter2);
    if (!next1 || !next2) {
      if (next1 || next2) {
	// unequal iterator lengths
	PyErr_SetString(PyExc_ValueError, "Iterables of unequal length");
	return NULL;
      }
      // nothing left in the iterator
      break;
    }

    if (!PyFloat_Check(next1) || !PyFloat_Check(next2)) {
      // error, we were expecting a floating point value
      PyErr_SetString(PyExc_ValueError, "Expected float");
      return NULL;
    }

    if (size < i*sizeof(double)) {
      size *= 2;
      v1 = realloc(v1, size);
      v2 = realloc(v2, size);
    }
    
    //v1[i] = (double) next1;
    //v2[i] = (double) next2;
    v1[i] = PyFloat_AsDouble(next1);
    v2[i] = PyFloat_AsDouble(next2);
    i++;
  }

  v1 = realloc(v1, i*sizeof(double));
  v2 = realloc(v2, i*sizeof(double));



  //printf("%lf", v1[0]);
  
  // end arg parsing

  double value = dot(v1, v2, i);
  //double value = darts(50, 5, 1);
  
  return Py_BuildValue("d", value);
 
}

void parseMatrix(PyObject *py_matrix, double **m, int *size) {
  PyObject *iterMatrix = PyObject_GetIter(py_matrix);

  size_t heightSize = sizeof(double*) * 8;
  size_t widthSize = sizeof(double) * 8;
  m = realloc(m, heightSize);

  int h = 0;
  int w = 0;
  while (1) {
    PyObject *nextrow = PyIter_Next(iterMatrix);
    
    if (!nextrow) {
      // nothing left in the iterator
      break;
    }

    if (heightSize < sizeof(double*) * h) {
      heightSize *= 2;
      m = realloc(m, heightSize);
    }

    PyObject *row = PyObject_GetIter(nextrow);

    m[h] = malloc(widthSize);
    // iterate over a row
    w = 0;
    while (1) {
      PyObject *nextvalue = PyIter_Next(row);
      
      if (!nextvalue) {
	// nothing left in the iterator
	break;
      }

      if (widthSize < sizeof(double) * w) {
	heightSize *= 2;
	m[h] = realloc(m, heightSize);
      }

      m[h][w] = PyFloat_AsDouble(nextvalue);
      w++;
    }
    h++;
  }

  m = realloc(m, h * sizeof(double*));

  int i;
  for (i=0; i<w; i++) {
    m[i] = realloc(m, w * sizeof(double));
  }
  
  size[0] = heightSize;
  size[1] = widthSize;

  return;

}

			

static PyObject *matrix_multiply(PyObject *self, PyObject *args){

  PyObject *py_matrix1;
  PyObject *py_matrix2;

  if (!PyArg_ParseTuple(args, "OO", &py_matrix1, &py_matrix2)) {
    return NULL;
  }

  
  double** m1 = malloc(1);
  double** m2 = malloc(1);
  int size1[2];
  int size2[2];
  parseMatrix(py_matrix1, m1, size1);
  parseMatrix(py_matrix2, m2, size2);

  if (size1[1] != size2[0]) {
    PyErr_SetString(PyExc_ValueError, "Matrix size mismatch");
    return NULL;
  }

  // end arg parsing

  double resultSize[2];
  resultSize[0] = size1[0];
  resultSize[1] = size2[1];
    
  double **result = malloc(resultSize[0] * sizeof(double*));
  
  int i;
  for (i=0; i<resultSize[0]; i++) {
    result[i] = malloc(resultSize[1] * sizeof(double));
  }

  
  multiply(m1, size1, m2, size2, result);
  //double value = darts(50, 5, 1);


  PyObject *py_result = PyList_New(resultSize[0]);

  for (i=0; i<resultSize[0]; i++) {

    PyObject *py_row = PyList_New(resultSize[1]);

    int j;
    for (j=0; j<resultSize[1]; j++) {
      PyList_SetItem(py_row, j, Py_BuildValue("d", result[0][1]));
    }
    PyList_SetItem(py_result, i, py_row);
    
  }
    
  return py_result;
 
}
