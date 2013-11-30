#ifndef __CWT_TYPES_H__
#define __CWT_TYPES_H__

#include <pfs/limits.h>

#ifndef HAVE_NULLPTR
#ifndef NULL
#	ifndef __cplusplus
#		define nullptr ((void *)0)
#	else
#		define nullptr 0
#	endif
#else
#	define nullptr NULL
#endif
#endif /* HAVE_NULLPTR */

#ifndef __cplusplus
	typedef unsigned char bool;
#	define false 0
#	define true (!(false))
#else
	/*typedef bool bool_t;*/
#endif

#ifndef HAVE_INT8_T
	typedef signed char        int8_t;
	typedef unsigned char      uint8_t;
	typedef signed short       int16_t;
	typedef unsigned short     uint16_t;
	typedef signed int         int32_t;
	typedef unsigned int       uint32_t;

#	ifdef HAVE_INT64
		typedef signed long long   int64_t;
		typedef unsigned long long uint64_t;
#	endif
#endif

typedef uint8_t            uchar_t;
typedef int8_t             sbyte_t;
typedef uint8_t            byte_t;
typedef int16_t            short_t;
typedef uint16_t           ushort_t;
typedef int32_t            int_t;
typedef uint32_t           uint_t;
#ifdef HAVE_INT64
	typedef int64_t        long_t;
	typedef uint64_t       ulong_t;
#else
	typedef long           long_t;
	typedef unsigned long  ulong_t;
#endif

#ifndef HAVE_SIZE_T
	typedef uint32_t size_t;
#endif

#ifndef HAVE_SSIZE_T
	typedef int32_t ssize_t;
#endif

typedef ssize_t offset_t;

#ifndef HAVE_INTPTR_T
#if PFS_OS_BITS == 64
	typedef long int          intptr_t;
#else
	typedef int               intptr_t;
#endif
#endif

#ifndef HAVE_UINTPTR_T
#if PFS_OS_BITS == 64
	typedef unsigned long int uintptr_t;
#else
	typedef unsigned int      uintptr_t;
#endif
#endif

#ifdef __COMMENT__
/* Unicode code units and points*/
typedef int16_t  uc_char16_t;
typedef int32_t  uc_char32_t;
typedef uint16_t uc_uchar16_t;
typedef uint32_t uc_uchar32_t;
typedef uint8_t  uc_u8_t;     /* UTF-8 code unit */
typedef uint16_t uc_u16_t;    /* UTF-16 code unit */
typedef uint32_t uc_u32_t;    /* UTF-32 code unit */

#ifdef CWT_UC_CHAR32
typedef uc_char32_t     uc_char_t;   /* unicode code point*/
typedef uc_uchar32_t    uc_uchar_t;   /* unicode code point*/
#	define CWT_UC_CHAR_MIN  ((uc_char_t)0)
#	define CWT_UC_CHAR_MAX  0x10FFFF
#else
typedef uc_char16_t     uc_char_t;   /* unicode code point*/
typedef uc_uchar16_t    uc_uchar_t;
#	define CWT_UC_CHAR_MIN  ((uc_char_t)0)
#	define CWT_UC_CHAR_MAX  0xFFFF
#endif
#endif

#undef uuid_t
typedef struct {
	uint32_t  time_low;
    uint16_t  time_mid;
    uint16_t  time_hi_and_version;
    uint8_t   clock_seq_hi_and_reserved;
    uint8_t   clock_seq_low;
    byte_t    node[6];
} uuid_t;

//#define CWT_NULL_UUID { 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0}}

#endif /* ! __CWT_TYPES_H__ */
