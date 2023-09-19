#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdbool>

#include "teracada_array.h"

static uint8_t _b8DATA_TYPE = TC_NONE;

EXTERN_C stdTeraArray* ta_arrayInit ( tc_int iDataType, tc_int iNumElements ) {
  if ( ! (iDataType >= TC_BYTE && iDataType <= TC_ARRAY) )
    return nullptr;

  _b8DATA_TYPE = iDataType;

  switch ( _b8DATA_TYPE ) {
    case TC_BYTE:
      return reinterpret_cast<stdTeraArray*>(new TeracadaArray<tc_byte>(iNumElements));

    case TC_INT:
      return reinterpret_cast<stdTeraArray*>(new TeracadaArray<tc_int>(iNumElements));

    case TC_DECIMAL:
      return reinterpret_cast<stdTeraArray*>(new TeracadaArray<tc_decimal>(iNumElements));

    case TC_CHAR:
      return reinterpret_cast<stdTeraArray*>(new TeracadaArray<tc_char>(iNumElements));

    case TC_STRING:
      return reinterpret_cast<stdTeraArray*>(new TeracadaArray<tc_str>(iNumElements));

    default:
      return nullptr;
  }
}


EXTERN_C void ta_arrayDelete ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return;

  switch ( _b8DATA_TYPE ) {
    case TC_BYTE:
      delete reinterpret_cast<TeracadaArray<tc_byte>*>(pstiTeraArray);

    case TC_INT:
      delete reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray);

    case TC_DECIMAL:
      delete reinterpret_cast<TeracadaArray<tc_decimal>*>(pstiTeraArray);

    case TC_CHAR:
      delete reinterpret_cast<TeracadaArray<tc_char>*>(pstiTeraArray);

    case TC_STRING:
      delete reinterpret_cast<TeracadaArray<tc_str>*>(pstiTeraArray);

    default:
      return;
  }
}

EXTERN_C tc_bool ta_isInitSuccess ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return false;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->isInitSuccess();
}


EXTERN_C tc_byte ta_getDataType ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return TC_NONE;

  return _b8DATA_TYPE;
}


EXTERN_C void* ta_getArray ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return nullptr;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->getArray();
}


EXTERN_C tc_int ta_getNumElements ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return -1;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->getNumElements();
}


EXTERN_C tc_bool ta_insertByte ( stdTeraArray* pstiTeraArray, tc_int iPosition, const tc_byte b8Value ) {
  if ( ! pstiTeraArray )
    return false;

  if ( _b8DATA_TYPE != TC_BYTE )
    return false;

  return reinterpret_cast<TeracadaArray<tc_byte>*>(pstiTeraArray)->insert(iPosition, b8Value);
}


EXTERN_C tc_bool ta_insertInt ( stdTeraArray* pstiTeraArray, tc_int iPosition, const tc_int iValue ) {
  if ( ! pstiTeraArray )
    return false;

  if ( _b8DATA_TYPE != TC_INT )
    return false;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->insert(iPosition, iValue);
}


EXTERN_C tc_bool ta_insertDecimal ( stdTeraArray* pstiTeraArray, tc_int iPosition, const tc_decimal dValue ) {
  if ( ! pstiTeraArray )
    return false;

  if ( _b8DATA_TYPE != TC_DECIMAL )
    return false;

  return reinterpret_cast<TeracadaArray<tc_decimal>*>(pstiTeraArray)->insert(iPosition, dValue);
}


// EXTERN_C tc_bool ta_insertBytes ( stdTeraArray* pstiTeraArray, const tc_byte* pb8Value, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
//   if ( ! pstiTeraArray )
//     return false;

//   return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(pb8Value, iLength, iPosition, bOverwrite);
// }


// EXTERN_C tc_bool ta_insertInts ( stdTeraArray* pstiTeraArray, const tc_int *piValue, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
//   if ( ! pstiTeraArray )
//     return false;

//   return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(piValue, iLength, iPosition, bOverwrite);
// }


// EXTERN_C tc_bool ta_insertDecimals ( stdTeraArray* pstiTeraArray, const tc_decimal *pdValue, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
//   if ( ! pstiTeraArray )
//     return false;

//   return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(pdValue, iLength, iPosition, bOverwrite);
// }


// EXTERN_C tc_bool ta_insertString ( stdTeraArray* pstiTeraArray, const tc_char *pcValue, tc_int iLength, tc_int iPosition, tc_bool bOverwrite ) {
//   if ( ! pstiTeraArray )
//     return false;

//   return reinterpret_cast<TeracadaArray*>(pstiTeraArray)->insert(pcValue, iLength, iPosition, bOverwrite);
// }


EXTERN_C void* ta_get ( stdTeraArray* pstiTeraArray, tc_int iPosition ) {
  if ( ! pstiTeraArray )
    return nullptr;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->get(iPosition);
}


EXTERN_C tc_int ta_getErrno ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return ERR_TA_NOT_INIT;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->getErrno();
}


EXTERN_C tc_void ta_errnoReset ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return;

  reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->setErrno(0);
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

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->getErrStr(iErrno);
}


EXTERN_C void ta_print ( stdTeraArray* pstiTeraArray ) {
  if ( ! pstiTeraArray )
    return;

  return reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->print();
}


EXTERN_C stdTeraArray* ta_printToBuff ( stdTeraArray* pstiTeraArray, stdTeraArray* pstiBuff ) {
  if ( ! pstiTeraArray )
    return nullptr;

  return (
    reinterpret_cast<stdTeraArray*>(
      reinterpret_cast<TeracadaArray<tc_int>*>(pstiTeraArray)->printToBuff(reinterpret_cast<TeracadaArray<tc_char>*>(pstiBuff))
    )
  );
}
