#include <cerrno>
#include <cstring>
#include <cstdint>
#include <fstream>
#include <cstdarg>
#include <algorithm>
#include <typeinfo>
#include <type_traits>

#include "teracada_array.h"


/*
  Function: TeracadaArray (Default constructor)
    Initializes m_pvArray and allocates memory
  Params:
    1. b8DataType: Data type of the Teracada array
    2. iNumElements: Total number of elements the Teracada array needs to accomodate
  Returns:
    (None)
  Notes:
    (None)
  TODO:
    (None)
*/
TeracadaArray::TeracadaArray ( tc_byte b8DataType, tc_int iNumElements ) :
  m_b8DataType(b8DataType),
  m_b8DataTypeSize(0),
  m_pvArray(nullptr),
  m_iIsInitSuccess(false),
  m_iMaxNumArrayElements(iNumElements),
  m_iArrayLastIndex(-1),
  m_b8ResizeAlgo(TA_RESIZE_ALGO_5PERCENT),
  m_b8ResizePaddingAlgo(TA_RESIZE_ALGO_STATIC10),
  m_uliTotalReallocAttempts(0),
  m_iErrno(0)
{
  void* pvPtrStartAddr = nullptr;

  tc_char acDataTypeStr[6][12] = {
    "TC_BYTE", "TC_INT", "TC_DECIMAL",
    "TC_CHAR", "TC_STRING", "TC_ARRAY"
  };

  TC_LOG(LOG_INFO, "TeracadaArray::TeracadaArray(): Initializing TeracadaArray [ DATA_TYPE: %s | NUM_ELEMENTS: %d ]",
      acDataTypeStr[getDataType()], getMaxNumElements());

  if ( getMaxNumElements() <= 0 ) {
    setArrayInitFailure();

    TC_LOG(LOG_ERR, "TeracadaArray::TeracadaArray(): Failed to initialize the Teracada array, minimum size of one element is required [ ELEMENTS_REQUESTED: %d ]", getMaxNumElements());

    return;
  }

  switch ( m_b8DataType ) {

    case TC_BYTE:
      setElementSize(sizeof(tc_byte));
      break;

    case TC_INT:
      setElementSize(sizeof(tc_int));
      break;

    case TC_DECIMAL:
      setElementSize(sizeof(tc_decimal));
      break;

    case TC_CHAR:
      setElementSize(sizeof(tc_char));
      break;

    case TC_STRING:
      setElementSize(sizeof(tc_char*));
      break;

    default:
      break;

  }

  setArray(calloc((getMaxNumElements() * getElementSize()), getElementSize()));

  if ( ! getArray() ) {
    setArrayInitFailure();

    TC_LOG(LOG_ERR, "TeracadaArray::TeracadaArray(): Failed to allocate buffer for the array");

    return;
  }

  pvPtrStartAddr = getArray();

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpointer-arith"

  TC_LOG(LOG_INFO, "TeracadaArray::TeracadaArray(): Allocated buffer of length %d bytes for the array [ TYPE: %s | START_ADDR: %p | END_ADDR: %p ]",
          (getMaxNumElements() * getDataType()),
           acDataTypeStr[getDataType()],
           pvPtrStartAddr,
          ((pvPtrStartAddr - 1) + (getMaxNumElements() * getDataType())));

  #pragma GCC diagnostic pop

  setArrayInitSuccess();

  return;
}


/*
  Function: TeracadaArray (Default destructor)
    Deallocates all the global heap allocations
  Params:
    (None)
  Returns:
    (None)
  Notes:
    (None)
  TODO:
    (None)
*/
TeracadaArray::~TeracadaArray ( void ) {
  switch ( getDataType() ) {

    case TC_STRING:
      if ( getArray() ) {
        for ( tc_int iIter = 0; iIter <= getLastElementIndex(); iIter++ ) {
          free(*((tc_char**) getArray() + iIter));
        }
      }
      [[fallthrough]];

    case TC_BYTE:
    case TC_INT:
    case TC_DECIMAL:
    case TC_CHAR:
      if ( getArray() )
        free(getArray());
      break;

    default:
      break;
  }
}


/*!
  @brief
    Validate the type safety of the template parameter
  @details
    The template parameter type should be according to the array data type
  @par Parameters
    None.
  @retval
    true Type safety check passed
  @retval
    false Type safety check failed
*/
template <typename tDataType>
bool TeracadaArray::validateTypeSafety ( void ) {

  switch ( getDataType() ) {

    case TC_BYTE:
      if ( std::is_same<tDataType, tc_byte>::value )
        goto EXIT;

      break;

    case TC_INT:
      if ( std::is_same<tDataType, tc_int>::value )
        goto EXIT;

      break;

    case TC_DECIMAL:
      if ( std::is_same<tDataType, tc_decimal>::value )
        goto EXIT;

      break;

    case TC_CHAR:
      if ( std::is_same<tDataType, tc_char>::value )
        goto EXIT;

      break;

    case TC_STRING:
      // Both data types tc_char (::insert()) and tc_char * (::remove()) are valid
      if ( std::is_same<tDataType, tc_char>::value ||
           std::is_same<tDataType, tc_char*>::value )
        goto EXIT;

      break;

    default:
      goto ERREXIT;

  }

  goto ERREXIT;

  EXIT:
    return true;

  ERREXIT:
    TC_LOG(LOG_ERR, "TeracadaArray::validateTypeSafety(): Validation of type safety failed");
    return false;
}


/*!
  @brief
    Convert array position value to array index value
  @details
    - Position value 1 represents the first array element
    - Position value 0 represents the index after the last array element
    - Positive position values are counted from array start to array end (left-to-right)
    - Negative position values are counted from array end to array start (right-to-left/reverse)
  @param[in]
    iPosition The position value
  @retval
    Index  The array index converted from the position
  @retval
    -1 When there is an invalid poition value passed in the parameter
*/
tc_int TeracadaArray::positionToIndex ( tc_int iPosition ) {
  tc_int iIndex = -1;

  // Position value 0 represents index after the last array element
  if ( iPosition == 0 ) {
    iIndex = getLastElementIndex() + 1;
    goto EXIT;
  }

  /* Positive positions (left to right) */
  if ( iPosition > 0 ) {
    iIndex = iPosition - 1;
    goto EXIT;
  }

  /* Negative positions (right to left) */
  iIndex = ((getLastElementIndex() < 0) ? 0 : getLastElementIndex()) + (iPosition + 1);

  // Invalid position
  if ( iIndex < 0 )
    goto ERREXIT;

  EXIT:
    return iIndex;

  ERREXIT:
    return -1;
}


/*!
  @brief
    Resize the array by reallocating the array buffer
  @details
    - Resize the array according to the value passed in the parameter iNumElements
    - Add extra padding on the right, according to the value of m_b8ResizePaddingAlgo, when iNumElements != 0
    - If iNumElements == 0, then resize according to the pre-defined resize algorithm (m_b8ResizeAlgo)
  @param[in]
    iNumElements (tc_int): The total number of new elements to be accomodated after resize
  @retval
    true Successfully resized the array
  @retval
    false Failed to resize the array
  @note
    Always pass minumum space required in the parameter iNumElements, let this function decide on the padding\n
    This function is automatically called when needed during insertion, hence protected access specifier
  @todo
    Shrink the array if iNumElements < 0
*/
bool TeracadaArray::resize ( tc_int iNumElements ) {
  void* pvReallocArray = nullptr;
  tc_int iNewNumElements = 0;
  tc_int iPreNumElements = getMaxNumElements();

  incrementTotalReallocAttempts();

  /* Log total reallocation attempts to the designated file */

  std::ofstream objFd;
  objFd.open(TA_REALLOC_ATTEMPTS_DUMP_FPATH, std::ios::out | std::ios::trunc);

  if ( ! objFd.fail() ) {
    objFd << getTotalReallocAttempts();
    objFd.flush();
    objFd.close();
  }

  if ( iNumElements <= 0 ) {
    TC_LOG(LOG_ERR, "TeracadaArray::resize(): Invalid number of elements requested for resize [ NUM_ELEMENTS: %d ]", iNumElements);
    goto ERREXIT;
  }

  /* If we have been requested to resize to a cerain size (iNumElements > 0) */

  if ( iNumElements ) {
    iNewNumElements = (iPreNumElements + iNumElements);

    /* Decide on additional padding */

    switch ( getResizePaddingAlgo() ) {

      case TA_RESIZE_ALGO_STATIC10:
        iNewNumElements += 10;
        break;

      case TA_RESIZE_ALGO_STATIC100:
        iNewNumElements += 100;
        break;

      case TA_RESIZE_ALGO_STATIC1000:
        iNewNumElements += 1000;
        break;

      default:
        TC_LOG(LOG_ERR, "TeracadaArray::resize(): Invalid resize padding algorithm");
        goto ERREXIT;
    }

    pvReallocArray = realloc(getArray(), (iNewNumElements * getElementSize()));

    if ( ! pvReallocArray )
      goto ERREXIT;

    goto EXIT;
  }

  /* If we have "not "been requested to resize to a cerain size (iNumElements = 0) */

  switch ( getResizeAlgo() ) {

    /* Decide on resize bounds */

    case TA_RESIZE_ALGO_5PERCENT:
      iNewNumElements = iPreNumElements + std::max<tc_int>(((iPreNumElements * 5) / 100), 1);
      break;

    case TA_RESIZE_ALGO_10PERCENT:
      iNewNumElements = iPreNumElements + std::max<tc_int>(((iPreNumElements * 10) / 100), 1);
      break;

    default:
      TC_LOG(LOG_ERR, "TeracadaArray::resize(): Invalid resize algorithm");
      goto ERREXIT;
  }

  pvReallocArray = realloc(m_pvArray, (iNewNumElements * m_b8DataTypeSize));

    if ( ! pvReallocArray )
      goto ERREXIT;

  EXIT:
    setArray(pvReallocArray);
    setMaxNumElements(iNewNumElements);

    TC_LOG(LOG_INFO, "TeracadaArray::resize(): Resized the array [ PRE_NUM_ELEMENTS: %d | NUM_ELEMENTS_REQUESTED: %d | NEW_NUM_ELEMENTS: %d | REALLOCATION_ATTEMPT: %d ]",
              iPreNumElements, iNumElements, iNewNumElements, getTotalReallocAttempts());

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-arith"

    TC_LOG(LOG_INFO, "TeracadaArray::resize(): Memory address values of the resized array [ START_ADDR: %p | END_ADDR: %p ]",
              getArray(), (getArray() - 1) + (iNewNumElements * getElementSize()));
  
    #pragma GCC diagnostic pop

    return true;

  ERREXIT:
    TC_LOG(LOG_ERR, "TeracadaArray::resize(): Failed to resize the array [ CURRENT_NUM_ELEMENTS: %d ]",
              iPreNumElements);
    return false;
}


tc_int TeracadaArray::resizeIfRequired ( tc_int iInsertIndex, tc_int iNumElements, bool bOverwrite) {
  tc_int iResizeNumElements = 0;
  tc_byte b8NullTermByte = 0;
  tc_int iLastIndexPostInsert = 0;

  if ( iInsertIndex < 0 || iNumElements <= 0 ) {
    TC_LOG(LOG_ERR, "TeracadaArray::resizeIfRequired(): Invalid function parameters [ INDEX: %d | NUM_ELEMENTS: %d ]", iInsertIndex, iNumElements);
    goto ERREXIT;
  }

  // Important: Althought we are considering here null terminator byte as part of
  //   estimated array last index post insert, it is not part of getLastElementIndex()
  //   we are only considering it here for resize calculations
  if ( getDataType() == TC_CHAR )
    b8NullTermByte = 1;

  if ( bOverwrite || iInsertIndex > getLastElementIndex() ) {
    iLastIndexPostInsert = std::max((tc_int) ((iInsertIndex - 1) + iNumElements + b8NullTermByte), getLastElementIndex());

  } else {
    iLastIndexPostInsert = (getLastElementIndex() + iNumElements + b8NullTermByte);
  }

  if ( (iLastIndexPostInsert - getLastIndex()) > 0 ) {
    iResizeNumElements = iLastIndexPostInsert - getLastIndex();

    if ( ! resize(iResizeNumElements) )
      goto ERREXIT;
  }

  EXIT:
    return iResizeNumElements;

  ERREXIT:
    return -1;
}


/*
  Function: TeracadaArray::insert()
    Insert "a single value" to the Teracada array
  Params:
    1. tValue: The "single" value
    2. iPosition: (Default: 0)
                  Position of insertion
                  Position is converted to index using positionToIndex()
                  Check positionToIndex() comments for further details
  Returns: (Boolean)
    1. True: If the value is inserted to the Teracada array successully
    2. False: When the function fails to insert the value
  Notes:
    Null terminator byte for TC_CHAR array is not part of m_iArrayLastIndex
  TODO:
    (None)
*/
template <typename tDataType>
bool TeracadaArray::insert ( tDataType tValue, tc_int iPosition, bool bOverwrite ) {
  tc_int iIndex = -1;

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    goto ERREXIT;

  // Template parameter data type safety validation
  if ( ! validateTypeSafety<tDataType>() )
    goto ERREXIT;

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 )
    goto ERREXIT;

  // Resize the Teracada array if required
  if ( resizeIfRequired(iIndex, 1, bOverwrite) < 0 )
    goto ERREXIT;

  /* Insert the value to a index after the last array value */

  if ( iIndex > getLastElementIndex() ) {
    *((tDataType *) m_pvArray + iIndex) = tValue;
    setLastElementIndex(iIndex);
    goto EXIT;
  }

  /* Insert the value to a index that is between other array values */

  if ( bOverwrite ) {
    *((tDataType *) m_pvArray + iIndex) = tValue;

  } else {
    for ( tc_int iIter = getLastElementIndex(); iIter >= iIndex; iIter-- ) {
      *((tDataType *) m_pvArray + iIter + 1) = *((tDataType *) m_pvArray + iIter);
    }

    *((tDataType *) m_pvArray + iIndex) = tValue;
    incrementLastElementIndexBy(1);
  }

  EXIT:
    // Set the null terminator for array type TC_CHAR
    // Don't move this out of goto label
    if ( m_b8DataType == TC_CHAR ) {
      *((tDataType *) m_pvArray + getLastElementIndex() + 1) = '\0';

      // Null terminator byte for TC_CHAR array is not part of getLastElementIndex()
      // So we will not incrementLastElementIndexBy() here
    }

    TC_LOG(LOG_INFO, "TeracadaArray::insert(): Teracada array insertion success [ POSITION: %d ]", iPosition);
    return true;

  ERREXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::insert(): Teracada array insertion failure [ POSITION: %d ]", iPosition);
    return false;
}

template bool TeracadaArray::insert < tc_byte > ( tc_byte tValue, tc_int iPosition, bool bOverwrite );
template bool TeracadaArray::insert < tc_int > ( tc_int tValue, tc_int iPosition, bool bOverwrite );
template bool TeracadaArray::insert < tc_decimal > ( tc_decimal tValue, tc_int iPosition, bool bOverwrite );
template bool TeracadaArray::insert < tc_char > ( tc_char tValue, tc_int iPosition, bool bOverwrite );


/*
  Function: TeracadaArray::insert()
    Insert "a set of values" to the Teracada array
  Params:
    1. tValue: The "set of values" pointer
    2. iPosition: (Default: 0)
                  Position of insertion
                  Position is converted to index using positionToIndex()
                  Check positionToIndex() comments for further details
    3. iLength: (Default: 0)
                 Lenth/Size of the the set of values
  Returns: (Boolean)
    1. True: If the values are inserted to the Teracada array successully
    2. False: When the function fails to insert the values
  Notes:
    Null terminator byte for TC_CHAR array is not part of m_iArrayLastIndex
  TODO:
    (None)
*/
template <typename tDataType>
bool TeracadaArray::insert ( const tDataType *tValue, tc_int iLength, tc_int iPosition, bool bOverwrite ) {
  tc_int iIndex = -1;
  tc_int uiNumEleToBeInserted = 0;

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    goto ERREXIT;

  // Template parameter data type safety validation
  if ( ! validateTypeSafety<tDataType>() )
    goto ERREXIT;

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 )
    goto ERREXIT;

  /* Sanity checks for parameter iLength */

  if ( ! iLength ) {
    if ( m_b8DataType == TC_CHAR || m_b8DataType == TC_STRING ) {

      // Cast tValue to tc_char * as C strlen is not templated and doesn't type check for tc_char *
      // Although this will never be called unless tDataType is of type tc_char
      iLength = strlen((tc_char *) tValue);

    } else {
      // We don't have the array length and the array itself is not a tc_char string array
      TC_LOG(LOG_INFO, "TeracadaArray::insert(): Teracada array insertion failure, length required for the array to be inserted");
      goto ERREXIT;
    }
  }

  /* Resize the Teracada array if required */

  uiNumEleToBeInserted = iLength;

  if ( getDataType() == TC_STRING )
    uiNumEleToBeInserted = 1;

  if ( resizeIfRequired(iIndex, uiNumEleToBeInserted, bOverwrite) < 0 )
    goto ERREXIT;

  if ( getDataType() == TC_STRING ) {

    /* For array type TC_STRING */

    tc_char *pcBuff = (tc_char *) calloc(iLength + 1, sizeof(tc_char));

    // Cast tValue to tc_char * as C strncpy is not templated
    // Although this will never be called unless tDataType is of type tc_char
    snprintf(pcBuff, iLength + 1, "%s", (tc_char*) tValue);

    /* Insert the value to a index after the last array value */

    if ( iIndex > getLastElementIndex() ) {
      *((tc_char **) m_pvArray + iIndex) = pcBuff;
      setLastElementIndex(iIndex);
      goto EXIT;
    }

    /* Insert the value to a index that is between other array values */

    if ( bOverwrite ) {
      free((tc_char **) m_pvArray + iIndex);
      *((tc_char **) m_pvArray + iIndex) = pcBuff;

    } else {
      for ( tc_int iIter = getLastElementIndex(); iIter >= iIndex; iIter-- ) {
        *((tc_char **) m_pvArray + iIter + 1) = *((tc_char **) m_pvArray + iIter);
      }

      *((tc_char **) m_pvArray + iIndex) = pcBuff;
      incrementLastElementIndexBy(1);
    }

  } else {
    /* For everything else except array type TC_STRING */

    /* Insert the value to a index after the last array value */

    if ( iIndex > getLastElementIndex() ) {
      memcpy((tDataType *) m_pvArray + iIndex, tValue, (iLength * sizeof(tDataType)));
      setLastElementIndex((iIndex - 1) + iLength); // -1 because we start inserting "at" the iIndex
      goto EXIT;
    }

    /* Insert the value to a index that is between other array values */

    if ( bOverwrite ) {
      memcpy(((tDataType *) m_pvArray + iIndex), tValue, (iLength * sizeof(tDataType)));

    } else {
      for ( tc_int iIter = getLastElementIndex() + iLength; iIter >= (iIndex + iLength); iIter-- ) {
        *((tDataType *) m_pvArray + iIter) = *((tDataType *) m_pvArray + (iIter - iLength));
      }

      memcpy(((tDataType *) m_pvArray + iIndex), tValue, (iLength * sizeof(tDataType)));
      incrementLastElementIndexBy(iLength);
    }

  }

  EXIT:
    // Set the null terminator for array type TC_CHAR
    // Don't move this out of goto label
    if ( m_b8DataType == TC_CHAR ) {
      *((tDataType *) m_pvArray + getLastElementIndex() + 1) = '\0';

      // Null terminator byte for TC_CHAR array is not part of getLastElementIndex()
      // So we will not incrementLastElementIndexBy() here
    }

    TC_LOG(LOG_INFO, "TeracadaArray::insert(): Teracada array insertion success [ POSITION: %d | LENGTH: %d | POST_LAST_ELEMENT_INDEX: %d ]", iPosition, iLength, getLastElementIndex());
    return true;

  ERREXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::insert(): Teracada array insertion failure [ POSITION: %d | LENGTH: %d ]", iPosition, iLength);
    return false;
}

template bool TeracadaArray::insert < tc_byte > ( const tc_byte *tValue, tc_int iLength, tc_int iPosition, bool bOverwrite );
template bool TeracadaArray::insert < tc_int > ( const tc_int *tValue, tc_int iLength, tc_int iPosition, bool bOverwrite );
template bool TeracadaArray::insert < tc_decimal > ( const tc_decimal *tValue, tc_int iLength, tc_int iPosition, bool bOverwrite );
template bool TeracadaArray::insert < tc_char > ( const tc_char *tValue, tc_int iLength, tc_int iPosition, bool bOverwrite );


/*
  Function: TeracadaArray::_remove()
    Main function implementing remove functionality
  Params:
    1. iPosition: Array element position to remove
    2. iNumElements: (Default: 1)
                      Number of array elements to remove from iPosition
  Returns: (Boolean)
    1. True: If the value(s) is removed successfully
    2. False: When the function fails to remove the value(s)
  Notes:
    (None)
  TODO:
    (None)
*/
template <typename tDataType>
bool TeracadaArray::_remove ( tc_int iPosition, tc_int iNumElements ) {
  tc_int iIndex = -1;

  // Check if the Teracada array was successfully initialized
  if ( ! m_iIsInitSuccess ) {
    TC_LOG(LOG_INFO, "TeracadaArray::_remove(): The Teracada array has not been initialized");
    goto ERREXIT;
  }

  // Template parameter data type safety validation
  if ( ! validateTypeSafety<tDataType>() )
    goto ERREXIT;

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 )
    goto ERREXIT;

  /* Check if the passed parameters try to remove the array element out of bound */

  // -1 because we start removing array elements "from" the iIndex
  if ( (iIndex + iNumElements - 1) > getLastElementIndex() ) {
    TC_LOG(LOG_INFO, "TeracadaArray::_remove(): Passed parameters try to remove the array element(s) out of bound");
    goto ERREXIT;
  }

  /* Free the tc_char ** buffers in case of array type TC_STRING */

  if ( m_b8DataType == TC_STRING ) {
    for ( tc_int iIter = iIndex; iIter < (iIndex + iNumElements); iIter++ ) {
      // Cast manually to tc_char ** as free() will not accept non pointer datatypes from the template
      free(*((tc_char **) m_pvArray + iIter));
    }
  }

  /* Remove value(s) */

  for ( tc_int iIter = iIndex; iIter <= (getLastElementIndex() - iNumElements); iIter++ ) {
    *((tDataType *) m_pvArray + iIter) = *((tDataType *) m_pvArray + iIter + iNumElements);
  }

  decrementLastElementIndexBy(iNumElements);

  // Set the free space create to '0', to clean it up
  // Plus we also don't have to think about the null terminator
  memset(((tDataType *) m_pvArray + getLastElementIndex() + 1), 0, (iNumElements * m_b8DataTypeSize));

  EXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::_remove(): Teracada array element removal success [ POSITION: %d | NUM_ELEMENTS: %d ]", iPosition, iNumElements);
    return true;

  ERREXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::_remove(): Teracada array element removal failure [ POSITION: %d | NUM_ELEMENTS: %d ]", iPosition, iNumElements);
    return false;
}


/*
  Function: TeracadaArray::remove()
    Wrapper function around TeracadaArray::_remove()
    Calls TeracadaArray::_remove() with proper template parameter data type
  Params:
    1. iPosition: Array element position to remove
    2. iNumElements: (Default: 1)
                      Number of array elements to remove from iPosition
  Returns: (Boolean)
    1. True: If the value(s) is removed successfully
    2. False: When the function fails to remove the value(s)
  Notes:
    (None)
  TODO:
    (None)
*/
bool TeracadaArray::remove ( tc_int iPosition, tc_int iNumElements ) {

  switch ( m_b8DataType ) {
    case TC_BYTE:
      return _remove<tc_byte>(iPosition, iNumElements);

    case TC_INT:
      return _remove<tc_int>(iPosition, iNumElements);

    case TC_DECIMAL:
      return _remove<tc_decimal>(iPosition, iNumElements);

    case TC_CHAR:
      return _remove<tc_char>(iPosition, iNumElements);

    case TC_STRING:
      return _remove<tc_char*>(iPosition, iNumElements);

    default:
      goto ERREXIT;
  }

  EXIT:
    return true;

  ERREXIT:
    return false;
}


/*
  Function: TeracadaArray::reset()
    Resets the Teracada array and clean everything
  Params:
    (None)
  Returns: (Boolean)
    (None)
  Notes:
    (None)
  TODO:
    (None)
*/
void TeracadaArray::reset ( void ) {
  if ( ! m_iIsInitSuccess ) {
    // Set all elements to 0
    memset(m_pvArray, 0, (m_iMaxNumArrayElements * m_b8DataTypeSize));

    // Set m_iArrayLastIndex to default
    setLastElementIndex(-1);

  }

  return;
}


/*
  Function: TeracadaArray::get()
    Get the pointer to value present on the requested position
  Params:
    1. iPosition: Teracada array position
  Returns: (Void pointer)
           Pointer to value present on the requested position
  Notes:
    (None)
  TODO:
    (None)
*/
void *TeracadaArray::get ( tc_int iPosition ) {
  tc_int iIndex = -1;
  void *pvValue = nullptr;

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() ) {
    TC_LOG(LOG_INFO, "TeracadaArray::get(): The Teracada array has not been initialized");
    setErrno(ERR_TA_NOT_INIT);
    goto ERREXIT;
  }

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 || iIndex > getLastElementIndex() ) {
    setErrno(ERR_TA_INVALID_POS);
    goto ERREXIT;
  }

  #pragma GCC dignostics push
  #pragma GCC diagnostic ignored "-Wpointer-arith"

  switch ( m_b8DataType ) {

    case TC_BYTE:
    case TC_INT:
    case TC_DECIMAL:
    case TC_CHAR:
      pvValue =  m_pvArray + (iIndex * m_b8DataTypeSize);
      break;

    case TC_STRING:
      pvValue =  *((tc_char **) m_pvArray + iIndex);
      break;

    default:
      goto ERREXIT;
  }

  #pragma GCC diagnostics pop

  EXIT:
    return pvValue;

  ERREXIT:
    return NULL;
}


/*
  Function: TeracadaArray::print()
    Print the Teracada array in a nice format
  Params:
    (None)
  Returns:
    (None)
  Notes:
    (None)
  TODO:
    (None)
*/
void TeracadaArray::print ( void ) {

  // Check if the Teracada array was successfully initialized
  if ( ! m_iIsInitSuccess ) {
    TC_LOG(LOG_INFO, "TeracadaArray::print(): The Teracada array has not been initialized");
    return;
  }

  TeracadaArray *tcaBuff = new TeracadaArray(TC_CHAR, 20);

  if ( printToBuff(tcaBuff) )
    printf("%s", (tc_char*) tcaBuff->getArray());

  delete tcaBuff;

  return;
}


TeracadaArray* TeracadaArray::printToBuff ( TeracadaArray* ptcaBuff ) {
  tc_char caIntStr[12] = {0};
  tc_char* pcIntStr = &caIntStr[0];

  if ( ! isInitSuccess() )
    goto ERREXIT;

  if ( ! ptcaBuff ) {
    // We alloc a large enough buffer to print all the elements as string
    //   and also set the resize padding algo as TA_RESIZE_ALGO_STATIC1000,
    //   large enough to reduce the number of reallocation required.
    //   This memory bloack is anyway shortlived
    ptcaBuff = new TeracadaArray(TC_CHAR, getNumElements() * 10 );
    ptcaBuff->setResizePaddingAlgo(TA_RESIZE_ALGO_STATIC1000);
  }

  if ( ! ptcaBuff->isInitSuccess() )
    goto ERREXIT;

  ptcaBuff->insert<tc_char>("array([ ");

  switch ( getDataType() ) {
    case TC_BYTE:
      {
        for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
          snprintf(pcIntStr, sizeof(caIntStr), "%d ", *((tc_byte*) get(iIter)));
          ptcaBuff->insert<tc_char>(pcIntStr);
          memset(pcIntStr, 0, sizeof(caIntStr));
        }
      }

      break;

    case TC_INT:
      {
        for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
          snprintf(pcIntStr, sizeof(caIntStr), "%ld ", *((tc_int*) get(iIter)));
          ptcaBuff->insert<tc_char>(pcIntStr);
          memset(pcIntStr, 0, sizeof(caIntStr));
        }
      }

      break;

    case TC_DECIMAL:
      {
        for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
          snprintf(pcIntStr, sizeof(caIntStr), "%lf ", *((tc_decimal*) get(iIter)));
          ptcaBuff->insert<tc_char>(pcIntStr);
          memset(pcIntStr, 0, sizeof(caIntStr));
        }
      }

      break;

    case TC_CHAR:
      ptcaBuff->insert<tc_char>((tc_char*) getArray());
      break;

    case TC_STRING:
      {
        for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
          ptcaBuff->insert<tc_char>((tc_char*) get(iIter));
          ptcaBuff->insert<tc_char>(",\n");
        }

      }

      break;

    default:
      goto ERREXIT;
  }

  ptcaBuff->insert<tc_char>("])");

  EXIT:
    return ptcaBuff;

  ERREXIT:
    return NULL;
}
