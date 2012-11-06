/*
 * sys.c
 *
 *  Created on: Sep 4, 2012
 *      Author: wladt
 */

#include <string.h>
#include <cwt/sys.h>
#include <cwt/logger.h>
#include <cwt/dl.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/txtcodec.h>

static BOOL __loadNS(const CWT_CHAR *dsn);

static CwtSysNS __cwtSysNS = {
	__loadNS
};


DLL_API_EXPORT CwtSysNS* cwtSysNS(void)
{
	return &__cwtSysNS;
}

static BOOL __loadDriver(const CWT_CHAR *dsn, const char *bootstrapNameLatin1)
{
	CwtDlNS  *dl    = cwtDlNS();
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
		CwtStringNS *stringNS = cwtStringNS();
		CwtString   *driverName;
		CWT_CHAR    *driverPath;

		driverName = stringNS->create();
		stringNS->sprintf(driverName, _T("cwt-%s-%s"), scheme, driver);

		driverPath = dl->buildDlFileName(stringNS->cstr(driverName));
		dlHandle = dl->open(driverPath, TRUE, TRUE);

		if( dlHandle ) {
			bootstrapName = cwtTextCodecNS()->fromLatin1(bootstrapNameLatin1, strlen(bootstrapNameLatin1));
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
					cwtLoggerNS()->error(_Tr("%s: failed to process '%s' function"), dsn, bootstrapName);
				}
			} else {
				cwtLoggerNS()->warn(_Tr("%s: '%s' function must be provided by CWT namespace (NS) driver"), bootstrapName);
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

static BOOL __loadNS(const CWT_CHAR *dsn)
{
	return __loadDriver(dsn, "__ns_init__");

/*	CwtDlNS  *dl    = cwtDlNS();
	BOOL ok = FALSE;
	CWT_CHAR *scheme = NULL;
	CWT_CHAR *driver = NULL;
	CWT_CHAR *params = NULL;

	__parseDSN(dsn, &scheme, &driver, &params);

	if( scheme && driver ) {
		DlHandle dlHandle;
		CwtStringNS *stringNS = cwtStringNS();
		CwtString   *driverName;
		CwtString   *driverPath;

		driverName = stringNS->create();
		driverPath = stringNS->create();

		stringNS->sprintf(driverName, _T("cwt-%s-%s"), scheme, driver);

		dl->buildDlFileName(stringNS->cstr(driverName), driverPath);
		dlHandle = dl->open(stringNS->cstr(driverPath), TRUE, TRUE);

		if( dlHandle ) {
#ifdef CWT_CC_MSC
			DlSymbol __ns_init__ = dl->symbol(dlHandle, "__ns_init__");
#else
			BOOL (*__ns_init__)(const CWT_CHAR*);
			__ns_init__ = dl->symbol(dlHandle, "__ns_init__");
#endif
			if( __ns_init__ ) {
				ok = (BOOL)__ns_init__(params);
				if( !ok ) {
					dl->close(dlHandle);
					dlHandle = NULL;
					cwtLoggerNS()->error(_Tr("%s: failed to process '__ns_init__' function"), dsn);
				}
			} else {
				cwtLoggerNS()->warn(_Tr("%s: '__ns_init__' function must be provided by CWT namespace (NS) driver"));
			}
		}

		stringNS->free(driverPath);
		stringNS->free(driverName);
	}

	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(params);

	return ok;*/
}
