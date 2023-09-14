#include <cstdio>
#include <cstring>

#include "teracada_error.h"


tc_char* TeracadaArray::getErrStr ( tc_int iErrno ) {
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

    case ERR_TA_INVALID_POS:
      snprintf(pcTERACADA_ERRSTR, TC_ERRORSTR_LENGTH, "%s", ERRSTR_TA_INVALID_POS);
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
