#ifndef _TERACADA_DICT_H
#define _TERACADA_DICT_H

#include <teracada_common.h>
#include <teracada_array.h>


class TeracadaDict {
  private:
    tca_dict*    m_ptcaDictRoot;
    tca_int*     m_ptcaInt;
    tca_decimal* m_ptcaDecimal;
    tca_str*     m_ptcaString;

  public:
    TeracadaDict( void );
    ~TeracadaDict( void );

    template <typename tDataTypeKey, typename tDataTypeVal>
    tc_dict addNode ( tDataTypeKey ptKey, tDataTypeVal ptValue );

    template <typename tDataType>
    tc_dict insertNode ( tc_dict pParentNode, tDataType tValue );

    template <typename tDataTypeKey, typename tDataTypeVal>
    tc_dict update ( tDataTypeKey tKey, tDataTypeVal tValue );

    template <typename tDataTypeKey, typename tDataTypeVal>
    tc_dict update ( tc_dict ptcdParentNode, tDataTypeKey tKey, tDataTypeVal tValue );

    tc_void* getNodeKey ( tc_dict ptcdNode );

    tc_void* getNodeValue ( tc_dict ptcdNode );

    template <typename tDataType>
    tc_void* get ( tDataType ptKey, tc_dict ptcdParentNode = nullptr );
};

#endif
