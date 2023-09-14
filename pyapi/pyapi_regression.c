// Prefix: teraLReg

#include <Python.h>

#include "pyapi_regression.h"


PyObject* teraLRegNewPyObj ( PyTypeObject* pstiTeraLRegPyObjTypeDef, PyObject* pArgs, PyObject* pKargs) {
  stdTeraLRegPyObj* pstiSelf;

  pstiSelf = (stdTeraLRegPyObj*) pstiTeraLRegPyObjTypeDef->tp_alloc(pstiTeraLRegPyObjTypeDef, 0);

  if ( ! pstiSelf ) {
    PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for TercadaLinearRegression type object");
    goto ERREXIT;
  }

  EXIT:
    return (PyObject*) pstiSelf;

  ERREXIT:
    return NULL;
}


int teraLRegInitPyObj ( stdTeraLRegPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs ) {
  pstiSelf->pstiTeraLReg = NULL;

  EXIT:
    return 0;
}


PyObject* teraLRegPyApiFit ( stdTeraLRegPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs) {

  EXIT:
    return NULL;

  ERREXIT:
    return NULL;
}