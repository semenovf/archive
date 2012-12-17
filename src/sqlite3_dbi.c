/**
 * @file   sqlite3_dbi.c
 * @author wladt
 * @date   Nov 28, 2012 9:59:57 AM
 *
 * @brief
 */


#include <time.h> /* strftime */
#include <cwt/txtcodec.h>
#include <cwt/dbi/dbi.h>
#include <cwt/logger.h>
#include <cwt/strlist.h>
#include <cwt/string.h>
#include <cwt/hashtab.h>
#include "sqlite3/sqlite3.h"

#define _LOG_PREFIX _T("sqlite3: ")
#define _DBH_CAST(dbh)  ((CwtSqliteDBHandler*)(dbh))
#define _STH_CAST(sth)  ((CwtSqliteStatement *)(sth))
#define _DBH(dbh)       (((CwtSqliteDBHandler*)(dbh))->dbh_native)
#define _STH(sth)       (((CwtSqliteStatement*)(sth))->stmt_native)
#define _DBH_RESET_ERRNO(dbh) _DBH_CAST(dbh)->errno = SQLITE_OK
#define _STH_RESET_ERRNO(sth) _STH_CAST(sth)->errno = SQLITE_OK

#define _MAX_SQL_TIMEOUT        10000 /* 10 seconds */
#define _MAX_EXEC_RETRY_COUNT	   10
#define _MAX_EXEC_RETRY_SLEEP	  200


typedef struct _CwtSqliteColumnInfo {
	int         index;
	CwtTypeEnum cwt_type;
} CwtSqliteColumnInfo;

typedef struct _CwtSqliteStatement {
	CwtStatement        __base;
	sqlite3_stmt        *stmt_native;
	CwtErrno             errno;
	CwtString            errorstr;
	CwtHashTable         column_map;     /* maps column names to index */
	CwtSqliteColumnInfo *column_info;    /* helper storage for column_map's values */
} CwtSqliteStatement;


typedef struct _CwtSqliteDBHandler
{
	CwtDBHandler __base;
	sqlite3     *dbh_native;
	CwtErrno     errno;
	CwtString    errorstr;
	CWT_CHAR    *csname;      /* character set name */
} CwtSqliteDBHandler;

static CwtStrNS       *__str_ns = NULL;
static CwtStringNS    *__string_ns = NULL;
static CwtUniTypeNS   *__ut_ns = NULL;
static CwtHashTableNS *__ht_ns = NULL;

/* DBI API functions implementations */
CwtDBHandler*           s3_dbd_connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void             s3_dbd_disconnect(CwtDBHandler *dbh);
static BOOL             s3_dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void             s3_dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL             s3_dbd_set_auto_commit (CwtDBHandler *dbh, BOOL on);
static BOOL             s3_dbd_auto_commit     (CwtDBHandler *dbh);
static CwtErrno         s3_dbd_err             (CwtDBHandler *dbh);
static const CWT_CHAR*  s3_dbd_strerror        (CwtDBHandler *dbh);
static const CWT_CHAR*  s3_dbd_state           (CwtDBHandler *dbh);
static BOOL             s3_dbd_query           (CwtDBHandler *dbh, const CWT_CHAR *sql);
static CwtStatement*    s3_dbd_prepare         (CwtDBHandler *dbh, const CWT_CHAR *statement);
static ULONGLONG        s3_dbd_affected_rows   (CwtDBHandler *dbh);
static BOOL             s3_dbd_tables          (CwtDBHandler *dbh, CwtStrList *tables);
static BOOL             s3_dbd_table_exists    (CwtDBHandler*, const CWT_CHAR *tname);
static char*            s3_dbd_encode_n        (CwtDBHandler *dbh, const CWT_CHAR *s, size_t n);
static CWT_CHAR*        s3_dbd_decode_n        (CwtDBHandler *dbh, const char *s, size_t n);
static char*            s3_dbd_encode          (CwtDBHandler *dbh, const CWT_CHAR *s)
									{ return sqlite_dbd_encode_n(dbh, s, __str_ns->strLen(s)); }
static CWT_CHAR*        s3_dbd_decode          (CwtDBHandler *dbh, const char *s)
									{ return sqlite_dbd_decode_n(dbh, s, strlen(s)); }
static BOOL             s3_dbd_begin           (CwtDBHandler *dbh);
static BOOL             s3_dbd_commit          (CwtDBHandler *dbh);
static BOOL             s3_dbd_rollback        (CwtDBHandler *dbh);

/*
extern CwtStrList*      __dbd_specForDeploy(CwtDDI *ddi, int flags);
extern CwtStrList*      __dbd_specForRecall(CwtDDI *ddi, int flags);
*/

static void             s3_stmt_close         (CwtStatement *sth);
static BOOL             s3_stmt_execute       (CwtStatement *sth);
static ULONGLONG        s3_stmt_last_id       (CwtStatement *sth);
static ULONGLONG        s3_stmt_affected_rows (CwtStatement *sth);
static ULONGLONG        s3_stmt_num_rows      (CwtStatement *sth);
static BOOL             s3_stmt_fetch_next    (CwtStatement *sth);
static BOOL             s3_stmt_fetch_column  (CwtStatement *sth, CWT_CHAR *col, CwtUniType *ut);
static CwtErrno         s3_stmt_err           (CwtStatement *sth);
static const CWT_CHAR*  s3_stmt_strerror      (CwtStatement *sth);
static BOOL             s3_stmt_bind_by_index (CwtStatement *sth, size_t index, CwtUniType *ut);
static size_t           s3_stmt_bind_parms_count(CwtStatement *sth);
static BOOL             s3_stmt_bind_parms_bounds(CwtStatement *sth, int index);
static BOOL             s3_stmt_set_parm      (CwtStatement *sth, CwtUniType *ut, const void *copy, size_t sz);

/* local helper functions */
static void             __s3_destroy          (CwtSqliteDBHandler *sdbh);
static void             __s3_assign_error     (int rc_native, const char *errstr_utf8, CwtErrno *perrno, CwtString *perrstr);
static inline void      __s3_assign_error_dbh (int rc_native, CwtSqliteDBHandler *dbh);
static inline void      __s3_assign_error_sth (int rc_native, CwtSqliteStatement *sth);
static CwtTypeEnum      __s3_from_sqlite3_type(const char *s3_type_utf8, int s3_type);


/*static BOOL __nConnections = 0;*/ /* number of connections */

static CwtDBIDriver __cwtDBIDriver = {
	  s3_dbd_connect
	, s3_dbd_disconnect
	, s3_dbd_func
	, s3_dbd_attr
	, s3_dbd_set_auto_commit
	, s3_dbd_auto_commit
	, s3_dbd_err
	, s3_dbd_strerror
	, s3_dbd_state
	, s3_dbd_query
	, s3_dbd_prepare
	, s3_dbd_affected_rows
	, s3_dbd_tables
	, s3_dbd_table_exists
	, s3_dbd_encode_n
	, s3_dbd_decode_n
	, s3_dbd_encode
	, s3_dbd_decode
	, s3_dbd_begin
    , s3_dbd_commit
    , s3_dbd_rollback

    , NULL /*__dbd_specForDeploy    */
    , NULL /*__dbd_specForRecall    */
};

static struct {
	const char *s3_decl_type;
	CwtTypeEnum cwt_type;
} __s3_types_map[] = {
	  { "TINYINT"    , CwtType_CHAR     }
	, { "SMALLINT"   , CwtType_SHORT    }
	, { "MEDIUMINT"  , CwtType_INT      }
	, { "INT"        , CwtType_INT      }
	, { "BIGINT"     , CwtType_LONGLONG }
	, { "FLOAT"      , CwtType_FLOAT    }
	, { "DOUBLE"     , CwtType_DOUBLE   }
	, { "DECIMAL"    , CwtType_TEXT     }
	, { "YEAR"       , CwtType_SHORT    }
	, { "TIME"       , CwtType_TIME     }
	, { "DATE"       , CwtType_DATE     }
	, { "DATETIME"   , CwtSql_DATETIME  }
	, { "TIMESTAMP"  , CwtType_DATETIME }
	, { "CHAR"       , CwtType_TEXT     }
	, { "BINARY"     , CwtType_BLOB     }
	, { "VARCHAR"    , CwtType_TEXT     }
	, { "VARBINARY"  , CwtType_BLOB     }
	, { "TINYBLOB"   , CwtType_BLOB     }
	, { "TINYTEXT"   , CwtType_TEXT     }
	, { "BLOB"       , CwtType_BLOB     }
	, { "TEXT"       , CwtType_TEXT     }
	, { "MEDIUMBLOB" , CwtType_BLOB     }
	, { "MEDIUMTEXT" , CwtType_TEXT     }
	, { "LONGBLOB"   , CwtType_BLOB     }
	, { "LONGTEXT"   , CwtType_TEXT     }
	, { "BIT"        , CwtType_TEXT     }
};

static struct {
	int s3_ret_code;
	CwtErrno errno;
} __s3_errno_map[] = {
	  { SQLITE_OK          , Cwt_NoError }
/* beginning-of-error-codes */
	, { SQLITE_ERROR       , Cwt_Err_DBI } /* SQL error or missing database */
	, { SQLITE_INTERNAL    , Cwt_Err_DBI } /* Internal logic error in SQLite */
	, { SQLITE_PERM        , Cwt_Err_Perm } /* Access permission denied */
	, { SQLITE_ABORT       , Cwt_Err_DBI } /* Callback routine requested an abort */
	, { SQLITE_BUSY        , Cwt_Err_Busy } /* The database file is locked */
	, { SQLITE_LOCKED      , Cwt_Err_DBI } /* A table in the database is locked */
	, { SQLITE_NOMEM       , Cwt_Err_DBI } /* A malloc() failed */
	, { SQLITE_READONLY    , Cwt_Err_DBI } /* Attempt to write a readonly database */
	, { SQLITE_INTERRUPT   , Cwt_Err_Interrupt } /* Operation terminated by sqlite3_interrupt()*/
	, { SQLITE_IOERR       , Cwt_Err_IO   } /* Some kind of disk I/O error occurred */
	, { SQLITE_CORRUPT     , Cwt_Err_DBI } /* The database disk image is malformed */
	, { SQLITE_NOTFOUND    , Cwt_Err_DBI } /* Unknown opcode in sqlite3_file_control() */
	, { SQLITE_FULL        , Cwt_Err_DBI } /* Insertion failed because database is full */
	, { SQLITE_CANTOPEN    , Cwt_Err_DBI } /* Unable to open the database file */
	, { SQLITE_PROTOCOL    , Cwt_Err_DBI } /* Database lock protocol error */
	, { SQLITE_EMPTY       , Cwt_Err_DBI } /* Database is empty */
	, { SQLITE_SCHEMA      , Cwt_Err_DBI } /* The database schema changed */
	, { SQLITE_TOOBIG      , Cwt_Err_DBI } /* String or BLOB exceeds size limit */
	, { SQLITE_CONSTRAINT  , Cwt_Err_DBI } /* Abort due to constraint violation */
	, { SQLITE_MISMATCH    , Cwt_Err_DBI } /* Data type mismatch */
	, { SQLITE_MISUSE      , Cwt_Err_DBI } /* Library used incorrectly */
	, { SQLITE_NOLFS       , Cwt_Err_DBI } /* Uses OS features not supported on host */
	, { SQLITE_AUTH        , Cwt_Err_Auth } /* Authorization denied */
	, { SQLITE_FORMAT      , Cwt_Err_DBI } /* Auxiliary database format error */
	, { SQLITE_RANGE       , Cwt_Err_Range } /* 2nd parameter to sqlite3_bind out of range */
	, { SQLITE_NOTADB      , Cwt_Err_DBI } /* File opened that is not a database file */
};


DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	if( !__str_ns ) {
		__str_ns    = cwt_str_ns();
		__string_ns = cwt_string_ns();
		__ut_ns     = cwt_unitype_ns();
		__ht_ns     = cwt_hashtable_ns();
	}
	return &__cwtDBIDriver;
}


/**
 *
 * @brief Connects to sqlite3 databases.
 *
 * @param driver_dsn Details see SQLite docs on sqlite3_open_v2 function.
 * 					 Examples:
 *                   path=data.db
 *                   	Open the file "data.db" in the current directory.
 *
 *                   path=/home/fred/data.db
 *                   path=///home/fred/data.db
 *                   path=//localhost/home/fred/data.db
 *                   	Open the database file "/home/fred/data.db".
 *
 *                   path=///C:/Documents%20and%20Settings/fred/Desktop/data.db
 *                   	Windows only: Open the file "data.db" on fred's desktop on drive C:.
 *                   	Note that the %20 escaping in this example is not
 *                   	strictly necessary - space characters can be used literally
 *                   	in URI filenames.
 *
 *                   path=data.db?mode=ro&cache=private
 *                   	Open file "data.db" in the current directory for read-only access.
 *                   	Regardless of whether or not shared-cache mode is enabled
 *                   	by default, use a private cache.
 *
 *                   path=/home/fred/data.db?vfs=unix-nolock
 *                   	Open file "/home/fred/data.db". Use the special VFS "unix-nolock".
 *
 *                   path=data.db?mode=readonly
 *                   	An error. "readonly" is not a valid option for the "mode" parameter.
 *
 * @param username   Unused.
 * @param password   Unused.
 * @param csname     Character set name (Sqlite supports only UTF-16, UTF-16le, UTF-16be, UTF-8).
 *                   Only UTF-8 is supported now by DBI driver.
 * @return DBI connection to specified sqlite3 databases.
 *
 * @note  Autocommit mode is on by default.
 */
CwtDBHandler* s3_dbd_connect(const CWT_CHAR *driver_dsn
		, const CWT_CHAR *username
		, const CWT_CHAR *password
		, const CWT_CHAR *csname)
{
	CwtStrListNS   *strlist_ns = cwt_strlist_ns();

	CwtSqliteDBHandler *dbh = NULL;
	int        rc;
	int        sqlite3_flags;
	CwtString  path;
	char      *path_utf8;
	CwtStrList opts;
	CwtStrListIterator opts_it;

	CWT_UNUSED3(username, password, csname);

    rc = SQLITE_OK;

	/* Parse driver DSN */
	strlist_ns->init(&opts);
	strlist_ns->split(opts, driver_dsn, _T(";"), NULL, 0);
	strlist_ns->begin(opts, &opts_it);

	sqlite3_flags = 0;
	__string_ns->init(&path);

	while( strlist_ns->hasMore(&opts_it) ) {
		const CWT_CHAR* opt = strlist_ns->next(&opts_it);

		if (__str_ns->strNCmp(_T("path="), opt, 5) == 0) {
			__string_ns->append(&path, _T("file:"));
			__string_ns->append(&path, &opt[5]);
		}
	}


#ifdef CWT_UNICODE
    path_utf8 = cwt_textcodec_ns()->toUtf8(string_ns->cstr(&path)
    		, string_ns->size(&path));
#else
    path_utf8 = path;
#endif

    rc = sqlite3_open_v2 (path_utf8, &dbh->dbh_native, sqlite3_flags, 0 );

	if( rc != SQLITE_OK ) {
		if( !dbh->dbh_native ) {
			cwt_logger_ns()->error(_LOG_PREFIX _Tr("Unable to allocate memory for database handler."));
		} else {
			switch( rc ) {
				case SQLITE_CANTOPEN:
					cwt_logger_ns()->error(_LOG_PREFIX _T("%s. Try to check path '%s'")
						, sqlite3_errmsg(dbh->dbh_native)
						, __string_ns->cstr(&path));
					break;
				default: break;
			}
			sqlite3_close(dbh->dbh_native);
		}
	} else {
		dbh = CWT_MALLOC(CwtSqliteDBHandler);

		/* TODO for what this call ?*/
		sqlite3_busy_timeout(_DBH(dbh), _MAX_SQL_TIMEOUT);

		dbh->__base.close      = s3_stmt_close;
		dbh->__base.execute    = s3_stmt_execute;
		dbh->__base.lastId     = s3_stmt_last_id;
		dbh->__base.err        = s3_stmt_err;
		dbh->__base.strerror   = s3_stmt_strerror;
		dbh->__base.bindByIndex= s3_stmt_bind_by_index;
		dbh->__base.bindParmsCount = s3_stmt_bind_parms_count;
		dbh->__base.isBindParmsBounds = s3_stmt_bind_parms_bounds;
		dbh->__base.setParm    = s3_stmt_set_parm;
		dbh->__base.rows       = s3_stmt_affected_rows;
		dbh->__base.size       = s3_stmt_num_rows;
		dbh->__base.fetchNext  = s3_stmt_fetch_next;
		dbh->__base.fetchColumn= s3_stmt_fetch_column;
		dbh->__base.driver     = cwtDBIDriverImpl;

		__string_ns->init(&dbh->errorstr);
		dbh->csname = __str_ns->strDup(csname);
		dbh->errno = SQLITE_OK;
	}

#ifdef CWT_UNICODE
    CWT_FREE(path_utf8);
#endif
    __string_ns->destroy(&path);
    strlist_ns->destroy(&opts);

    return (CwtDBHandler*)dbh;
}


static void s3_dbd_disconnect (CwtDBHandler *dbh)
{
	sqlite3_stmt* stmt;

	if (!dbh )
		return;

	while ((stmt = sqlite3_next_stmt(_DBH(dbh), 0)) != 0)
		sqlite3_finalize(stmt);

	sqlite3_close(_DBH(dbh));
	__string_ns->destroy(&_DBH_CAST(dbh)->errorstr);
	__s3_destroy(dbh);
	CWT_FREE(dbh);
}


static BOOL s3_dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CWT_UNUSED3(dbh, func_name, argv);
	return FALSE;
}

static void s3_dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CWT_UNUSED3(dbh, attr_name, attr_value);
}

static BOOL s3_dbd_set_auto_commit (CwtDBHandler *dbh, BOOL on)
{
	return TRUE;
}

static BOOL s3_dbd_auto_commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return 0 == sqlite3_get_autocommit(_DBH(dbh))
			? FALSE
			: TRUE;
}

static CwtErrno s3_dbd_err(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return _DBH_CAST(dbh)->errno;
}

static const CWT_CHAR* s3_dbd_strerror(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return SQLITE_OK == _DBH_CAST(dbh)->errno
			? __str_ns->constEmptyStr()
			: __string_ns->cstr(&_DBH_CAST(dbh)->errorstr);
}


/**
 * @brief Returns a null-terminated string containing the SQLSTATE error code.
 *
 * @param dbh Database handle
 * @return a null-terminated string containing the SQLSTATE error code.
 */
static const CWT_CHAR*  s3_dbd_state(CwtDBHandler *dbh)
{
	/* TODO could this be implemented? */
	return _T("00000");
}


static BOOL s3_dbd_query (CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	int rc;
	char *sql_utf8;

	CWT_ASSERT(dbh);

	_DBH_RESET_ERRNO(dbh);

	sql_utf8 = s3_dbd_encode(dbh, sql);
	rc = sqlite3_exec(_DBH(dbh), sql_utf8, NULL, NULL, NULL);
	CWT_FREE(sql_utf8);

	if (SQLITE_OK != rc)
		__s3_assign_error_dbh(rc, dbh);

	return 0 == rc ? TRUE : FALSE;
}


static CwtStatement* s3_dbd_prepare(CwtDBHandler *dbh, const CWT_CHAR *statement)
{
	sqlite3_stmt *stmt_native;
	CwtSqliteStatement *sth;
	char *sql_utf8;
	int rc;
	int column_count;

	_DBH_RESET_ERRNO(dbh);

	sql_utf8 = cwt_textcodec_ns()->toUtf8(statement, __str_ns->strLen(statement));
	rc = sqlite3_prepare_v2(
			_DBH(dbh)       /* Database handle */
			, sql_utf8      /* SQL statement, UTF-8 encoded */
			, -1            /* Maximum length of zSql in bytes. */
			, &stmt_native  /* OUT: Statement handle */
			, NULL );       /* OUT: Pointer to unused portion of zSql */
	CWT_FREE(sql_utf8);

	if (rc != SQLITE_OK) {
		__s3_assign_error_dbh(rc, dbh);
		return NULL;
	}

	sth = CWT_MALLOC(CwtSqliteStatement);
	sth->stmt_native = stmt_native;
	sth->errno = SQLITE_OK;
	__string_ns->init(&sth->errorstr);
	__ht_ns->init(&sth->column_map, __ht_ns->strHash, __ht_ns->streq, cwt_free, NULL);
	sth->column_info = NULL;

	column_count = sqlite3_column_count(_STH(sth));
	if (column_count > 0) { /* SELECT statement */
		int i;

		sth->column_info = CWT_MALLOCA(CwtSqliteColumnInfo, column_count);

		for (i = 0; i < column_count; ++i) {
			char *cname_utf8;

			sth->column_info[i].index = i;
			sth->column_info[i].cwt_type = __s3_from_sqlite3_type(
					sqlite3_column_decltype(_STH(sth), i)
					, sqlite3_column_type(_STH(sth), i));

			cname_utf8 = sqlite3_column_name(_STH(sth), i);

			__ht_ns->insert(&sth->column_map
				, cwt_textcodec_ns()->fromUtf8(cname_utf8, strlen(cname_utf8))
				, &sth->column_info[i]);
		}
	}

	return (CwtStatement *)sth;
}

static ULONGLONG s3_dbd_affected_rows (CwtDBHandler *dbh)
{
	int nrows;
	nrows = sqlite3_changes(_DBH(dbh));

	if (nrows < 0)
		return (ULONGLONG)0;

	return (ULONGLONG)nrows;
}

/*
;
*/
static BOOL s3_dbd_tables (CwtDBHandler *dbh, CwtStrList *tables)
{
	CwtStrListNS *sl_ns  = cwt_strlist_ns();
	CwtString sql;
	CwtSqliteStatement *sth;
	BOOL ok = TRUE;
	CwtString tname;

	CWT_ASSERT(dbh);
	CWT_ASSERT(_DBH(dbh));
	CWT_ASSERT(tables);

	_DBH_RESET_ERRNO(dbh);

	__string_ns->init(&sql);
	__string_ns->init(&tname);
	__string_ns->assign(&sql, _T("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name"));

	sth = s3_dbd_prepare(dbh, __string_ns->cstr(&sql));

	if (sth) {
		CwtUniType ut;
		__ut_ns->init(&ut);

		if (s3_stmt_execute(sth)) {
			while (s3_stmt_fetch_next(sth)) {
				if (s3_stmt_fetch_column(sth, _T("name"), &ut)) {
					sl_ns->append(tables, __ut_ns->toString(ut, &tname));
				}
			}
		} else {
			ok = FALSE;
		}

		__ut_ns->destroy(&ut);
		s3_stmt_close(sth);

	}
	__string_ns->destroy(&tname);
	__string_ns->destroy(&sql);

	return ok;
}

static BOOL s3_dbd_table_exists (CwtDBHandler *dbh, const CWT_CHAR *tname)
{
	BOOL ok = FALSE;
	CwtString sql;
	CwtSqliteStatement *sth;

	_DBH_RESET_ERRNO(dbh);

	__string_ns->init(&sql);
	__string_ns->sprintf(&sql
			, _T("SELECT COUNT(*) as table_count FROM sqlite_master WHERE type = 'table' AND name = '%s'")
			, tname);

	sth = s3_dbd_prepare(dbh, __string_ns->cstr(&sql));


	if (sth) {
		CwtUniType ut;

		__ut_ns->init(&ut);

		if (s3_stmt_bind_by_index(sth, 1, &ut)
				&& s3_stmt_set_parm(sth, &ut, tname, __str_ns->strLen(tname))
				&& s3_stmt_execute(sth)
				&& s3_stmt_fetch_next(sth)) {

			if (s3_stmt_fetch_column(sth, _T("name"), &ut)) {
				if (__ut_ns->toINT(&ut, NULL) > 0)
					ok = TRUE;
			}

		}
		__ut_ns->destroy(&ut);
		s3_stmt_close(sth);
	}
	__string_ns->destroy(&sql);

	return ok;
}

static char* s3_dbd_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n)
{
	CwtSqliteDBHandler *sdbh = (CwtSqliteDBHandler *)dbh;

	CWT_ASSERT(sdbh);
	CWT_ASSERT(_DBH(sdbh));

	return cwt_textcodec_ns()->toMBCS(s, sdbh->csname, n);
}

static CWT_CHAR* s3_dbd_decode_n(CwtDBHandler *dbh, const char *s, size_t n)
{
	CwtSqliteDBHandler *sdbh = (CwtSqliteDBHandler *)dbh;

	CWT_ASSERT(sdbh);
	CWT_ASSERT(_DBH(sdbh));

	return cwt_textcodec_ns()->fromMBCS(s, sdbh->csname, n);
}


static BOOL s3_dbd_begin (CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return s3_dbd_query(dbh, _T("BEGIN"));
}

static BOOL s3_dbd_commit (CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return s3_dbd_query(dbh, _T("COMMIT"));
}

static BOOL s3_dbd_rollback (CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return s3_dbd_query(dbh, _T("ROLLBACK"));

}


static void s3_stmt_close (CwtStatement *sth)
{
	if (sth && _STH(sth)) {
		int rc;
		rc = sqlite3_finalize(_STH(sth));
		if (rc != SQLITE_OK) {
			cwt_logger_ns()->error(_T("Close statement error: %s")
					, s3_dbd_strerror(sth->dbh));
		}
		_STH(sth) = NULL;
		__string_ns->destroy(&_STH_CAST(sth)->errorstr);
		__ht_ns->destroy(&_STH_CAST(sth)->column_map);

		if (_STH_CAST(sth)->column_info)
			CWT_FREE(_STH_CAST(sth)->column_info);

		CWT_FREE(sth);
	}
}


static BOOL s3_stmt_execute (CwtStatement *sth)
{
	sqlite3_reset(_STH(sth));
	return TRUE;
}

static BOOL s3_stmt_fetch_next (CwtStatement *sth)
{
	int rc;
	int nretries = _MAX_EXEC_RETRY_COUNT;

	CWT_ASSERT(sth);
	CWT_ASSERT(_STH(sth));

	_STH_RESET_ERRNO(sth);

	do {
		rc = sqlite3_step(_STH(sth));

		if( rc == SQLITE_BUSY ) {
			if( nretries++ > _MAX_EXEC_RETRY_COUNT ) {
				break;
			} else {
				sqlite3_sleep(_MAX_EXEC_RETRY_SLEEP);
			}
		}
	} while ( rc == SQLITE_BUSY );

	if( rc == SQLITE_DONE ) {
		return TRUE;
	}
	if( rc == SQLITE_ROW ) {
		return TRUE;
	}
	__s3_assign_error_sth(rc, sth);

	return FALSE;
}

/**
 *
 * @param sth
 * @param col
 * @param ut
 * @return @c TRUE if column fetching is successful. @c FALSE if column not found
 * 	       (error code set to SQLITE_RANGE)
 */
static BOOL s3_stmt_fetch_column (CwtStatement *sth, CWT_CHAR *col, CwtUniType *ut)
{
	CwtTypeEnum cwt_type;
	BOOL ok = FALSE;
	CwtSqliteColumnInfo *pcol_info;

	CWT_ASSERT(sth);
	CWT_ASSERT(ut);

	pcol_info = (CwtSqliteColumnInfo *)__ht_ns->lookup(_STH_CAST(sth)->column_map, col);

	if( !pcol_info ) {
		_STH_CAST(sth)->errno = SQLITE_RANGE;
		__string_ns->sprintf(&_STH_CAST(sth)->errorstr, _T("column '%s' not found"), col);
		return FALSE;
	}

	if (pcol_info->index < 0 || pcol_info->index >= sqlite3_column_count(_STH(sth))) {
		_STH_CAST(sth)->errno = SQLITE_RANGE;
		__string_ns->sprintf(&_STH_CAST(sth)->errorstr, _T("column index '%d' is out of bounds")
				, pcol_info->index);
		return FALSE;
	}

	cwt_type = pcol_info->cwt_type;


	switch (cwt_type) {
	case CwtType_TIME:
	case CwtType_DATE:
	case CwtType_DATETIME: {
			CWT_TIME cwtm;
			struct tm tm;
			const char *bytes;
			size_t nchars;
			CWT_CHAR *s;

			bytes = sqlite3_column_text(_STH(sth), pcol_info->index);
			s = s3_dbd_decode_n(sth->dbh, (const char*)bytes, strlen((const char*)bytes));
			nchars = __str_ns->strLen(s);
			cwt_bzero(&cwtm, sizeof(CWT_TIME));

			if (CwtType_TIME == cwt_type && nchars == 6) {  /* format 'hhmmss' */
				__str_ns->strPtime(s, _T("%H%M%S"), &tm);
				cwtm.hour     = tm.tm_hour;
				cwtm.min      = tm.tm_min;
				cwtm.sec      = tm.tm_sec;
				cwtm.sec_part = 0;
				ok = __ut_ns->setTIME(ut, &cwtm);
			} else if (CwtType_DATE == cwt_type && nchars == 8) { /* format 'YYYYMMDD' */
				__str_ns->strPtime(s, _T("%Y%m%d"), &tm);
				cwtm.year     = tm.tm_year;
				cwtm.mon      = tm.tm_mon;
				cwtm.day      = tm.tm_mday;
				ok = __ut_ns->setDATE(ut, &cwtm);
			} else if (CwtType_DATETIME == cwt_type && nchars == 14) { /* format 'YYYYMMDDhhmmss' */
				__str_ns->strPtime(s, _T("%Y%m%d%H%M%S"), &tm);
				cwtm.year     = tm.tm_year;
				cwtm.mon      = tm.tm_mon;
				cwtm.day      = tm.tm_mday;
				cwtm.hour     = tm.tm_hour;
				cwtm.min      = tm.tm_min;
				cwtm.sec      = tm.tm_sec;
				cwtm.sec_part = 0;
				ok = __ut_ns->setDATETIME(ut, &cwtm);
			} else {
				_STH_CAST(sth)->errno = Cwt_Err_BadVal;
				__string_ns->sprintf(_STH_CAST(sth)->errorstr, _Tr("Bad value for date/time"));
			}
			CWT_FREE(s);
		}
		break;

	case CwtType_CHAR:
		int ch;
		ch = sqlite3_column_int(_STH(sth), pcol_info->index);
		if (ch >= CWT_CHAR_MIN && ch <= CWT_CHAR_MAX)
			ok = __ut_ns->setCHAR(ut, (CWT_CHAR)ch);
		else
			; /* error */
		break;

	case CwtType_SHORT:
		int i;
		i = sqlite3_column_int(_STH(sth), pcol_info->index);
		if (i >= CWT_SHORT_MIN && i <= CWT_SHORT_MAX)
			ok = __ut_ns->setSHORT(ut, (SHORT)i);
		else
			; /* error */
		break;

	case CwtType_INT:
		ok = __ut_ns->setINT(ut, sqlite3_column_int(_STH(sth), pcol_info->index));
		break;

	case CwtType_LONGLONG:
		ok = __ut_ns->setLONGLONG(ut, (LONGLONG)sqlite3_column_int64(_STH(sth), pcol_info->index));
		break;

	case CwtType_FLOAT:
		ok = __ut_ns->setFLOAT(ut, (float)sqlite3_column_double(_STH(sth), pcol_info->index));
		break;

	case CwtType_DOUBLE:
		ok = __ut_ns->setDOUBLE(ut, sqlite3_column_double(_STH(sth), pcol_info->index));
		break;

	case CwtType_TEXT: {
			const unsigned char *bytes;
			CWT_CHAR *s;

			bytes = sqlite3_column_text(_STH(sth), pcol_info->index);
			CWT_CHAR *s = s3_dbd_decode_n(sth->dbh, (const char*)bytes, strlen((const char*)bytes));
			ok = __ut_ns->setTEXT(ut, s, __str_ns->strLen(s));
			CWT_FREE(s);
		}
		break;

	case CwtType_BLOB: {
			const void *bytes;
			int nbytes;

			bytes = sqlite3_column_blob(_STH(sth), pcol_info->index);
			nbytes = sqlite3_column_bytes(_STH(sth), pcol_info->index);
			ok = __ut_ns->setBLOB(ut, bytes, (size_t)nbytes);
		}
		break;

	default:
		_STH_CAST(sth)->errno = Cwt_Err_BadVal;
		__string_ns->sprintf(_STH_CAST(sth)->errorstr
				, _Tr("Bad type of fetched value for column '%s'"), col);
		break;
	}

	return ok;
}

static ULONGLONG s3_stmt_last_id (CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)sqlite3_last_insert_rowid(_DBH(sth->dbh));
}

static ULONGLONG s3_stmt_affected_rows (CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return s3_dbd_affected_rows(sth->dbh);
}


/**
 * @fn ULONGLONG CwtSqliteDBHandler::rows(CwtStatement *sth)
 *
 * @brief Calculates number of rows in result set.
 *
 * @param sth Statement handler.
 * @return Number of rows in result set. Zero value may signal about error occurred while
 * 		counting the number or rows. In this case appropriate error is set.
 *
 * @note It calculates rows through a sequence of 'sqlite3_step' calls.
 * 		 It is not recommended to use this function unless you need only result without
 * 		 the following extract of data.
 */
static ULONGLONG s3_stmt_num_rows (CwtStatement *sth)
{
	CWT_ASSERT(sth);

	int rc;
	int nretries = _MAX_EXEC_RETRY_COUNT;
	ULONGLONG nrows = 0;

	CWT_ASSERT(sth);
	CWT_ASSERT(_STH(sth));
	_STH_RESET_ERRNO(sth);

	sqlite3_reset(_STH(sth));
	__s3_assign_error_sth(SQLITE_OK, sth); /* reset errors */

	do {
		rc = sqlite3_step(_STH(sth));

		if( rc == SQLITE_ROW ) {
			nrows++;
		} else if( rc == SQLITE_BUSY ) {
			if( nretries++ > _MAX_EXEC_RETRY_COUNT ) {
				__s3_assign_error_sth(rc, sth);
				nrows = 0;
				break;
			} else {
				sqlite3_sleep(_MAX_EXEC_RETRY_SLEEP);
			}
		} else if (rc == SQLITE_DONE) {
			;
		} else {
			__s3_assign_error_sth(rc, sth);
			nrows = 0;
			break;
		}
	} while ( rc != SQLITE_DONE );

	return nrows;
}

static CwtErrno s3_stmt_err (CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return _STH_CAST(sth)->errno;
}

static const CWT_CHAR*  s3_stmt_strerror (CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return Cwt_NoError == _STH_CAST(sth)->errno
			? __str_ns->constEmptyStr()
			: __string_ns->cstr(&_STH_CAST(sth)->errorstr);
}

static BOOL s3_stmt_bind_by_index (CwtStatement *sth, size_t index, CwtUniType *ut)
{
	/* Real bind made while setting value */
	CWT_UNUSED3(sth, index, ut);
	return TRUE;
}



static size_t s3_stmt_bind_parms_count(CwtStatement *sth)
{
	CWT_ASSERT(sth);

	/* 'sqlite3_bind_parameter_count'
	 * routine actually returns the index of the largest (rightmost) parameter */
	return (size_t)sqlite3_bind_parameter_count(_STH(sth)) + 1;
}


static BOOL s3_stmt_bind_parms_bounds(CwtStatement *sth, int index)
{
	if( index < 0 || index > sqlite3_bind_parameter_count(_STH(sth)) )
		return FALSE;

	return TRUE;
}


static BOOL s3_stmt_set_parm (CwtStatement *sth, CwtUniType *ut, const void *copy, size_t sz)
{
	int rc;
	char buf[20]; /*YYYYMMDDhhmmss.SSS*/
	BOOL ok = TRUE;

	CWT_ASSERT(sth);

	/* Unbound parameters are interpreted as NULL. */
	if (!ut)
		return TRUE;

	ok = __ut_ns->set(ut, ut->type, copy, sz);

	if (ok) {
		switch (ut->type) {
		case CwtType_BOOL:
			/* Boolean values are stored as integers 0 (false) and 1 (true). */
			rc = sqlite3_bind_int(_STH(sth)
					, (int)index
					, __ut_ns->toBOOL(ut, &ok) == TRUE ? 1 : 0);
			break;
		case CwtType_CHAR:
		case CwtType_SBYTE:
		case CwtType_BYTE:
		case CwtType_SHORT:
		case CwtType_USHORT:
		case CwtType_INT:
		case CwtType_UINT:
			rc = sqlite3_bind_int(_STH(sth)
					, (int)index
					, __ut_ns->toINT(ut, &ok));
			break;
		case CwtType_LONG:
		case CwtType_ULONG:
		case CwtType_LONGLONG:
		case CwtType_ULONGLONG:
			rc = sqlite3_bind_int64(_STH(sth)
					, (int)index
					, __ut_ns->toLONGLONG(ut, &ok));
			break;
		case CwtType_FLOAT:
		case CwtType_DOUBLE:
			rc = sqlite3_bind_double(_STH(sth)
					, (int)index
					, __ut_ns->toDOUBLE(ut, &ok));
			break;

		case CwtType_TEXT: {
				CWT_CHAR *orig;
				char *utf8;

				orig = __ut_ns->toTEXT(ut, &ok);
				if (ok && orig) {
					utf8 = s3_dbd_encode(_DBH(sth->dbh), orig);

					/* utf8 will be freed automatically (see last argument)*/
					rc = sqlite3_bind_text(_STH(sth), (int)index, utf8, strlen(utf8), cwt_free);
				}

				CWT_FREE(orig);
			}	if (!ok) {
				_STH_CAST(sth)->errno = Cwt_Err_BadVal;
				__string_ns->sprintf(_STH_CAST(sth)->errorstr
						, _Tr("Bad bind value with index %u"), index);
				return FALSE;
			}

			break;

		case CwtType_BLOB: {
				size_t sz;
				const void *bytes;

				bytes = __ut_ns->toBLOB(ut, &sz);
				CWT_ASSERT(sz <= CWT_INT_MAX);
				rc = sqlite3_bind_blob(_STH(sth), (int)index, bytes, (int)sz, SQLITE_STATIC);
			}
			break;

		case CwtType_TIME: {
				CWT_TIME tm;
				__ut_ns->toTIME(ut, &tm, &ok);
				strftime(buf
					, "%H%M%S"
					, tm.hour, tm.min, tm.sec);
				rc = sqlite3_bind_text(_STH(sth), (int)index, buf, strlen(buf), SQLITE_STATIC);
			}
			break;
		case CwtType_DATE: {
				CWT_TIME tm;
				__ut_ns->toDATE(ut, &tm, &ok);
				strftime(buf
					, "%Y%m%d"
					, tm.year, tm.mon, tm.hour);
				rc = sqlite3_bind_text(_STH(sth), (int)index, buf, strlen(buf), SQLITE_STATIC);
			}
			break;

		case CwtType_DATETIME: {
				CWT_TIME tm;
				__ut_ns->toDATETIME(ut, &tm, &ok);
				strftime(buf
					, "%Y%m%d%H%M%S"
					, tm.year, tm.mon, tm.hour
					, tm.hour, tm.min, tm.sec);
				rc = sqlite3_bind_text(_STH(sth), (int)index, buf, strlen(buf), SQLITE_STATIC);
			}
			break;

		default:
			ok = FALSE;
			break;
		}
	}

	if (!ok) {
		_STH_CAST(sth)->errno = Cwt_Err_BadVal;
		__string_ns->sprintf(_STH_CAST(sth)->errorstr
				, _Tr("Bad bind value with index %u"), index);
		return FALSE;
	}

	if (SQLITE_OK != rc) {
		__s3_assign_error_sth (rc, sth);
		return FALSE;
	}

	return TRUE;
}

static void __s3_destroy(CwtSqliteDBHandler *sdbh)
{
	CWT_ASSERT(sdbh);
	__string_ns->destroy(&sdbh->errorstr);
	CWT_FREE(sdbh->csname);
}

static void  __s3_assign_error(int rc_native, const char *errstr_utf8, CwtErrno *perrno, CwtString *errstr)
{
	CWT_CHAR *errstr_decoded;

	if (SQLITE_OK == rc_native) {
		if (errstr)
			__string_ns->clear(errstr);
	} else if (errstr) {
		errstr_decoded = cwt_textcodec_ns()->fromUtf8(errstr_utf8, strlen(errstr_utf8));
		__string_ns->assign(errstr, errstr_decoded);
		CWT_FREE(errstr_decoded);
	}

	if (perrno) {
		int i, n;
		*perrno = Cwt_Err_DBI;

		n = sizeof(__s3_errno_map)/sizeof(__s3_errno_map[0]);
		for (i = 0; i < n; ++i) {
			if (rc_native == __s3_errno_map[i].s3_ret_code) {
				*perrno = __s3_errno_map[i].errno;
				break;
			}
		}

		if (*perrno == Cwt_Err_DBI) {
			cwt_logger_ns()->warn(_Tr("Native Sqlite3 error is not mapped yet: %d"), rc_native);
		}
	}
}

static inline void __s3_assign_error_dbh (int rc_native, CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	__s3_assign_error(rc_native
			, SQLITE_OK == rc_native ? NULL : sqlite3_errmsg(_DBH(dbh))
			, &_DBH_CAST(dbh)->errno
			, _DBH_CAST(dbh)->errorstr);
}

static inline void __s3_assign_error_sth (int rc_native, CwtStatement *sth)
{
	CWT_ASSERT(sth);
	__s3_assign_error(rc_native
			, SQLITE_OK == rc_native ? NULL : sqlite3_errmsg(_DBH(sth->dbh))
			, &_STH_CAST(sth)->errno
			, _STH_CAST(sth)->errorstr);
}

static CwtTypeEnum __s3_from_sqlite3_type (const char *s3_type_utf8, int s3_type)
{
	CwtTypeEnum cwt_type;
	int i, types_count;

	cwt_type = CwtType_UNKNOWN;

	if (s3_type_utf8) {
		types_count = sizeof(__s3_types_map)/sizeof(__s3_types_map[0]);

		for (i = 0; i < types_count; ++i) {
			if (strstr(s3_type_utf8, __s3_types_map[i].s3_decl_type)) {
				cwt_type = __s3_types_map[i].cwt_type;
				break;
			}
		}
	}

	if (cwt_type == CwtType_UNKNOWN) {
		switch( s3_type ) {
		case SQLITE_INTEGER:
			cwt_type = CwtType_LONGLONG;
			break;
		case SQLITE_FLOAT:
			cwt_type = CwtType_DOUBLE;
			break;
		case SQLITE_BLOB:
			cwt_type = CwtType_BLOB;
			break;
#if defined(SQLITE3_TEXT) && SQLITE_TEXT != SQLITE3_TEXT
		case SQLITE3_TEXT:
#endif
		case SQLITE_TEXT:
		case SQLITE_NULL:
		default:
			cwt_type = CwtType_TEXT;
			break;
		}
	}

	return cwt_type;
}
