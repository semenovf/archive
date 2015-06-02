/*
 * assert.h
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_SYS_ASSERT_H__
#define __PFS_SYS_ASSERT_H__

#include <stdio.h>

#ifndef NDEBUG
/*
 * PFS_VERIFY_IMPL emulates this code
 * if (!predicate)
 *      fprintf(stderr, "%s(%s[%d]): %s\n", prefix, file, lineno, text);
 * return predicate;
 */
#   define PFS_VERIFY_IMPL(predicate,prefix,file,lineno,text) \
        (!(!(predicate) && fprintf(stderr, "%s(%s[%d]): %s\n", prefix, file, lineno, text)) || (predicate))
#   define PFS_VERIFY(expr) PFS_VERIFY_IMPL((expr), "WARN: ", __TFILE__, __LINE__, #expr)
#   define PFS_VERIFY_X(expr,text) PFS_VERIFY_IMPL((expr), "WARN: ", __TFILE__, __LINE__, (text))
#else /* !NDEBUG */
#	define PFS_VERIFY(x) (x)
#	define PFS_VERIFY_X(x,text) (x)
#endif

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
//#		define PFS_ASSERT_TRACE(expr,trace_exp) if (!(expr)) { (void)trace_exp; assert(expr); }
#		define PFS_ASSERT_X(expr,text) if (! PFS_VERIFY_IMPL((expr), "ERROR: ", __TFILE__, __LINE__, (text))) { ::exit(-1);/*assert(expr);*/ }
#	endif

#else
#	define PFS_DEBUG(x)
#	define PFS_ASSERT(x)
/*#	define PFS_ASSERT_TRACE(expr,trace_expr)*/
#	define PFS_ASSERT_X(x,msg)

#endif /* !NDEBUG */

#define PFS_ASSERT_FORMAT(x)    PFS_ASSERT(x)
#define PFS_ASSERT_RANGE(x)     PFS_ASSERT(x)
#define PFS_ASSERT_OVERFLOW(x)  PFS_ASSERT(x)
#define PFS_ASSERT_BAD_CAST(x)  PFS_ASSERT(x)
#define PFS_ASSERT_UNEXPECTED() PFS_ASSERT(false)
#define PFS_ASSERT_NULLPTR(x)   PFS_ASSERT((x) != nullptr)
#define PFS_ASSERT_IS_NULL(x)   PFS_ASSERT(!(x).isNull())

/*
 * Special case of assert.
 * Used when need to implement some code.
 */
#define PFS_ASSERT_TODO() PFS_ASSERT(false)

#endif /* __PFS_SYS_ASSERT_H__ */
