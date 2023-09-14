#include <random>
#include <cmath>

#include "teracada_array.h"


template <typename tDataType>
tDataType TeracadaArray::_min ( TeracadaArray *tcaMinValPositions ) {
  tDataType tMinVal = 0;

  if ( ! isInitSuccess() )
    goto ERREXIT;

  for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
    if ( *((tDataType *) m_pvArray + iIter) < tMinVal ) {
      tMinVal = *((tDataType *) m_pvArray + iIter);
    }
  }

  if ( tcaMinValPositions ) {
    for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
      if ( *((tDataType *) m_pvArray + iIter) == tMinVal ) {
        tcaMinValPositions->insert((tc_int) indexToPosition(iIter));
      }
    }
  }

  EXIT:
    return tMinVal;

  ERREXIT:
    return 0;
}

template tc_byte TeracadaArray::_min <tc_byte> ( TeracadaArray * );
template tc_int TeracadaArray::_min <tc_int> ( TeracadaArray * );
template tc_decimal TeracadaArray::_min <tc_decimal> ( TeracadaArray * );


template <typename tDataType>
tDataType TeracadaArray::_max ( TeracadaArray *tcaMinValPositions ) {
  tDataType tMinVal = 0;

  if ( ! isInitSuccess() )
    goto ERREXIT;

  for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
    if ( *((tDataType *) m_pvArray + iIter) > tMinVal ) {
      tMinVal = *((tDataType *) m_pvArray + iIter);
    }
  }

  if ( tcaMinValPositions ) {
    for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
      if ( *((tDataType *) m_pvArray + iIter) == tMinVal ) {
        tcaMinValPositions->insert((tc_int) indexToPosition(iIter));
      }
    }
  }

  EXIT:
    return tMinVal;

  ERREXIT:
    return 0;
}

template tc_byte TeracadaArray::_max <tc_byte> ( TeracadaArray * );
template tc_int TeracadaArray::_max <tc_int> ( TeracadaArray * );
template tc_decimal TeracadaArray::_max <tc_decimal> ( TeracadaArray * );


template <typename tDataType>
tc_decimal TeracadaArray::_mean ( void ) {
  tc_decimal dMean = 0;

  if ( ! isInitSuccess() )
    goto ERREXIT;


  for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
    dMean += *((tDataType *) m_pvArray + iIter);
  }

  dMean = (dMean / getNumElements());

  EXIT:
    return dMean;

  ERREXIT:
    return 0;
}

template tc_decimal TeracadaArray::_mean <tc_byte> (void);
template tc_decimal TeracadaArray::_mean <tc_int> (void);
template tc_decimal TeracadaArray::_mean <tc_decimal> (void);


template <typename tDataType>
tc_decimal TeracadaArray::_variance ( tc_int iDeltaDOF ) {
  tc_decimal dVariance = 0, dMean = 0;

  if ( ! isInitSuccess() )
    goto ERREXIT;

  dMean = mean();

  for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
    dVariance += pow((*((tDataType *) m_pvArray + iIter) - dMean), 2);
  }

  dVariance = (dVariance / (getNumElements() - iDeltaDOF));

  EXIT:
    return dVariance;

  ERREXIT:
    // Return negative for error as variance can never be negative
    return -1;
}

template tc_decimal TeracadaArray::_variance < tc_byte > ( tc_int );
template tc_decimal TeracadaArray::_variance < tc_int > ( tc_int );
template tc_decimal TeracadaArray::_variance < tc_decimal > ( tc_int );


template < typename tDataType >
tc_decimal TeracadaArray::_standardDeviation ( tc_int iDeltaDOF ) {
  tc_decimal dVariance = 0, dSD = 0;

  if ( ! isInitSuccess() )
    goto ERREXIT;

  dVariance = _variance<tDataType>(iDeltaDOF);

  dSD = sqrt(dVariance);

  EXIT:
    return dSD;

  ERREXIT:
    // Return negative for error as SD can never be negative
    return -1;
}

template tc_decimal TeracadaArray::_standardDeviation < tc_byte > ( tc_int );
template tc_decimal TeracadaArray::_standardDeviation < tc_int > ( tc_int );
template tc_decimal TeracadaArray::_standardDeviation < tc_decimal > ( tc_int );


template < typename tDataType >
tc_decimal TeracadaArray::_range ( void ) {
  tc_decimal dRange = -1;

  if ( ! isInitSuccess() )
    goto ERREXIT;

  dRange = _max<tDataType>() - _min<tDataType>();

  EXIT:
    return dRange;

  ERREXIT:
    return -1;
}

template tc_decimal TeracadaArray::_range < tc_byte > ( void );
template tc_decimal TeracadaArray::_range < tc_int > ( void );
template tc_decimal TeracadaArray::_range < tc_decimal > ( void );


tc_bool TeracadaArray::arrayInitRandomUniformDist ( tc_int iLow, tc_int iHigh, tc_int iSeed ) {
  std::mt19937 randomGenerator;

  if ( ! iSeed ) {
    std::random_device randomDevice;
    randomGenerator.seed(randomDevice());

  } else {
    randomGenerator.seed(iSeed);
  }

  std::uniform_int_distribution uniformDistribution(iLow, iHigh);

  for ( tc_int iIter = 1; iIter <= m_iMaxNumArrayElements; iIter++ ) {
    insert(uniformDistribution(randomGenerator));
  }

  EXIT:
    return true;

  ERREXIT:
    return false;
}


tc_bool TeracadaArray::arrayInitRandomUniformDist ( tc_decimal dLow, tc_decimal dHigh, tc_int iSeed ) {
  std::mt19937 randomGenerator;

  if ( ! iSeed ) {
    std::random_device randomDevice;
    randomGenerator.seed(randomDevice());

  } else {
    randomGenerator.seed(iSeed);
  }

  std::uniform_real_distribution uniformDistribution(dLow, dHigh);

  for ( tc_int iIter = 1; iIter <= m_iMaxNumArrayElements; iIter++ ) {
    insert(uniformDistribution(randomGenerator));
  }

  EXIT:
    return true;

  ERREXIT:
    return false;
}
