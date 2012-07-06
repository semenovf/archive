/*
 * dbi.c
 *
 *  Created on: 04.07.2012
 *      Author: wladt
 */
#include <cwt/dbi/dbi.h>
#include <cwt/logger.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
/*
#include <cwt/strutils.h>
#include <cwt/strbuf.h>
*/

#define __LOG_PREFIX "dbi: "

extern CwtDBHandler __connect(const CWT_CHAR *driverDSN, const CWT_CHAR *username, const CWT_CHAR *password);

DLL_API_EXPORT void cwtDBIParseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN)
{
	CwtStringListPtr opts;
	CWT_CHAR *opt;

    opts = cwtNewStringList();
    cwtStringListSplit(opts, dsn, ":");

    opt = cwtStringListAt(opts, 0);
    if( scheme && opt )
    	*scheme = cwtStrDup(opt);

    opt = cwtStringListAt(opts, 1);
    if( driver && opt )
    	*driver = cwtStrDup(opt);

    opt = cwtStringListAt(opts, 2);
    if( driverDSN && opt )
    	*driverDSN = cwtStrDup(opt);

    cwtDeleteStringList(opts);
}


DLL_API_EXPORT CwtDBHandler cwtDBIConnect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password)
{
	CWT_CHAR *scheme;
	CWT_CHAR *driver;
	CWT_CHAR *driverDSN;
	CwtDBHandler dbh;

	cwtDBIParseDSN(dsn, &scheme, &driver, &driverDSN);

	if( cwtStrEqi("dbi", scheme) ) {
		if( cwtStrEqi("mysql", driver) ) {
			dbh = __connect(driverDSN, username, password);
		} else {
			printf_error(__LOG_PREFIX "unsupported driver: %s", driver);
		}
	} else {
		printf_error(__LOG_PREFIX "unsupported scheme: %s, only 'dbi' acceptable ", scheme);
	}

	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	return dbh;

}
