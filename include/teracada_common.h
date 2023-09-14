#ifndef _TERACADA_COMMON_H
#define _TERACADA_COMMON_H

#include <cstdint>

#include "logman.h"


#define TC_LOG(level, fmt, ...) LOGMAN("Teracada", level, fmt, ##__VA_ARGS__)

#define EXTERN_C extern "C"


/* All Teracada generic data types (start)

  Notes:
    1. Always use tc_* data types instead of the generics,
         so that their size and definition can be controlled at compile time.
         This also provide the much required homogeneity.
    2. The tc_byte will always be unsigned 8-bit. For signed variables, just use tc_int.
*/
#if defined(_TERACADA_INT32) || defined(_TERACADA_DTYPE32)
  #pragma message "TERACADA BUILD: _TERACADA_INT32 (_TERACADA_DTYPE32)"
  typedef int32_t tc_int;
#elif defined(_TERACADA_INT64) || defined(_TERACADA_DTYPE64)
  #pragma message "TERACADA BUILD: _TERACADA_INT64 (_TERACADA_DTYPE64)"
  typedef int64_t tc_int;
#else
  #pragma message "TERACADA BUILD: _TERACADA_INT32 (_TERACADA_DTYPE32)"
  typedef int32_t tc_int;
#endif 

#if defined(_TERACADA_DECIMAL32) || defined(_TERACADA_DTYPE32)
  #pragma message "TERACADA BUILD: _TERACADA_DECIMAL32 (_TERACADA_DTYPE32)"
  typedef float  tc_decimal;
#elif defined(_TERACADA_DECIMAL64) || defined(_TERACADA_DTYPE64)
  #pragma message "TERACADA BUILD: _TERACADA_DECIMAL64 (_TERACADA_DTYPE64)"
  typedef double tc_decimal;
#else
  #pragma message "TERACADA BUILD: _TERACADA_DECIMAL32 (_TERACADA_DTYPE32)"
  typedef float     tc_decimal;
#endif 

typedef uint8_t tc_byte;
typedef char    tc_char;
typedef char    tc_str;
typedef bool    tc_bool;

/* All Teracada generic data types (end) */


/* All defined Teracada data types */

enum {
  TC_BYTE,
  TC_INT,
  TC_DECIMAL,
  TC_CHAR,
  TC_STRING,
  TC_ARRAY
};

#endif
