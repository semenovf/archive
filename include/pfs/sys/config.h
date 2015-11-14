/**
 * @file   config.h
 * @author wladt
 * @date   Nov 30, 2012 8:08:31 AM
 *
 * @brief
 */

#ifndef __PFS_SYS_CONFIG_H__
#define __PFS_SYS_CONFIG_H__

#include <sys/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h> /* for NAN */

/*
 * C99-specific header
 */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) /* c99 or higher */ \
	|| (defined(_MSC_VER) && _MSC_VER >= 1600) /* msvc >= MSVC++ 10.0 */           \
    || defined(__INT8_TYPE__) /* gcc w/o -std=c99 or higher option */

#   define __STDC_LIMIT_MACROS /* see stdint.h for comments about this macro */
#   include <stdint.h>

#	define PFS_HAVE_INT8_T 1 // TODO deprecated
#   define PFS_HAVE_STDINT 1
#endif

#ifndef SIZE_MAX
#	error NO SIZE_MAX
#endif

#ifdef _MSC_VER
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

/* Defined in WinDef.h (included in windows.h) */
#ifdef max
#	undef max
#endif

/* Defined in WinDef.h (included in windows.h) */
#ifdef min
#	undef min
#endif

#ifdef __DEPRECATED__
#if defined(_MSC_VER)/* && defined(__cplusplus)*/
/* for suppressing warning 'C4996' in MSVC warning :
 *      'posix_name': The POSIX name for this item is deprecated.
 *      Instead, use the ISO C++ conformant name: _posix_name.
 *      See online help for details.
 */
#   define PFS_ISO_CPP_NAME(posix_name) _##posix_name
#else
#   define PFS_ISO_CPP_NAME(posix_name) posix_name
#endif
#endif


#ifdef EXTERN_C_BEGIN
#   undef EXTERN_C_BEGIN
#endif

#ifdef __cplusplus
#   ifndef  EXTERN_C_BEGIN
#       define EXTERN_C_BEGIN extern "C" {
#   endif
#   define EXTERN_C_END }
#else
#   ifndef  EXTERN_C_BEGIN
#       define EXTERN_C_BEGIN
#   endif
#   define EXTERN_C_END
#endif

#if defined(_UNICODE) || defined(UNICODE)
#   define PFS_UNICODE
#endif

/* http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958 */
#ifdef DLL_API
#   undef DLL_API
#endif
#if defined(_MSC_VER)
#   define DLL_API_EXPORT __declspec(dllexport)
#   define DLL_API_IMPORT __declspec(dllimport)
#   ifdef DLL_EXPORTS
#       define DLL_API DLL_API_EXPORT
#       define DLL_TEMPLATE
#   else
#       define DLL_API DLL_API_IMPORT
#       define DLL_TEMPLATE extern
#   endif
/*# define _UNICODE*/
#else
#   define DLL_API_EXPORT
#   define DLL_API_IMPORT
#   define DLL_API
#   define DLL_TEMPLATE
#endif

#if _POSIX_C_SOURCE >= 199309L
#	define PFS_HAVE_NANOSLEEP
#endif

#if _POSIX_C_SOURCE >= 200112L
#	define PFS_HAVE_POLL   1
#	define PFS_HAVE_SELECT 1
#	define PFS_HAVE_PTHREAD 1
#endif

#if _POSIX_C_SOURCE >= 1 || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define PFS_HAVE_SIGACTION 1
#endif

#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
#	define PFS_HAVE_LOCALTIME_R 1
#endif

#if defined(NDEBUG) || defined(_NDEBUG)
#	undef NDEBUG
#	define NDEBUG 1
#endif

#if defined(_SIZE_T_DEFINED_)    \
	|| defined(_SIZE_T_DEFINED)  \
	|| defined(__DJ_size_t)      \
	|| defined(__size_t_defined) \
	|| defined(_SIZE_T_)         \
	|| defined(_SIZE_T)              /* borland c++ */
#	define PFS_HAVE_SIZE_T 1
#endif

#if defined(_SSIZE_T_DEFINED_)   \
	|| defined(_SSIZE_T_DEFINED) \
	|| defined(__DJ_ssize_t)     \
	|| defined(__ssize_t_defined)
#	define PFS_HAVE_SSIZE_T 1
#endif

#if defined(_INTPTR_T_DEFINED) /* msvc */     \
    || defined(__intptr_t_defined) /* gcc */
#	define PFS_HAVE_INTPTR_T 1
#endif

#if defined(_UINTPTR_T_DEFINED) \
    || defined(__intptr_t_defined) /* gcc */
#	define PFS_HAVE_UINTPTR_T 1
#endif

#if defined(__PTRDIFF_TYPE__)                  \
	|| defined(_PTRDIFF_T_DEFINED)  /* msvc */ \
	|| defined(_PTRDIFF_T)          /* borland c++ */
#	define PFS_HAVE_PTRDIFF_T 1
#endif

#ifdef __cplusplus
#	if __cplusplus >= 201103L
#		define PFS_HAVE_NULLPTR          1
#		define PFS_HAVE_KEYWORD_DECLTYPE 1
#		define PFS_HAVE_KEYWORD_FINAL    1
#		define PFS_HAVE_KEYWORD_OVERRIDE 1

#		define override override
#		define final final
#	else
#		define override
#		define final
#	endif
#endif

#if _XOPEN_SOURCE >= 600 || _ISOC99_SOURCE || _POSIX_C_SOURCE >= 200112L
#	define PFS_HAVE_STRTOLD 1
#endif

#if _BSD_SOURCE || _XOPEN_SOURCE || _POSIX_C_SOURCE >= 200112L
#   define PFS_HAVE_FSYNC 1
#endif

#if _BSD_SOURCE || _XOPEN_SOURCE >= 500 || _ISOC99_SOURCE || _POSIX_C_SOURCE >= 200112L
#	define PFS_HAVE_SNPRINTF  1
#	define PFS_HAVE_VSNPRINTF 1
#endif


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
    || defined(_WIN64)                                              \
    || (defined(__GNUC__) && (__x86_64__ || __ppc64__))             \
    || defined(__LP64__)                                            \
    || defined(_LP64)
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

#ifdef ULLONG_MAX
#   define PFS_HAVE_LONGLONG 1
#endif

#ifdef LDBL_MIN
#   define PFS_HAVE_LONG_DOUBLE 1
#endif

#if !defined(__cplusplus)
#   if !defined(inline)
#       define inline __inline
#   endif
#endif

#if defined(_MSC_VER) && defined(PFS_UNICODE)
#   define __WIDEN(x) x /* http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
#   define __TFILE__ __WIDEN(__FILE__)
#else
#   define __WIDEN(x) x
#   define __TFILE__ __FILE__
#endif

#define _Tr(s)  s

#define PFS_UNUSED(x)         ((void)(x))
#define PFS_UNUSED2(x1,x2)    ((void)(x1));((void)(x2))
#define PFS_UNUSED3(x1,x2,x3) ((void)(x1));((void)(x2));((void)(x3))

#endif /* __PFS_SYS_CONFIG_H__ */
