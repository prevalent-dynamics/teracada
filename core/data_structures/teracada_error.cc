#include <cstdio>
#include <cstring>

#include "teracada_error.h"


template <typename tDataType>
tc_char* TeracadaArray<tDataType>::getErrStr ( tc_int iErrno ) {
  if ( ! iErrno )
    iErrno = getErrno();

  if ( ! pcTERACADA_ERRSTR )
    goto ERREXIT;

  memset(pcTERACADA_ERRSTR, 0, TC_ERRORSTR_LENGTH);

  switch ( iErrno ) {

    case ERR_TA_NOT_INIT:
      snprintf(pcTERACADA_ERRSTR, TC_ERRORSTR_LENGTH, "%s", ERRSTR_TA_NOT_INIT);
      break;
  
    case ERR_TA_INIT_FAILED:
      snprintf(pcTERACADA_ERRSTR, TC_ERRORSTR_LENGTH, "%s", ERRSTR_TA_INIT_FAILED);
      break;

    case ERR_TA_INVALID_POSITION_OR_INDEX:
      snprintf(pcTERACADA_ERRSTR, TC_ERRORSTR_LENGTH, "%s", ERRSTR_TA_INVALID_POSITION_OR_INDEX);
      break;

    default:
      snprintf(pcTERACADA_ERRSTR, TC_ERRORSTR_LENGTH, "%s", ERRSTR_TA_RUNTIME);
      break;
  }

  EXIT:
    return pcTERACADA_ERRSTR;

  ERREXIT:
    return NULL;
}

// We don't overwrite previously set error
template <typename tDataType>
tc_void TeracadaArray<tDataType>::throwException ( tc_int iErrno ) {
  // Error numbers are negative
  if ( ! (iErrno <= TC_ERRNO_START && iErrno >= TC_ERRNO_END) )
    setErrno(ERR_TA_RUNTIME);
  else
    setErrno(iErrno);

  // Use getErrno() and getErrStr() so that we don't overwrite previous set error
  if ( isExceptionsEnabled() )
    throw TeracadaException(getErrno(), getErrStr());

  return;
}
