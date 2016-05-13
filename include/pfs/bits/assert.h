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
#	define PFS_ASSERT(expr) if (!(expr)) { pfs_assert(__TFILE__, __LINE__, #expr); }
#	define PFS_ASSERT_X(expr,text) if (!(expr)) { pfs_assert(__TFILE__, __LINE__, text); }
#else
#	define PFS_ASSERT(x) PFS_UNUSED(x)
#	define PFS_ASSERT_X(x,y) PFS_UNUSED(x)
#endif /* !NDEBUG */

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
