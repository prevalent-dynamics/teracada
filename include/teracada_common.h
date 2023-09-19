#ifndef _TERACADA_COMMON_H
#define _TERACADA_COMMON_H

#include <cstdint>

#include "logman.h"


#define TC_LOG(level, fmt, ...) LOGMAN("Teracada", level, fmt, ##__VA_ARGS__)

#define EXTERN_C extern "C"


typedef uint8_t  tc_byte;
typedef int32_t  tc_int32;
typedef int64_t  tc_int64;
typedef float    tc_float;
typedef double   tc_double;
typedef char     tc_char;
typedef char*    tc_str;
typedef bool     tc_bool;
typedef void     tc_void;

#if defined(_TERACADA_INT32) || defined(_TERACADA_DTYPE32)
  #pragma message "TERACADA BUILD: _TERACADA_INT32 (_TERACADA_DTYPE32)"
  typedef tc_int32 tc_int;
#elif defined(_TERACADA_INT64) || defined(_TERACADA_DTYPE64)
  #pragma message "TERACADA BUILD: _TERACADA_INT64 (_TERACADA_DTYPE64)"
  typedef tc_int64 tc_int;
#else
  #pragma message "TERACADA BUILD: _TERACADA_INT32 (_TERACADA_DTYPE32)"
  typedef tc_int32 tc_int;
#endif 

#if defined(_TERACADA_DECIMAL32) || defined(_TERACADA_DTYPE32)
  #pragma message "TERACADA BUILD: _TERACADA_DECIMAL32 (_TERACADA_DTYPE32)"
  typedef tc_float tc_decimal;
#elif defined(_TERACADA_DECIMAL64) || defined(_TERACADA_DTYPE64)
  #pragma message "TERACADA BUILD: _TERACADA_DECIMAL64 (_TERACADA_DTYPE64)"
  typedef tc_double tc_decimal;
#else
  #pragma message "TERACADA BUILD: _TERACADA_DECIMAL32 (_TERACADA_DTYPE32)"
  typedef tc_float tc_decimal;
#endif 

struct stdTeracadaDictNode {
  tc_byte b8DataType;
  tc_void* pvValue;
  struct stdTeracadaDictNode* stiChild;
};

typedef struct stdTeracadaDictNode tc_dict;

enum {
  TC_NONE,
  TC_BYTE,
  TC_INT,
  TC_DECIMAL,
  TC_CHAR,
  TC_STRING,
  TC_ARRAY,
  TC_DICT
};

#endif
