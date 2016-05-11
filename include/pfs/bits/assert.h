/*
 * assert.h
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_ASSERT_H__
#define __PFS_BITS_ASSERT_H__

#include <pfs/bits/config.h>

EXTERN_C_BEGIN

void pfs_assert (const char * file, int line, const char * text);

EXTERN_C_END

#ifndef NDEBUG
#	define PFS_DEBUG(x) x
#	define PFS_ASSERT(expr) if (!(expr)) { pfs_assert(__TFILE__, __LINE__, #expr); }
#	define PFS_ASSERT_X(expr,text) if (!(expr)) { pfs_assert(__TFILE__, __LINE__, text); }

//#	define PFS_WARN(x) \
//		fprintf(stderr, "WARN: (%s[%d]): %s\n", __TFILE__, __LINE__, x)

#else
#	define PFS_DEBUG(x)
#	define PFS_ASSERT(x)
#	define PFS_ASSERT_X(x,y)
#endif /* !NDEBUG */

///*
// * System error exception
// */
//#define PFS_THROW_X(errstr)                                 \
//	if (true) {                                             \
//		fprintf(stderr, "ERROR: (%s[%d]): %s\n"             \
//				, __TFILE__                                 \
//				, __LINE__                                  \
//				, errstr);                                  \
//		::exit(-1);                                         \
//	}
//
///*
// * System error exception
// */
//#define PFS_THROW_SYSERR(xerrno)                            \
//	if ((xerrno) != 0) {                                    \
//		fprintf(stderr, "ERROR: (%s[%d]): %s (errno=%d)\n"  \
//				, __TFILE__                                 \
//				, __LINE__                                  \
//				, strerror(xerrno)                          \
//				, xerrno);                                  \
//		::exit(-1);                                         \
//	}

#define PFS_ASSERT_FORMAT(x)      PFS_ASSERT(x)
#define PFS_ASSERT_RANGE(x)       PFS_ASSERT(x)
#define PFS_ASSERT_OVERFLOW(x)    PFS_ASSERT(x)
#define PFS_ASSERT_BAD_CAST(x)    PFS_ASSERT(x)
#define PFS_ASSERT_UNEXPECTED()   PFS_ASSERT(false)
#define PFS_ASSERT_UNEXPECTED_X(errstr) PFS_ASSERT_X(false, errstr)
#define PFS_ASSERT_NULLPTR(x)     PFS_ASSERT((x) != 0)
#define PFS_ASSERT_IS_NULL(x)     PFS_ASSERT(!(x).is_null())
#define PFS_ASSERT_DOMAIN(errstr) PFS_ASSERT_X(false, errstr)


/*
 * Special case of assert.
 * Used when need to implement some code.
 */
#define PFS_ASSERT_TODO() PFS_ASSERT(false)

#endif /* __PFS_BITS_ASSERT_H__ */
