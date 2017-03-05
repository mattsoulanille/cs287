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
  int dim;
  if (!PyArg_ParseTuple(args, "OOi", &py_vector1, &py_vector2, &dim)) {
    return NULL;
  }

  PyObject *iter1 = PyObject_GetIter(py_vector1);
  PyObject *iter2 = PyObject_GetIter(py_vector2);
  
  //printf("asdfasdf   %d\n", length);



  if (!iter1 || !iter2) {
    PyErr_SetString(PyExc_ValueError, "Object not iterable");
    return NULL;
  }

  double *v1 = calloc(sizeof(double), dim);
  double *v2 = calloc(sizeof(double), dim);

  
  int i = 0;
  for (i=0; i<dim; i++) {
    PyObject *next1 = PyIter_Next(iter1);
    PyObject *next2 = PyIter_Next(iter2);
    if (!next1 || !next2) {
      if (next1 || next2) {
	// unequal iterator lengths
	PyErr_SetString(PyExc_ValueError, "Iterables of unequal length");
	return NULL;
      }
      PyErr_SetString(PyExc_ValueError, "Size error");
      return NULL;
      // nothing left in the iterator
      break;
    }

    if (!PyFloat_Check(next1) || !PyFloat_Check(next2)) {
      // error, we were expecting a floating point value
      PyErr_SetString(PyExc_ValueError, "Expected float");
      return NULL;
    }

    v1[i] = PyFloat_AsDouble(next1);
    v2[i] = PyFloat_AsDouble(next2);
  }


  //printf("%lf", v1[0]);
  
  // end arg parsing

  double value = dot(v1, v2, i);
  //double value = darts(50, 5, 1);
  
  return Py_BuildValue("d", value);
 
}

static PyObject *matrix_multiply(PyObject *self, PyObject *args){

  PyObject *py_matrix1;
  PyObject *py_matrix2;
  int dim;
  int threads;

  if (!PyArg_ParseTuple(args, "OOii", &py_matrix1, &py_matrix2, &dim, &threads)) {
    return NULL;
  }


  PyObject *iter1 = PyObject_GetIter(py_matrix1);
  PyObject *iter2 = PyObject_GetIter(py_matrix2);
  if (!iter1 || !iter2) {
    PyErr_SetString(PyExc_ValueError, "Object not iterable");
    return NULL;
  }

  double *m1 = calloc(sizeof(double), dim*dim);
  double *m2 = calloc(sizeof(double), dim*dim);
  
  int w;
  int h;
  for (h=0; h<dim; h++) {
    PyObject *nextrow1 = PyIter_Next(iter1);
    PyObject *nextrow2 = PyIter_Next(iter2);

    if (!nextrow1 || !nextrow2) {
      PyErr_SetString(PyExc_ValueError, "Matrix Size error (height)");
      return NULL;
    }

    PyObject *rowiter1 = PyObject_GetIter(nextrow1);
    PyObject *rowiter2 = PyObject_GetIter(nextrow2);

    for (w=0; w<dim; w++) {
      PyObject *nextvalue1 = PyIter_Next(rowiter1);
      PyObject *nextvalue2 = PyIter_Next(rowiter2);
      
      if (!nextvalue1 || !nextvalue2) {
	PyErr_SetString(PyExc_ValueError, "Matrix Size error (width)");
	return NULL;
      }

      *(m1 + h*dim + w) = PyFloat_AsDouble(nextvalue1);
      *(m2 + h*dim + w) = PyFloat_AsDouble(nextvalue2);
    }
  }

  // end arg parsing

    
  double *result = calloc(sizeof(double), dim*dim);
  
  multiply_parallel(m1, m2, dim, result, threads);

  PyObject *py_result = PyList_New(dim);

  for (h=0; h<dim; h++) {

    PyObject *py_row = PyList_New(dim);

    for (w=0; w<dim; w++) {
      PyList_SetItem(py_row, w, Py_BuildValue("d", *(result + h*dim + w) ));
    }
    PyList_SetItem(py_result, h, py_row);
    
  }
    
  return py_result;
 
}

