/*
 * dllapi.h
 *
 *  Created on: 08 июля 2014 г.
 *      Author: wladt
 */

#ifndef __PFS_DLLAPI_H__
#define __PFS_DLLAPI_H__

// http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
// see pfs/global.h
#ifdef DLL_API
#	undef DLL_API
#endif
#if defined(_MSC_VER)
#	define DLL_API_EXPORT __declspec(dllexport)
#	define DLL_API_IMPORT __declspec(dllimport)
#	ifdef DLL_EXPORTS
#		define DLL_API DLL_API_EXPORT
#	else
#		define DLL_API DLL_API_IMPORT
#	endif
#else
#	define DLL_API_EXPORT
#	define DLL_API_IMPORT
#	define DLL_API
#endif

#endif /* __PFS_DLLAPI_H__ */
