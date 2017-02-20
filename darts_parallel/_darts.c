//Matthew Soulanille

#ifdef __linux__
  #include <Python.h>
#elif defined(__APPLE__)
  #include <Python/Python.h>
#endif
#include "dartsmodule.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SLICE_SIZE (M_PI/10.0)

/*static char module_docstring[] =
  "This module implements a monte carlo simulation for throwing darts at a board";*/

static char darts_docstring[] =
  "darts(count, accuracy, target)\n\n\
Does a monte carlo simulation of throwing darts at a dart board and \
returns the average number of points\n\n\
count: The number of darts to throw\n\
accuracy: How much to scale the randomly generated numbers (in cm)\n\
target: Which number to throw at (1 or 20)";

static PyObject * darts_darts(PyObject *self, PyObject *args);
static PyObject * darts_darts_parallel(PyObject *self, PyObject *args);
static PyObject * darts_darts_parallel2(PyObject *self, PyObject *args);

static PyMethodDef DartsMethods[] = {
  {"darts", darts_darts, METH_VARARGS, darts_docstring},
  {"darts_parallel", darts_darts_parallel, METH_VARARGS, darts_docstring},
  {"darts_parallel2", darts_darts_parallel2, METH_VARARGS, darts_docstring},
  {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC init_darts(void) {
  //PyImport_AddModule("darts");
  PyObject *m;
  
  m = Py_InitModule("_darts", DartsMethods);

  if (m == NULL) {
    return;
  }
  //Py_InitModule("_darts", module_methods, module_docstring);
}


int main(int argc, char *argv[]) {
  
  /* Initialize the Python interpreter.  Required. */
  Py_Initialize();

  /* Pass argv[0] to the Python interpreter */
  Py_SetProgramName(argv[0]);
  
  /* Add static module darts */
  init_darts();
}


static PyObject *darts_darts(PyObject *self, PyObject *args){
  long count;
  double accuracy;
  int target;

  /* const int *t; */
  /* if (!PyArg_ParseTuple(args, "i", &t)) { */
  /*   return NULL; */
  /* } */
  
  if (!PyArg_ParseTuple(args, "ldi", &count, &accuracy, &target)) {
    return NULL;
  }


  if ((target != 1) && (target != 20)) {
    PyErr_SetString(PyExc_ValueError, "Target must be 1 or 20");
    return NULL;
  }

  // end arg parsing

  double value = darts(count, accuracy, target);
  //double value = darts(50, 5, 1);
  
  return Py_BuildValue("d", value);
 
}

static PyObject *darts_darts_parallel(PyObject *self, PyObject *args){
  long count;
  double accuracy;
  int target;
  int num_threads;
  /* const int *t; */
  /* if (!PyArg_ParseTuple(args, "i", &t)) { */
  /*   return NULL; */
  /* } */
  
  if (!PyArg_ParseTuple(args, "ldii", &count, &accuracy, &target, &num_threads)) {
    return NULL;
  }


  if ((target != 1) && (target != 20)) {
    PyErr_SetString(PyExc_ValueError, "Target must be 1 or 20");
    return NULL;
  }

  // end arg parsing

  double value = darts_parallel(count, accuracy, target, num_threads);
  //double value = darts(50, 5, 1);
  
  return Py_BuildValue("d", value);
 
}


static PyObject *darts_darts_parallel2(PyObject *self, PyObject *args){
  long count;
  double accuracy;
  int target;
  int num_threads;
  /* const int *t; */
  /* if (!PyArg_ParseTuple(args, "i", &t)) { */
  /*   return NULL; */
  /* } */
  
  if (!PyArg_ParseTuple(args, "ldii", &count, &accuracy, &target, &num_threads)) {
    return NULL;
  }


  if ((target != 1) && (target != 20)) {
    PyErr_SetString(PyExc_ValueError, "Target must be 1 or 20");
    return NULL;
  }

  // end arg parsing

  double value = darts_parallel2(count, accuracy, target, num_threads);
  //double value = darts(50, 5, 1);
  
  return Py_BuildValue("d", value);
 
}
