#include "teracada_regression.h"


TeracadaLinearRegression::TeracadaLinearRegression ( void ) {

}


TeracadaLinearRegression::~TeracadaLinearRegression ( void ) {

}


bool TeracadaLinearRegression::ordinaryLeastSquares ( TeracadaArray* tcaX, TeracadaArray* tcaY, int *dSlope, int *dIntercept ) {
  double dXMean = 0, dYMean = 0;
  double dCovariance = 0, dVariance = 0;
  double dXi = 0, dYi = 0;

  if ( tcaX->getNumElements() != tcaY->getNumElements() ) {
    goto ERREXIT;
  }

  dXMean = tcaX->mean();
  dYMean = tcaY->mean();

  for ( int iIter = 1; iIter <= tcaX->getNumElements(); iIter++ ) {
    // TODO: Case for data type TC_INT
    dXi = *((double*) tcaX->get(iIter));
    dYi = *((double*) tcaY->get(iIter));
    dCovariance += (dXi - dXMean) * (dYi - dYMean);
    dVariance += (dXi - dXMean) * (dXi - dXMean);
  }

  *dSlope = dCovariance / dVariance;

  *dIntercept = dYMean - *dSlope * dXMean;

  EXIT:
    return true;

  ERREXIT:
    return false;
}


bool TeracadaLinearRegression::linearRegression ( TeracadaArray tcaX, TeracadaArray tcaY, stdLinearRegression* stiLinearRegression ) {


  EXIT:
    return true;

  ERREXIT:
    return false;
}
