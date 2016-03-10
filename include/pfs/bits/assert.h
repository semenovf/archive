/*
 * assert.h
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_ASSERT_H__
#define __PFS_BITS_ASSERT_H__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifndef NDEBUG
#	define PFS_DEBUG(x)       x
#	if defined(__cplusplus) && ! defined(PFS_CC_BORLAND_REAL)
#		include <cassert>
#	else
#		include <assert.h>
#	endif
#	define PFS_ASSERT(expr) assert(expr)

#   ifndef __ENDL__
#		define __ENDL__ "\n"
# 	endif

#	if defined(PFS_CC_BORLAND) /* && defined(__GRAPHICS_H) */
#		define PFS_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; \
			(void) __assertfail(                                           \
					"Assertion failed: %s, file %s, line %d" __ENDL__,     \
                    #p, __FILE__, __LINE__ ); }
#		define PFS_ASSERT_X(p,str) if( !(p) ) {                            \
			(void) __assertfail(                                           \
					"Assertion failed: %s, file %s, line %d" __ENDL__,     \
                    #p, __FILE__, __LINE__ ); }
#	elif defined(__dj_include_assert_h_)
#		define PFS_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; \
			(void) __dj_assert(#p,__FILE__,__LINE__); }
#		define PFS_ASSERT_X(p,str) if( !(p) ) { PFS_ERROR(str);            \
			(void) __dj_assert(#p,__FILE__,__LINE__); }
#	else
#		define PFS_ASSERT_X(expr,text)                                           \
		if (!(expr)) {                                                           \
			fprintf(stderr, "ERROR: (%s[%d]): %s\n", __TFILE__, __LINE__, text); \
			::exit(-1);                                                          \
		}
#	endif

#	define PFS_WARN(x) \
		fprintf(stderr, "WARN: (%s[%d]): %s\n", __TFILE__, __LINE__, x)

#else
#	define PFS_DEBUG(x)
#	define PFS_ASSERT(x)
#	define PFS_ASSERT_X(x,msg)
#	define PFS_WARN(x)

#endif /* !NDEBUG */

/*
 * System error exception
 */
#define PFS_THROW_X(errstr)                                 \
	if (true) {                                             \
		fprintf(stderr, "ERROR: (%s[%d]): %s\n"             \
				, __TFILE__                                 \
				, __LINE__                                  \
				, errstr);                                  \
		::exit(-1);                                         \
	}

/*
 * System error exception
 */
#define PFS_THROW_SYSERR(xerrno)                            \
	if ((xerrno) != 0) {                                    \
		fprintf(stderr, "ERROR: (%s[%d]): %s (errno=%d)\n"  \
				, __TFILE__                                 \
				, __LINE__                                  \
				, strerror(xerrno)                          \
				, xerrno);                                  \
		::exit(-1);                                         \
	}

#define PFS_ASSERT_FORMAT(x)      PFS_ASSERT(x)
#define PFS_ASSERT_RANGE(x)       PFS_ASSERT(x)
#define PFS_ASSERT_OVERFLOW(x)    PFS_ASSERT(x)
#define PFS_ASSERT_BAD_CAST(x)    PFS_ASSERT(x)
#define PFS_ASSERT_UNEXPECTED()   PFS_ASSERT(false)
#define PFS_ASSERT_UNEXPECTED_X(errstr) PFS_THROW_X(errstr)
#define PFS_ASSERT_NULLPTR(x)     PFS_ASSERT((x) != 0)
#define PFS_ASSERT_IS_NULL(x)     PFS_ASSERT(!(x).is_null())
#define PFS_ASSERT_DOMAIN(errstr) PFS_THROW_X(errstr)


/*
 * Special case of assert.
 * Used when need to implement some code.
 */
#define PFS_ASSERT_TODO() PFS_ASSERT(false)


/*
 * Backtrace output
 */
#if defined(__GNUC__)
#	ifndef PFS_BACKTRACE_SIZE
#		define PFS_BACKTRACE_SIZE 20
#	endif
#	ifndef PFS_BACKTRACE_FD
#		define PFS_BACKTRACE_FD    2 /* standard error */
#	endif

#	include <execinfo.h>

#	define PFS_ASSERT_BT(x)                                                             \
		if (!(x)) {                                                                     \
			void * __pfs_bt_buffer__[PFS_BACKTRACE_SIZE];                               \
			int __pfs_bt_size__ = backtrace(__pfs_bt_buffer__, PFS_BACKTRACE_SIZE);     \
			fprintf(stderr, "===BACKTRACE: (%s[%d]): %s\n", __TFILE__, __LINE__, #x);   \
			backtrace_symbols_fd(__pfs_bt_buffer__, __pfs_bt_size__, PFS_BACKTRACE_FD); \
			fprintf(stderr, "===END OF BACKTRACE\n");                                   \
			::abort();                                                                  \
		}
#else
#	define PFS_ASSERT_BT(x) PFS_ASSERT(x)
#endif

#endif /* __PFS_BITS_ASSERT_H__ */
