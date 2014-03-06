/**
 * @file   config.h
 * @author wladt
 * @date   Nov 30, 2012 8:08:31 AM
 *
 * @brief
 */

#ifndef __PFS_CONFIG_H__
#define __PFS_CONFIG_H__

#if _POSIX_C_SOURCE >= 199309L
#	define HAVE_NANOSLEEP
#endif

#if _POSIX_C_SOURCE >= 200112L
#	define HAVE_POLL   1
#	define HAVE_SELECT 1
#	define HAVE_PTHREAD 1
#endif

#if _POSIX_C_SOURCE >= 1 || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define HAVE_SIGACTION 1
#endif

#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _BSD_SOURCE || _SVID_SOURCE || _POSIX_SOURCE
#	define HAVE_LOCALTIME_R 1
#endif

#if defined(NDEBUG) || defined(_NDEBUG)
#	undef NDEBUG
#	define NDEBUG 1
#endif

#if defined(__int8_t_defined)
#	define HAVE_INT8_T 1
#endif

#if defined(_SIZE_T_DEFINED_)    \
	|| defined(_SIZE_T_DEFINED)  \
	|| defined(__DJ_size_t)      \
	|| defined(__size_t_defined) \
	|| defined(_SIZE_T_)
#	define HAVE_SIZE_T
#endif

#if defined(_SSIZE_T_DEFINED_)   \
	|| defined(_SSIZE_T_DEFINED) \
	|| defined(__DJ_ssize_t)     \
	|| defined(__ssize_t_defined)
#	define HAVE_SSIZE_T 1
#endif

#if defined(_INTPTR_T_DEFINED)
#	define HAVE_INTPTR_T 1
#endif

#if defined(_UINTPTR_T_DEFINED)
#	define HAVE_UINTPTR_T 1
#endif

#ifdef __cplusplus
#	if __cplusplus >= 201103L
#		define HAVE_NULLPTR          1
#		define HAVE_KEYWORD_DECLTYPE 1
#		define HAVE_KEYWORD_FINAL    1
#		define HAVE_KEYWORD_OVERRIDE 1

#		define override override
#		define final final
#	else
#		define override
#		define final
#	endif
#endif

#endif /* __PFS_CONFIG_H__ */