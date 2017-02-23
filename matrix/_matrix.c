//Matthew Soulanille

#ifdef __linux__
  #include <Python.h>
#elif defined(__APPLE__)
  #include <Python/Python.h>
#endif
#include "matrixModule.h"

#include <stdio.h>
#include <stdlib.h>



/* static char multiply_docstring[] = */
/*   "multiply(m1, m2)\n\n\ */
/* Multiplies matrix m1 by matrix m2."; */

static char dot_docstring[] =
  "dot(v1, v2)\n\n\
Calculates the dot product of v1 and v2.";

//static PyObject * matrix_multiply(PyObject *self, PyObject *args);
static PyObject * matrix_dot(PyObject *self, PyObject *args);
//static PyObject * matrix_darts_parallel2(PyObject *self, PyObject *args);

static PyMethodDef DartsMethods[] = {
  {"dot", matrix_dot, METH_VARARGS, dot_docstring},
  //  {"darts_parallel", darts_darts_parallel, METH_VARARGS, darts_docstring},
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
