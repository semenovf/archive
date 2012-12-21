/**
 * @file   dlopen_unix.c
 * @author wladt
 * @date   Dec 21, 2012 4:03:16 PM
 *
 * @brief
 */
#include <cwt/dl.h>
#include <cwt/logger.h>
#include <cwt/txtcodec.h>

DlHandle dl_open(const CWT_CHAR *path, BOOL global, BOOL resolve)
{
	DlHandle h = NULL;
	char *path_utf8;

	dlerror(); /* clear error */

	path_utf8 = cwt_textcodec_ns()->toUtf8(path);
	h = dlopen( path, (global ? RTLD_GLOBAL : RTLD_LOCAL) | ( resolve ? RTLD_NOW : RTLD_LAZY ) );
	CWT_FREE(path_utf8);

	if( !h ) {
		CWT_CHAR *dlerror_str;

		dlerror_str = cwt_textcodec_ns()->fromUtf8(dlerror());
		cwt_logger_ns()->error( _Tr("%s: failed to open dynamic library: %s"), path, dlerror_str );
		CWT_FREE(dlerror_str);
	}

	return h;
}
