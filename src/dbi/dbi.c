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
#include <cwt/sys.h>
#include <cwt/dl.h>

#define __LOG_PREFIX _Tr("dbi: ")

static void            dbi_parse_dsn        (const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN);
static CwtDBIDriver*   dbi_load             (const CWT_CHAR *scheme, const CWT_CHAR *driver);
static CwtDBHandler*   dbi_connect          (const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void            dbi_disconnect       (CwtDBHandler*);
static BOOL            dbi_func             (CwtDBHandler*, const CWT_CHAR*, CWT_CHAR**);
static void            dbi_attr             (CwtDBHandler*, const CWT_CHAR*, void*);
static BOOL            dbi_set_auto_commit  (CwtDBHandler*, BOOL);
static BOOL            dbi_auto_commit      (CwtDBHandler*);
static CwtDBI_RC       dbi_err              (CwtDBHandler*);
static const CWT_CHAR* dbi_strerror         (CwtDBHandler*);
static const CWT_CHAR* dbi_state            (CwtDBHandler*);
static BOOL            dbi_query            (CwtDBHandler*, const CWT_CHAR *sql);   /* cannot be used for statements that contain binary data */
static BOOL            dbi_query_bin        (CwtDBHandler*, const CWT_CHAR *sql, size_t length); /* can be used for statements that contain binary data */
static CwtStatement*   dbi_prepare          (CwtDBHandler*, const CWT_CHAR *sql);
static BOOL            dbi_execute          (CwtStatement*);
static ULONGLONG       dbi_last_id          (CwtStatement*);
static ULONGLONG       dbi_rows             (CwtDBHandler*);
static BOOL            dbi_tables           (CwtDBHandler*, CwtStrList *tables);
static BOOL            dbi_table_exists     (CwtDBHandler*, const CWT_CHAR *tname);
static char*           dbi_encode_n         (CwtDBHandler*, const CWT_CHAR *s, size_t n);
static CWT_CHAR*       dbi_decode_n         (CwtDBHandler*, const char *s, size_t n);
static char*           dbi_encode           (CwtDBHandler*, const CWT_CHAR *s);
static CWT_CHAR*       dbi_decode           (CwtDBHandler*, const char *s);
static BOOL            dbi_begin            (CwtDBHandler *dbh);
static BOOL            dbi_commit           (CwtDBHandler *dbh);
static BOOL            dbi_rollback         (CwtDBHandler *dbh);

static void            dbi_close            (CwtStatement *sth);
static CwtUniType*     dbi_bind             (CwtStatement *sth, size_t index, CwtTypeEnum cwtType);
static CwtUniType*     dbi_bind_text        (CwtStatement *sth, size_t index, size_t length);
static CwtUniType*     dbi_bind_blob        (CwtStatement *sth, size_t index, size_t sz);
static BOOL            dbi_set_from_string  (CwtStatement *sth, CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s);
static BOOL            dbi_set_unitype      (CwtStatement *sth, CwtUniType *ut, CwtUniType *val);
static BOOL            dbi_set_BOOL         (CwtStatement *sth, CwtUniType *ut, BOOL b)      { return sth->dbh->setParm(sth, ut, &b, 0); }
static BOOL            dbi_set_CHAR         (CwtStatement *sth, CwtUniType *ut, CWT_CHAR ch) { return sth->dbh->setParm(sth, ut, &ch, 0); }
static BOOL            dbi_set_SBYTE        (CwtStatement *sth, CwtUniType *ut, SBYTE n)     { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_BYTE         (CwtStatement *sth, CwtUniType *ut, BYTE n)      { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_SHORT        (CwtStatement *sth, CwtUniType *ut, SHORT n)     { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_USHORT       (CwtStatement *sth, CwtUniType *ut, USHORT n)    { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_INT          (CwtStatement *sth, CwtUniType *ut, INT n)       { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_UINT         (CwtStatement *sth, CwtUniType *ut, UINT n)      { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_LONG         (CwtStatement *sth, CwtUniType *ut, LONG n)      { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_ULONG        (CwtStatement *sth, CwtUniType *ut, ULONG n)     { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_LONGLONG     (CwtStatement *sth, CwtUniType *ut, LONGLONG n)  { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_ULONGLONG    (CwtStatement *sth, CwtUniType *ut, ULONGLONG n) { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_FLOAT        (CwtStatement *sth, CwtUniType *ut, float n)     { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_DOUBLE       (CwtStatement *sth, CwtUniType *ut, double n)    { return sth->dbh->setParm(sth, ut, &n, 0); }
static BOOL            dbi_set_TEXT         (CwtStatement *sth, CwtUniType *ut, const CWT_CHAR *p, size_t length) { return sth->dbh->setParm(sth, ut, p, length); }
static BOOL            dbi_set_BLOB         (CwtStatement *sth, CwtUniType *ut, const void *p, size_t sz) { return sth->dbh->setParm(sth, ut, p, sz); }
static BOOL            dbi_set_TIME         (CwtStatement *sth, CwtUniType *ut, const CWT_TIME *p)        { return sth->dbh->setParm(sth, ut, p, 0); }
static BOOL            dbi_set_DATE         (CwtStatement *sth, CwtUniType *ut, const CWT_TIME *p)        { return sth->dbh->setParm(sth, ut, p, 0); }
static BOOL            dbi_set_DATETIME     (CwtStatement *sth, CwtUniType *ut, const CWT_TIME *p)        { return sth->dbh->setParm(sth, ut, p, 0); }

static BOOL            dbi_fetch_next       (CwtStatement *sth) { return sth->dbh->fetchNext(sth); }
static BOOL            dbi_fetch_column     (CwtStatement *sth, CWT_CHAR *col, CwtUniType *value) { return sth->dbh->fetchColumn(sth, col, value); }


static CwtTypeEnum     dbi_toCwtTypeEnum    (CwtSqlTypeEnum sqlType);
static CwtSqlTypeEnum  dbi_toSqlTypeEnum    (CwtTypeEnum cwtType);

extern CwtDDI*         ddi_create_DDI       (const CWT_CHAR *name, const CWT_CHAR *charset);
extern CwtDDI*         ddi_build_DDI        (const CWT_CHAR *name, const CWT_CHAR *charset, CwtPersistEntity *entities[], size_t nentries);
extern void            ddi_free_DDI         (CwtDDI *ddi);
extern CwtDDITable*    ddi_new_table        (CwtDDI *ddi, const CWT_CHAR *name);
extern CwtDDIColumn*   ddi_new_column       (CwtDDITable *table, const CWT_CHAR *name);
extern CwtDDITable*    ddi_find_table       (CwtDDI*, const CWT_CHAR *name);
extern CwtDDIColumn*   ddi_find_column      (CwtDDITable*, const CWT_CHAR *name);

extern BOOL            ddi_set_type_bool    (CwtDDIColumn*);
extern BOOL            ddi_set_type_char    (CwtDDIColumn*);
extern BOOL            ddi_set_type_int     (CwtDDIColumn*, LONGLONG min, ULONGLONG max);
extern BOOL            ddi_set_type_float   (CwtDDIColumn*);
extern BOOL            ddi_set_type_double  (CwtDDIColumn*);
extern BOOL            ddi_set_type_decimal (CwtDDIColumn*, UINT prec, UINT scale);
extern BOOL            ddi_set_type_text    (CwtDDIColumn*, ULONGLONG maxlen);
extern BOOL            ddi_set_type_blob    (CwtDDIColumn*, ULONGLONG maxlen);
extern BOOL            ddi_set_type_time    (CwtDDIColumn*, CwtTypeEnum time_type, BOOL stmap);
extern BOOL            ddi_set_type_ref     (CwtDDIColumn*, CwtDDITable*);
extern BOOL            ddi_set_default      (CwtDDIColumn*, const CWT_CHAR *defaultValue);
extern BOOL            ddi_set_autoinc      (CwtDDIColumn*, UINT inc);
extern BOOL            ddi_set_nullable     (CwtDDIColumn*, BOOL yes);
extern BOOL            ddi_set_indexable    (CwtDDIColumn*, BOOL yes);
extern BOOL            ddi_set_unique       (CwtDDIColumn*, BOOL yes);
extern BOOL            ddi_set_pk           (CwtDDIColumn*);

extern BOOL            ddi_deploy           (CwtDBHandler *dbh, CwtDDI *ddi, int flags);
extern BOOL            ddi_recall           (CwtDBHandler *dbh, CwtDDI *ddi, int flags);

static CwtDBI __cwtDBI = {
	  dbi_parse_dsn
	, dbi_load
	, dbi_connect
	, dbi_disconnect
	, dbi_func
	, dbi_attr
	, dbi_set_auto_commit
	, dbi_auto_commit
	, dbi_err
	, dbi_strerror
	, dbi_state
	, dbi_query
	, dbi_query_bin
	, dbi_prepare
	, dbi_execute
	, dbi_last_id
	, dbi_rows
	, dbi_tables
	, dbi_table_exists
	, dbi_encode_n
	, dbi_decode_n
	, dbi_encode
	, dbi_decode
	, dbi_begin
	, dbi_commit
	, dbi_rollback

	, dbi_close
	, dbi_bind
	, dbi_bind_text
	, dbi_bind_blob
	, dbi_set_from_string
	, dbi_set_unitype
	, dbi_set_BOOL
	, dbi_set_CHAR
	, dbi_set_SBYTE
	, dbi_set_BYTE
	, dbi_set_SHORT
	, dbi_set_USHORT
	, dbi_set_INT
	, dbi_set_UINT
	, dbi_set_LONG
	, dbi_set_ULONG
	, dbi_set_LONGLONG
	, dbi_set_ULONGLONG
	, dbi_set_FLOAT
	, dbi_set_DOUBLE
	, dbi_set_TEXT
	, dbi_set_BLOB
	, dbi_set_TIME
	, dbi_set_DATE
	, dbi_set_DATETIME

	, dbi_fetch_next
	, dbi_fetch_column

	, dbi_toCwtTypeEnum
	, dbi_toSqlTypeEnum

	, ddi_create_DDI
	, ddi_build_DDI
	, ddi_free_DDI
	, ddi_new_table
	, ddi_new_column
	, ddi_find_table
	, ddi_find_column

	, ddi_set_type_bool
	, ddi_set_type_char
    , ddi_set_type_int
    , ddi_set_type_float
    , ddi_set_type_double
    , ddi_set_type_decimal
    , ddi_set_type_text
    , ddi_set_type_blob
    , ddi_set_type_time
    , ddi_set_type_ref
    , ddi_set_default
    , ddi_set_autoinc
    , ddi_set_nullable
    , ddi_set_indexable
    , ddi_set_unique
    , ddi_set_pk

	, ddi_deploy
	, ddi_recall
};


DLL_API_EXPORT CwtDBI* cwtDBI(void)
{
	return &__cwtDBI;
}


/**
 * @fn void CwtSysNS::parseDSN(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **params)
 *
 * @brief Parses data source name (DSN).
 *
 * @param dsn [in] Data source name.
 * @param scheme [out] Scheme name.
 * @param driver [out] Database driver.
 * @param arams [out] Parameters passed to database driver.
 */
static void dbi_parse_dsn(const CWT_CHAR *dsn, CWT_CHAR **scheme, CWT_CHAR **driver, CWT_CHAR **driverDSN)
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
    if( driverDSN && opt )
    	*driverDSN = strNS->strDup(opt);

    slNS->free(opts);
}


static CwtDBIDriver* dbi_load(const CWT_CHAR *scheme, const CWT_CHAR *driver)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtDlNS  *dl    = cwtDlNS();

	CwtDBIDriver *dbd = (CwtDBIDriver*)NULL;

	if( strNS->strCaseEq(_T("dbi"), scheme) ) {
		DlHandle dlHandle;
		CwtStringNS *stringNS = cwtStringNS();
		CwtString   *driverName;
		CWT_CHAR    *driverPath;

		driverName = stringNS->create();
		stringNS->sprintf(driverName, _T("cwt-%s"), driver);

		driverPath = dl->buildDlFileName(stringNS->cstr(driverName));
		dlHandle = dl->open(driverPath, TRUE, TRUE);

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

		CWT_FREE(driverPath);
		stringNS->free(driverName);
	} else {
		printf_error(__LOG_PREFIX _Tr("unsupported scheme: %s, only 'dbi' acceptable "), scheme);
	}

	return dbd;
}


static CwtDBHandler* dbi_connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname)
{
	CwtDBIDriver *dbd = NULL;
	CwtDBHandler *dbh = NULL;

	CWT_CHAR *scheme = NULL;
	CWT_CHAR *driver = NULL;
	CWT_CHAR *driverDSN = NULL;

	dbi_parse_dsn(dsn, &scheme, &driver, &driverDSN);

	dbd = dbi_load(scheme, driver);

	if( dbd ) {
		dbh = dbd->connect(driverDSN, username, password, csname);
	}

	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	return dbh;
}

static inline void dbi_disconnect(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	dbh->driver()->disconnect(dbh);
}

static inline BOOL dbi_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CWT_ASSERT(dbh);
	return dbh->driver()->func(dbh, func_name, argv);
}

static inline void dbi_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CWT_ASSERT(dbh);
	dbh->driver()->attr(dbh, attr_name, attr_value);

}

static inline BOOL dbi_set_auto_commit(CwtDBHandler *dbh, BOOL on)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->setAutoCommit(dbh, on);
}

static inline BOOL dbi_auto_commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->autoCommit(dbh);
}

static inline CwtDBI_RC dbi_err(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->err(dbh);
}

static inline const CWT_CHAR* dbi_strerror(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->strerror(dbh);
}

static inline const CWT_CHAR* dbi_state(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->state(dbh);
}

/* cannot be used for statements that contain binary data */
static inline BOOL dbi_query(CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->query(dbh, sql);
}


/* can be used for statements that contain binary data */
static inline BOOL dbi_query_bin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->queryBin(dbh, sql, length);
}

static inline CwtStatement* dbi_prepare(CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	CwtStrNS     *strNS = cwtStrNS();
	CwtUniTypeNS *utNS  = cwtUniTypeNS();
	CwtStatement* sth;

	CWT_ASSERT(dbh);

	sth = dbh->driver()->prepare(dbh, sql);
	if( sth ) {
		size_t nbind_params;
		size_t i;
		sth->dbh    = dbh;
		sth->bind_params = NULL;

		nbind_params = dbh->bindParmsCount(sth);

		sth->bind_params = CWT_MALLOCA(CwtUniType*, nbind_params);
		strNS->bzero(sth->bind_params, sizeof(CwtUniType*) * nbind_params);

		for( i = 0; i < nbind_params; i++ ) {
			sth->bind_params[i] = utNS->create();
		}
	}
	return sth;
}


static BOOL dbi_execute(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	CWT_ASSERT(sth->dbh);
	return sth->dbh->execute(sth);
}

static ULONGLONG dbi_last_id(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	CWT_ASSERT(sth->dbh);
	return sth->dbh->lastId(sth);
}

static inline ULONGLONG dbi_rows(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->rows(dbh);
}

static inline BOOL dbi_tables(CwtDBHandler *dbh, CwtStrList *tables)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->tables(dbh, tables);
}

static inline BOOL dbi_table_exists(CwtDBHandler *dbh, const CWT_CHAR *tname)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->tableExists(dbh, tname);
}

static inline char* dbi_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->encode_n(dbh, s, n);
}

static inline CWT_CHAR* dbi_decode_n(CwtDBHandler *dbh, const char *s, size_t n)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->decode_n(dbh, s, n);
}

static inline char* dbi_encode(CwtDBHandler *dbh, const CWT_CHAR *s)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->encode(dbh, s);
}

static inline CWT_CHAR* dbi_decode(CwtDBHandler *dbh, const char *s)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->decode(dbh, s);
}


static inline BOOL dbi_begin(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->begin(dbh);
}

static BOOL dbi_commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->commit(dbh);
}

static BOOL dbi_rollback(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return dbh->driver()->rollback(dbh);
}


static void dbi_close(CwtStatement *sth)
{
	CwtUniTypeNS *utNS = cwtUniTypeNS();
	size_t nbind_params;

	if( !sth )
		return;

	nbind_params = sth->dbh->bindParmsCount(sth);

	if( nbind_params > 0 && sth->bind_params ) {
		size_t i;
		for( i = 0; i < nbind_params; i++ ) {
			utNS->free(sth->bind_params[i]);
			sth->bind_params[i] = NULL;
		}

		CWT_FREE(sth->bind_params);
		sth->bind_params = NULL;
	}

	sth->dbh->close(sth);
}

static CwtUniType* __dbi_bind_helper(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, size_t sz)
{
	CwtUniType *ut;

	CWT_ASSERT(sth);
	CWT_ASSERT(sth->dbh);

	if( index >= sth->dbh->bindParmsCount(sth) ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
		return NULL;
	}

	ut = sth->bind_params[index];
	cwtUniTypeNS()->set(ut, cwtType, NULL, sz);
	sth->dbh->bindByIndex(sth, index, ut);

	return ut;
}

static CwtUniType* dbi_bind(CwtStatement *sth, size_t index, CwtTypeEnum cwtType)
{
	CwtUniType *ut = NULL;

	CWT_ASSERT(sth);
	CWT_ASSERT(cwtType != CwtType_TEXT);
	CWT_ASSERT(cwtType != CwtType_BLOB);

	ut = __dbi_bind_helper(sth, index, cwtType, 0);
	return ut;
}

static CwtUniType* dbi_bind_text(CwtStatement *sth, size_t index, size_t length)
{
	return __dbi_bind_helper(sth, index, CwtType_TEXT, length);
}

static CwtUniType* dbi_bind_blob( CwtStatement *sth, size_t index, size_t sz)
{
	return __dbi_bind_helper(sth, index, CwtType_BLOB, sz);
}


static BOOL dbi_set_from_string(CwtStatement *sth, CwtUniType *ut, CwtTypeEnum type, const CWT_CHAR *s)
{
	CwtUniTypeNS *utNS = cwtUniTypeNS();
	CwtUniType *val;
	BOOL ok = TRUE;

	val = utNS->create();
	ok = utNS->setFromString(val, type, s);
	dbi_set_unitype(sth, ut, val);
	utNS->free(val);

	return ok;
}


static BOOL dbi_set_unitype(CwtStatement *sth, CwtUniType *ut, CwtUniType *val)
{
	CWT_ASSERT(sth);
	CWT_ASSERT(sth->dbh);
	CWT_ASSERT(ut);
	CWT_ASSERT(val);

	switch( val->type ) {
	case CwtType_TIME:
	case CwtType_DATE:
	case CwtType_DATETIME:
		return sth->dbh->setParm(sth, ut, val->value.ptr, 0);

	case CwtType_TEXT:
	case CwtType_BLOB:
		return sth->dbh->setParm(sth, ut, val->value.ptr, val->length);

	case CwtType_FLOAT:
		return sth->dbh->setParm(sth, ut, &val->value.float_val, 0);

	case CwtType_DOUBLE:
		return sth->dbh->setParm(sth, ut, &val->value.double_val, 0);

	default:
		return sth->dbh->setParm(sth, ut, &val->value.llong_val, 0);
	}

	return FALSE;
}

static CwtTypeEnum dbi_toCwtTypeEnum (CwtSqlTypeEnum sqlType)
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

static CwtSqlTypeEnum dbi_toSqlTypeEnum (CwtTypeEnum cwtType)
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

