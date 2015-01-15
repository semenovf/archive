/*
 * assert.h
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_PP_ASSERT_H__
#define __PFS_PP_ASSERT_H__

#ifndef NDEBUG
#	if defined(__cplusplus) && ! defined(PFS_CC_BORLAND_REAL)
namespace pfs {
struct __verify
{
	bool operator () (bool predicate
			, const char * prefix
			, const char * file, int lineno, const char * text) const
	{
		if (!predicate)
			fprintf(stderr, "%s(%s[%d]): %s\n", prefix, file, lineno, text);
		return predicate;
	}
};
}
#		define PFS_VERIFY(expr)       pfs::__verify()((expr), "WARN: ", __TFILE__, __LINE__, #expr)
#		define PFS_VERIFY_X(expr,text) pfs::__verify()((expr), "WARN: ", __TFILE__, __LINE__, (text))
#	else /* !__cplusplus */
#
#	endif
#else /* !NDEBUG */
#	define PFS_VERIFY(expr) (expr)
#	define PFS_VERIFY_X(expr,text) (expr)
#endif

#ifndef NDEBUG
#	define PFS_DEBUG(expr)       expr
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
#		define PFS_ASSERT_X(expr,text) if (! pfs::__verify()((expr), "ERROR: ", __TFILE__, __LINE__, (text))) { ::exit(-1);/*assert(expr);*/ }
#	endif

#else
#	define PFS_DEBUG(expr)
#	define PFS_ASSERT(expr)
//#	define PFS_ASSERT_TRACE(expr,trace_expr)
#	define PFS_ASSERT_X(expr,msg)

#endif /* !NDEBUG */

#define PFS_ASSERT_FORMAT(expr) PFS_ASSERT(expr)
#define PFS_ASSERT_RANGE(expr) PFS_ASSERT(expr)
#define PFS_ASSERT_OVERFLOW(expr) PFS_ASSERT(expr)


#endif /* __PFS_ASSERT_H__ */
