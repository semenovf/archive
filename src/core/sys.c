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
#include <cwt/strlist.h>
#include <cwt/txtcodec.h>


static void __parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverSN);
static BOOL __loadNS(const CWT_CHAR *dsn);

static CwtSysNS __cwtSysNS = {
	  __parseDSN
	, __loadNS
};


DLL_API_EXPORT CwtSysNS* cwtSysNS(void)
{
	return &__cwtSysNS;
}


/**
 * @fn void CwtSysNS::parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **params)
 *
 * @brief Parses driver source name (DSN).
 *
 * @details Driver is a dynamically loadable module.
 *
 * @param dsn [in] Driver source name.
 * @param pscheme [out] Scheme name.
 * @param pdriver [out] driver name.
 * @param params [out] Parameters to driver.
 */
static void __parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **params)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *slNS = cwtStrListNS();
	CwtStrList *opts;
	const CWT_CHAR *opt;

    opts = slNS->create();
    slNS->splitAny(opts, dsn, _T(":"), CWT_QUOTES_BOTH, 0);

    opt = slNS->at(opts, 0);
    if( scheme && opt )
    	*scheme = strNS->strDup(opt);

    opt = slNS->at(opts, 1);
    if( driver && opt )
    	*driver = strNS->strDup(opt);

    opt = slNS->at(opts, 2);
    if( params && opt )
    	*params = strNS->strDup(opt);

    slNS->free(opts);
}


static BOOL __loadDriver(const CWT_CHAR *dsn, const char *bootstrapNameLatin1)
{
	CwtDlNS  *dl    = cwtDlNS();
	BOOL ok = FALSE;
	CWT_CHAR *scheme = NULL;
	CWT_CHAR *driver = NULL;
	CWT_CHAR *params = NULL;
	CWT_CHAR *bootstrapName;

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
			bootstrapName = cwtTextCodecNS()->fromLatin1(bootstrapNameLatin1, strlen(bootstrapNameLatin1));

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

			CWT_FREE(bootstrapName);
		}

		stringNS->free(driverPath);
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
