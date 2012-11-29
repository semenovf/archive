/*
 * sys.c
 *
 *  Created on: Sep 4, 2012
 *      Author: wladt
 */

#include <string.h>
#include <cwt/logger.h>
#include <cwt/dl.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/txtcodec.h>

static BOOL __load_driver(const CWT_CHAR *dsn, const char *bootstrapNameLatin1)
{
	CwtDlNS  *dl    = cwt_dl_ns();
	BOOL ok = FALSE;
	CWT_CHAR *scheme = NULL;
	CWT_CHAR *driver = NULL;
	CWT_CHAR *params = NULL;
	CWT_CHAR *bootstrapName;

	/*FIXME replace with URI*/
	/*__parseDSN(dsn, &scheme, &driver, &params);*/

	CWT_UNUSED(dsn);

	if( scheme && driver ) {
		DlHandle dlHandle;
		CwtStringNS *stringNS = cwt_string_ns();
		CwtString   *driverName;
		CWT_CHAR    *driverPath;

		driverName = stringNS->create();
		stringNS->sprintf(driverName, _T("cwt-%s-%s"), scheme, driver);

		driverPath = dl->buildDlFileName(stringNS->cstr(driverName));
		dlHandle = dl->open(driverPath, TRUE, TRUE);

		if( dlHandle ) {
			bootstrapName = cwt_textcodec_ns()->fromLatin1(bootstrapNameLatin1, strlen(bootstrapNameLatin1));
#ifdef __COMMENT__
#ifdef CWT_CC_MSC
			DlSymbol bootstrap = dl->symbol(dlHandle, bootstrapNameLatin1);
#else
			BOOL (*bootstrap)(const void*);
			bootstrap = dl->symbol(dlHandle, bootstrapNameLatin1);
#endif
			if( bootstrap ) {
				ok = (BOOL)bootstrap(params);
				if( !ok ) {
					dl->close(dlHandle);
					dlHandle = NULL;
					cwt_logger_ns()->error(_Tr("%s: failed to process '%s' function"), dsn, bootstrapName);
				}
			} else {
				cwt_logger_ns()->warn(_Tr("%s: '%s' function must be provided by CWT namespace (NS) driver"), bootstrapName);
			}
#endif
			CWT_FREE(bootstrapName);
		}

		CWT_FREE(driverPath);
		stringNS->free(driverName);
	}

	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(params);

	return ok;
}

BOOL dl_load_ns(const CWT_CHAR *dsn)
{
	return __load_driver(dsn, "__ns_init__");
}
