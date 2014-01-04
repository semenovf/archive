#ifndef __PFS_GLOBAL_H__
#define __PFS_GLOBAL_H__

#include <pfs/global_os.h>
#include <pfs/global_cc.h>
#include <pfs/global_endian.h>
#include <pfs/global_bits.h>
#ifdef __cplusplus
#	include <cstdlib>
#	include <cstring> // for strerror
#else
#	include <stdlib.h>
#	include <string.h> // for strerror
#endif


#ifdef _MSC_VER
#   define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

#if defined(_UNICODE) || defined(UNICODE)
#	define PFS_UNICODE
#endif


/* http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958 */
#ifdef DLL_API
#	undef DLL_API
#endif
#if defined(PFS_CC_MSC) && defined(PFS_CC_MSVC)
#	define DLL_API_EXPORT __declspec(dllexport)
#	define DLL_API_IMPORT __declspec(dllimport)
#	ifdef DLL_EXPORTS
#		define DLL_API DLL_API_EXPORT
#    	define DLL_TEMPLATE
#	else
#		define DLL_API DLL_API_IMPORT
#    	define DLL_TEMPLATE extern
#	endif
/*#	define _UNICODE*/
#else
#	define DLL_API_EXPORT
#	define DLL_API_IMPORT
#	define DLL_API
#	define DLL_TEMPLATE
#endif


#ifdef EXTERN_C_BEGIN
#	undef EXTERN_C_BEGIN
#endif
#ifdef __cplusplus
#	ifndef	EXTERN_C_BEGIN
#		define EXTERN_C_BEGIN extern "C" {
#	endif
#	define EXTERN_C_END }
#else
#	ifndef	EXTERN_C_BEGIN
#		define EXTERN_C_BEGIN
#	endif
#	define EXTERN_C_END
#endif

#if defined(PFS_CC_MSVC)
#	ifdef HAVE_INT64
#		define strtolong  _strtoi64
#		define strtoulong _strtoui64
#	else
#		define strtolong   strtol
#		define strtoulong  strtoull
#	endif
#else
#	ifdef HAVE_INT64
#		define strtolong  strtoll
#		define strtoulong strtoull
#	else
#		define strtolong  strtol
#		define strtoulong strtoull
#	endif
#endif

#if defined(PFS_CC_MSVC)/* && defined(__cplusplus)*/
/* for suppressing warning 'C4996' in MSVC warning :
 * 		'posix_name': The POSIX name for this item is deprecated.
 * 		Instead, use the ISO C++ conformant name: _posix_name.
 * 		See online help for details.
 */
#	define PFS_ISO_CPP_NAME(posix_name) _##posix_name
#else
#	define PFS_ISO_CPP_NAME(posix_name) posix_name
#endif


#define WORD_LOW_BYTE(WORD) ((char)((WORD)&0x00FF))
#define WORD_HI_BYTE(WORD)  ((char)(((WORD)>>8)&0x00FF))

#define PFS_MIN(a,b)  (((a) < (b)) ? (a) : (b))
#define PFS_MAX(a,b)  (((a) > (b)) ? (a) : (b))

#define _Tr(s)  s

#if defined(PFS_OS_WIN) && defined(PFS_UNICODE)
#	define __WIDEN(x) x /* http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
#	define __TFILE__ __WIDEN(__FILE__)
#else
#	define __WIDEN(x) x
#	define __TFILE__ __FILE__
#endif

#define PFS_FPRINT(stream,prefix,str) fprintf(stream, "%s (%s[%d]): %s\n", prefix, __TFILE__, __LINE__, str)
#define PFS_PRINT(prefix,str) PFS_FPRINT(stdout,prefix,str)
#define PFS_INFO(str)         PFS_FPRINT(stdout,_Tr("INFO"),str)
#define PFS_WARN(str)         PFS_FPRINT(stderr,_Tr("WARN"),str)
#define PFS_ERROR(str)        PFS_FPRINT(stderr,_Tr("ERROR"),str)
#define PFS_FATAL(str)      { PFS_FPRINT(stderr,_Tr("FATAL"),str); abort(); }

#define PFS_VERIFY(expr) if (! (expr)) { PFS_ERROR(#expr); }
#define PFS_VERIFY_ERRNO(expr,errn) if (! (expr)) {                    \
		fprintf(stderr, "ERROR (%s[%d]): %s: [errno=%d]: %s\n"         \
				, __TFILE__, __LINE__, #expr, errn, strerror(errn)); }
#define PFS_VERIFY_X(expr,str) if (! (expr)) {                         \
		fprintf(stderr, "ERROR (%s[%d]): %s: %s\n"                     \
				, __TFILE__, __LINE__, #expr, (str)); }


#ifndef NDEBUG
#	define PFS_TRACE(str)        PFS_FPRINT(stdout,_Tr("TRACE"),str)
#	define PFS_DEBUG(str)        PFS_FPRINT(stdout,_Tr("DEBUG"),str)
#	ifdef __cplusplus
#		include <cassert>
#	else
#		include <assert.h>
#	endif
#	define PFS_ASSERT(p) assert(p)

#   ifndef __ENDL__
#		define __ENDL__ "\n"
# 	endif

#	if defined(PFS_CC_BORLAND) /* && defined(__GRAPHICS_H) */
#		define PFS_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; \
			(void) __assertfail(                                           \
					_Tr("Assertion failed: %s, file %s, line %d") __ENDL__,   \
                    #p, __FILE__, __LINE__ ); }
#		define PFS_ASSERT_X(p,str) if( !(p) ) { PFS_ERROR(str); \
			(void) __assertfail(                                           \
					_Tr("Assertion failed: %s, file %s, line %d") __ENDL__,   \
                    #p, __FILE__, __LINE__ ); }
#	elif defined(__dj_include_assert_h_)
#		define PFS_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; \
			(void) __dj_assert(#p,__FILE__,__LINE__); }
#		define PFS_ASSERT_X(p,str) if( !(p) ) { PFS_ERROR(str);            \
			(void) __dj_assert(#p,__FILE__,__LINE__); }
#	else
#		define PFS_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; assert(p); }
#		define PFS_ASSERT_X(p,str) if( !(p) ) { PFS_ERROR(str); assert(p); }
#	endif

#else
#	define PFS_TRACE(str)
#	define PFS_DEBUG(str)
#	define PFS_ASSERT(exp)
#	define PFS_ASSERT_TRACE(exp,trace_exp)
#	define PFS_ASSERT_X(exp,str)

#endif /* !NDEBUG */

#if !defined(__cplusplus)
#	if !defined(inline)
#		define inline __inline
#	endif
#endif

#define PFS_UNUSED(x)         ((void)(x))
#define PFS_UNUSED2(x1,x2)    ((void)(x1));((void)(x2))
#define PFS_UNUSED3(x1,x2,x3) ((void)(x1));((void)(x2));((void)(x3))

#define PFS_MALLOCT(T)    ((T*)malloc(sizeof(T)))
#define PFS_MALLOCA(T,n)  ((T*)malloc(sizeof(T)*(n)))
#define PFS_MALLOC(sz)    malloc(sz)
#define PFS_FREE(v)       free(v)
#define PFS_REALLOC(p,sz) realloc(p,sz);

/* Two-dimensional array macros */
#define PFS_A2_NCOLS(a)          sizeof((a)[0])/sizeof((a)[0][0])
#define PFS_A2_NROWS(a)          sizeof(a)/sizeof((a)[0])
#define PFS_A2_ELEM(a,ncols,i,j) (a)[(i) * (ncols) + (j)]

#ifndef NULL
#	define NULL ((void*)0)
#endif

#ifdef __cplusplus
#	define PFS_DENY_COPY(Class)             \
	private:                                \
		Class(const Class &);               \
		Class & operator = (const Class &);
#endif


#endif /* __PFS_GLOBAL_H__ */

