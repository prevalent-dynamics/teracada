#ifndef _TERACADA_ARRAY_ERROR_H
#define _TERACADA_ARRAY_ERROR_H

#include "teracada_common.h"
#include "teracada_array.h"

extern char* pcTERACADA_ERRSTR;

#define TC_ERRORSTR_LENGTH 100

#define ERR_TA_RUNTIME -100
#define ERRSTR_TA_RUNTIME "TeracadaArray runtime error (ERR_TA_RUNTIME)"

#define ERR_TA_NOT_INIT -101
#define ERRSTR_TA_NOT_INIT "TeracadaArray not initialized (ERR_TA_NOT_INIT)"

#define ERR_TA_INIT_FAILED -102
#define ERRSTR_TA_INIT_FAILED "TeracadaArray initilization failed (ERR_TA_INIT_FAILED)"

#define ERR_TA_INVALID_POS -103
#define ERRSTR_TA_INVALID_POS "TeracadaArray invalid position (ERR_TA_INVALID_POS)"


#endif
