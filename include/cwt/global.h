#ifndef __CWT_GLOBAL_H__
#define __CWT_GLOBAL_H__

#include <stdlib.h>
#include <cwt/global_os.h>
#include <cwt/global_cc.h>

#ifdef _MSC_VER
#   define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

/* GNU C compiler */
#ifdef __GNUC__
#	define CWT_CC_GNUC
#endif

#if defined(_UNICODE) || defined(UNICODE)
#	define CWT_UNICODE
#endif

#if _POSIX_C_SOURCE >= 200112L
#	define CWT_HAVE_POLL   1
#	define CWT_HAVE_SELECT 1
#endif

#if _POSIX_C_SOURCE >= 1 || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#	define CWT_HAVE_SIGACTION 1
#endif


/* http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958 */
#ifdef DLL_API
#	undef DLL_API
#endif
#if defined(CWT_CC_MSC)
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

/* TODO depricated */
#ifdef __cplusplus
#	define CWT_NS_BEGIN          namespace jq {
#	define CWT_NS_END            }
#	define CWT_NS_PREFIXED(name) jq::name
#else
#	define CWT_NS_BEGIN
#	define CWT_NS_END
#	define CWT_NS_PREFIXED(name) name
#endif

#ifdef __cplusplus
#	define CWT_NS_BEGIN          namespace cwt {
#	define CWT_NS_END            }
#	define CWT_NS_PREFIXED(name) cwt::name
#else
#	define CWT_NS_BEGIN
#	define CWT_NS_END
#	define CWT_NS_PREFIXED(name) name
#endif


#if defined(_MSC_VER)/* && defined(__cplusplus)*/
/* for suppressing warning 'C4996' in MSVC
 * warning : 'posix_name': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _posix_name. See online help for details.
 */
#	define CWT_ISO_CPP_NAME(posix_name) _##posix_name
#else
#	define CWT_ISO_CPP_NAME(posix_name) posix_name
#endif


#define WORD_LOW_BYTE(WORD) ((char)((WORD)&0x00FF))
#define WORD_HI_BYTE(WORD)  ((char)(((WORD)>>8)&0x00FF))


#ifdef CWT_CC_WATCOM
#   ifndef inportb
#       include <conio.h>
#       define inportb inp
#       define outportb outp
#   endif
#   define getvect _dos_getvect
#   define setvect _dos_setvect
#   define enable  _enable
#   define disable _disable
#	define bioskey _bios_keybrd
#endif

#undef __interrupt__
#ifdef CWT_CC_BORLAND
#	define __interrupt__ void interrupt
#else
#	define __interrupt__ void
#endif

#if 0
#ifdef CWT_USE_GNU_GETTEXT
#	include <libintl.h>
/*#	define _T(s)         s*/
#	define _Trc(s)       s
#	define _Tr(s)        gettext (s)
#	define _Trn(s1,s2,n) ngettext (s1, s2, n)
#else
/*#	define _T(s)   s*/
#	define _Trc(s) s
#	define _Tr(s)  s
#	define _Trn(s) s
#endif
#endif

/*
#ifndef CWT_ASSERT
#	include <assert.h>
#	define CWT_ASSERT(exp) assert(exp)
#	define CWT_ASSERT_TRACE(exp,trace_exp) if( !exp ) { trace_exp; assert(exp); }
#endif
*/

#define CWT_MIN(a,b)  (((a) < (b)) ? (a) : (b))
#define CWT_MAX(a,b)  (((a) > (b)) ? (a) : (b))

#ifndef NDEBUG
#	define CWT_FPRINT(stream,prefix,str) _std_fprintf(stream, _Tr("%s (%s[%d]): %s\n"), prefix, _TFILE_, __LINE__, str)
#	define CWT_PRINT(prefix,str) CWT_FPRINT(stdout,prefix,str)
#	define CWT_TRACE(str) CWT_FPRINT(stdout,_Tr("TRACE"),str)
#	define CWT_INFO(str) CWT_FPRINT(stdout,_Tr("INFO"),str)
#	define CWT_WARN(str) CWT_FPRINT(stderr,_Tr("WARN"),str)
#	define CWT_ERROR(str) CWT_FPRINT(stderr,_Tr("ERROR"),str)
#	define CWT_FATAL(str) { CWT_FPRINT(stderr,_Tr("FATAL"),str); abort(); }
#	include <assert.h>
#	define CWT_ASSERT(p) assert(p)
#   ifndef _ENDL
#		define _ENDL "\n"
# 	 endif

#	if defined(__GRAPHICS_H) && defined(CWT_CC_BORLAND)
#		define CWT_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; \
			(void) __assertfail( \
					_Tr("Assertion failed: %s, file %s, line %d") _ENDL, \
                    #p, __FILE__, __LINE__ ); }
#	elif defined(__dj_include_assert_h_)
#		define CWT_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; \
			(void) __dj_assert(#p,__FILE__,__LINE__); }
#	else
#		define CWT_ASSERT_TRACE(p,trace_exp) if( !(p) ) { (void)trace_exp; assert(p); }
#	endif

#else
#	define CWT_FPRINT(stream,prefix,str)
#	define CWT_PRINT(prefix,str)
#	define CWT_TRACE(str)
#	define CWT_INFO(str)
#	define CWT_WARN(str)
#	define CWT_ERROR(str)
#	define CWT_FATAL(str)
#	define CWT_ASSERT(exp)
#	define CWT_ASSERT_TRACE(exp,trace_exp)

#endif /* !NDEBUG */

#if !defined(__cplusplus)
#	if !defined(inline)
#		define inline __inline
#	endif
#endif

#ifdef CWT_CC_GNUC
#define CWT_UNUSED(x) ((void)(x))
#define CWT_UNUSED2(x1,x2) ((void)(x1));((void)(x2))
#define CWT_UNUSED3(x1,x2,x3) ((void)(x1));((void)(x2));((void)(x3))
#else
#define CWT_UNUSED(x)
#define CWT_UNUSED2(x1,x2)
#define CWT_UNUSED3(x1,x2,x3)
#endif

#ifndef NDEBUG
EXTERN_C_BEGIN
DLL_API_EXPORT void* cwtMalloc(size_t size);
DLL_API_EXPORT void  cwtFree(void*);
EXTERN_C_END
#else
#	define cwtMalloc malloc
#	define cwtFree   free
#endif

#define CWT_MALLOC(T)     ((T*)cwtMalloc(sizeof(T)))
#define CWT_MALLOCA(T,sz) ((T*)cwtMalloc(sizeof(T)*(sz)))
#define CWT_FREE(v)       cwtFree(v)

/* Two-dimensional array macros */
#define CWT_A2_NCOLS(a)          sizeof(a[0])/sizeof(a[0][0])
#define CWT_A2_NROWS(a)          sizeof(a)/sizeof(a[0])
#define CWT_A2_ELEM(a,ncols,i,j) a[(i) * (ncols) + (j)]

#ifndef NULL
#	define NULL ((void*)0)
#endif

#ifdef CWT_MT
#	include <cwt/mt.h>
#endif

#endif /* __CWT_GLOBAL_H__ */

