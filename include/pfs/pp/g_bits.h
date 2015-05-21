/**
 * @file   global_bits.h
 * @author wladt
 * @date   Jan 16, 2013 12:25:00
 *
 */

#ifndef __PFS_GLOBAL_BITS_H__
#define __PFS_GLOBAL_BITS_H__

#ifdef PFS_CC_BORLAND_REAL

#	include <limits.h>

#	define PFS_OS_16BITS
#	define PFS_OS_BITS    16

#else

#include <stdint.h>
#include <limits.h>

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
 *
 */


#if (defined(__WORDSIZE) && __WORDSIZE == 64)                         \
	|| (defined(UINTPTR_MAX) && UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF)    \
	|| defined(_WIN64)                                                \
	|| (defined(__GNUC__) && (__x86_64__ || __ppc64__))               \
	|| defined(__LP64__)
#	define PFS_OS_64BITS
#	define PFS_OS_BITS    64
#	define HAVE_INT64 1        /* TODO deprecated */
#	define PFS_HAVE_INT64 1
#else
#	define PFS_OS_32BITS
#	define PFS_OS_BITS    32
#endif

#ifndef PFS_HAVE_INT64
/*
 * Valid for ILP32
 */
#	if defined(ULLONG_MAX) && defined(ULONG_MAX) && (ULLONG_MAX > ULONG_MAX)
#		define HAVE_INT64 1        /* TODO deprecated */
#		define PFS_HAVE_INT64 1
#	endif
#endif

#ifdef ULLONG_MAX
#	define HAVE_LONGLONG 1     /* TODO deprecated */
#	define PFS_HAVE_LONGLONG 1
#endif

#endif /* ! __BORLANDC__ */

#endif /* __PFS_GLOBAL_BITS_H__ */
