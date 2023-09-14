#include "teracada_regression.h"


EXTERN_C stdTeraLReg* tr_objInit () {
  return reinterpret_cast<stdTeraLReg*>(new TeracadaLinearRegression());
}