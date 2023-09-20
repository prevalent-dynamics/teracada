#include <type_traits>

#include <stdio.h>

#include <teracada_dict.h>


TeracadaDict::TeracadaDict ( void ) :
  m_ptcaDictRoot(nullptr),
  m_ptcaInt(nullptr),
  m_ptcaDecimal(nullptr),
  m_ptcaString(nullptr)
{
  m_ptcaDictRoot = new TeracadaArray<tc_dict>(10);
  m_ptcaInt = new TeracadaArray<tc_int>(10);
  m_ptcaDecimal = new TeracadaArray<tc_decimal>(10);
  m_ptcaString = new TeracadaArray<tc_str>(10);
}


TeracadaDict::~TeracadaDict ( void ) {

}

template <typename tDataTypeKey, typename tDataTypeVal>
tc_dict TeracadaDict::addNode ( tDataTypeKey ptKey, tDataTypeVal ptValue ) {
  tc_dict ptcdNewNode = (tc_dict) malloc(sizeof(tc_dict));

  ptcdNewNode->b8DataTypeKey = TC_NONE;
  ptcdNewNode->iArrayPosKey = TA_NONE_INDEX;
  ptcdNewNode->b8DataTypeVal = TC_NONE;
  ptcdNewNode->iArrayPosVal = TA_NONE_INDEX;
  ptcdNewNode->ptcaNext = nullptr;


  if constexpr ( std::is_same_v<tDataTypeKey, tc_int> ) {
    ptcdNewNode->b8DataTypeKey = TC_INT;
    ptcdNewNode->iArrayPosKey = m_ptcaInt->insertBack(ptKey);

    if ( ptcdNewNode->iArrayPosKey < 0 )
      goto ERREXIT;
  }

  if constexpr ( std::is_same_v<tDataTypeKey, tc_decimal> ) {
    ptcdNewNode->b8DataTypeKey = TC_DECIMAL;
    ptcdNewNode->iArrayPosKey = m_ptcaDecimal->insertBack(ptKey);

    if ( ptcdNewNode->iArrayPosKey < 0 )
      goto ERREXIT;
  }

  if constexpr ( std::is_same_v<tDataTypeKey, tc_str> ) {
    tc_str ptKeyHeap = (tc_str) malloc(strlen(ptKey) + 1);

    if ( ! ptKeyHeap )
      goto ERREXIT;

    strcpy(ptKeyHeap, ptKey);

    ptcdNewNode->b8DataTypeKey = TC_STRING;
    ptcdNewNode->iArrayPosKey = m_ptcaString->insertBack(ptKeyHeap);

    if ( ptcdNewNode->iArrayPosKey < 0 )
      goto ERREXIT;
  }

  if constexpr ( std::is_same_v<tDataTypeVal, tc_int> ) {
    ptcdNewNode->b8DataTypeVal = TC_INT;
    ptcdNewNode->iArrayPosVal = m_ptcaInt->insertBack(ptValue);

    if ( ptcdNewNode->iArrayPosVal < 0 )
      goto ERREXIT;
  }

  if constexpr ( std::is_same_v<tDataTypeVal, tc_decimal> ) {
    ptcdNewNode->b8DataTypeVal = TC_DECIMAL;
    ptcdNewNode->iArrayPosVal = m_ptcaDecimal->insertBack(ptValue);

    if ( ptcdNewNode->iArrayPosVal < 0 )
      goto ERREXIT;
  }

  if constexpr ( std::is_same_v<tDataTypeVal, tc_str> ) {
    tc_str ptValueHeap = (tc_str) malloc(strlen(ptValue) + 1);

    if ( ! ptValueHeap )
      goto ERREXIT;

    strcpy(ptValueHeap, ptValue);

    ptcdNewNode->b8DataTypeVal = TC_STRING;
    ptcdNewNode->iArrayPosVal = m_ptcaString->insertBack(ptValueHeap);

    if ( ptcdNewNode->iArrayPosVal < 0 )
      goto ERREXIT;
  }

  EXIT:
    return ptcdNewNode;

  ERREXIT:
    return nullptr;
}

template tc_dict TeracadaDict::addNode<tc_int, tc_int> ( tc_int ptKey, tc_int ptValue );
template tc_dict TeracadaDict::addNode<tc_int, tc_decimal> ( tc_int ptKey, tc_decimal ptValue );
template tc_dict TeracadaDict::addNode<tc_int, tc_str> ( tc_int ptKey, tc_str ptValue );

template tc_dict TeracadaDict::addNode<tc_decimal, tc_int> ( tc_decimal ptKey, tc_int ptValue );
template tc_dict TeracadaDict::addNode<tc_decimal, tc_decimal> ( tc_decimal ptKey, tc_decimal ptValue );
template tc_dict TeracadaDict::addNode<tc_decimal, tc_str> ( tc_decimal ptKey, tc_str ptValue );

template tc_dict TeracadaDict::addNode<tc_str, tc_int> ( tc_str ptKey, tc_int ptValue );
template tc_dict TeracadaDict::addNode<tc_str, tc_decimal> ( tc_str ptKey, tc_decimal ptValue );
template tc_dict TeracadaDict::addNode<tc_str, tc_str> ( tc_str ptKey, tc_str ptValue );


// template <typename tDataType>
// tc_dict TeracadaDict::insertNode ( tc_dict pParentNode, tDataType tValue ) {
//   tc_dict ptcdNewNode = nullptr;

//   if ( ! pParentNode ) {
//     ptcdNewNode = addNode(tValue);

//     if ( ! ptcdNewNode )
//       goto ERREXIT;

//     if ( ! m_ptcaDictRoot->insertBack(ptcdNewNode, 1) )
//       goto ERREXIT;

//     goto EXIT;
//   }

//   while ( true ) {
//     if ( pParentNode->ptcaNext ) {
//       insertNode(pParentNode->ptcaNext, tValue);
//       continue;
//     }

//     ptcdNewNode = addNode(tValue);

//     if ( ! ptcdNewNode )
//       goto ERREXIT;

//     pParentNode->ptcaNext = ptcdNewNode;
//     break;
//   }

//   EXIT:
//     return ptcdNewNode;

//   ERREXIT:
//     return nullptr;
// }

// template tc_dict TeracadaDict::insertNode<tc_int> ( tc_dict pParentNode, tc_int tValue );
// template tc_dict TeracadaDict::insertNode<tc_decimal> ( tc_dict pParentNode, tc_decimal tValue );
// template tc_dict TeracadaDict::insertNode<tc_str> ( tc_dict pParentNode, tc_str tValue );


template <typename tDataTypeKey, typename tDataTypeVal>
tc_dict TeracadaDict::update ( tDataTypeKey tKey, tDataTypeVal tValue ) {
  tc_dict ptcdNewNode = nullptr;

  ptcdNewNode = addNode(tKey, tValue);

  if ( ! ptcdNewNode )
    goto ERREXIT;

  m_ptcaDictRoot->insertBack(ptcdNewNode);

  EXIT:
    return ptcdNewNode;

  ERREXIT:
    return nullptr;
}


template tc_dict TeracadaDict::update<tc_int, tc_int> ( tc_int tKey, tc_int tValue );
template tc_dict TeracadaDict::update<tc_int, tc_decimal> ( tc_int tKey, tc_decimal tValue );
template tc_dict TeracadaDict::update<tc_int, tc_str> ( tc_int tKey, tc_str tValue );

template tc_dict TeracadaDict::update<tc_decimal, tc_int> ( tc_decimal tKey, tc_int tValue );
template tc_dict TeracadaDict::update<tc_decimal, tc_decimal> ( tc_decimal tKey, tc_decimal tValue );
template tc_dict TeracadaDict::update<tc_decimal, tc_str> ( tc_decimal tKey, tc_str tValue );

template tc_dict TeracadaDict::update<tc_str, tc_int> ( tc_str tKey, tc_int tValue );
template tc_dict TeracadaDict::update<tc_str, tc_decimal> ( tc_str tKey, tc_decimal tValue );
template tc_dict TeracadaDict::update<tc_str, tc_str> ( tc_str tKey, tc_str tValue );


template <typename tDataTypeKey, typename tDataTypeVal>
tc_dict TeracadaDict::update ( tc_dict ptcdParentNode, tDataTypeKey tKey, tDataTypeVal tValue ) {
  tc_dict ptcdNewNode = nullptr;

  ptcdNewNode = addNode(tKey, tValue);

  if ( ! ptcdParentNode->ptcaNext )
    ptcdParentNode->ptcaNext = new TeracadaArray<tc_dict>(1);

  ptcdParentNode->ptcaNext->insertBack(ptcdNewNode);


  EXIT:
    return ptcdNewNode;

  ERREXIT:
    return nullptr;
}

template tc_dict TeracadaDict::update<tc_int, tc_int> ( tc_dict ptcdParentNode, tc_int tKey, tc_int tValue );
template tc_dict TeracadaDict::update<tc_int, tc_decimal> ( tc_dict ptcdParentNode, tc_int tKey, tc_decimal tValue );
template tc_dict TeracadaDict::update<tc_int, tc_str> ( tc_dict ptcdParentNode, tc_int tKey, tc_str tValue );

template tc_dict TeracadaDict::update<tc_decimal, tc_int> ( tc_dict ptcdParentNode, tc_decimal tKey, tc_int tValue );
template tc_dict TeracadaDict::update<tc_decimal, tc_decimal> ( tc_dict ptcdParentNode, tc_decimal tKey, tc_decimal tValue );
template tc_dict TeracadaDict::update<tc_decimal, tc_str> ( tc_dict ptcdParentNode, tc_decimal tKey, tc_str tValue );

template tc_dict TeracadaDict::update<tc_str, tc_int> ( tc_dict ptcdParentNode, tc_str tKey, tc_int tValue );
template tc_dict TeracadaDict::update<tc_str, tc_decimal> ( tc_dict ptcdParentNode, tc_str tKey, tc_decimal tValue );
template tc_dict TeracadaDict::update<tc_str, tc_str> ( tc_dict ptcdParentNode, tc_str tKey, tc_str tValue );


tc_void* TeracadaDict::getNodeKey ( tc_dict ptcdNode ) {
  tc_void* pvKey = nullptr;

  switch ( ptcdNode->b8DataTypeKey ) {
    case TC_INT:
      pvKey = m_ptcaInt->get(ptcdNode->iArrayPosKey);
      break;

    case TC_DECIMAL:
      pvKey = m_ptcaDecimal->get(ptcdNode->iArrayPosKey);
      break;

    case TC_STRING:
      pvKey = m_ptcaString->get(ptcdNode->iArrayPosKey);
      break;

    default:
      goto ERREXIT;

  }

  EXIT:
    return pvKey;

  ERREXIT:
    return nullptr;
}


tc_void* TeracadaDict::getNodeValue ( tc_dict ptcdNode ) {
  tc_void* pvValue = nullptr;

  switch ( ptcdNode->b8DataTypeVal ) {
    case TC_INT:
      pvValue = m_ptcaInt->get(ptcdNode->iArrayPosVal);
      break;

    case TC_DECIMAL:
      pvValue = m_ptcaDecimal->get(ptcdNode->iArrayPosVal);
      break;

    case TC_STRING:
      pvValue = m_ptcaString->get(ptcdNode->iArrayPosVal);
      break;

    default:
      goto ERREXIT;

  }

  EXIT:
    return pvValue;

  ERREXIT:
    return nullptr;
}


template <typename tDataType>
tc_void* TeracadaDict::get ( tDataType ptKey, tc_dict ptcdParentNode ) {
  tc_void* pvKey = nullptr;
  tc_void* pvValue = nullptr;
  tc_dict ptcdNode = nullptr;
  tca_dict* ptcaChildren = nullptr;

  if ( ! ptcdParentNode ) {
    ptcaChildren = m_ptcaDictRoot;
  } else {
    if ( ptcdParentNode->ptcaNext )
      ptcaChildren = ptcdParentNode->ptcaNext;
    else
      goto ERREXIT;
  }

  m_ptcaInt->print();
  m_ptcaString->print();

  for ( int iIter = 1; iIter <= ptcaChildren->getNumElements(); iIter++ ) {
    ptcdNode = (tc_dict) ptcaChildren->get(iIter);
    pvKey = getNodeKey(ptcdNode);

    if constexpr ( std::is_same_v<tDataType, tc_int> ) {
      if ( *((tc_int*) pvKey) == ptKey ) {
        pvValue = getNodeValue(ptcdNode);
        break;
      }
    }

    if constexpr ( std::is_same_v<tDataType, tc_decimal> ) {
      if ( *((tc_decimal*) pvKey) == ptKey ) {
        pvValue = getNodeValue(ptcdNode);
        break;
      }
    }

    if constexpr ( std::is_same_v<tDataType, tc_str> ) {
      if ( strcmp((tc_str) pvKey, ptKey) == 0 ) {
        pvValue = getNodeValue(ptcdNode);
        break;
      }
    }

  }

  EXIT:
    return pvValue;

  ERREXIT:
    return nullptr;
}

template tc_void* TeracadaDict::get<tc_int> ( tc_int ptKey, tc_dict ptcdParentNode );

template tc_void* TeracadaDict::get<tc_decimal> ( tc_decimal ptKey, tc_dict ptcdParentNode );

template tc_void* TeracadaDict::get<tc_str> ( tc_str ptKey, tc_dict ptcdParentNode );

