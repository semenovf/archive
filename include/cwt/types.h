#ifndef __CWT_TYPES_H__
#define __CWT_TYPES_H__

#include <sys/types.h>
#include <cwt/global.h>

#ifndef _WINDEF_
	typedef enum BOOL { FALSE, TRUE } BOOL;
#endif

#ifdef CWT_UNICODE
#	ifdef CWT_CC_MSC
#		include <tchar.h>
#		ifdef __cplusplus
#			include <xstring>
#		endif
#	endif
#	include <wchar.h>
#	include <wctype.h>
	typedef wchar_t 		   CWT_CHAR;
#else
	typedef char 		       CWT_CHAR;
#endif

typedef unsigned char          UCHAR;
typedef char                   SBYTE;
typedef unsigned char          BYTE;
typedef short int              SHORT;
typedef unsigned short int     USHORT;
typedef int                    INT;
typedef unsigned int           UINT;
typedef long                   LONG;
typedef unsigned long          ULONG;
typedef long long              LONGLONG;
typedef unsigned long long     ULONGLONG;
typedef char*                  TEXT;

#ifndef CWT_CC_MSC
	typedef char               INT8;
	typedef unsigned char      UINT8;
	typedef short int          INT16;
	typedef unsigned short int UINT16;
	typedef long               INT32;
	typedef unsigned long      UINT32;
	typedef long long          INT64;
	typedef unsigned long long UINT64;
#endif

typedef enum CwtTypeId {
	  CwtType_NULL
	, CwtType_CHAR
	, CwtType_UCHAR
	, CwtType_SHORT
	, CwtType_USHORT
	, CwtType_INT
	, CwtType_UINT
	, CwtType_LONG
	, CwtType_ULONG
	, CwtType_LONGLONG
	, CwtType_ULONGLONG
	, CwtType_FLOAT
	, CwtType_DOUBLE
	, CwtType_STRING
	, CwtType_TEXT
	, CwtType_BLOB
	, CwtType_TIME
	, CwtType_DATE
	, CwtType_TIMESTAMP
	, CwtType_SBYTE  = CwtType_CHAR
	, CwtType_BYTE   = CwtType_UCHAR
	, CwtType_INT8   = CwtType_SBYTE
	, CwtType_UINT8  = CwtType_BYTE
	, CwtType_INT16  = CwtType_SHORT
	, CwtType_UINT16 = CwtType_USHORT
	, CwtType_INT32  = CwtType_LONG
	, CwtType_UINT32 = CwtType_ULONG
	, CwtType_INT64  = CwtType_LONGLONG
	, CwtType_UINT64 = CwtType_ULONGLONG
} CwtTypeId;

#define cwtIsNumberTypeId(tid) (((tid) > CwtType_NULL && (tid) < CwtType_STRING) ? TRUE : FALSE)

#if !(defined(_SIZE_T_DEFINED) || defined(__DJ_size_t) )
	typedef unsigned int size_t;
#	define _SIZE_T_DEFINED
#	define __DJ_size_t
#endif

#if !(defined(_SSIZE_T_DEFINED) || defined(__DJ_ssize_t))
	typedef long int ssize_t;
#define 	_SSIZE_T_DEFINED
#define 	__DJ_ssize_t
#endif

#ifndef CWT_CC_WATCOM
	typedef ssize_t off_t;
#endif

#include <limits.h>

#define CWT_INT8_MAX  127
#define CWT_INT8_MIN  (-127-1)
#define CWT_UINT8_MAX  255
#define CWT_INT16_MAX  32767
#define	CWT_INT16_MIN  (-32768)
#define	CWT_UINT16_MAX 65535
#define CWT_INT32_MAX  2147483647
#define	CWT_INT32_MIN (-2147483647-1)
#define	CWT_UINT32_MAX 4294967295
#define CWT_INT64_MIN (-9223372036854775807LL-1LL)
#define CWT_INT64_MAX  9223372036854775807LL
#define CWT_UINT64_MAX 18446744073709551615ULL

#define CWT_SBYTE_MAX  127
#define CWT_SBYTE_MIN  (-127-1)
#define CWT_BYTE_MAX   255
#define CWT_SHORT_MAX  32767
#define CWT_SHORT_MIN  (-32768)
#define CWT_USHORT_MAX 65535
#define CWT_INT_MAX    INT_MAX
#define CWT_INT_MIN    INT_MIN
#define CWT_UINT_MAX   UINT_MAX
#define CWT_LONG_MAX   LONG_MAX
#define CWT_LONG_MIN   LONG_MIN
#define CWT_ULONG_MAX  ULONG_MAX
#define CWT_LONGLONG_MIN (-9223372036854775807LL-1LL)
#define CWT_LONGLONG_MAX  9223372036854775807LL
#define CWT_ULONGLONG_MAX 18446744073709551615ULL

#define CWT_SIZE_T_MAX UINT_MAX

#if defined(_T)
#	undef _T
#endif
#ifdef CWT_UNICODE
#	define _T(s)   L##s
#	define _Tr(s)  L##s
/*
#	define _Tr(s)  jq::String().fromUtf8(s)
*/
#	define _WIDEN(x) _T(x) /* http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
#	define _TFILE_ _WIDEN(__FILE__)
#else
#	define _T(s)  s
#	define _Tr(s)  s
#	define _TFILE_	__FILE__
#endif

#endif /* ! __CWT_TYPES_H__ */
