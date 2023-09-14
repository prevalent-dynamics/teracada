#ifndef _RCORE_NUMC_ARRAY_H
#define _RCORE_NUMC_ARRAY_H

#include <cstdint>

#include "teracada_common.h"
#include "teracada_error.h"

enum {
  TA_RESIZE_ALGO_5PERCENT,
  TA_RESIZE_ALGO_10PERCENT,
  TA_RESIZE_ALGO_STATIC10,
  TA_RESIZE_ALGO_STATIC100,
  TA_RESIZE_ALGO_STATIC1000
};

#define TA_REALLOC_ATTEMPTS_DUMP_FPATH     "/dev/shm/teracada_realloc_attempts"


class TeracadaArray {
  private:
    // Teracada array data type
    tc_byte           m_b8DataType;
    tc_byte           m_b8DataTypeSize;

    // Main array pointers
    void*             m_pvArray;

    // If Teracada array has been successfully initialized
    tc_bool           m_iIsInitSuccess;

    // Number of elements in the array (m_iArrayLastIndex + 1)
    tc_int            m_iMaxNumArrayElements;

    // Actual last used index of array starting from 0
    // Maximum value of m_iArrayLastIndex = m_iMaxNumArrayElements - 1
    // For TERACADA_DTYPE_CHAR/m_pcArray it does not include the null character
    // Value should only be updated after successfull insert/remove operation
    tc_int            m_iArrayLastIndex;

    // The resize/padding algorithm to use
    tc_byte           m_b8ResizeAlgo;
    tc_byte           m_b8ResizePaddingAlgo;

    // Total number of reallocation attempts
    tc_int            m_uliTotalReallocAttempts;

    // Value 0 represents no error
    tc_int            m_iErrno;


  protected:
    tc_byte getElementSize ( void ) const {
      return m_b8DataTypeSize;
    }

    tc_byte setElementSize ( tc_byte b8ArrayElementSize ) {
      return ( m_b8DataTypeSize = b8ArrayElementSize );
    }

    void* setArray ( void* pvArrayAllocMemBlock) {
      return (m_pvArray = pvArrayAllocMemBlock);
    }

    void setArrayInitSuccess () {
      m_iIsInitSuccess = true;
    }

    void setArrayInitFailure () {
      m_iIsInitSuccess = false;
    }

    tc_int setMaxNumElements ( tc_int iMaxNumElements ) {
      return (m_iMaxNumArrayElements = iMaxNumElements);
    }

    void incrementTotalReallocAttempts ( void ) {
      m_uliTotalReallocAttempts ++;
    }

    template <typename tDataType>
    tc_bool validateTypeSafety ( void );

    tc_int positionToIndex ( tc_int iPosition );

    tc_bool resize ( tc_int iNumElements = 0 );
    tc_int resizeIfRequired ( tc_int iInsertIndex, tc_int uiNumElements, tc_bool overwrite);

    template <typename tDataType>
    tc_bool _remove ( tc_int iPosition, tc_int uiNumElements );


  public:
    TeracadaArray ( tc_byte b8DataType, tc_int iNumElements = 100 );
    ~TeracadaArray();

    tc_byte getDataType ( void ) const {
      return m_b8DataType;
    }

    void* getArray ( void ) const {
      return m_pvArray;
    }

    tc_int getLastElementIndex ( void ) const {
      return m_iArrayLastIndex;
    }

    tc_int setLastElementIndex ( tc_int iLastIndex ) {
      return (m_iArrayLastIndex = iLastIndex);
    }

    tc_int incrementLastElementIndexBy ( tc_int iNumElements = 1 ) {
      return (m_iArrayLastIndex += iNumElements);
    }

    tc_int decrementLastElementIndexBy ( tc_int iNumElements = 1 ) {
      return (m_iArrayLastIndex -= iNumElements);
    }

    tc_int getNumElements ( void ) const {
      return m_iArrayLastIndex + 1;
    }

    void setNumElements ( tc_int iArrayElements ) {
      if ( iArrayElements <= m_iMaxNumArrayElements ) {
        m_iArrayLastIndex = iArrayElements - 1;
      }
    }

    tc_int getMaxNumElements ( void ) const {
      return m_iMaxNumArrayElements;
    }

    tc_int getLastIndex ( void ) const {
      return m_iMaxNumArrayElements - 1;
    }

    tc_int getArraySize ( void ) const {
      return (m_iMaxNumArrayElements * m_b8DataTypeSize);
    }

    tc_byte getResizeAlgo ( void ) const {
      return m_b8ResizeAlgo;
    }

    tc_byte setResizeAlgo ( tc_byte b8ResizeAlgo ) {
      return (m_b8ResizeAlgo = b8ResizeAlgo);
    }

    tc_byte getResizePaddingAlgo ( void ) const {
      return m_b8ResizePaddingAlgo;
    }

    tc_byte setResizePaddingAlgo ( tc_byte b8PaddingAlgo ) {
      return (m_b8ResizePaddingAlgo = b8PaddingAlgo);
    }

    tc_int getTotalReallocAttempts ( void ) const {
      return m_uliTotalReallocAttempts;
    }

    tc_int getErrno ( void ) const {
      return m_iErrno;
    }

    tc_int setErrno ( tc_int iErrno ) {
      return (m_iErrno = iErrno);
    }

    tc_bool isInitSuccess ( void ) const {
      return m_iIsInitSuccess;
    };

    tc_int indexToPosition ( tc_int iIndex ) const {
      return iIndex + 1;
    }

    template <typename tDataType>
    tc_bool insert ( tDataType tValue, tc_int iPosition = 0, tc_bool bOverwrite = false );

    template <typename tDataType>
    tc_bool insert ( const tDataType* tValue, tc_int iLength = 0, tc_int iPosition = 0, tc_bool bOverwrite = false );

    // tc_bool insert ( const tc_char* pcVal, tc_int iPosition, tc_int uiMaxLenVal, ... );

    tc_bool remove ( tc_int iPosition = 0, tc_int uiNumElements = 1 );

    void reset ( void );

    void* get ( tc_int iPosition = 1 );

    void print ( void );

    TeracadaArray* printToBuff ( TeracadaArray *ptcaBuff = nullptr );


    /* Function declarations for teracada_error.cc */

    tc_char* getErrStr ( tc_int iErrno = 0 );


    /* Function declarations for (teracada_stats.cc) */

    template <typename tDataType>
    tDataType _min ( TeracadaArray *tcaMinValPositions = nullptr );

    inline tc_decimal min ( TeracadaArray *tcaMinValPositions = nullptr ) {
      switch ( getDataType() ) {
        case TC_BYTE:
          return _min<tc_byte>(tcaMinValPositions);

        case TC_INT:
          return _min<tc_int>(tcaMinValPositions);

        case TC_DECIMAL:
          return _min<tc_decimal>(tcaMinValPositions);

        default:
          return 0;
      }
    }

    template <typename tDataType>
    tDataType _max ( TeracadaArray *tcaMinValPositions = nullptr );

    inline tc_decimal max ( TeracadaArray *tcaMaxValPositions = nullptr ) {
      switch ( getDataType() ) {
        case TC_BYTE:
          return _max<tc_byte>(tcaMaxValPositions);

        case TC_INT:
          return _max<tc_int>(tcaMaxValPositions);

        case TC_DECIMAL:
          return _max<tc_decimal>(tcaMaxValPositions);

        default:
          return 0;
      }
    }

    template <typename tDataType>
    tc_decimal _mean ( void );

    inline tc_decimal mean ( void ) {
      switch ( getDataType() ) {
        case TC_BYTE:
          return _mean<tc_byte>();

        case TC_INT:
          return _mean<tc_int>();

        case TC_DECIMAL:
          return _mean<tc_decimal>();

        default:
          return 0;
      }
    }

    template < typename tDataType >
    tc_decimal _variance ( tc_int iDeltaDOF = 0 );

    inline tc_decimal variance ( tc_int iDeltaDOF = 0 ) {
      switch ( getDataType() ) {
        case TC_BYTE:
          return _variance<tc_byte>(iDeltaDOF);

        case TC_INT:
          return _variance<tc_int>(iDeltaDOF);

        case TC_DECIMAL:
          return _variance<tc_decimal>(iDeltaDOF);

        default:
          return -1;
      }
    }

    template < typename tDataType >
    tc_decimal _standardDeviation ( tc_int iDeltaDOF = 0 );

    inline tc_decimal standardDeviation ( tc_int iDeltaDOF = 0 ) {
      switch ( getDataType() ) {
        case TC_BYTE:
          return _standardDeviation<tc_byte>(iDeltaDOF);

        case TC_INT:
          return _standardDeviation<tc_int>(iDeltaDOF);

        case TC_DECIMAL:
          return _standardDeviation<tc_decimal>(iDeltaDOF);

        default:
          return -1;
      }
    }

    template < typename tDataType >
    tc_decimal _range ( void );

    inline tc_decimal range ( void ) {
      switch ( getDataType() ) {
        case TC_BYTE:
          return _range<tc_byte>();

        case TC_INT:
          return _range<tc_int>();

        case TC_DECIMAL:
          return _range<tc_decimal>();

        default:
          return -1;
      }
    }

    tc_bool arrayInitRandomUniformDist ( tc_int iLow, tc_int iHigh, tc_int iSeed = 0 );
    tc_bool arrayInitRandomUniformDist ( tc_decimal dLow, tc_decimal dHigh, tc_int iSeed = 0 );

    /* Functions declarations for (teracada_bitwise.cc) */

    TeracadaArray *bitwiseAND ( TeracadaArray *tcaA, TeracadaArray *tcaB, TeracadaArray *tcaC = nullptr );
    TeracadaArray *bitwiseOR ( TeracadaArray *tcaA, TeracadaArray *tcaB, TeracadaArray *tcaC = nullptr );
    TeracadaArray *bitwiseXOR ( TeracadaArray *tcaA, TeracadaArray *tcaB, TeracadaArray *tcaC = nullptr );
};


struct stdTeracadaArrayClass;
typedef struct stdTeracadaArrayClass stdTeraArray;

/* TeracadaArray C-API prefix: teraArray from now onwards (including pyapi) */

EXTERN_C stdTeraArray* ta_arrayInit ( tc_int iDataType, tc_int iNumElements );
EXTERN_C void ta_arrayDelete ( stdTeraArray* pstiTerracadaArray );
EXTERN_C tc_bool ta_isInitSuccess ( stdTeraArray* pstiTeracadaArray );

EXTERN_C tc_int ta_getDataType ( stdTeraArray* pstiTeracadaArray );
EXTERN_C void* ta_getArray ( stdTeraArray* pstiTeracadaArray );
EXTERN_C tc_int ta_getNumElements ( stdTeraArray* pstiTeracadaArray );

EXTERN_C tc_bool ta_insertByte ( stdTeraArray* pstiTeracadaArray, tc_byte b8Value, tc_int iPosition = 0, tc_bool bOverwrite = false );
EXTERN_C tc_bool ta_insertInt ( stdTeraArray* pstiTeracadaArray, tc_int iValue, tc_int iPosition = 0, tc_bool bOverwrite = false );
EXTERN_C tc_bool ta_insertDecimal ( stdTeraArray* pstiTeracadaArray, tc_decimal dValue, tc_int iPosition = 0, tc_bool bOverwrite = false );
EXTERN_C tc_bool ta_insertChar ( stdTeraArray* pstiTeracadaArray, tc_char cValue, tc_int iPosition = 0, tc_bool bOverwrite = false );

EXTERN_C tc_bool ta_insertBytes ( stdTeraArray* pstiTeracadaArray, const tc_byte* pb8Value, tc_int iLength = 0, tc_int iPosition = 0, tc_bool bOverwrite = false );
EXTERN_C tc_bool ta_insertInts ( stdTeraArray* pstiTeracadaArray, const tc_int *piValue, tc_int iLength = 0, tc_int iPosition = 0, tc_bool bOverwrite = false );
EXTERN_C tc_bool ta_insertDecimals ( stdTeraArray* pstiTeracadaArray, const tc_decimal *pdValue, tc_int iLength = 0, tc_int iPosition = 0, tc_bool bOverwrite = false );
EXTERN_C tc_bool ta_insertString ( stdTeraArray* pstiTeracadaArray, const tc_char *pcValue, tc_int iLength = 0, tc_int iPosition = 0, tc_bool bOverwrite = false );

EXTERN_C void* ta_get ( stdTeraArray* pstiTeracadaArray, tc_int iPosition );

EXTERN_C tc_int ta_getErrno ( stdTeraArray* pstiTeracadaArray );
EXTERN_C tc_int ta_errnoReset ( stdTeraArray* pstiTeracadaArray );
EXTERN_C tc_char* ta_getErrStr ( stdTeraArray* pstiTeracadaArray, tc_int iErrno = 0 );

EXTERN_C void ta_print ( stdTeraArray* pstiTeracadaArray );
EXTERN_C stdTeraArray* ta_printToBuff ( stdTeraArray* pstiTeracadaArray, stdTeraArray *pstiBuff = NULL );


#endif
