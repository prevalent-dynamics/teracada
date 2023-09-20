/*!
  @file
  @author Rishabh Soni (Prevalent Dynamics)

  @brief
    Implementation of TeracadaArray core functions
  
  @attention
    All api functions MUST make sure that whenever they ERREXIT, they should properly call throwException(ERR),
    either just before goto ERREXIT (all), or in the ERREXIT label or both if it is better,
    as throwException will not overwrite previously set error. This step can only be skipped if it sure/known
    that throwException(ERR) has definitely been called by one of the previous function calls.

  @note
    For both tc_str and tc_char*, strings must be null terminated, before passing to the api functions
*/


#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <type_traits>

#include "teracada_array.h"


/*!
  @brief
    The class constructor.

  @details
    This is the default class constructor.
    While creating the object, accepted data types are:
    - tc_byte
    - tc_int
    - tc_decimal
    - tc_char
    - tc_str

  @param[in]
    iNumElements The number of array elements, the default is 100

  @par Returns
    None.

  @note
    The difference between tc_char and tc_str is that with tc_str, passed strings will be stored in a seperate char buffer and the pointer to that buffer will be stored in the main array object

  @par Examples
    @code{.cpp}
    TeracadaArray<tc_byte> arrayObject(10);
    TeracadaArray<tc_int> arrayObject(10);
    TeracadaArray<tc_decimal> arrayObject(10);
    TeracadaArray<tc_char> arrayObject(10);
    TeracadaArray<tc_str> arrayObject(10);
    @endcode

  @par Errors/Exceptions
    - ERR_TA_MEMALLOC_FAILED
    - ERR_TA_INIT_FAILED
*/
template <typename tDataType>
TeracadaArray<tDataType>::TeracadaArray ( tc_int iNumElements ) :
  m_b8DataType(TC_NONE), // TC_INT is the default data type
  m_pvArray(nullptr),
  m_bIsInitSuccess(false),
  m_iMaxNumArrayElements(iNumElements),
  m_iArrayLastIndex(TA_NONE_INDEX),
  m_b8ResizeAlgo(TA_RESIZE_ALGO_5PERCENT),
  m_b8ResizePaddingAlgo(TA_RESIZE_ALGO_STATIC10),
  m_bOverwrite(false),
  m_uliTotalReallocAttempts(0),
  m_iErrno(0),
  m_bEnableExceptions(true)
{
  tc_void* pvBuff = nullptr;

  tc_char acDataTypeStr[8][12] = {
    "TC_NONE", "TC_BYTE", "TC_INT", "TC_DECIMAL",
    "TC_CHAR", "TC_STRING", "TC_ARRAY", "TC_DICT"
  };

  if ( ! setDataType() ) {
    setArrayInitFailure();
    TC_LOG(LOG_ERR, "TeracadaArray::TeracadaArray(): Failed to set the array data type");
    goto ERREXIT;
  }

  TC_LOG(LOG_INFO, "TeracadaArray::TeracadaArray(): Initializing TeracadaArray [ DATA_TYPE: %s | NUM_ELEMENTS: %d ]", acDataTypeStr[getDataType()], getMaxNumElements());

  if ( getMaxNumElements() <= 0 ) {
    setArrayInitFailure();
    TC_LOG(LOG_ERR, "TeracadaArray::TeracadaArray(): Invalid size requested, minimum size of one array element is required [ ELEMENTS_REQUESTED: %d ]", getMaxNumElements());
    goto ERREXIT;
  }

  pvBuff = calloc((getMaxNumElements() * getElementSize()), getElementSize());

  if ( ! pvBuff ) {
    setArrayInitFailure();
    TC_LOG(LOG_ERR, "TeracadaArray::TeracadaArray(): Failed to allocate buffer for the array");
    throwException(ERR_TA_MEMALLOC_FAILED);
    goto ERREXIT;
  }

  setArray(pvBuff);
  setArrayInitSuccess();

  EXIT:
    /* Log array start/end address and length */

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpointer-arith"

    TC_LOG(LOG_INFO, "TeracadaArray::TeracadaArray(): Allocated buffer of length %d bytes for the array [ TYPE: %s | START_ADDR: %p | END_ADDR: %p ]",
            (getMaxNumElements() * getDataType()),
            acDataTypeStr[getDataType()],
            getArray(),
            ((getArray() - 1) + (getMaxNumElements() * getDataType())));

    #pragma GCC diagnostic pop

    return;

  ERREXIT:
    TC_LOG(LOG_ERR, "TeracadaArray::TeracadaArray(): Failed to initialize the array");
    throwException(ERR_TA_INIT_FAILED);
    return;

}


template <typename tDataType>
TeracadaArray<tDataType>::~TeracadaArray ( void ) {
  if ( getArray() ) {
    free(getArray());
  }
}


/*!
  @brief
    Sets data type based on template argument of object initialization.

  @details
    Sets the value of member variable (m_b8DataType) based on object initialization.

  @par Parameters
    None.

  @par Returns
    true On sucess
    false On failure

  @par Errors/Exceptions
    - ERR_TA_INVALID_DATA_TYPE
*/
template <typename tDataType>
tc_bool TeracadaArray<tDataType>::setDataType ( void ) {

  if constexpr ( std::is_same<tDataType, tc_byte>::value )
    m_b8DataType = TC_BYTE;

  else if constexpr ( std::is_same<tDataType, tc_int>::value )
    m_b8DataType = TC_INT;

  else if constexpr ( std::is_same<tDataType, tc_decimal>::value )
    m_b8DataType = TC_DECIMAL;

  else if constexpr ( std::is_same<tDataType, tc_char>::value )
    m_b8DataType = TC_CHAR;

  else if constexpr ( std::is_same<tDataType, tc_str>::value )
    m_b8DataType = TC_STRING;

  else if constexpr ( std::is_same<tDataType, tc_dict>::value )
    m_b8DataType = TC_DICT;

  else {
    goto ERREXIT;
  }

  EXIT:
    return true;

  ERREXIT:
    TC_LOG(LOG_ERR, "TeracadaArray::setDataType(): Failed to set the array data type");
    throwException(ERR_TA_INVALID_DATA_TYPE);
    return false;
}


/*!
  @brief
    Convert array position value to array index value

  @details
    This function converts a position value to actual array index.
    - Position value 1 represents the first array element.
    - Position value 0 represents the index after the last array element.
    - Positive position values are counted from array start to array end (left-to-right).
    - Negative position values are counted from array end to array start (right-to-left/reverse).

  @param[in]
    iPosition The position value

  @retval
    Index The array index converted from the position

  @retval
    -1 When there is an invalid poition value passed in the parameter

  @note
    This function doesn't perform validation to check if the passed position value actually exists in the main array

  @par Errors/Exceptions
    - ERR_TA_INVALID_POSITION_OR_INDEX
*/
template <typename tDataType>
tc_int TeracadaArray<tDataType>::positionToIndex ( tc_int iPosition ) {
  tc_int iIndex = TA_NONE_INDEX;

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
    TC_LOG(LOG_ERR, "TeracadaArray::positionToIndex(): Failed to convert the poition value to a valid array index");
    throwException(ERR_TA_INVALID_POSITION_OR_INDEX);
    return TA_NONE_INDEX;
}


/*!
  @brief
    Resize the main array by reallocating the array buffer

  @details
    - Resize the array according to the value passed in the parameter iNumElements.
    - Add extra padding on the right, according to the value of m_b8ResizePaddingAlgo, when iNumElements != 0.
    - If iNumElements == 0 (no specific size requested), then resize according to the pre-defined resize algorithm (m_b8ResizeAlgo).

  @param[in]
    iNumElements The total number of new elements to be accomodated after resize

  @retval
    true Successfully resized the array

  @retval
    false Failed to resize the array

  @note
    - Always pass minumum space required in the parameter iNumElements, let this function decide on padding.
    - This function is automatically called when needed during insertion, hence protected access specifier.

  @todo
    Shrink the array if iNumElements < 0
*/
template <typename tDataType>
bool TeracadaArray<tDataType>::resize ( tc_int iNumElements ) {
  tc_void* pvReallocArray = nullptr;
  tc_int iNewNumElements = 0;
  tc_int iPreNumElements = getMaxNumElements();

  /* Log total reallocation attempts to the designated file (even before validation checks) */

  incrementTotalReallocAttempts();

  std::ofstream objFd;
  objFd.open(TA_REALLOC_ATTEMPTS_DUMP_FPATH, std::ios::out | std::ios::trunc);

  if ( ! objFd.fail() ) {
    objFd << getTotalReallocAttempts();
    objFd.flush();
    objFd.close();
  }

  if ( iNumElements <= 0 ) {
    TC_LOG(LOG_ERR, "TeracadaArray::resize(): Invalid number of elements requested for resize [ NUM_ELEMENTS: %d ]", iNumElements);
    throwException(ERR_TA_INVALID_PARAM);
    goto ERREXIT;
  }

  /* If we have been requested to resize to a cerain size (iNumElements > 0) */

  if ( iNumElements ) {
    iNewNumElements = (iPreNumElements + iNumElements);

    /** Decide on additional padding **/

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

    pvReallocArray = realloc(getArray(), (iNewNumElements * sizeof(tDataType)));

    if ( ! pvReallocArray ) {
      TC_LOG(LOG_ERR, "TeracadaArray::resize(): Failed to reallocate main array buffer [ SIZE: %d ]", (iNewNumElements * sizeof(tDataType)));
      throwException(ERR_TA_MEMALLOC_FAILED);
      goto ERREXIT;
    }

    goto EXIT;
  }

  /* If we have "not "been requested to resize to a cerain size (iNumElements = 0) */

  switch ( getResizeAlgo() ) {

    /** Decide on resize bounds **/

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

  pvReallocArray = realloc(getArray(), (iNewNumElements * sizeof(tDataType)));

  if ( ! pvReallocArray ) {
    TC_LOG(LOG_ERR, "TeracadaArray::resize(): Failed to reallocate main array buffer [ SIZE: %d ]", (iNewNumElements * sizeof(tDataType)));
    throwException(ERR_TA_MEMALLOC_FAILED);
    goto ERREXIT;
  }

  EXIT:
    // Don't move setArray() out of EXIT label
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
    TC_LOG(LOG_ERR, "TeracadaArray::resize(): Failed to resize the array [ CURRENT_NUM_ELEMENTS: %d ]", iPreNumElements);
    throwException(ERR_TA_RESIZE_FAILED);
    return false;
}


/*!
  @brief
    Check if resizing of the main array is required.

  @details
    - Check if resizing of the main array is required before inserting new elements in the main array.
    - Resizing is only done if all or any of the elements to be inserted in the main array requires first resizing of the main buffer.

  @param[in]
    iInsertIndex The main array start index for insertion

  @param[in]
    iNumElements The number of elements to be inserted, starting from iInsertIndex

  @retval
    NumElements The number of out of bound insertion elements for which resizing was performed

  @retval
    0 Resizing was not performed (was not required, the main array buffer had enough space)

  @retval
    -1 In case of an error

  @note
    - Always pass minumum space required in the parameter iNumElements, let this function decide on padding.
    - This function is automatically called when needed during insertion, hence protected access specifier.

  @todo
    Shrink the array if iNumElements < 0
*/
template <typename tDataType>
tc_int TeracadaArray<tDataType>::resizeBeforeInsert ( tc_int iInsertIndex, tc_int iNumElements ) {
  tc_int iResizeNumElements = 0;
  tc_int iLastIndexPostInsert = 0;
  tc_byte b8NullTermByte = 0;

  // Important: Althought we are considering here null terminator byte as part of
  //   estimated array last index post insert, it is not part of getLastElementIndex()
  //   we are only considering it here for resize calculations
  if constexpr ( std::is_same_v<tDataType, tc_char> ) {
    b8NullTermByte = 1;
  }

  if ( iInsertIndex < 0 || iNumElements <= 0 ) {
    TC_LOG(LOG_ERR, "TeracadaArray::resizeBeforeInsert(): Invalid function parameters [ INDEX: %d | NUM_ELEMENTS: %d ]", iInsertIndex, iNumElements);
    throwException(ERR_TA_INVALID_POSITION_OR_INDEX);
    goto ERREXIT;
  }

  if ( isOverwriteEnabled() || iInsertIndex > getLastElementIndex() ) {
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
    TC_LOG(LOG_ERR, "TeracadaArray::resizeBeforeInsert(): Failed while checking if resizing the array is required");
    throwException(ERR_TA_RESIZE_FAILED);
    return -1;
}


/*!
  @brief
    Insert a single value of the template argument type

  @details
    Note that a template argument can be of a pointer type itself (Example: tc_str).
    - If the template argument is of simple data type (tc_int, tc_decimal etc.), insert the value at the designated position.
    - If the template argument is of pointer data type, insert the pointer address in the main array at the designated position.

  @param[in]
    iPosition The main array position to insert the value

  @param[in]
    tDataType The value to be inserted

  @retval
    iPosition On success, the function returns the position (from start/left) of element insertion

  @retval
    -1 On failure
*/
template <typename tDataType>
tc_int TeracadaArray<tDataType>::insert ( tc_int iPosition, tDataType tValue ) {
  tc_int iIndex = TA_NONE_INDEX;

  if ( ! isInitSuccess() )
    goto ERREXIT;

  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 )
    goto ERREXIT;

  // Resize the Teracada array if required
  if ( resizeBeforeInsert(iIndex, 1) < 0 )
    goto ERREXIT;

  /* Insert the value to a index after the last array value */

  if ( iIndex > getLastElementIndex() ) {
    *((tDataType *) getArray() + iIndex) = tValue;
    setLastElementIndex(iIndex);
    goto EXIT;
  }

  /* Insert the value to a index that is between other array values */

  if ( isOverwriteEnabled() ) {
    *((tDataType *) getArray() + iIndex) = tValue;

  } else {
    for ( tc_int iIter = getLastElementIndex(); iIter >= iIndex; iIter-- ) {
      *((tDataType *) getArray() + iIter + 1) = *((tDataType *) getArray() + iIter);
    }

    *((tDataType *) getArray() + iIndex) = tValue;
    incrementLastElementIndexBy(1);
  }

  EXIT:
    // Set the null terminator for array type TC_CHAR
    // Don't move this out of goto label
    if constexpr ( std::is_same_v<tDataType, tc_char> ) {
      *((tDataType*) getArray() + getLastElementIndex() + 1) = '\0';

      // Null terminator byte for TC_CHAR array is not part of getLastElementIndex()
      // So we will not incrementLastElementIndexBy() here
    }

    if constexpr ( std::is_same_v<tDataType, tc_str> ) {
      TC_LOG(LOG_INFO, "TeracadaArray::insert(): Array insertion operation success [ POSITION: %d | VALUE: %s ]", iPosition, tValue);
    } else {
      TC_LOG(LOG_INFO, "TeracadaArray::insert(): Array insertion operation success [ POSITION: %d ]", iPosition);
    }

    return iIndex + 1;

  ERREXIT:
    TC_LOG(LOG_ERR, "TeracadaArray::insert(): Array insertion operation failure [ POSITION: %d ]", iPosition);
    throwException(ERR_TA_INSERTION_FAILURE);
    return TA_NONE_INDEX;
}


/*!
  @brief
    Insert multiple values of the template argument type

  @param[in]
    iPosition The main array position to insert the first value

  @param[in]
    tDataType The pointer to the array of values to be inserted

  @param[in]
    iLength Length of the the array of values to be inserted in the main array

  @retval
    true Successfully inserted the values in the main array, starting at the designated position

  @retval
    false Failed to insert the values in the array
*/
template <typename tDataType>
bool TeracadaArray<tDataType>::insert ( tc_int iPosition, tDataType* ptValue, tc_int iLength ) {

  tc_int iIndex = TA_NONE_INDEX;

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    goto ERREXIT;

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 )
    goto ERREXIT;

  if constexpr ( std::is_same_v<tDataType, tc_char> ) {
    if ( iLength <= 0 )
      iLength = strlen(ptValue);
  }

  if ( iLength <= 0 ) {
    TC_LOG(LOG_ERR, "TeracadaArray::insert(): Invalid length value for the element list [ LENGTH: %d ]", iLength);
    throwException(ERR_TA_INVALID_PARAM);
    goto ERREXIT;
  }

  // Resize the array if required
  if ( resizeBeforeInsert(iIndex, iLength) < 0 )
    goto ERREXIT;

  /* Insert the value to a index after the last array value */

  if ( iIndex > getLastElementIndex() ) {
    memcpy((tDataType *) getArray() + iIndex, ptValue, (iLength * sizeof(tDataType)));
    setLastElementIndex((iIndex - 1) + iLength); // -1 because we start inserting "at" the iIndex
    goto EXIT;
  }

  /* Insert the value to a index that is between other array values */

  if ( isOverwriteEnabled() ) {
    memcpy(((tDataType *) getArray() + iIndex), ptValue, (iLength * sizeof(tDataType)));

  } else {
    for ( tc_int iIter = getLastElementIndex() + iLength; iIter >= (iIndex + iLength); iIter-- ) {
      *((tDataType *) getArray() + iIter) = *((tDataType *) getArray() + (iIter - iLength));
    }

    memcpy(((tDataType *) getArray() + iIndex), ptValue, (iLength * sizeof(tDataType)));
    incrementLastElementIndexBy(iLength);
  }


  EXIT:
    // Set the null terminator for array type TC_CHAR
    // Don't move this out of goto label
    if constexpr ( std::is_same_v<tDataType, tc_char> ) {
      *((tDataType*) getArray() + getLastElementIndex() + 1) = '\0';

      // Null terminator byte for TC_CHAR array is not part of getLastElementIndex()
      // So we will not incrementLastElementIndexBy() here
    }

    TC_LOG(LOG_INFO, "TeracadaArray::insert(): Array insertion operation success [ POSITION: %d | LENGTH: %d | POST_LAST_ELEMENT_INDEX: %d ]", iPosition, iLength, getLastElementIndex());
    return true;

  ERREXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::insert(): Array insertion operation failure [ POSITION: %d | LENGTH: %d ]", iPosition, iLength);
    throwException(ERR_TA_INSERTION_FAILURE);
    return false;
}


template <typename tDataType>
bool TeracadaArray<tDataType>::remove ( tc_int iPosition, tc_int iNumElements ) {
  tc_int iIndex = TA_NONE_INDEX;

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    goto ERREXIT;

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 )
    goto ERREXIT;

  /* Check if the passed parameters try to remove the array element out of bound */

  // -1 because we start removing array elements "from" the iIndex
  if ( (iIndex + iNumElements - 1) > getLastElementIndex() ) {
    TC_LOG(LOG_INFO, "TeracadaArray::remove(): Passed parameters tries to remove array element(s) out of bound");
    throwException(ERR_TA_INVALID_POSITION_OR_INDEX);
    goto ERREXIT;
  }

  /* Remove value(s) */

  for ( tc_int iIter = iIndex; iIter <= (getLastElementIndex() - iNumElements); iIter++ ) {
    *((tDataType *) getArray() + iIter) = *((tDataType *) getArray() + iIter + iNumElements);
  }

  decrementLastElementIndexBy(iNumElements);

  // Set the free space created to '0', to clean it up
  // Plus we also don't have to think about the null terminator
  memset(((tDataType *) getArray() + getLastElementIndex() + 1), 0, (iNumElements * sizeof(tDataType)));

  EXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::remove(): Array element removal success [ POSITION: %d | NUM_ELEMENTS: %d ]", iPosition, iNumElements);
    return true;

  ERREXIT:
    TC_LOG(LOG_INFO, "TeracadaArray::remove(): Array element removal failure [ POSITION: %d | NUM_ELEMENTS: %d ]", iPosition, iNumElements);
    throwException(ERR_TA_REMOVE_FAILURE);
    return false;
}


template <typename tDataType>
tc_bool TeracadaArray<tDataType>::reset ( void ) {
  if ( ! isInitSuccess() ) 
    goto ERREXIT;

  memset(getArray(), 0, (m_iMaxNumArrayElements * sizeof(tDataType)));
  setLastElementIndex(-1);

  EXIT:
    return true;

  ERREXIT:
    return false;
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
template <typename tDataType>
tc_void* TeracadaArray<tDataType>::get ( tc_int iPosition ) {
  tc_int iIndex = TA_NONE_INDEX;
  tc_void* pvValue = nullptr;

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    goto ERREXIT;

  // Convert position to index value
  iIndex = positionToIndex(iPosition);

  if ( iIndex < 0 || iIndex > getLastElementIndex() )
    goto ERREXIT;

  #pragma GCC dignostics push
  #pragma GCC diagnostic ignored "-Wpointer-arith"

  if constexpr ( std::is_pointer_v<tDataType> ) {
    pvValue = *((tDataType*) getArray() + iIndex);
  } else {
    pvValue = getArray() + (iIndex * sizeof(tDataType));
  }

  #pragma GCC diagnostics pop

  EXIT:
    return pvValue;

  ERREXIT:
    return NULL;
}


template <typename tDataType>
tc_void TeracadaArray<tDataType>::print ( void ) {

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    return;

  TeracadaArray<tc_char>* ptcaBuff = new TeracadaArray<tc_char>(20);

  if ( ! ptcaBuff->isInitSuccess() ) {
    TC_LOG(LOG_ERR, "TeracadaArray::print(): Failed to initialize print buffer array");
    return;
  }

  ptcaBuff->setResizePaddingAlgo(TA_RESIZE_ALGO_STATIC1000);

  if ( printToBuff(ptcaBuff) ) {
    printf("%s\n", (tc_str) ptcaBuff->getArray());
    fflush(stdout);
  }

  delete ptcaBuff;

  return;
}


template <typename tDataType>
TeracadaArray<tc_char>* TeracadaArray<tDataType>::printToBuff ( TeracadaArray<tc_char>* ptcaBuff ) {
  tc_char caIntStr[12] = {0};
  tc_str pcIntStr = &caIntStr[0];

  // Check if the Teracada array was successfully initialized
  if ( ! isInitSuccess() )
    goto ERREXIT;

  if ( ! ptcaBuff ) {
    // We alloc a large enough buffer to print all the elements as string
    //   and also set the resize padding algo as TA_RESIZE_ALGO_STATIC1000,
    //   large enough to reduce the number of reallocation required.
    //   This memory bloack is anyway shortlived
    ptcaBuff = new TeracadaArray<tc_char>(getNumElements() * 10 );
    ptcaBuff->setResizePaddingAlgo(TA_RESIZE_ALGO_STATIC1000);

    if ( ! ptcaBuff->isInitSuccess() ) {
      TC_LOG(LOG_ERR, "TeracadaArray::printToBuff(): Failed to initialize print buffer array");
      goto ERREXIT;
    }
  }

  ptcaBuff->insertBack("array([ ");

  if constexpr ( std::is_same_v<tDataType, tc_str> ) {
    if ( std::is_same_v<tDataType, tc_str> ) {
      for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
        ptcaBuff->insertBack((tDataType) get(iIter));
        ptcaBuff->insertBack(",\n");
      }
    }

  } else {
    for ( tc_int iIter = 1; iIter <= getNumElements(); iIter++ ) {
      if constexpr (std::is_same_v<tDataType, tc_byte>)
        snprintf(pcIntStr, sizeof(caIntStr), "%d ", *((tc_byte*) get(iIter)));

      if constexpr (std::is_same_v<tDataType, tc_int>)
        snprintf(pcIntStr, sizeof(caIntStr), "%ld ", *((tc_int*) get(iIter)));

      if constexpr (std::is_same_v<tDataType, tc_decimal>)
        snprintf(pcIntStr, sizeof(caIntStr), "%lf ", *((tc_decimal*) get(iIter)));

      if constexpr (std::is_same_v<tDataType, tc_char>)
        snprintf(pcIntStr, sizeof(caIntStr), "%c", *((tc_char*) get(iIter)));

      ptcaBuff->insertBack(pcIntStr);
      memset(pcIntStr, 0, sizeof(caIntStr));
    }
  }

  ptcaBuff->insertBack("])");

  EXIT:
    return ptcaBuff;

  ERREXIT:
    return NULL;
}
