#ifndef __PFS_GLOBAL_H__
#define __PFS_GLOBAL_H__

#if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS)
#	include <pfs/pp/g_os.h>
#	include <pfs/pp/g_cc.h>
#	include <pfs/pp/g_endian.h>
#	include <pfs/pp/g_bits.h>
#else
#	include <pfs/pp/global_os.h>
#	include <pfs/pp/global_cc.h>
#	include <pfs/pp/global_endian.h>
#	include <pfs/pp/global_bits.h>
#endif /* !MS-DOS*/

#if defined(__cplusplus) && ! defined(PFS_CC_BORLAND_REAL)
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

#if defined(__cplusplus) && ! defined(PFS_CC_BORLAND_REAL)
namespace pfs {
	template <typename T>
	inline T min (T a, T b) { return a <= b ? a : b; }

	template <typename T>
	inline T max (T a, T b) { return a >= b ? a : b; }

	template <typename T>
	inline T abs (T x) { return x < 0 ? x * T(-1) : x; }
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


#if !defined(__cplusplus)
#	if !defined(inline)
#		define inline __inline
#	endif
#endif

#define PFS_UNUSED(x)         ((void)(x))
#define PFS_UNUSED2(x1,x2)    ((void)(x1));((void)(x2))
#define PFS_UNUSED3(x1,x2,x3) ((void)(x1));((void)(x2));((void)(x3))

#endif /* __PFS_GLOBAL_H__ */

