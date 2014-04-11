#ifndef __PFS_GLOBAL_H__
#define __PFS_GLOBAL_H__

#include <pfs/pp/global_os.h>
#include <pfs/pp/global_cc.h>
#include <pfs/pp/global_endian.h>
#include <pfs/pp/global_bits.h>
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
#if defined(PFS_CC_MSVC)
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

#ifdef __DEPRECATED__
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
#endif

//#define WORD_LOW_BYTE(WORD) ((char)((WORD)&0x00FF))
//#define WORD_HI_BYTE(WORD)  ((char)(((WORD)>>8)&0x00FF))

// max/min defined as macros in WinDef.h
#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif

#ifdef __cplusplus
namespace pfs {
	template <typename T>
	inline T min(T a, T b) { return a <= b ? a : b; }

	template <typename T>
	inline T max(T a, T b) { return a >= b ? a : b; }
}
#else
#	define max(a,b)   (((a) > (b)) ? (a) : (b))
#	define min(a,b)   (((a) < (b)) ? (a) : (b))
#endif

#define _Tr(s)  s

#if defined(PFS_CC_MSVC) && defined(PFS_UNICODE)
#	define __WIDEN(x) x /* http://msdn.microsoft.com/en-us/library/b0084kay(v=vs.80).aspx */
#	define __TFILE__ __WIDEN(__FILE__)
#else
#	define __WIDEN(x) x
#	define __TFILE__ __FILE__
#endif

// TODO DEPRECATED {
#define PFS_FPRINT(stream,prefix,str) fprintf(stream, "%s (%s[%d]): %s\n", prefix, __TFILE__, __LINE__, str)
//#define PFS_PRINT(prefix,str) PFS_FPRINT(stdout,prefix,str)
//#define PFS_INFO(str)         PFS_FPRINT(stdout,"INFO",str)
#define PFS_WARN(str)         PFS_FPRINT(stderr,"WARN",str)
#define PFS_ERROR(str)        PFS_FPRINT(stderr,"ERROR",str)
//#define PFS_FATAL(str)      { PFS_FPRINT(stderr,"FATAL",str); abort(); }
// }

#define PFS_VERIFY(expr) if (! (expr)) {                               \
		fprintf(stderr, "ERROR (%s[%d]): %s\n"                         \
				, __TFILE__, __LINE__, #expr); }
#define PFS_VERIFY_ERRNO(expr,errn) if (! (expr)) {                    \
		fprintf(stderr, "ERROR (%s[%d]): %s: [errno=%d]: %s\n"         \
				, __TFILE__, __LINE__, #expr, errn, strerror(errn)); }
#define PFS_VERIFY_X(expr,str) if (! (expr)) {                         \
		fprintf(stderr, "ERROR (%s[%d]): %s: %s\n"                     \
				, __TFILE__, __LINE__, #expr, (str)); }


#ifndef NDEBUG
#	define PFS_DEBUG(expr)       expr
#	ifdef __cplusplus
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
#		define PFS_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; assert(p); }
#		define PFS_ASSERT_X(p,str) if( !(p) ) { PFS_ERROR(str); assert(p); }
#	endif

#else
#	define PFS_DEBUG(expr)
#	define PFS_ASSERT(expr)
#	define PFS_ASSERT_TRACE(expr,trace_expr)
#	define PFS_ASSERT_X(expr,msg)

#endif /* !NDEBUG */

#if !defined(__cplusplus)
#	if !defined(inline)
#		define inline __inline
#	endif
#endif

#define PFS_UNUSED(x)         ((void)(x))
#define PFS_UNUSED2(x1,x2)    ((void)(x1));((void)(x2))
#define PFS_UNUSED3(x1,x2,x3) ((void)(x1));((void)(x2));((void)(x3))

/*
#ifdef __cplusplus
#	define PFS_DENY_COPY(Class)             \
	private:                                \
		Class(const Class &);               \
		Class & operator = (const Class &);
#endif
*/

#endif /* __PFS_GLOBAL_H__ */

