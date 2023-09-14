#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdbool>

#include "teracada_array.h"


EXTERN_C stdTeraArray* ta_arrayInit ( tc_int iDataType, tc_int iNumElements ) {
  return reinterpret_cast<stdTeraArray*>(new TeracadaArray(iDataType, iNumElements));
}


EXTERN_C void ta_arrayDelete ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return;

  delete reinterpret_cast<TeracadaArray*>(pstiTeraArray);
}

EXTERN_C tc_bool ta_isInitSuccess ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->isInitSuccess();
}


EXTERN_C tc_int ta_getDataType ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return -1;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->getDataType();
}


EXTERN_C void* ta_getArray ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return nullptr;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->getArray();
}


EXTERN_C tc_int ta_getNumElements ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return -1;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->getNumElements();
}


EXTERN_C tc_bool ta_insertByte ( stdTeraArray* pstiTeraArray, tc_byte b8Value, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(b8Value, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertInt ( stdTeraArray* pstiTeraArray, tc_int iValue, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(iValue, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertDecimal ( stdTeraArray* pstiTeraArray, tc_decimal dValue, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(dValue, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertChar ( stdTeraArray* pstiTeraArray, tc_char cValue, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(cValue, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertBytes ( stdTeraArray* pstiTeraArray, const tc_byte* pb8Value, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(pb8Value, iLength, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertInts ( stdTeraArray* pstiTeraArray, const tc_int *piValue, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(piValue, iLength, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertDecimals ( stdTeraArray* pstiTeraArray, const tc_decimal *pdValue, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(pdValue, iLength, iPosition, bOverwrite);
}


EXTERN_C tc_bool ta_insertString ( stdTeraArray* pstiTeraArray, const tc_char *pcValue, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(pcValue, iLength, iPosition, bOverwrite);
}


EXTERN_C void* ta_get ( stdTeraArray* pstiTeraArray, tc_int iPosition ) {
  if ( ! pstiTeraArray )
    return nullptr;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->get(iPosition);
}


EXTERN_C tc_int ta_getErrno ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return ERR_TA_NOT_INIT;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->getErrno();
}


EXTERN_C tc_int ta_errnoReset ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return ERR_TA_NOT_INIT;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->setErrno(0);
}


EXTERN_C tc_char* ta_getErrStr ( stdTeraArray* pstiTeraArray, tc_int iErrno ) {
  if ( ! pstiTeraArray ) {
    if ( pcTERACADA_ERRSTR ) {
      memset(pcTERACADA_ERRSTR, 0, TC_ERRORSTR_LENGTH);
      snprintf(pcTERACADA_ERRSTR, TC_ERRORSTR_LENGTH, "%s", ERRSTR_TA_NOT_INIT);
      return pcTERACADA_ERRSTR;
    } else {
      return nullptr;
    }
  }

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->getErrStr(iErrno);
}


EXTERN_C void ta_print ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return;

  return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->print();
}


EXTERN_C stdTeraArray* ta_printToBuff ( stdTeraArray* pstiTeraArray, stdTeraArray* pstiBuff ) {
  if ( ! pstiTeraArray )
    return nullptr;

  return (
    reinterpret_cast<stdTeraArray*>(
      reinterpret_cast<TeracadaArray*>(pstiTeraArray)->printToBuff(reinterpret_cast<TeracadaArray*>(pstiBuff))
    )
  );
}
