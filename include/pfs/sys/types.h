/*
 * types.h
 *
 *  Created on: May 22, 2015
 *      Author: wladt
 */

#ifndef __PFS_SYS_TYPES_H__
#define __PFS_SYS_TYPES_H__

#ifdef PFS_HAVE_STDINT
    typedef int8_t       int8_t;
    typedef uint8_t      uint8_t;
    typedef int16_t      int16_t;
    typedef uint16_t     uint16_t;
    typedef int32_t      int32_t;
    typedef uint32_t     uint32_t;

#   ifdef PFS_HAVE_INT64
        typedef int64_t  int64_t;
        typedef uint64_t uint64_t;
#   endif
#elif defined(PFS_CC_BORLAND_REAL)
    typedef signed char        int8_t;
    typedef unsigned char      uint8_t;
    typedef signed int         int16_t;
    typedef unsigned int       uint16_t;
    typedef signed long        int32_t;
    typedef unsigned long      uint32_t;
#else
#   error "Unsupported platform yet: PFS_HAVE_STDINT is undefined"
#endif

typedef uint8_t byte_t;

#ifdef PFS_HAVE_INT64
    typedef int64_t  integral_t;
    typedef uint64_t uintegral_t;
#else
    typedef int32_t  integral_t;
    typedef uint32_t uintegral_t;
#endif

#ifdef PFS_HAVE_LONG_DOUBLE
    typedef long double    real_t;
#else
    typedef double         real_t;
#endif

#if ! defined(__cplusplus) || defined(PFS_CC_BORLAND_REAL)
    typedef unsigned char bool;
#   define false 0
#   define true (!(false))
#endif

#ifdef PFS_HAVE_SIZE_T
    typedef size_t size_t;
#else
    /*typedef unsigned long int size_t;*/
#   error "Unsupported platform yet: PFS_HAVE_SIZE_T is undefined"
#endif

#ifdef PFS_HAVE_SSIZE_T
    typedef ssize_t ssize_t;
#else
#	ifdef PFS_HAVE_INT64
    	typedef int64_t ssize_t;
    #else
    	typedef long int ssize_t;
#	endif
#endif

typedef ssize_t offset_t;

#ifdef PFS_HAVE_INTPTR_T
    typedef intptr_t intptr_t;
#else
/*
#   if PFS_OS_BITS == 64
        typedef long int          intptr_t;
#   else
        typedef int               intptr_t;
#   endif
*/
#   error "Unsupported platform yet: PFS_HAVE_INTPTR_T is undefined"
#endif

#ifdef PFS_HAVE_UINTPTR_T
    typedef uintptr_t uintptr_t;
#else
/*
#   if PFS_OS_BITS == 64
        typedef unsigned long int uintptr_t;
#   else
        typedef unsigned int      uintptr_t;
#   endif
*/
#   error "Unsupported platform yet: PFS_HAVE_UINTPTR_T is undefined"
#endif

#ifdef PFS_HAVE_PTRDIFF_T
    typedef ptrdiff_t ptrdiff_t;
#else
    /*typedef long int ptrdiff_t;*/
#   error "Unsupported platform yet: PFS_HAVE_PTRDIFF_T is undefined"
#endif

#if (defined(PFS_OS_WIN32) || defined(PFS_OS_WIN64)) && defined(PFS_UNICODE)
	typedef wchar_t argv_t;
#else
	typedef char argv_t;
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
/*#define PFS_NULL_UUID { 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0}}*/

#ifndef PFS_HAVE_NULLPTR
#ifndef NULL
#   ifndef __cplusplus
#       define nullptr ((void *)0)
#   else
#       if __cplusplus >= 201103L
#           define nullptr nullptr
#       else
#           define nullptr 0
#       endif
#   endif
#else
#   define nullptr NULL
#endif
#endif /* PFS_HAVE_NULLPTR */

#endif /* __PFS_SYS_TYPES_H__ */
