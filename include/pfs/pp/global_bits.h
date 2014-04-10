/**
 * @file   global_bits.h
 * @author wladt
 * @date   Jan 16, 2013 12:25:00
 *
 */

#ifndef __PFS_GLOBAL_BITS_H__
#define __PFS_GLOBAL_BITS_H__

#include <stdint.h>
#include <limits.h>

#if (defined(__WORDSIZE) && __WORDSIZE == 64)                         \
	|| (defined(UINTPTR_MAX) && UINTPTR_MAX == 0xFFFFFFFFFFFFFFFF)    \
	|| defined(_WIN64)                                                \
	|| (defined(__GNUC__) && (__x86_64__ || __ppc64__))               \
	|| defined(__LP64__)
#	define PFS_OS_64BITS
#	define PFS_OS_BITS    64
#else
#	define PFS_OS_32BITS
#	define PFS_OS_BITS    32
#endif

#ifdef ULLONG_MAX
#	define HAVE_INT64
#	define HAVE_LONGLONG
#endif

#endif /* __PFS_GLOBAL_BITS_H__ */
