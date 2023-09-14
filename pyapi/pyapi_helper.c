#include <stdint.h>
#include <stdbool.h>
#include <Python.h>

#include "pyapi_helper.h"


uint8_t deducePyListDataType (PyObject* pList) {
  PyObject* pIter = NULL;
  PyObject* pElement = NULL;

  bool bDataTypeIsInt = false;
  bool bDataTypeIsDecimal = false;
  bool bDataTypeIsString = false;

  uint8_t ui8DataType = -1;

  if ( ! PyList_Check(pList) )
    goto ERREXIT;

  pIter = PyObject_GetIter(pList);

  while ( true ) {
    pElement = PyIter_Next(pIter);
    if ( ! pElement )
      break;

    if ( PyLong_Check(pElement) )
      bDataTypeIsInt = true;

    else if ( PyFloat_Check(pElement) )
      bDataTypeIsDecimal = true;

    else if ( PyUnicode_Check(pElement) && PyUnicode_AsUTF8String(pElement))
      bDataTypeIsString = true;

    else
      continue;
  }

  if ( bDataTypeIsString )
    ui8DataType = TC_STRING;

  else if ( bDataTypeIsDecimal )
    ui8DataType = TC_DECIMAL;

  else if ( bDataTypeIsInt )
    ui8DataType = TC_INT;

  else
    goto ERREXIT;

  EXIT:
    return ui8DataType;

  ERREXIT:
    return -1;
}


bool setPyErrorStr ( stdTeraArray* pstiTeracadaArray ) {
  int iErrno = 0;
  static PyObject* pPyErrType = NULL;

  if ( pPyErrType ) {
    // We have already set the error string before
    return true;
  }

  iErrno = ta_getErrno(pstiTeracadaArray);

  switch ( iErrno ) {
    case ERR_TA_INVALID_POS:
      pPyErrType = PyExc_IndexError;
      break;

    case ERR_TA_NOT_INIT:
    case ERR_TA_INIT_FAILED:
    case ERR_TA_RUNTIME:
    default:
      pPyErrType = PyExc_RuntimeError;
  }

  PyErr_SetString(pPyErrType, ta_getErrStr(pstiTeracadaArray, iErrno));

  EXIT:

    return true;

  ERREXIT:
    return false;
}


bool teraArrayInsertPyList ( stdTeraArray* pstiTeracadaArray, PyObject* pList ) {
  char acStrElement[40] = {0};
  PyObject* pIter = NULL;
  PyObject* pElement = NULL;

  if ( ! ta_isInitSuccess(pstiTeracadaArray) )
    goto ERREXIT;

  pIter = PyObject_GetIter(pList);

  while ( true ) {
    pElement = PyIter_Next(pIter);

    if ( ! pElement )
      break;

    switch ( ta_getDataType(pstiTeracadaArray) ) {

      case TC_INT:
        {
          if ( PyFloat_Check(pElement) )
            pElement = PyNumber_Long(pElement);

          if ( PyLong_Check(pElement) )
            ta_insertInt(pstiTeracadaArray, PyLong_AsLong(pElement));
          else
            goto ERREXIT;
        }

        break;

      case TC_DECIMAL:
        {
          if ( PyLong_Check(pElement) )
            pElement = PyNumber_Float(pElement);

          if ( PyFloat_Check(pElement) )
            ta_insertDecimal(pstiTeracadaArray, PyFloat_AsDouble(pElement));
          else
            goto ERREXIT;
        }

        break;

      case TC_STRING:
        {
          memset(acStrElement, 0, sizeof(acStrElement));

          if ( PyLong_Check(pElement) ) {
            snprintf(acStrElement, sizeof(acStrElement), "%ld", PyLong_AsLong(pElement));

            if ( ! ta_insertString(pstiTeracadaArray, acStrElement) )
              goto ERREXIT;
          }

          else if ( PyFloat_Check(pElement) ) {
            snprintf(acStrElement, sizeof(acStrElement), "%lf", PyFloat_AsDouble(pElement));

            if ( ! ta_insertString(pstiTeracadaArray, acStrElement) )
              goto ERREXIT;
          }

          else if ( PyUnicode_Check(pElement) && PyUnicode_AsUTF8String(pElement) ) {
            if ( ! ta_insertString(pstiTeracadaArray, PyUnicode_AsUTF8AndSize(pElement, NULL)) )
              goto ERREXIT;
          }

          else {
            goto ERREXIT;
          }
        }

        break;

      default:
        goto ERREXIT;
    }
  }

  EXIT:
    return true;

  ERREXIT:
    return false;
}

/*
  Assumes that the TeracadaArray is properly initialized
*/
PyObject* teraArrayGetElementAsPyObj ( stdTeraArray* pstiTeracadaArray, int iPosition ) {
  PyObject* pElement = NULL;
  void* pvElement = NULL;

  pvElement = ta_get(pstiTeracadaArray, iPosition);

  if ( ! pvElement ) {
    setPyErrorStr(pstiTeracadaArray);
    goto ERREXIT;
  }

  switch ( ta_getDataType(pstiTeracadaArray) ) {

    case TC_INT:
      pElement = PyLong_FromLong(*((int*) pvElement));
      break;

    case TC_DECIMAL:
      pElement = PyFloat_FromDouble(*((double*) pvElement));
      break;

    case TC_STRING:
      pElement = PyUnicode_FromString((char*) pvElement);
      break;

    default:
      setPyErrorStr(pstiTeracadaArray);
      goto ERREXIT;
  }

  EXIT:
    return pElement;

  ERREXIT:
    return NULL;
}
