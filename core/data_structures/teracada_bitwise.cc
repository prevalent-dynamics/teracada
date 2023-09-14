#include <cstdint>

#include "teracada_array.h"


TeracadaArray *TeracadaArray::bitwiseAND ( TeracadaArray *tcaA, TeracadaArray *tcaB, TeracadaArray *tcaC ) {
  if ( tcaA->getNumArrayElements() != tcaB->getNumArrayElements() ) {
    goto ERREXIT;
  }

  if ( tcaA->getArrayDataType() != TERACADA_DTYPE_INT ) {
    goto ERREXIT;
  }

  if ( ! tcaC ) {
    tcaC = new TeracadaArray(TERACADA_DTYPE_INT, tcaA->getNumArrayElements());
  }

  for ( int iIter = 1; iIter <= tcaA->getNumArrayElements(); iIter++ ) {
    tcaC->insert<int>((*((int *) tcaA->get(iIter)) & *((int *) tcaB->get(iIter))), iIter);
  }

  EXIT:
    return tcaC;

  ERREXIT:
    return nullptr;

}


TeracadaArray *TeracadaArray::bitwiseOR ( TeracadaArray *tcaA, TeracadaArray *tcaB, TeracadaArray *tcaC ) {
  if ( tcaA->getNumArrayElements() != tcaB->getNumArrayElements() ) {
    goto ERREXIT;
  }

  if ( tcaA->getArrayDataType() != TERACADA_DTYPE_INT ) {
    goto ERREXIT;
  }

  if ( ! tcaC ) {
    tcaC = new TeracadaArray(TERACADA_DTYPE_INT, tcaA->getNumArrayElements());
  }

  for ( int iIter = 1; iIter <= tcaA->getNumArrayElements(); iIter++ ) {
    tcaC->insert<int>((*((int *) tcaA->get(iIter)) | *((int *) tcaB->get(iIter))), iIter);
  }

  EXIT:
    return tcaC;

  ERREXIT:
    return nullptr;

}


TeracadaArray *TeracadaArray::bitwiseXOR ( TeracadaArray *tcaA, TeracadaArray *tcaB, TeracadaArray *tcaC ) {
  if ( tcaA->getNumArrayElements() != tcaB->getNumArrayElements() ) {
    goto ERREXIT;
  }

  if ( tcaA->getArrayDataType() != TERACADA_DTYPE_INT ) {
    goto ERREXIT;
  }

  if ( ! tcaC ) {
    tcaC = new TeracadaArray(TERACADA_DTYPE_INT, tcaA->getNumArrayElements());
  }

  for ( int iIter = 1; iIter <= tcaA->getNumArrayElements(); iIter++ ) {
    tcaC->insert<int>((*((int *) tcaA->get(iIter)) ^ *((int *) tcaB->get(iIter))), iIter);
  }

  EXIT:
    return tcaC;

  ERREXIT:
    return nullptr;

}
