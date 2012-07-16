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
#include <cwt/str.h>
#include <cwt/dl.h>

#define __LOG_PREFIX _Tr("dbi: ")

static void            __parseDSN      (const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN);
static CwtDBIDriver*   __load          (const CWT_CHAR *dsn);
static CwtTypeEnum     __toCwtTypeEnum (CwtSqlTypeEnum sqlType);
static CwtSqlTypeEnum  __toSqlTypeEnum (CwtTypeEnum cwtType);


static CwtDBI __cwtDBI = {
	  __parseDSN
	, __load
	, __toCwtTypeEnum
	, __toSqlTypeEnum
};


DLL_API_EXPORT CwtDBI* cwtDBI(void)
{
	return &__cwtDBI;
}


static void __parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *strListNS = cwtStrListNS();
	CwtStrList opts;
	CWT_CHAR *opt;

    strListNS->init(&opts);
    strListNS->split(&opts, dsn, _T(":"));

    opt = strListNS->at(&opts, 0);
    if( scheme && opt )
    	*scheme = strNS->strdup(opt);

    opt = strListNS->at(&opts, 1);
    if( driver && opt )
    	*driver = strNS->strdup(opt);

    opt = strListNS->at(&opts, 2);
    if( driverDSN && opt )
    	*driverDSN = strNS->strdup(opt);

    strListNS->destroy(&opts);
}


static CwtDBIDriver* __load(const CWT_CHAR *dsn)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtDlNS  *dl    = cwtDlNS();
	CWT_CHAR *scheme;
	CWT_CHAR *driverName;
	CWT_CHAR *driverDSN;
	CwtDBIDriver *driver = (CwtDBIDriver*)NULL;

	__parseDSN(dsn, &scheme, &driverName, &driverDSN);

	if( strNS->strieq(_T("dbi"), scheme) ) {
		DlHandle dlHandle;
		CwtStringNS *stringNS = cwtStringNS();
		CwtString driverPath;

		stringNS->init(&driverPath);
		stringNS->append(&driverPath, _T("cwt-"));
		stringNS->append(&driverPath, driverName);

		CWT_FREE(driverName);
		driverName = strNS->strdup(stringNS->cstr(&driverPath));
		stringNS->clear(&driverPath);

		dl->buildDlFileName(driverName, &driverPath);
		dlHandle = dl->open(stringNS->cstr(&driverPath), TRUE, TRUE);

		if( dlHandle ) {
			DlSymbol driverImpl = dl->symbol(dlHandle, "cwtDBIDriverImpl");

			if( driverImpl ) {
				driver = (CwtDBIDriver*)driverImpl();
			}
		}

		stringNS->destroy(&driverPath);
	} else {
		printf_error(__LOG_PREFIX _Tr("unsupported scheme: %s, only 'dbi' acceptable "), scheme);
	}

	CWT_FREE(scheme);
	CWT_FREE(driverName);
	CWT_FREE(driverDSN);

	return driver;

}


static CwtTypeEnum __toCwtTypeEnum (CwtSqlTypeEnum sqlType)
{
	CwtTypeEnum cwtType;

	switch(sqlType) {
	case CwtSql_TINYINT:
		cwtType = CwtType_CHAR;
		break;
	case CwtSql_SMALLINT:
		cwtType = CwtType_SHORT;
		break;
	case CwtSql_MEDIUMINT:
	case CwtSql_INT:
		cwtType = CwtType_INT;
		break;
	case CwtSql_BIGINT:
		cwtType = CwtType_LONGLONG;
		break;
	case CwtSql_FLOAT:
		cwtType = CwtType_FLOAT;
		break;
	case CwtSql_DOUBLE:
		cwtType = CwtType_DOUBLE;
		break;
	case CwtSql_DECIMAL:
		cwtType = CwtType_TEXT;
		break;
	case CwtSql_YEAR:
		cwtType = CwtType_SHORT;
		break;
	case CwtSql_TIME:
		cwtType = CwtType_TIME;
		break;
	case CwtSql_DATE:
		cwtType = CwtType_DATE;
		break;
	case CwtSql_DATETIME:
	case CwtSql_TIMESTAMP:
		cwtType = CwtType_DATETIME;
		break;

	case CwtSql_CHAR:
	case CwtSql_VARCHAR:
	case CwtSql_TINYTEXT:
	case CwtSql_TEXT:
	case CwtSql_MEDIUMTEXT:
	case CwtSql_LONGTEXT:
		cwtType = CwtType_TEXT;
		break;

	case CwtSql_BINARY:
	case CwtSql_VARBINARY:
	case CwtSql_TINYBLOB:
	case CwtSql_BLOB:
	case CwtSql_MEDIUMBLOB:
	case CwtSql_LONGBLOB:
		cwtType = CwtType_BLOB;
		break;

	default:
	case CwtSql_BIT:
		cwtType = CwtType_TEXT;
		break;
	}

	return cwtType;
}

static CwtSqlTypeEnum __toSqlTypeEnum (CwtTypeEnum cwtType)
{

}

