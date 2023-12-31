#ifndef _TERACADA_ERROR_H
#define _TERACADA_ERROR_H

#include <exception>

#include "teracada_common.h"
#include "teracada_array.h"


extern char* pcTERACADA_ERRSTR;

#define TC_ERRORSTR_LENGTH 100

#define TC_ERRNO_START -100

#define ERR_TA_RUNTIME -100
#define ERRSTR_TA_RUNTIME "TeracadaArray: Runtime error (ERR_TA_RUNTIME)"

#define ERR_TA_MEMALLOC_FAILED -101
#define ERRSTR_TA_MEMALLOC_FAILED "TeracadaArray: Failed while allocating heap memory (ERR_TA_MEMALLOC_FAILED)"

#define ERR_TA_INVALID_PARAM -102
#define ERRSTR_TA_INVALID_PARAM "TeracadaArray: Invalid parameter value (ERR_TA_INVALID_PARAM)"

#define ERR_TA_NOT_INIT -103
#define ERRSTR_TA_NOT_INIT "TeracadaArray: Not initialized (ERR_TA_NOT_INIT)"

#define ERR_TA_INIT_FAILED -104
#define ERRSTR_TA_INIT_FAILED "TeracadaArray: Initilization failed (ERR_TA_INIT_FAILED)"

#define ERR_TA_INVALID_POSITION_OR_INDEX -105
#define ERRSTR_TA_INVALID_POSITION_OR_INDEX "TeracadaArray: Invalid position or index value (ERR_TA_INVALID_POSITION_OR_INDEX)"

#define ERR_TA_RESIZE_FAILED -106
#define ERRSTR_TA_RESIZE_FAILED "TeracadaArray: Failed to resize the array buffer (ERR_TA_RESIZE_FAILED)"

#define ERR_TA_INVALID_DATA_TYPE -107
#define ERRSTR_TA_INVALID_DATA_TYPE "TeracadaArray: Invalid data type (ERR_TA_INVALID_DATA_TYPE)"

#define ERR_TA_INSERTION_FAILURE -108
#define ERRSTR_TA_INSERTION_FAILURE "TeracadaArray: Array element insertion operation failure (ERR_TA_INSERTION_FAILURE)"

#define ERR_TA_REMOVE_FAILURE -109
#define ERRSTR_TA_REMOVE_FAILURE "TeracadaArray: Array element remove operation failure (ERR_TA_REMOVE_FAILURE)"

#define TC_ERRNO_END -500

class TeracadaException : public std::exception {
  private:
    tc_int m_iErrno;
    tc_str m_pcErrStr;

  public:
    explicit TeracadaException ( tc_int iErrno, tc_str pcErrStr ) :
      m_iErrno(iErrno),
      m_pcErrStr(pcErrStr) { }

    virtual ~TeracadaException ( void ) noexcept { }

    virtual const char* what ( void ) const noexcept override {
      return m_pcErrStr;
    }
};

#endif
