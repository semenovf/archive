/**
 * @file   dlopen_win.c
 * @author wladt
 * @date   Dec 25, 2012 20:53:16 PM
 *
 * @brief
 */
#include <cwt/dl.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>
#include <cwt/str.h>

DlHandle dl_open(const CWT_CHAR *path, BOOL global, BOOL resolve)
{
	DlHandle h = NULL;
	DWORD dwFlags = 0;

	CWT_UNUSED(global);

	if( !resolve )
		dwFlags |= DONT_RESOLVE_DLL_REFERENCES;
	h = LoadLibraryEx(path, NULL, dwFlags);
	if( !h ) {
		cwt_logger_ns()->error( _Tr("%s: failed to open dynamic library: %s")
				, path
				, cwt_str_ns()->strError(GetLastError()));
	} else {
	   /* TODO use the result in a call to GetProcAddress*/
	}
	return h;
}
