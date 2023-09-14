#ifndef _TERACADA_REGRESSION_H
#define _TERACADA_REGRESSION_H

#include "teracada_common.h"
#include "teracada_error.h"
#include "teracada_array.h"


typedef struct {
  double *dSlope;
  double *dIntercept;
} stdLinearRegression;


class TeracadaLinearRegression {
  private:

  public:
    TeracadaLinearRegression ( void );
    ~TeracadaLinearRegression ( void );

  bool ordinaryLeastSquares ( TeracadaArray* tcaX, TeracadaArray* tcaY, int *dSlope, int *dIntercept );
  bool linearRegression ( TeracadaArray tcaX, TeracadaArray tcaY, stdLinearRegression* stiLinearRegression );
};

struct stdTeracadaLinearRegressionClass;
typedef struct stdTeracadaLinearRegressionClass stdTeraLReg;

#endif
