// Prefix: teraArray

#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <Python.h>

#include "pyapi_array.h"


PyObject* teraArrayNew ( PyTypeObject* pstiTeraArrayPyObjTypeDef, PyObject* pArgs, PyObject *pKargs ) {
  stdTeraArrayPyObj* pstiSelf;

  pstiSelf = (stdTeraArrayPyObj*) pstiTeraArrayPyObjTypeDef->tp_alloc(pstiTeraArrayPyObjTypeDef, 0);

  if ( ! pstiSelf ) {
    PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for TeracadaArray type object");
    goto ERREXIT;
  }

  EXIT:
    return (PyObject*) pstiSelf;

  ERREXIT:
    return NULL;
}


int teraArrayInit ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs ) {
  pstiSelf->pstiTeraArray = NULL;

  EXIT:
    return 0;
}


PyObject* teraArrayRepr ( stdTeraArrayPyObj* pstiSelf ) {
  stdTeraArray* pstiBuff = NULL;
  PyObject* pUnicodeString = NULL;

  if ( ! (pstiBuff = ta_printToBuff(pstiSelf->pstiTeraArray)) ) {
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  EXIT:
    pUnicodeString = PyUnicode_FromString((char*) ta_getArray(pstiBuff));

    if ( pstiBuff )
      ta_arrayDelete(pstiBuff);

    return pUnicodeString;

  ERREXIT:
    if ( pstiBuff )
      ta_arrayDelete(pstiBuff);

    return NULL;
}


PyObject* teraArrayPyApiDType ( stdTeraArrayPyObj* pstiSelf, PyObject* Py_UNUSED(ignored) ) {
  PyObject* pDataType = NULL;

  if ( ! ta_isInitSuccess(pstiSelf->pstiTeraArray) ) {
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  pDataType = PyLong_FromLong(ta_getDataType(pstiSelf->pstiTeraArray));

  EXIT:
    return pDataType;

  ERREXIT:
    return NULL;
}


PyObject* teraArrayPyApiArray ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs, PyObject* pKargs ) {

  int iDataType = -1;
  PyObject* pElements = NULL;
  stdTeraArray* stiTeraArray = NULL;
  char *pcKargsKeys[] = {"list", "dtype", NULL};

  // PyArg_ParseTupleAndKeywords automatically sets the error string
  PyArg_ParseTupleAndKeywords(pArgs, pKargs, "O|i", pcKargsKeys, &pElements, &iDataType);

  if ( ! PyList_Check(pElements) ) {
    PyErr_SetString(PyExc_TypeError, "Method array() expects a list as the first argument");
    goto ERREXIT;
  }

  if ( ! (iDataType == TC_INT || iDataType == TC_DECIMAL || iDataType == TC_STRING) ) {
    if ( (iDataType = deducePyListDataType(pElements)) < 0 ) {
      PyErr_SetString(PyExc_RuntimeError, "Unable to deduce element list data type");
      goto ERREXIT;
    }
  }

  stiTeraArray = ta_arrayInit(iDataType, PyList_Size(pElements));

  if ( ! ta_isInitSuccess(stiTeraArray) ) {
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  pstiSelf->pstiTeraArray = stiTeraArray;

  if ( ! teraArrayInsertPyList(pstiSelf->pstiTeraArray, pElements) ) {
    // Error string should have already set by teraArrayInsertPyList
    // We call the below one more time just to be safe
    // This will automatically result in RuntimeError if no error originated from TeracadaArray
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  EXIT:
    return teraArrayRepr(pstiSelf);

  ERREXIT:
    return NULL;
}


PyObject* teraArrayPyApiInsert ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs ) {
  PyObject* pElements;
  int iPosition = 0;

  // if ( ! pstiSelf->pstiTeraArray ) {
  //   PyErr_SetString(PyExc_RuntimeError, ta_getErrStr(pstiSelf->pstiTeraArray, ERR_TA_NOT_INIT));
  //   goto ERREXIT;
  // }

  // PyArg_ParseTuple automatically sets the error string
  PyArg_ParseTuple(pArgs, "O|i", &pElements, &iPosition);

  if ( ! PyList_Check(pElements) ){
    PyErr_SetString(PyExc_TypeError, "Method insert() expects a list of array elements as the first argument");
    goto ERREXIT;
  }

  if ( ! teraArrayInsertPyList(pstiSelf->pstiTeraArray, pElements) ) {
    // Error string should have already set by teraArrayInsertPyList
    // We call the below one more time just to be safe
    // This will automatically result in RuntimeError if no error originated from TeracadaArray
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  EXIT:
    return teraArrayRepr(pstiSelf);

  ERREXIT:
    return NULL;
}


PyObject* teraArrayPyApiGet ( stdTeraArrayPyObj* pstiSelf, PyObject* pArgs ) {
  // Get the last array element (pop) as default
  int iPosition = -1;
  PyObject* pElement = NULL;

  // PyArg_ParseTuple automatically sets the error string
  PyArg_ParseTuple(pArgs, "|i", &iPosition);

  pElement = teraArrayGetElementAsPyObj(pstiSelf->pstiTeraArray, iPosition);

  if ( ! pElement ) {
    // Error string should have already set by teraArrayGetElementAsPyObj
    // We call the below one more time just to be safe
    // This will automatically result in RuntimeError if no error originated from TeracadaArray
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  EXIT:
    return pElement;

  ERREXIT:
    return NULL;
}


PyObject* teraArrayPyApiPrint ( stdTeraArrayPyObj* pstiSelf, PyObject* Py_UNUSED(ignored)) {
  stdTeraArray *pstiBuff;

  if ( ! (pstiBuff = ta_printToBuff(pstiSelf->pstiTeraArray )) ) {
    setPyErrorStr(pstiSelf->pstiTeraArray);
    goto ERREXIT;
  }

  EXIT:
    printf("%s\n", (char*) ta_getArray(pstiBuff));

    if ( pstiBuff )
      ta_arrayDelete(pstiBuff);

    Py_RETURN_NONE;

  ERREXIT:
    if ( pstiBuff )
      ta_arrayDelete(pstiBuff);

    return NULL;
}
