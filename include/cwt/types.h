#ifndef __CWT_TYPES_H__
#define __CWT_TYPES_H__

#include <sys/types.h>
#include <cwt/global.h>

#ifndef _WINDEF_
#	ifdef TRUE
#		undef TRUE
#		undef FALSE
#	endif
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
#	include <ctype.h>
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
#ifdef CWT_OS_DOS
	typedef long               INT32;
	typedef unsigned long      UINT32;
#else
	typedef int                INT32;
	typedef unsigned int       UINT32;
#endif
	typedef long long          INT64;
	typedef unsigned long long UINT64;
#endif

/*
 * year <= 9999,month <= 12, day <= 31, hour <= 23, hour <= 59, hour <= 59.
 *
 * Compatible with MYSQL_TIME
*/
typedef struct CWT_TIME {
	UINT  year
		, mon
		, day;
	UINT  hour
		, min
		, sec;
	ULONG sec_part;
} CWT_TIME;



typedef enum _CwtTypeEnum {
	  CwtType_UNKNOWN
	, CwtType_BOOL
	, CwtType_CHAR
	, CwtType_CWT_CHAR
	, CwtType_SBYTE  = CwtType_CHAR
	, CwtType_INT8   = CwtType_SBYTE
	, CwtType_UCHAR
	, CwtType_BYTE   = CwtType_UCHAR
	, CwtType_UINT8  = CwtType_BYTE
	, CwtType_SHORT
	, CwtType_INT16  = CwtType_SHORT
	, CwtType_USHORT
	, CwtType_UINT16 = CwtType_USHORT
	, CwtType_INT
	, CwtType_INT32  = CwtType_INT
	, CwtType_UINT
	, CwtType_UINT32 = CwtType_UINT
	, CwtType_LONG
	, CwtType_ULONG
	, CwtType_LONGLONG
	, CwtType_INT64  = CwtType_LONGLONG
	, CwtType_ULONGLONG
	, CwtType_UINT64 = CwtType_ULONGLONG
	, CwtType_FLOAT
	, CwtType_DOUBLE
	, CwtType_TEXT
	, CwtType_BLOB
	, CwtType_TIME
	, CwtType_DATE
	, CwtType_DATETIME
	, CwtType_CWT_STRING
} CwtTypeEnum;

#define CWT_TYPE_IS_SCALAR(tid)  (((tid) >= CwtType_BOOL && (tid) < CwtType_TEXT) ? TRUE : FALSE)
#define CWT_TYPE_IS_INTEGER(tid) (((tid) >= CwtType_CHAR && (tid) < CwtType_FLOAT) ? TRUE : FALSE)
#define CWT_TYPE_IS_FLOAT(tid)   (((tid) >= CwtType_FLOAT && (tid) <= CwtType_DOUBLE) ? TRUE : FALSE)

#if !(defined(_SIZE_T_DEFINED) || defined(__DJ_size_t) )
	typedef unsigned int size_t;
#	define _SIZE_T_DEFINED
#	define __DJ_size_t
#endif

#if !(defined(_SSIZE_T_DEFINED) || defined(__DJ_ssize_t) || defined(__ssize_t_defined) )
	typedef long int ssize_t;
#define 	_SSIZE_T_DEFINED
#define 	__DJ_ssize_t
#endif

#if !( defined(CWT_CC_WATCOM) || defined(__off_t_defined) || defined(CWT_CC_MSC) )
	typedef ssize_t off_t;
#endif

#include <limits.h>

#define CWT_INT8_MAX  127
#define CWT_INT8_MIN  (-127-1)
#define CWT_UINT8_MAX  255
#define CWT_INT16_MAX  32767
#define	CWT_INT16_MIN  (-32768)
#define	CWT_UINT16_MAX 65535
#define	CWT_INT24_MAX  8388607
#define	CWT_INT24_MIN  (-8388608)
#define	CWT_UINT24_MAX 16777215
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
#	define _WIDEN(x) _T(x) /* http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
#	define _TFILE_ _WIDEN(__FILE__)
#else
#	define _T(s)  s
#	define _Tr(s)  s
#	define _TFILE_	__FILE__
#endif

#endif /* ! __CWT_TYPES_H__ */
