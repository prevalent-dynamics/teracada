#ifndef _TERACADA_PYAPI_HELPER_H
#define _TERACADA_PYAPI_HELPER_H

#include <Python.h>

#include "teracada_array.h"


uint8_t deducePyListDataType (PyObject* pList);

bool setPyErrorStr ( stdTeraArray* pstiTeraArray );

bool teraArrayInsertPyList ( stdTeraArray* pstiTeraArray, PyObject* pElements );

PyObject* teraArrayGetElementAsPyObj ( stdTeraArray* pstiTeraArray, int iPosition );

#endif
