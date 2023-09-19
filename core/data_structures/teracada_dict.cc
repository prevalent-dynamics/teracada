#include <type_traits>

#include <teracada_dict.h>


TeracadaDict::TeracadaDict ( void ) :
  ptcaDictRoot(nullptr),
  ptcaInt(nullptr),
  ptcaDecimal(nullptr),
  ptcaString(nullptr)
{
  ptcaDictRoot = new TeracadaArray<tc_dict>(10);
  ptcaInt = new TeracadaArray<tc_int>(10);
  ptcaDecimal = new TeracadaArray<tc_decimal>(10);
  ptcaString = new TeracadaArray<tc_str>(10);
}


TeracadaDict::~TeracadaDict ( void ) {

}

template <typename tDataType>
tc_dict* TeracadaDict::addNode ( tDataType ptValue ) {
  tc_dict* pNode = (tc_dict*) malloc(sizeof(tc_dict));

  if constexpr ( std::is_same_v<tDataType, tc_int> ) {
    pNode->b8DataType = TC_INT;
    if ( ! ptcaInt->insertBack(ptValue) )
      goto ERREXIT;

    pNode->pvValue = ptcaInt->get(-1);
  }

  if constexpr ( std::is_same_v<tDataType, tc_decimal> ) {
    pNode->b8DataType = TC_INT;
    if ( ! ptcaDecimal->insertBack(ptValue) )
      goto ERREXIT;

    pNode->pvValue = ptcaDecimal->get(-1);
  }

  if constexpr ( std::is_same_v<tDataType, tc_str> ) {
    pNode->b8DataType = TC_STRING;
    if ( ! ptcaString->insertBack(ptValue) )
      goto ERREXIT;

    pNode->pvValue = ptcaString->get(-1);
  }

  EXIT:
    return pNode;

  ERREXIT:
    return nullptr;
}

template tc_dict* TeracadaDict::addNode<tc_int> ( tc_int tValue );
template tc_dict* TeracadaDict::addNode<tc_decimal> ( tc_decimal tValue );
template tc_dict* TeracadaDict::addNode<tc_str> ( tc_str tValue );


template <typename tDataType>
bool TeracadaDict::insertNode ( tc_dict* pParentNode, tDataType tValue ) {
  tc_dict* newNode = nullptr;

  if ( ! pParentNode ) {
    newNode = addNode(tValue);

    if ( ! newNode )
      goto ERREXIT;

    if ( ! ptcaDictRoot->insertBack(newNode) )
      goto ERREXIT;

    goto EXIT;
  }

  while ( true ) {
    if ( pParentNode->stiChild ) {
      insertNode(pParentNode->stiChild, tValue);
      continue;
    }

    newNode = addNode(tValue);

    if ( ! newNode )
      goto ERREXIT;

    pParentNode->stiChild = addNode(tValue);
    break;
  }

  EXIT:
    return true;

  ERREXIT:
    return false;
}

template bool TeracadaDict::insertNode<tc_int> ( tc_dict* pParentNode, tc_int tValue );
template bool TeracadaDict::insertNode<tc_decimal> ( tc_dict* pParentNode, tc_decimal tValue );
template bool TeracadaDict::insertNode<tc_str> ( tc_dict* pParentNode, tc_str tValue );


tc_void TeracadaDict::printToBuff ( void ) {

}
