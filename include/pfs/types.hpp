/*
 * types.hpp
 *
 *  Created on: Nov 24, 2015
 *      Author: wladt
 */

#ifndef __PFS_TYPES_HPP__
#define __PFS_TYPES_HPP__

/* @see http://www.ibm.com/developerworks/ru/library/l-port64/
 *
 * Type model
 * ===================================
 *         ILP32  LP64  LLP64  ILP64
 * -----------------------------------
 * char        8     8      8      8
 * short      16    16     16     16
 * int        32    32     32     64
 * long       32    64     32     64
 * long long  64    64     64     64
 * pointer    32    64     64     64
 * ===================================
 */
#if (defined(__WORDSIZE) && __WORDSIZE == 64)                       \
    || (defined(UINTPTR_MAX) && UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF)  \
    ||  defined(_WIN64)                                             \
    || (defined(__GNUC__) && (__x86_64__ || __ppc64__))             \
    ||  defined(__LP64__)                                           \
    ||  defined(_LP64)                                              \
	||  defined(__INT64_TYPE__)

#   define PFS_HAVE_INT64 1
#endif

#ifndef PFS_HAVE_INT64
/*
 * Valid for ILP32
 */
#   if defined(ULLONG_MAX) && defined(ULONG_MAX) && (ULLONG_MAX > ULONG_MAX)
#       define PFS_HAVE_INT64 1
#   endif
#endif

/*
 * C99-specific header
 */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) /* c99 or higher */ \
	|| (defined(_MSC_VER) && _MSC_VER >= 1600) /* msvc >= MSVC++ 10.0 */           \
    || defined(__INT8_TYPE__) /* gcc w/o -std=c99 or higher option */

#   define __STDC_LIMIT_MACROS /* see stdint.h for comments about this macro */
#   include <stdint.h>

#	define PFS_HAVE_INT8_T 1
#   define PFS_HAVE_STDINT 1
#endif

#include <cfloat>

#if PFS_HAVE_INT8_T
    typedef int8_t   int8_t;
    typedef uint8_t  uint8_t;
    typedef int16_t  int16_t;
    typedef uint16_t uint16_t;
    typedef int32_t  int32_t;
    typedef uint32_t uint32_t;

#   ifdef PFS_HAVE_INT64
        typedef int64_t  int64_t;
        typedef uint64_t uint64_t;
#   endif

    typedef intmax_t  intmax_t;
    typedef uintmax_t uintmax_t;
#else
#	error "Need to implement intX_t types"

#ifdef PFS_HAVE_INT64
	typedef int64_t  intmax_t;
	typedef uint64_t uintmax_t;
#else
	typedef int32_t  intmax_t;
	typedef uint32_t uintmax_t;
#endif

#endif

typedef uint8_t byte_t;

#endif /* __PFS_TYPES_HPP__ */