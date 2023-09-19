#ifndef _TERACADA_DICT_H
#define _TERACADA_DICT_H

#include <teracada_common.h>
#include <teracada_array.h>


class TeracadaDict {
  private:
    tca_dict*    ptcaDictRoot;
    tca_int*     ptcaInt;
    tca_decimal* ptcaDecimal;
    tca_str*     ptcaString;

  public:
    TeracadaDict( void );
    ~TeracadaDict( void );

    template <typename tDataType>
    tc_dict* addNode ( tDataType ptValue );

    template <typename tDataType>
    bool insertNode ( tc_dict* pParentNode, tDataType tValue );
};

#endif
