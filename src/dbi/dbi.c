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

static CwtDBHandler*   __dbi_connect   (const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void            __dbi_disconnect(CwtDBHandler*);
static BOOL            __dbi_func          (CwtDBHandler*, const CWT_CHAR*, CWT_CHAR**);
static void            __dbi_attr          (CwtDBHandler*, const CWT_CHAR*, void*);
static BOOL            __dbi_setAutoCommit (CwtDBHandler*, BOOL);
static BOOL            __dbi_autoCommit    (CwtDBHandler*);
static CwtDBI_RC       __dbi_err           (CwtDBHandler*);
static const CWT_CHAR* __dbi_strerror      (CwtDBHandler*);
static const CWT_CHAR* __dbi_state         (CwtDBHandler*);
static BOOL            __dbi_query         (CwtDBHandler*, const CWT_CHAR *sql);   /* cannot be used for statements that contain binary data */
static BOOL            __dbi_queryBin      (CwtDBHandler*, const CWT_CHAR *sql, size_t length); /* can be used for statements that contain binary data */
static CwtStatement*   __dbi_prepare       (CwtDBHandler*, const CWT_CHAR *sql);
static ULONGLONG       __dbi_rows          (CwtDBHandler*);
static BOOL            __dbi_tables        (CwtDBHandler*, CwtStrList *tables);
static BOOL            __dbi_tableExists   (CwtDBHandler*, const CWT_CHAR *tname);
static char*           __dbi_encode        (CwtDBHandler*, const CWT_CHAR *s);
static CWT_CHAR*       __dbi_decode        (CwtDBHandler*, const char *s);
static BOOL            __dbi_begin         (CwtDBHandler *dbh);
static BOOL            __dbi_commit        (CwtDBHandler *dbh);
static BOOL            __dbi_rollback      (CwtDBHandler *dbh);

static void            __dbi_close         (CwtStatement *sth);
static CwtUniType*     __dbi_bind          (CwtStatement *sth, size_t index, CwtTypeEnum cwtType);
static CwtUniType*     __dbi_bindText      (CwtStatement *sth, size_t index, size_t length);
static CwtUniType*     __dbi_bindBlob      (CwtStatement *sth, size_t index, size_t sz);

static CwtTypeEnum     __toCwtTypeEnum (CwtSqlTypeEnum sqlType);
static CwtSqlTypeEnum  __toSqlTypeEnum (CwtTypeEnum cwtType);

extern CwtDDI*         __ddi_createDDI     (const CWT_CHAR *name, const CWT_CHAR *charset);
extern void            __ddi_freeDDI       (CwtDDI *ddi);
extern CwtDDITable*    __ddi_newTable      (CwtDDI *ddi, const CWT_CHAR *name);
extern CwtDDIColumn*   __ddi_newColumn     (CwtDDITable *table, const CWT_CHAR *name);
extern CwtDDITable*    __ddi_findTable     (CwtDDI*, const CWT_CHAR *name);
extern CwtDDIColumn*   __ddi_findColumn    (CwtDDITable*, const CWT_CHAR *name);

extern BOOL            __ddi_cTypeBool     (CwtDDIColumn*);
extern BOOL            __ddi_cTypeInt      (CwtDDIColumn*, LONGLONG min, ULONGLONG max);
extern BOOL            __ddi_cTypeFloat    (CwtDDIColumn*, UINT prec, UINT scale);
extern BOOL            __ddi_cTypeText     (CwtDDIColumn*, ULONGLONG maxlen);
extern BOOL            __ddi_cTypeBlob     (CwtDDIColumn*, ULONGLONG maxlen);
extern BOOL            __ddi_cTypeTime     (CwtDDIColumn*, CwtTypeEnum time_type, BOOL stmap);
extern BOOL            __ddi_cTypeRef      (CwtDDIColumn*, CwtDDITable*);
extern BOOL            __ddi_cDefault      (CwtDDIColumn*, const CWT_CHAR *defaultValue);
extern BOOL            __ddi_cAutoinc      (CwtDDIColumn*, UINT inc);
extern BOOL            __ddi_cNull         (CwtDDIColumn*, BOOL yes);
extern BOOL            __ddi_cIndex        (CwtDDIColumn*, BOOL yes);
extern BOOL            __ddi_cUniq         (CwtDDIColumn*, BOOL yes);
extern BOOL            __ddi_cPK           (CwtDDIColumn*);

extern BOOL            __ddi_deploy        (CwtDBHandler *dbh, CwtDDI *ddi, int flags);
extern BOOL            __ddi_recall        (CwtDBHandler *dbh, CwtDDI *ddi, int flags);


/* helper functions */
static void            __parseDSN      (const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN);
static CwtDBIDriver*   __load          (const CWT_CHAR *scheme, const CWT_CHAR *driver);


static CwtDBI __cwtDBI = {
	  __dbi_connect
	, __dbi_disconnect
	, __dbi_func
	, __dbi_attr
	, __dbi_setAutoCommit
	, __dbi_autoCommit
	, __dbi_err
	, __dbi_strerror
	, __dbi_state
	, __dbi_query
	, __dbi_queryBin
	, __dbi_prepare
	, __dbi_rows
	, __dbi_tables
	, __dbi_tableExists
	, __dbi_encode
	, __dbi_decode
	, __dbi_begin
	, __dbi_commit
	, __dbi_rollback

	, __dbi_close
	, __dbi_bind
	, __dbi_bindText
	, __dbi_bindBlob

	, __toCwtTypeEnum
	, __toSqlTypeEnum

	, __ddi_createDDI
	, __ddi_freeDDI
	, __ddi_newTable
	, __ddi_newColumn
	, __ddi_findTable
	, __ddi_findColumn

	, __ddi_cTypeBool
    , __ddi_cTypeInt
    , __ddi_cTypeFloat
    , __ddi_cTypeText
    , __ddi_cTypeBlob
    , __ddi_cTypeTime
    , __ddi_cTypeRef
    , __ddi_cDefault
    , __ddi_cAutoinc
    , __ddi_cNull
    , __ddi_cIndex
    , __ddi_cUniq
    , __ddi_cPK

	, __ddi_deploy
	, __ddi_recall
};


DLL_API_EXPORT CwtDBI* cwtDBI(void)
{
	return &__cwtDBI;
}


static void __parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *slNS = cwtStrListNS();
	CwtStrList *opts;
	const CWT_CHAR *opt;

    opts = slNS->create();
    slNS->splitAny(opts, dsn, _T(":"), CWT_QUOTES_BOTH);

    opt = slNS->at(opts, 0);
    if( scheme && opt )
    	*scheme = strNS->strdup(opt);

    opt = slNS->at(opts, 1);
    if( driver && opt )
    	*driver = strNS->strdup(opt);

    opt = slNS->at(opts, 2);
    if( driverDSN && opt )
    	*driverDSN = strNS->strdup(opt);

    slNS->free(opts);
}


static CwtDBIDriver* __load(const CWT_CHAR *scheme, const CWT_CHAR *driver)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtDlNS  *dl    = cwtDlNS();

	CwtDBIDriver *dbd = (CwtDBIDriver*)NULL;

	if( strNS->strieq(_T("dbi"), scheme) ) {
		DlHandle dlHandle;
		CwtStringNS *stringNS = cwtStringNS();
		CwtString   *driverName;
		CwtString   *driverPath;

		driverName = stringNS->create();
		driverPath = stringNS->create();

		stringNS->sprintf(driverName, _T("cwt-%s"), driver);

		dl->buildDlFileName(stringNS->cstr(driverName), driverPath);
		dlHandle = dl->open(stringNS->cstr(driverPath), TRUE, TRUE);

		if( dlHandle ) {
#ifdef CWT_CC_MSC
			DlSymbol driverImpl = dl->symbol(dlHandle, "cwtDBIDriverImpl");
#else
			CwtDBIDriver* (*driverImpl)(void);
			driverImpl = dl->symbol(dlHandle, "cwtDBIDriverImpl");
#endif
			if( driverImpl ) {
				dbd = (CwtDBIDriver*)driverImpl();
			}
		}

		stringNS->free(driverPath);
		stringNS->free(driverName);
	} else {
		printf_error(__LOG_PREFIX _Tr("unsupported scheme: %s, only 'dbi' acceptable "), scheme);
	}

	return dbd;

}


static CwtDBHandler* __dbi_connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname)
{
	CwtStrNS   *strNS = cwtStrNS();

	CwtDBIDriver *dbd = NULL;
	CwtDBHandler *dbh = NULL;

	CWT_CHAR *scheme = NULL;
	CWT_CHAR *driver = NULL;
	CWT_CHAR *driverDSN = NULL;

	__parseDSN(dsn, &scheme, &driver, &driverDSN);

	dbd = __load(dsn, driver);

	if( dbd ) {
		dbh = dbd->connect(driverDSN, username, password, csname);
	}

	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	return dbh;
}

static inline void __dbi_disconnect(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	dbh->driver()->disconnect(dbh);
}

static inline BOOL __dbi_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CWT_ASSERT(dbh);
	return dbh->driver()->func(dbh, func_name, argv);
}

static inline void __dbi_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CWT_ASSERT(dbh);
	dbh->driver()->attr(dbh, attr_name, attr_value);

}

static inline BOOL __dbi_setAutoCommit(CwtDBHandler *dbh, BOOL on)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->setAutoCommit(dbh, on);
}

static inline BOOL __dbi_autoCommit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->autoCommit(dbh);
}

static inline CwtDBI_RC __dbi_err(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->err(dbh);
}

static inline const CWT_CHAR* __dbi_strerror(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->strerror(dbh);
}

static inline const CWT_CHAR* __dbi_state(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->state(dbh);
}

/* cannot be used for statements that contain binary data */
static inline BOOL __dbi_query(CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->query(dbh, sql);
}


/* can be used for statements that contain binary data */
static inline BOOL __dbi_queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->queryBin(dbh, sql, length);
}

static inline CwtStatement* __dbi_prepare(CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStatement* sth;

	CWT_ASSERT(dbh);

	sth = dbh->driver()->prepare(dbh, sql);
	if( sth ) {
		size_t nbind_params;
		sth->dbh    = dbh;
		sth->bind_params = NULL;

		nbind_params = dbh->bindParamsCount(sth);

		sth->bind_params = CWT_MALLOCA(CwtUniType*, nbind_params);
		strNS->bzero(sth->bind_params, sizeof(CwtUniType*) * nbind_params);


	}
	return sth;
}

static inline ULONGLONG __dbi_rows(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->rows(dbh);
}

static inline BOOL __dbi_tables(CwtDBHandler *dbh, CwtStrList *tables)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->tables(dbh, tables);
}

static inline BOOL __dbi_tableExists(CwtDBHandler *dbh, const CWT_CHAR *tname)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->tableExists(dbh, tname);
}

static inline char* __dbi_encode(CwtDBHandler *dbh, const CWT_CHAR *s)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->encode(dbh, s);
}

static inline CWT_CHAR* __dbi_decode(CwtDBHandler *dbh, const char *s)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->decode(dbh, s);
}

static inline BOOL __dbi_begin(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->begin(dbh);
}

static BOOL __dbi_commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->commit(dbh);
}

static BOOL __dbi_rollback(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->rollback(dbh);
}


static void __dbi_close(CwtStatement *sth)
{
	CwtUniTypeNS *utNS = cwtUniTypeNS();
	size_t nbind_params;
	CWT_ASSERT(sth);

	nbind_params = sth->dbh->bindParamsCount(sth);

	if( nbind_params > 0 && sth->bind_params ) {
		size_t i;
		for( i = 0; i < nbind_params; i++ ) {
			utNS->free(sth->bind_params[i]);
			sth->bind_params[i] = NULL;
		}

		CWT_FREE(sth->bind_params);
		sth->bind_params = NULL;
	}
}

static CwtUniType* __dbi_bind_helper(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, size_t length)
{
	size_t nbind_params;
	CwtUniType *ut;

	CWT_ASSERT(sth);

	nbind_params = sth->dbh->bindParamsCount(sth);

	if( index >= nbind_params ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
		return NULL;
	}

	ut = sth->bind_params[index];

	if( CwtType_TEXT == cwtType ) {
		char *s;
		s = CWT_MALLOCA(char, length);
		cwtUniTypeNS()->setTextSized(ut, s, length);
	} else if( CwtType_BLOB == cwtType ) {
		char *p;
		p = CWT_MALLOCA(char, length);
		cwtUniTypeNS()->setBlob(ut, p, length);
	} else if( CwtType_TIME == cwtType || CwtType_DATE == cwtType || CwtType_DATETIME == cwtType ) {
		CWT_TIME *tm = sth->dbh->driver()->createTime();
		cwtUniTypeNS()->setDate(ut, tm);
		sth->dbh->bind(sth, index, ut);
	} else {
		cwtUniTypeNS()->set(ut, cwtType, void *copy);
	}

	return ut;
}

static CwtUniType* __dbi_bind(CwtStatement *sth, size_t index, CwtTypeEnum cwtType)
{
	size_t nbind_params;
	CwtUniType *ut;

	CWT_ASSERT(sth);
	CWT_ASSERT(cwtType != CwtType_TEXT);
	CWT_ASSERT(cwtType != CwtType_BLOB);

	nbind_params = sth->dbh->bindParamsCount(sth);

	if( index >= nbind_params ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
		return NULL;
	}

	ut = sth->bind_params[index];

	__dbi_cleanupUniType(ut);

	if( CwtType_TIME == cwtType || CwtType_DATE == cwtType || CwtType_DATETIME == cwtType ) {
		CWT_TIME *tm = sth->dbh->driver()->createTime();
		cwtUniTypeNS()->setDate(ut, tm);
		sth->dbh->bind(sth, index, ut);
	}

	return ut;
}

static CwtUniType* __dbi_bindText(CwtStatement *sth, size_t index, size_t length)
{
	size_t nbind_params;
	CwtUniType *ut;

	CWT_ASSERT(sth);

	nbind_params = sth->dbh->bindParamsCount(sth);

	if( index >= nbind_params ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
		return NULL;
	}

	ut = sth->bind_params[index];

	__dbi_cleanupUniType(ut);
}

static CwtUniType* __dbi_bindBlob( CwtStatement *sth, size_t index, size_t sz )
{

}


static BOOL __dbi_bind(CwtStatement *sth, size_t index, CwtUniType *ut)
{
	CWT_ASSERT(sth);
	CWT_ASSERT(ut);
	return sth->dbh->bind(sth, index, ut);
}
/*

static CwtDBIBindGroup* __dbi_createBindGroup(void)
{
	CwtDBIBindGroup *bg;
	bg = CWT_MALLOC(CwtDBIBindGroup);

	return bg;
}

static void __dbi_freeBindGroup(CwtDBIBindGroup *bg)
{
	if( bg ) {
		CWT_FREE(bg);
	}
}
*/


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
	CwtSqlTypeEnum sqlType;

	switch(cwtType) {
	case CwtType_SBYTE:
	case CwtType_BYTE:
		sqlType = CwtSql_TINYINT;
		break;
	case CwtType_SHORT:
	case CwtType_USHORT:
		sqlType = CwtSql_SMALLINT;
		break;
	case CwtType_INT:
	case CwtType_UINT:
	case CwtType_LONG:
	case CwtType_ULONG:
		sqlType = CwtSql_INT;
		break;
	case CwtType_LONGLONG:
	case CwtType_ULONGLONG:
		sqlType = CwtSql_BIGINT;
		break;
	case CwtType_FLOAT:
		sqlType = CwtSql_FLOAT;
		break;
	case CwtType_DOUBLE:
		sqlType = CwtSql_DOUBLE;
		break;
	case CwtType_TIME:
		sqlType = CwtSql_TIME;
		break;
	case CwtType_DATE:
		sqlType = CwtSql_DATE;
		break;
	case CwtType_DATETIME:
		sqlType = CwtSql_DATETIME;
		break;

	case CwtType_BLOB:
		sqlType = CwtSql_BLOB;
		break;

	default:
	case CwtType_TEXT:
		sqlType = CwtSql_TEXT;
		break;
	}

	return sqlType;
}

