#define PY_SSIZE_T_CLEAN

#include <stdio.h>
#include <Python.h>

#include "pyapi_array.h"
#include "pyapi_regression.h"


/* Allocate buffer for global Teracada error strings */

static char acTERACADA_ERRSTR_BUFFER[TC_ERRORSTR_LENGTH];
char* pcTERACADA_ERRSTR = &acTERACADA_ERRSTR_BUFFER[0];

static PyModuleDef stiTERACADA_MODULE_DEF = {
  PyModuleDef_HEAD_INIT,
  .m_name = "Teracada",
  .m_doc = "Teracada: __doc__",
  .m_size = -1,
};


/* ********** Teracada array definitions (start) ********** */

static PyObject* pOBJ_TERA_ARRAY = NULL;

static PyMethodDef stiTERA_ARRAY_METHODS[] = {
  {
    "dtype",
    (PyCFunction) teraArrayPyApiDType,
    METH_NOARGS,
    "dtype(): __doc__"
  },

  {
    "array",
    (PyCFunction) teraArrayPyApiArray,
    METH_VARARGS | METH_KEYWORDS,
    "array(): __doc__"
  },

  {
    "insert",
    (PyCFunction) teraArrayPyApiInsert,
    METH_VARARGS,
    "insert(): __doc__"
  },

  {
    "get",
    (PyCFunction) teraArrayPyApiGet,
    METH_VARARGS,
    "get(): __doc__"
  },

  {
    "print",
    (PyCFunction) teraArrayPyApiPrint,
    METH_VARARGS,
    "print(): __doc__"
  },

  {
    NULL 
  }
};


static PyTypeObject stiTERA_ARRAY_OBJ_TYPEDEF = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "TeracadaArray",
  .tp_basicsize = sizeof(stdTeraArrayPyObj),
  .tp_itemsize = 0,
  .tp_repr = (reprfunc) teraArrayRepr,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("TeracadaArray: __doc__"),
  .tp_methods = stiTERA_ARRAY_METHODS,
  .tp_init = (initproc) teraArrayInit,
  .tp_new = (newfunc) teraArrayNew
};

/* ********** Teracada array definitions (end) ********** */


/* ********** Teracada regression definitions (start) ********** */

static PyMethodDef stiTERA_LREG_METHODS [] = {
  {
    "fit",
    (PyCFunction) teraLRegPyApiFit,
    METH_VARARGS,
    "fit(): __doc__"
  },

  {
    NULL
  }
};

static PyTypeObject stiTERA_LREG_OBJ_TYPEDEF = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "LinearRegression",
  .tp_basicsize = sizeof(stdTeraLRegPyObj),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("LinearRegression: __doc__"),
  .tp_methods = stiTERA_LREG_METHODS,
  .tp_init = (initproc) teraLRegInitPyObj,
  .tp_new = (newfunc) teraLRegNewPyObj,
};

/* ********** Teracada regression definitions (end) ********** */

PyMODINIT_FUNC PyInit_Teracada ( void ) {
  PyObject* pObjTeracadaModule = NULL;

  pObjTeracadaModule = PyModule_Create(&stiTERACADA_MODULE_DEF);

  if ( ! pObjTeracadaModule )
    goto ERREXIT;

  if ( PyType_Ready(&stiTERA_ARRAY_OBJ_TYPEDEF) < 0 )
    goto ERREXIT;

  Py_INCREF(&stiTERA_ARRAY_OBJ_TYPEDEF);

  pOBJ_TERA_ARRAY = teraArrayNew(&stiTERA_ARRAY_OBJ_TYPEDEF, NULL, NULL);

  if ( ! pOBJ_TERA_ARRAY )
    goto ERREXIT;

  Py_INCREF(pOBJ_TERA_ARRAY);

  if ( PyModule_AddObject(pObjTeracadaModule, "TeracadaArray", pOBJ_TERA_ARRAY) < 0 )
    goto ERREXIT;

  if ( PyType_Ready(&stiTERA_LREG_OBJ_TYPEDEF) < 0 )
    goto ERREXIT;

  Py_INCREF(&stiTERA_LREG_OBJ_TYPEDEF);

  if ( PyModule_AddObject(pObjTeracadaModule, "LinearRegression", (PyObject*) &stiTERA_LREG_OBJ_TYPEDEF) < 0 )
    goto ERREXIT;

  EXIT:
    return pObjTeracadaModule;

  ERREXIT:
    Py_XDECREF(pOBJ_TERA_ARRAY);
    Py_XDECREF(&stiTERA_LREG_OBJ_TYPEDEF);
    Py_XDECREF(&stiTERA_LREG_OBJ_TYPEDEF);
    Py_XDECREF(pObjTeracadaModule);
    return NULL;
}
