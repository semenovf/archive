/**
 * @file   sqlite3_dbd.cpp
 * @author wladt
 * @date   Aug 19, 2013 14:45
 *
 * @brief
 */

//#include <ctime> /* strftime */
#include <cstdlib>
#include <cwt/logger.hpp>
#include <cwt/string.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/hash.hpp>
#include <cwt/dbd.hpp>
#include <cwt/dbh.hpp>
#include <cwt/unitype.hpp>
#include <cwt/vector.hpp>
#include <cwt/bytearray.hpp>

/*
#include <cwt/txtcodec.h>
#include <cwt/dbi/dbi.h>
#include <cwt/logger.h>
#include <cwt/strlist.h>
#include <cwt/string.h>
#include <cwt/hashtab.h>
*/
#include "../sqlite3/sqlite3.h"

CWT_NS_BEGIN

static int __refs = 0;
static DbDriver * __dbd = nullptr;

/*
#define _LOG_PREFIX _T("sqlite3: ")
#define _DBH_CAST(dbh)  ((CwtSqliteDBHandler*)(dbh))
#define _STH_CAST(sth)  ((CwtSqliteStatement *)(sth))
#define _DBH(dbh)       (((CwtSqliteDBHandler*)(dbh))->dbh_native)
#define _STH(sth)       (((CwtSqliteStatement*)(sth))->stmt_native)
#define _DBH_RESET_ERRNO(dbh) _DBH_CAST(dbh)->errno = SQLITE_OK
#define _STH_RESET_ERRNO(sth) _STH_CAST(sth)->errno = SQLITE_OK
*/

const int __MAX_SQL_TIMEOUT      = 10000; /* 10 seconds */
const int __MAX_EXEC_RETRY_COUNT = 10;
const int __MAX_EXEC_RETRY_SLEEP = 200;

/*
typedef struct _CwtSqliteColumnInfo {
	int         index;
	CwtTypeEnum cwt_type;
} CwtSqliteColumnInfo;
*/

//typedef struct _CwtSqliteStatement {
//	CwtStatement        __base;
//	sqlite3_stmt        *stmt_native;
//	CwtErrno             errno;
//	CwtString            errorstr;
//	CwtHashTable         column_map;     /* maps column names to index */
//	CwtSqliteColumnInfo *column_info;    /* helper storage for column_map's values */
//} CwtSqliteStatement;


struct Sqlite3DbHandler : public DbHandlerData
{
	sqlite3 * dbh_native;
};

struct Sqlite3DbStatement : public DbStatementData
{
	sqlite3_stmt * sth_native;
};


/* DBI API functions implementations */
DbHandlerData *         s3_dbd_open  (const String & driver_uri
		, const String & username
		, const String & password
		, const Hash<String, String> & params);
static void             s3_dbd_close           (DbHandlerData * dbh);
static bool             s3_dbd_set_auto_commit (DbHandlerData & dbh, bool on);
static bool             s3_dbd_auto_commit     (DbHandlerData & dbh);
static long_t           s3_dbd_errno           (DbHandlerData & dbh);
static String           s3_dbd_strerror        (DbHandlerData & dbh);

static bool             s3_dbd_query           (DbHandlerData & dbh, const String & sql);
static DbStatementData * s3_dbd_prepare        (DbHandlerData & dbh, const String & sql);
static ulong_t          s3_dbd_affected_rows   (DbHandlerData & dbh);
static bool             s3_dbd_begin           (DbHandlerData & dbh);
static bool             s3_dbd_commit          (DbHandlerData & dbh);
static bool             s3_dbd_rollback        (DbHandlerData & dbh);

static void             s3_dbd_stmt_close      (DbStatementData * sth);
static bool             s3_dbd_stmt_exec       (DbStatementData & sth);
static Vector<UniType>  s3_dbd_stmt_fetch_row_array (DbStatementData & sth);
static Hash<String, UniType>  s3_dbd_stmt_fetch_row_hash (DbStatementData & sth);

#ifdef __COMMENT__
static BOOL             s3_dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void             s3_dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL             s3_dbd_set_auto_commit (CwtDBHandler *dbh, BOOL on);
static BOOL             s3_dbd_auto_commit     (CwtDBHandler *dbh);
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

/*
extern CwtStrList*      __dbd_specForDeploy(CwtDDI *ddi, int flags);
extern CwtStrList*      __dbd_specForRecall(CwtDDI *ddi, int flags);
*/



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
	, s3_dbd_func
	, s3_dbd_attr
	, s3_dbd_err
	, s3_dbd_strerror
	, s3_dbd_state
	, s3_dbd_prepare
	, s3_dbd_tables
	, s3_dbd_table_exists
	, s3_dbd_encode_n
	, s3_dbd_decode_n
	, s3_dbd_encode
	, s3_dbd_decode
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
	, { "DATETIME"   , CwtType_DATETIME  }
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
#endif


extern "C" DbDriver * __open__()
{
	if (!__dbd) {
		__dbd = new DbDriver;

		__dbd->open          = s3_dbd_open;
		__dbd->close         = s3_dbd_close;
		__dbd->autoCommit    = s3_dbd_auto_commit;
		__dbd->setAutoCommit = s3_dbd_set_auto_commit;
		__dbd->errno         = s3_dbd_errno;
		__dbd->strerror      = s3_dbd_strerror;
		__dbd->query         = s3_dbd_query;
		__dbd->prepare       = s3_dbd_prepare;
		__dbd->rows          = s3_dbd_affected_rows;
		__dbd->begin         = s3_dbd_begin;
		__dbd->commit        = s3_dbd_commit;
		__dbd->rollback      = s3_dbd_rollback;
		__dbd->closeStmt     = s3_dbd_stmt_close;
		__dbd->execStmt      = s3_dbd_stmt_exec;
		__dbd->fetchRowArray = s3_dbd_stmt_fetch_row_array;
		__dbd->fetchRowHash  = s3_dbd_stmt_fetch_row_hash;

		/*
				dbh->__base.close      = s3_stmt_close;
				dbh->__base.execute    = s3_stmt_execute;
				dbh->__base.lastId     = s3_stmt_last_id;
				dbh->__base.err        = s3_stmt_err;
				dbh->__base.strerror   = s3_stmt_strerror;
				dbh->__base.bindByIndex= s3_stmt_bind_by_index;
				dbh->__base.bindParmsCount = s3_stmt_bind_parms_count;
				dbh->__base.isBindParmsBounds = s3_stmt_bind_parms_bounds;
				dbh->__base.setParm    = s3_stmt_set_parm;
				dbh->__base.size       = s3_stmt_num_rows;
				dbh->__base.fetchNext  = s3_stmt_fetch_next;
				dbh->__base.fetchColumn= s3_stmt_fetch_column;
		*/

	}

	return __dbd;
}

/**
 *
 * @brief Connects to sqlite3 databases.
 *
 * @param driver_dsn Details see SQLite docs on sqlite3_open_v2 function.
 *				Examples:
 *              	file:data.db
 *                 		Open the file "data.db" in the current directory.
 *
 *                  file:/home/fred/data.db
 *                  file:///home/fred/data.db
 *                  file://localhost/home/fred/data.db
 *                  	Open the database file "/home/fred/data.db".
 *
 *                  file:///C:/Documents%20and%20Settings/fred/Desktop/data.db
 *                  	Windows only: Open the file "data.db" on fred's desktop on drive C:.
 *                  	Note that the %20 escaping in this example is not
 *                  	strictly necessary - space characters can be used literally
 *                  	in URI filenames.
 *
 *                  file:data.db?mode=ro&cache=private
 *                  	Open file "data.db" in the current directory for read-only access.
 *                  	Regardless of whether or not shared-cache mode is enabled
 *                  	by default, use a private cache.
 *
 *                  file:/home/fred/data.db?vfs=unix-nolock
 *                  	Open file "/home/fred/data.db". Use the special VFS "unix-nolock".
 *
 *                  file:data.db?mode=readonly
 *                  	An error. "readonly" is not a valid option for the "mode" parameter.
 *
 *				Mode values:
 *					mode=ro
 *				    mode=rw
 *                  mode=rwc
 *                  mode=memory
 *
 *              Cache values:
 *              	cache=shared
 *              	cache=private
 *
 * @param username   Unused.
 * @param password   Unused.
 * //@param csname     Character set name (Sqlite supports only UTF-16, UTF-16le, UTF-16be, UTF-8).
 * //                  Only UTF-8 is supported now by DBI driver.
 * @return DBI connection to specified sqlite3 databases.
 *
 * @note  Autocommit mode is on by default.
 */
DbHandlerData * s3_dbd_open(const String & path
		, const String & username
		, const String & password
		, const Hash<String, String> & params)
{
	Sqlite3DbHandler * dbh = nullptr;
	sqlite3 *  dbh_native = nullptr;
	int        rc = SQLITE_OK;
	int        s3_flags;
	int        s3_flag_mode;

	CWT_UNUSED2(username, password);

    s3_flags = SQLITE_OPEN_URI;
	s3_flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

	Hash<String, String>::const_iterator mode = params.find(String("mode"));

	if (mode != params.cend()) {
		if (*mode == "ro")
			s3_flag_mode = SQLITE_OPEN_READONLY;
		else if (*mode == "rw")
			s3_flag_mode = SQLITE_OPEN_READWRITE;
		else if (*mode == "rwc")
			s3_flag_mode = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
		else if (*mode == "memory")
			s3_flag_mode = SQLITE_OPEN_MEMORY;
	}

	s3_flags |= s3_flag_mode;

    rc = sqlite3_open_v2(path.c_str(), & dbh_native, s3_flags, NULL);

	if (rc != SQLITE_OK) {
		if (!dbh_native) {
			Logger::error(_Tr("sqlite3_dbd: Unable to allocate memory for database handler."));
		} else {
			switch( rc ) {
				case SQLITE_CANTOPEN:
					Logger::error(_Fr("sqlite3_dbd: %s. Try to check path '%s'")
						% sqlite3_errmsg(dbh_native)
						% path);
					break;
				default: break;
			}
			sqlite3_close(dbh_native);
			dbh_native = nullptr;
		}
	} else {
		CWT_ASSERT(__dbd);
		dbh = new Sqlite3DbHandler;

		// TODO for what this call ?
		sqlite3_busy_timeout(dbh_native, __MAX_SQL_TIMEOUT);

		// Enable extended result codes
		sqlite3_extended_result_codes(dbh_native, 1);

		dbh->driver = __dbd;
		dbh->dbh_native = dbh_native;
	}

	++__refs;

    return reinterpret_cast<DbHandlerData*>(dbh);
}


static void s3_dbd_close (DbHandlerData * dbh)
{
	//sqlite3_stmt * stmt;
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler*>(dbh);

	if (!dbh )
		return;

/*
	while ((stmt = sqlite3_next_stmt(s3_dbh->dbh_native, 0)) != 0)
		sqlite3_finalize(stmt);
*/

	sqlite3_close_v2(s3_dbh->dbh_native);
	s3_dbh->driver = nullptr;
	s3_dbh->dbh_native = nullptr;
	delete s3_dbh;

	--__refs;

	CWT_ASSERT(__dbd);
	CWT_ASSERT(__refs >= 0);
	if (!__refs) {
		delete __dbd;
		__dbd = nullptr;
		__refs = 0;
	}
}

bool s3_dbd_set_auto_commit (DbHandlerData & , bool )
{
	return true;
}

bool s3_dbd_auto_commit(DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	return 0 == sqlite3_get_autocommit(s3_dbh->dbh_native)
			? false
			: true;
}

long_t s3_dbd_errno (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	int rc = sqlite3_errcode(s3_dbh->dbh_native);
	if (rc == SQLITE_OK
			|| rc == SQLITE_DONE
			|| rc == SQLITE_ROW)
		rc = 0;
	return long_t(rc);
}

String s3_dbd_strerror (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	return String(sqlite3_errmsg(s3_dbh->dbh_native));
}

bool s3_dbd_query (DbHandlerData & dbh, const String & sql)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);

	int rc = sqlite3_exec(s3_dbh->dbh_native, sql.c_str(), NULL, NULL, NULL);

	if (SQLITE_OK != rc) {
		return false;
	}

	return true;
}


DbStatementData * s3_dbd_prepare(DbHandlerData & dbh, const String & sql)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	sqlite3_stmt * sth_native;

	int rc = sqlite3_prepare_v2(
			s3_dbh->dbh_native
			, sql.c_str()      /* SQL statement, UTF-8 encoded */
			, -1               /* Maximum length of zSql in bytes. */
			, & sth_native     /* OUT: Statement handle */
			, NULL );          /* OUT: Pointer to unused portion of zSql */

	if (rc != SQLITE_OK) {
		return nullptr;
	}

	Sqlite3DbStatement * sth = new Sqlite3DbStatement;
	sth->sth_native = sth_native;

	int column_count = sqlite3_column_count(sth->sth_native);

	if (column_count > 0) { /* SELECT statement */
/*
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
*/
	}

	return dynamic_cast<DbStatementData*>(sth);
}


ulong_t s3_dbd_affected_rows (DbHandlerData & dbh)
{
	Sqlite3DbHandler * s3_dbh = reinterpret_cast<Sqlite3DbHandler *>(& dbh);
	int nrows = sqlite3_changes(s3_dbh->dbh_native);
	return nrows < 0 ? ulong_t(0) : ulong_t(nrows);
}


bool s3_dbd_begin (DbHandlerData & dbh)
{
	return s3_dbd_query(dbh, "BEGIN");
}

bool s3_dbd_commit (DbHandlerData & dbh)
{
	return s3_dbd_query(dbh, "COMMIT");
}

bool s3_dbd_rollback (DbHandlerData & dbh)
{
	return s3_dbd_query(dbh, "ROLLBACK");
}

void s3_dbd_stmt_close (DbStatementData * sth)
{
	if (sth)
		return;

	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(sth);

	int rc = sqlite3_finalize(s3_sth->sth_native);
	if (rc != SQLITE_OK) {
		Logger::error(_Tr("Failed to close statement"));
	}
	s3_sth->sth_native = nullptr;
	delete s3_sth;
}

bool s3_dbd_stmt_exec (DbStatementData & sth)
{
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);
	int rc = sqlite3_reset(s3_sth->sth_native);
	return rc == SQLITE_OK ? true : false;
}

static int __fetch_helper (Sqlite3DbStatement * s3_sth)
{
	int rc = SQLITE_OK;
	int nretries = __MAX_EXEC_RETRY_COUNT;

	do {
		rc = sqlite3_step(s3_sth->sth_native);

		if( rc == SQLITE_BUSY ) {
			if( nretries++ > __MAX_EXEC_RETRY_COUNT ) {
				break;
			} else {
				sqlite3_sleep(__MAX_EXEC_RETRY_SLEEP);
			}
		}
	} while (rc == SQLITE_BUSY);

	if (rc == SQLITE_DONE) {
		; // ok
	} else if (rc == SQLITE_ROW) {
		; // ok
	}

	return rc;
}

Vector<UniType> s3_dbd_stmt_fetch_row_array (DbStatementData & sth)
{
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);
	int rc = __fetch_helper(s3_sth);
	Vector<UniType> r;

	if (rc == SQLITE_ROW) {
		int ncols = sqlite3_column_count(s3_sth->sth_native);
		for (int i = 0; i < ncols; ++i) {
			//String column_name (sqlite3_column_name(s3_sth->sth_native, i));

			switch (sqlite3_column_type(s3_sth->sth_native, i)) {
			case SQLITE_INTEGER:
				r.append(UniType(sqlite3_column_int64(s3_sth->sth_native, i)));
				break;
			case SQLITE_FLOAT:
				r.append(UniType(sqlite3_column_double(s3_sth->sth_native, i)));
				break;
			case SQLITE_TEXT:
				r.append(UniType(sqlite3_column_text(s3_sth->sth_native, i)));
				break;
			case SQLITE_BLOB: {
				const char * bytes = (const char *)sqlite3_column_blob(s3_sth->sth_native, i);
				int nbytes = sqlite3_column_bytes(s3_sth->sth_native, i);
				CWT_ASSERT(nbytes >= 0);
				r.append(UniType(ByteArray(bytes, size_t(nbytes))));
				}
				break;
			case SQLITE_NULL:
			default:
				r.append(UniType());
				break;
			}
		}
	}

	return r;
}

Hash<String, UniType>  s3_dbd_stmt_fetch_row_hash (DbStatementData & sth)
{
	Sqlite3DbStatement * s3_sth = reinterpret_cast<Sqlite3DbStatement*>(& sth);
	int rc = __fetch_helper(s3_sth);
	Hash<String, UniType> r;

	if (rc == SQLITE_ROW) {
		int ncols = sqlite3_column_count(s3_sth->sth_native);
		for (int i = 0; i < ncols; ++i) {
			String column_name (sqlite3_column_name(s3_sth->sth_native, i));

			switch (sqlite3_column_type(s3_sth->sth_native, i)) {
			case SQLITE_INTEGER:
				r.insert(column_name, UniType(sqlite3_column_int64(s3_sth->sth_native, i)));
				break;
			case SQLITE_FLOAT:
				r.insert(column_name, UniType(sqlite3_column_double(s3_sth->sth_native, i)));
				break;
			case SQLITE_TEXT:
				r.insert(column_name, UniType(sqlite3_column_text(s3_sth->sth_native, i)));
				break;
			case SQLITE_BLOB: {
				const char * bytes = (const char *)sqlite3_column_blob(s3_sth->sth_native, i);
				int nbytes = sqlite3_column_bytes(s3_sth->sth_native, i);
				CWT_ASSERT(nbytes >= 0);
				r.insert(column_name, UniType(ByteArray(bytes, size_t(nbytes))));
				}
				break;
			case SQLITE_NULL:
			default:
				r.insert(column_name, UniType());
				break;
			}
		}
	}

	return r;
}

#ifdef __COMMENT_
static BOOL s3_dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CWT_UNUSED3(dbh, func_name, argv);
	return FALSE;
}

static void s3_dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CWT_UNUSED3(dbh, attr_name, attr_value);
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
#endif
CWT_NS_END
