#ifndef _RCORE_TERACADA_MATRIX_H
#define _RCORE_TERACADA_MATRIX_H


#include <cstdint>


#include "teracada_common.h"
#include "teracada_error.h"
#include "teracada_array.h"


class TeracadaMatrix : public TeracadaArray {
  private:
    int      m_iNumRows;
    int      m_iNumCols;

  public:
    TeracadaMatrix ( uint8_t ui8DataType, uint iNumRows, uint iNumCols );
    ~TeracadaMatrix ( void );

    void *operator () ( uint uiRow, uint uiCol ) {
      return get(uiRow, uiCol);
    }

    int numRows ( void ) const {
      return m_iNumRows;
    }

    int numCols ( void ) const {
      return m_iNumCols;
    }

    int numElementsInRow ( void ) const {
      return m_iNumCols;
    }

    int incrementNumRowsBy ( uint iNumRows = 0 ) {
      return (m_iNumRows += iNumRows);
    }

    int decrementNumRowsBy ( uint iNumRows = 0 ) {
      return (m_iNumRows += iNumRows);
    }

    int rowToArrayPosition ( int iRowPosition = 0 ) {
      return ((((iRowPosition ? iRowPosition : m_iNumRows + 1) - 1) * numElementsInRow()) + 1);
    }

    int rowColToArrayPosition ( uint uiRow, uint uiCol ) {
      return ((uiRow - 1) * numElementsInRow() + uiCol);
    }

    void *get ( uint uiRow, uint uiCol ) {
      return TeracadaArray::get(rowColToArrayPosition(uiRow, uiCol));
    }

    template < typename tDataType >
    bool insertRow ( const tDataType *tValue, int iNumElements, int iRow = 0, bool bOverwrite = false );

    template < typename tDataType >
    bool removeRow ( const tDataType *tValue, int iCol );


    void print ( void );
};

#endif
