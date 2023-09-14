#ifndef _TERACADA_PYAPI_ARRAY_H
#define _TERACADA_PYAPI_ARRAY_H

#include <Python.h>

#include "teracada_array.h"
#include "pyapi_helper.h"


typedef struct {
  PyObject_HEAD
  stdTeraArray* pstiTeraArray;
} stdTeraArrayPyObj;


PyObject* teraArrayNew ( PyTypeObject* pstiTeracadaArrayPyObjTypeDef, PyObject *pArgs, PyObject *pKargs );

int teraArrayInit ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs, PyObject *pKargs );

PyObject* teraArrayRepr ( stdTeraArrayPyObj* pstiSelf );

PyObject* teraArrayPyApiDType ( stdTeraArrayPyObj* pstiSelf, PyObject* Py_UNUSED(ignored) );

PyObject* teraArrayPyApiArray ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs );

PyObject* teraArrayPyApiInsert ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs );

PyObject* teraArrayPyApiGet ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs );

PyObject* teraArrayPyApiPrint ( stdTeraArrayPyObj* pstiSelf, PyObject* Py_UNUSED(ignored));

#endif
