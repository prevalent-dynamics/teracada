#ifndef _TERACADA_PYAPI_REGRESSION_H
#define _TERACADA_PYAPI_REGRESSION_H

#include <Python.h>

#include "teracada_regression.h"
#include "pyapi_helper.h"


typedef struct {
  PyObject_HEAD
  stdTeraLReg* pstiTeraLReg;
} stdTeraLRegPyObj;

PyObject* teraLRegNewPyObj ( PyTypeObject* pstiTeraLRegPyObjTypeDef, PyObject* pArgs, PyObject* pKargs);

int teraLRegInitPyObj ( stdTeraLRegPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs );

PyObject* teraLRegPyApiFit ( stdTeraLRegPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs);
#endif
