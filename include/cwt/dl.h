/*
 * dl.h
 *
 *  Created on: 06.07.2012
 *      Author: wladt
 */


#ifndef __CWT_DL_H__
#define __CWT_DL_H__

// see http://en.wikipedia.org/wiki/Dynamic_loading
#include <cwt/types.h>

#ifdef CWT_CC_MSC
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

#ifdef CWT_CC_MSC
	typedef HMODULE DlHandle;
	typedef FARPROC DlSymbol;
#else
	typedef void* DlHandle;
	typedef void* DlSymbol;
#endif


/*Dynamic library*/
typedef struct CwtDlNS
{
	DlHandle  (*open)            (const CWT_CHAR *path, BOOL global, BOOL resolve);
	DlSymbol  (*symbol)          (DlHandle h, const char *name);
	void      (*close)           (DlHandle h);
	CWT_CHAR* (*buildDlFileName) (const CWT_CHAR *name);
} CwtDlNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtDlNS* cwtDlNS(void);
EXTERN_C_END

#endif // __CWT_DL_H__
