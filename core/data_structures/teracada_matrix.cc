#include <iostream>

#include "teracada_matrix.h"


TeracadaMatrix::TeracadaMatrix ( uint8_t ui8DataType, uint iNumRows, uint iNumCols ) :
  TeracadaArray ( ui8DataType, (iNumRows * iNumCols)),
  m_iNumRows(iNumRows),
  m_iNumCols(iNumCols)
{
}


TeracadaMatrix::~TeracadaMatrix ( void ) {

}


template < typename tDataType >
bool TeracadaMatrix::insertRow ( const tDataType *tValue, tc_int iNumElements, tc_int iRow, bool bOverwrite ) {
  tc_int iRtn = 0;
  tc_int iArrayPosition = 0;

  if ( iRow > (numRows() + 1) ) {
    TC_LOG(LOG_ERR, "TeracadaMatrix::insertRow(): Invalid row number passed as function parameter [ ROW_NUMBER: %d | MATRIX_NUM_TOTAL_ROWS: %d ]", iRow, numRows());
    goto ERREXIT;
  }

  if ( iNumElements != numElementsInRow() ) {
    TC_LOG(LOG_ERR, "TeracadaMatrix::insertRow(): Inconsistent number of elements in the row to be inserted [ NUM_ELEMENTS: %d | MATRIX_NUM_ROW_ELEMENTS: %d ]", iNumElements, numElementsInRow());
    goto ERREXIT;
  }

  iArrayPosition = rowToArrayPosition(iRow);

  iRtn = insert<tDataType>(tValue, iArrayPosition, iNumElements, bOverwrite);

  if ( ! iRtn )
    goto ERREXIT;

  if ( iRow > numRows() || (iRow < numRows() && ! bOverwrite) ) {
    incrementNumRowsBy(1);
  }

  EXIT:
    return true;

  ERREXIT:
    return false;
}

template bool TeracadaMatrix::insertRow < tc_int > ( const tc_int *tValue, tc_int iNumElements, tc_int iRow, bool bOverwrite );
template bool TeracadaMatrix::insertRow < tc_decimal > ( const tc_decimal *tValue, tc_int iNumElements, tc_int iRow, bool bOverwrite );


template < typename tDataType >
bool TeracadaMatrix::removeRow ( const tDataType *tValue, tc_int iRow ) {
  tc_int iRtn = 0;
  tc_int iArrayPosition = 0;

  if ( iRow > numRows() ) {
    TC_LOG(LOG_ERR, "TeracadaMatrix::removeRow(): Invalid row number passed as function parameter [ ROW_NUMBER: %d | MATRIX_NUM_TOTAL_ROWS: %d ]", iRow, numRows());
    goto ERREXIT;
  }

  iArrayPosition = rowToArrayPosition(iRow);

  iRtn = remove(iArrayPosition, numElementsInRow());

  if ( ! iRtn )
    goto ERREXIT;

  EXIT:
    decrementNumRowsBy(1);
    return true;

  ERREXIT:
    return false;
}

template bool TeracadaMatrix::removeRow < tc_int > ( const tc_int *tValue, tc_int iRow );
template bool TeracadaMatrix::removeRow < tc_decimal > ( const tc_decimal *tValue, tc_int iRow );


void TeracadaMatrix::print ( void ) {
  uint iRow = 1;

  switch ( getDataType() ) {
    case TC_INT:
      {
        printf("\n");

        for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
          printf("%d ", *((tc_int *) TeracadaArray::get(iIter)));

          if ( iIter == numElementsInRow() * iRow ) {
            printf("\n");
            iRow++;
          }
        }

        printf("\n\n");
      }

      break;
  }
}
