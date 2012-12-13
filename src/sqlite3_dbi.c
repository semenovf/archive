/**
 * @file   sqlite3_dbi.c
 * @author wladt
 * @date   Nov 28, 2012 9:59:57 AM
 *
 * @brief
 */


#include <cwt/txtcodec.h>
#include <cwt/dbi/dbi.h>
#include <cwt/logger.h>
#include <cwt/strlist.h>
#include <cwt/string.h>
#include "sqlite3/sqlite3.h"
/*#include <cwt/hashtab.h>*/

#define _LOG_PREFIX _T("sqlite3: ")
#define _DBH(dbh)  (((CwtSqliteDBHandler*)(dbh))->dbh_native)
#define _STH(sth)  (((CwtSqliteStatement*)(sth))->stmt_native)

#define _MAX_SQL_TIMEOUT 10000 // 10 seconds

typedef struct _CwtSqliteStatement {
	CwtStatement __base;
	sqlite3_stmt *stmt_native;
} CwtSqliteStatement;


typedef struct _CwtSqliteDBHandler
{
	CwtDBHandler __base;
	sqlite3     *dbh_native;
	CwtString    errorstr;
	CWT_CHAR    *csname;      /* character set name */
} CwtSqliteDBHandler;


/*
typedef struct _CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;
*/

static CwtStrNS       *__str_ns = NULL;
static CwtStringNS    *__string_ns = NULL;
static CwtUniTypeNS   *__ut_ns = NULL;

/* DBI API functions implementations */
CwtDBHandler*           sqlite3_dbd_connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void             sqlite3_dbd_disconnect(CwtDBHandler *dbh);
static BOOL             sqlite3_dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void             sqlite3_dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL             sqlite3_dbd_set_auto_commit (CwtDBHandler *dbh, BOOL on);
static BOOL             sqlite3_dbd_auto_commit     (CwtDBHandler *dbh);
static CwtDBI_RC        sqlite3_dbd_err             (CwtDBHandler *dbh);
static const CWT_CHAR*  sqlite3_dbd_strerror        (CwtDBHandler *dbh);
static const CWT_CHAR*  sqlite3_dbd_state           (CwtDBHandler *dbh);
static BOOL             sqlite3_dbd_query           (CwtDBHandler *dbh, const CWT_CHAR *sql);
static CwtStatement*    sqlite3_dbd_prepare         (CwtDBHandler *dbh, const CWT_CHAR *statement);
static ULONGLONG        sqlite3_dbd_affected_rows   (CwtDBHandler *dbh);
static BOOL             sqlite3_dbd_tables          (CwtDBHandler *dbh, CwtStrList *tables);
static BOOL             sqlite3_dbd_table_exists    (CwtDBHandler*, const CWT_CHAR *tname);
static char*            sqlite3_dbd_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n);
static CWT_CHAR*        sqlite3_dbd_decode_n(CwtDBHandler *dbh, const char *s, size_t n);
static char*            sqlite3_dbd_encode(CwtDBHandler *dbh, const CWT_CHAR *s) { return sqlite_dbd_encode_n(dbh, s, __str_ns->strLen(s)); }
static CWT_CHAR*        sqlite3_dbd_decode(CwtDBHandler *dbh, const char *s) { return sqlite_dbd_decode_n(dbh, s, strlen(s)); }

/*
static BOOL             __dbd_begin(CwtDBHandler *dbh);
static BOOL             __dbd_commit(CwtDBHandler *dbh);
static BOOL             __dbd_rollback(CwtDBHandler *dbh);

extern CwtStrList*      __dbd_specForDeploy(CwtDDI *ddi, int flags);
extern CwtStrList*      __dbd_specForRecall(CwtDDI *ddi, int flags);
*/

/*
static void             __stmt_close       (CwtStatement *sth);
static BOOL             __stmt_execute     (CwtStatement *sth);
static ULONGLONG        __stmt_lastId      (CwtStatement *sth);
static ULONGLONG        __stmt_affectedRows(CwtStatement *sth);
static ULONGLONG        __stmt_numRows     (CwtStatement*);
static BOOL             __stmt_fetchNext   (CwtStatement*);
static BOOL             __stmt_fetchColumn (CwtStatement *sth, CWT_CHAR *col, CwtUniType *ut);
static CwtDBI_RC        __stmt_err         (CwtStatement *sth);
static const CWT_CHAR*  __stmt_strerror    (CwtStatement *sth);
static BOOL             __stmt_bindByIndex (CwtStatement *sth, size_t index, CwtUniType *ut);
static inline size_t    __stmt_bindParmsCount(CwtStatement *sth) { CWT_ASSERT(sth); return ((CwtMySqlStatement*)sth)->nbind_params; }
static BOOL             __stmt_setParm     (CwtStatement *sth, CwtUniType *ut, const void *copy, size_t sz);
*/

/* local helper functions */
static void             __destroy(CwtSqliteDBHandler *sdbh);
/*
static BOOL             __mysql_isBlob(int mysqltype);
static BOOL             __mysql_isInteger(int mysqltype);
static BOOL             __mysql_isTime(int mysqltype);
static enum enum_field_types
                        __mysql_toMysqlType(CwtTypeEnum cwtType);
static CwtTypeEnum      __mysql_fromMysqlType( enum enum_field_types mysqlType, UINT field_flags);
static BOOL             __mysql_isUnsigned(CwtTypeEnum cwtType);
static void             __mysql_stmtDestroy(CwtMySqlStatement *sth);
static BOOL             __mysql_buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[]);
static BOOL             __mysql_buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[]);
static int              __mysql_realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, size_t length);
*/

/*static BOOL __nConnections = 0;*/ /* number of connections */

static CwtDBIDriver __cwtDBIDriver = {
	  sqlite3_dbd_connect
	, sqlite3_dbd_disconnect
	, sqlite3_dbd_func
	, sqlite3_dbd_attr
	, sqlite3_dbd_set_auto_commit
	, sqlite3_dbd_auto_commit
	, sqlite3_dbd_err
	, sqlite3_dbd_strerror
	, sqlite3_dbd_state
	, sqlite3_dbd_query
	, sqlite3_dbd_prepare
	, sqlite3_dbd_affected_rows
	, sqlite3_dbd_tables
	, sqlite3_dbd_table_exists
	, sqlite3_dbd_encode_n
	, sqlite3_dbd_decode_n
	, sqlite3_dbd_encode
	, sqlite3_dbd_decode
	, NULL /*__dbd_begin            */
    , NULL /*__dbd_commit           */
    , NULL /*__dbd_rollback         */

    , NULL /*__dbd_specForDeploy    */
    , NULL /*__dbd_specForRecall    */
};

DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	if( !__str_ns ) {
		__str_ns    = cwt_str_ns();
		__string_ns = cwt_string_ns();
		__ut_ns     = cwt_unitype_ns();
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
CwtDBHandler* sqlite3_dbd_connect(const CWT_CHAR *driver_dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname)
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
		__string_ns->init(&dbh->errorstr);
		dbh->csname = __str_ns->strDup(csname);

		/* TODO for what this call ?*/
		sqlite3_busy_timeout(_DBH(dbh), _MAX_SQL_TIMEOUT);

		/*
			dbh->__base.close      = my_stmt_close;
			dbh->__base.execute    = my_stmt_execute;
			dbh->__base.lastId     = my_stmt_last_id;
			dbh->__base.err        = my_stmt_err;
			dbh->__base.strerror   = my_stmt_strerror;
			dbh->__base.bindByIndex     = my_stmt_bind_by_index;
			dbh->__base.bindParmsCount = my_stmt_bind_parms_count;
			dbh->__base.setParm    = my_stmt_set_parm;
			dbh->__base.rows       = my_stmt_affected_rows;
			dbh->__base.size       = my_stmt_num_rows;
			dbh->__base.fetchNext  = my_stmt_fetch_next;
			dbh->__base.fetchColumn= my_stmt_fetch_column;
			dbh->__base.driver     = cwtDBIDriverImpl;
			dbh->auto_commit       = FALSE;
			dbh->csname            = NULL;
			dbh->errorstr          = NULL;
			dbh->sqlstate          = NULL;
		*/
	}

#ifdef CWT_UNICODE
    CWT_FREE(path_utf8);
#endif
    __string_ns->destroy(&path);
    strlist_ns->destroy(&opts);

    return (CwtDBHandler*)dbh;
}


static void sqlite3_dbd_disconnect (CwtDBHandler *dbh)
{
	sqlite3_stmt* stmt;

	if (!dbh )
		return;

	while ((stmt = sqlite3_next_stmt(_DBH(dbh), 0)) != 0)
		sqlite3_finalize(stmt);

	sqlite3_close(_DBH(dbh));
	__destroy(dbh);
	CWT_FREE(dbh);
}


static BOOL sqlite3_dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CWT_UNUSED3(dbh, func_name, argv);
	return FALSE;
}

static void sqlite3_dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CWT_UNUSED3(dbh, attr_name, attr_value);
}

static BOOL sqlite3_dbd_set_auto_commit (CwtDBHandler *dbh, BOOL on)
{
	return TRUE;
}

static BOOL sqlite3_dbd_auto_commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return 0 == sqlite3_get_autocommit(_DBH(dbh))
			? FALSE
			: TRUE;
}

static CwtDBI_RC sqlite3_dbd_err(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return (CwtDBI_RC)sqlite3_errcode(_DBH(dbh));
}

static const CWT_CHAR* sqlite3_dbd_strerror(CwtDBHandler *dbh)
{
	CwtSqliteDBHandler *sdbh = (CwtSqliteDBHandler *)dbh;
	CWT_CHAR *s;
	char     *errstr;
	size_t    len;
	sqlite3_mutex *lock;

	CWT_ASSERT(sdbh);
	CWT_ASSERT(_DBH(sdbh));

	__string_ns->clear(&sdbh->errorstr);

	lock = sqlite3_db_mutex(_DBH(sdbh));
	sqlite3_mutex_enter(lock);

	errstr = sqlite3_errmsg(_DBH(sdbh));
	s = cwt_textcodec_ns()->fromUtf8(errstr, strlen(errstr));
	__string_ns->initWithBuffer(sdbh->errorstr, s, __str_ns->strLen(s)-1);
	sqlite3_mutex_leave(lock);

	return __string_ns->cstr(&sdbh->errorstr);
}


/**
 * @brief Returns a null-terminated string containing the SQLSTATE error code.
 *
 * @param dbh Database handle
 * @return a null-terminated string containing the SQLSTATE error code.
 */
static const CWT_CHAR*  sqlite3_dbd_state(CwtDBHandler *dbh)
{
	/* TODO could this be implemented? */
	return _T("00000");
}


static BOOL sqlite3_dbd_query (CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	int rc;
	char *sql_utf8;

	CWT_ASSERT(dbh);

	sql_utf8 = sqlite3_dbd_encode(dbh, sql);
	rc = sqlite3_exec(_DBH(dbh), sql_utf8, NULL, NULL, NULL);
	CWT_FREE(sql_utf8);

	return 0 == rc ? TRUE : FALSE;
}


static CwtStatement* sqlite3_dbd_prepare(CwtDBHandler *dbh, const CWT_CHAR *statement)
{
	sqlite3_stmt *stmt_native;
	CwtSqliteStatement *stmt;
	char *sql_utf8;
	int rc;

	sql_utf8 = cwt_textcodec_ns()->toUtf8(statement, __str_ns->strLen(statement));
	rc = sqlite3_prepare_v2(
			_DBH(dbh)       /* Database handle */
			, sql_utf8      /* SQL statement, UTF-8 encoded */
			, -1            /* Maximum length of zSql in bytes. */
			, &stmt_native  /* OUT: Statement handle */
			, NULL );       /* OUT: Pointer to unused portion of zSql */
	CWT_FREE(sql_utf8);

	if (rc != SQLITE_OK)
		return NULL;

	stmt = CWT_MALLOC(CwtSqliteStatement);
	stmt->stmt_native = stmt_native;

	return (CwtStatement *)stmt;
}

static ULONGLONG sqlite3_dbd_affected_rows (CwtDBHandler *dbh)
{
	int nrows;
	nrows = sqlite3_changes(_DBH(dbh));

	if (nrows < 0)
		return (ULONGLONG)0;

	return (ULONGLONG)nrows;
}

static BOOL sqlite3_dbd_tables (CwtDBHandler *dbh, CwtStrList *tables)
{
	CwtStrListNS *sl_ns  = cwt_strlist_ns();

	CwtSqliteDBHandler *sdbh = (CwtSqliteDBHandler*)dbh;

	CWT_ASSERT(dbh);
	CWT_ASSERT(_DBH(dbh));
	CWT_ASSERT(tables);

	res = mysql_list_tables(mdbh->conn, NULL);

	if( !res ) {
		cwt_logger_ns()->error(__LOG_PREFIX _Tr("failed to list tables: %s"), __cwtDBIDriver.strerror(dbh));
		return FALSE;
	}

	while( res ) {
		CWT_CHAR* decoded;

		mysql_data_seek(res, i);

		row = mysql_fetch_row(res);

		if( !row ) {
			break;
		}

		decoded = my_dbd_decode(dbh, row[0]);
		slNS->append( tables, decoded);

		i++;
	}

	mysql_free_result(res);

	return ok;
}

static BOOL sqlite3_dbd_table_exists (CwtDBHandler *dbh, const CWT_CHAR *tname)
{
	CwtString sql;
	CwtSqliteStatement *stmt;

	__string_ns->init(&sql);
	__string_ns->sprintf(&sql
			, _T("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = '%s'")
			, tname);


	__string_ns->destroy(&sql);

	stmt = sqlite3_dbd_prepare(dbh, __string_ns->cstr(&sql));


	stmt.exec();
	int c = stmt.column_int(0);
	return ( c > 0 );
}

static char* sqlite3_dbd_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n)
{
	CwtSqliteDBHandler *sdbh = (CwtSqliteDBHandler *)dbh;

	CWT_ASSERT(sdbh);
	CWT_ASSERT(_DBH(sdbh));

	return cwt_textcodec_ns()->toMBCS(s, sdbh->csname, n);
}

static CWT_CHAR* sqlite3_dbd_decode_n(CwtDBHandler *dbh, const char *s, size_t n)
{
	CwtSqliteDBHandler *sdbh = (CwtSqliteDBHandler *)dbh;

	CWT_ASSERT(sdbh);
	CWT_ASSERT(_DBH(sdbh));

	return cwt_textcodec_ns()->fromMBCS(s, sdbh->csname, n);
}


static void __destroy(CwtSqliteDBHandler *sdbh)
{
	CWT_ASSERT(sdbh);
	__string_ns->destroy(&sdbh->errorstr);
	CWT_FREE(sdbh->csname);
}

#ifdef __COMMENT__

#include <sqlite3.h>
#include <jq/Defs.hpp>
#include <jq/String.hpp>
#include <jq/DBI.hpp>


#define MAX_RETRY_COUNT	10
#define MAX_RETRY_SLEEP	1000 //  1 second
#define MAX_SQL_TIMEOUT 10000 // 10 seconds


extern "C" jq::SqlHandler* New( const jq::String& dsn );
extern "C" bool Free( jq::SqlHandler* );


class SqlValue : public jq::SqlValue
{
public:
	SqlValue() : type(jq::SqlNull) { data.blob_value = 0; }
	~SqlValue() { if(type == jq::SqlText && data.text_value ) delete data.text_value; }

	jq::SqlType type;
	union {
		int           int_value;
		double        float_value;
		jq::String*   text_value;
		void*         blob_value;
	} data;
};


#define CHECK_FOR_CONNECTION	if(!this->m_dbh) {                             \
	throw jq::SqlException(jq::DBI::SQL_DISCONNECTED, _Tr("database is disconnected")); }

#define CHECK_FOR_STATEMENT  	if(!this->m_stmt) {                            \
		throw jq::SqlException(jq::DBI::SQL_NULLPTR, _Tr("bad statement")); }

#define CHECK_FOR_RANGE(i)      if( (i) < 0 && (i) >= this->column_count() ) { \
		throw jq::SqlException(jq::DBI::SQL_RANGE, _Tr("column index is out of range")); }




class SqlHandler;

class SqlStatement : public jq::SqlStatement
{
	friend class SqlHandler;

protected:
	SqlStatement() {}

public:
	SqlStatement( sqlite3* dbh ) : m_dbh(dbh),  m_stmt(0) {}
	SqlStatement( sqlite3* dbh, sqlite3_stmt* stmt ): m_dbh(dbh), m_stmt(stmt) {}
	virtual ~SqlStatement() { finish(); }

	jq::SqlResult bind(int i, int val);
	jq::SqlResult bind(int i, double val);
	jq::SqlResult bind(int i, const jq::String& val);
	jq::SqlResult bind(int i, const void* val, int sz);

	int            column_int(int i);
	double         column_float(int i);
	jq::String     column_text(int i);
	const void*    column_blob(int i);
	jq::SqlValue*  column_value(int i, jq::SqlValue* val  = 0 );


	int            column_bytes(int i);
	jq::SqlType    column_type(int i);
	int            column_count();

	//static  SqlResult exec( const jq::String& stmt );
	bool exec();

	bool fetchRow();

	void reset() { CHECK_FOR_STATEMENT; sqlite3_reset(m_stmt); }

	void finish() { if(m_stmt) sqlite3_finalize(m_stmt); }
private:
	sqlite3* m_dbh;
	sqlite3_stmt* m_stmt;
};


class SqlHandler : public jq::SqlHandler
{
protected:


public:
	SqlHandler(): m_dbh(0) {}
	virtual ~SqlHandler();

	jq::SqlResult connect( const jq::String &dsn, bool create = false );
	jq::SqlResult disconnect();

	bool isConnected() const { return m_dbh != 0 ? true : false; }
	virtual jq::StringList tables() { return jq::StringList(); }
	bool tableExists( const jq::String &name );

	bool info( jq::String* dsn, jq::String* dbname );

	/**
	 * Prepares the statement
	 */
	jq::SqlStatement* prepare( const jq::String &stmt );
	jq::SqlResult prepare( const jq::String &stmt, jq::SqlStatement* st );

	/**
	 * Starts the transaction
	 */
	bool begin();

	/**
	 * Ends the transaction
	 */
	bool end();

	/**
	 * Commits the transaction
	 */
	bool commit();

	/**
	 * Rollbacks (undo) the transacion
	 */
	bool rollback();

	jq::SqlResult exec(const jq::String &sql);

	void free( jq::SqlStatement* stmt );

private:
	sqlite3* m_dbh;       // Sqlite3 database handler
	jq::String m_path;  // Path to the database
};




jq::SqlResult SqlStatement::bind(int i, int val)
{
	CHECK_FOR_STATEMENT;

	int rc = sqlite3_bind_int(m_stmt, i, val);
	if( rc != SQLITE_OK )
		throw jq::SqlException(SQL_ERROR(rc), _Tr("bind integer failed"));

	return jq::DBI::SQL_SUCCESS;
}

jq::SqlResult SqlStatement::bind(int i, double val)
{
	CHECK_FOR_STATEMENT;

	int rc = sqlite3_bind_double(m_stmt, i, val);
	if( rc != SQLITE_OK )
		throw jq::SqlException(SQL_ERROR(rc), _Tr("bind double failed"));

	return jq::DBI::SQL_SUCCESS;
}

jq::SqlResult SqlStatement::bind(int i, const jq::String& val)
{
	CHECK_FOR_STATEMENT;

	int rc = sqlite3_bind_text( m_stmt, i, (const char*)val, val.length(), 0 );
	if( rc != SQLITE_OK )
		throw jq::SqlException(SQL_ERROR(rc), _Tr("bind string failed"));

	return jq::DBI::SQL_SUCCESS;
}

jq::SqlResult SqlStatement::bind(int i, const void* val, int sz )
{
	CHECK_FOR_STATEMENT;

	int rc = sqlite3_bind_blob( m_stmt, i, val, sz, 0 );
	if( rc != SQLITE_OK )
		throw jq::SqlException(SQL_ERROR(rc), _Tr("bind blob failed"));

	return jq::DBI::SQL_SUCCESS;
}


int SqlStatement::column_int(int i)
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);
	return sqlite3_column_int(m_stmt, i);
}

double SqlStatement::column_float(int i)
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);
	return sqlite3_column_double(m_stmt, i);
}

jq::String SqlStatement::column_text(int i)
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);
	return jq::String((char*)sqlite3_column_text(m_stmt, i));
}

const void* SqlStatement::column_blob(int i)
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);
	return sqlite3_column_blob(m_stmt, i);
}


jq::SqlValue* SqlStatement::column_value(int i, jq::SqlValue* val )
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);

	SqlValue* v;

	if( !val ) {
		v = new SqlValue();
	} else {
		v = dynamic_cast<SqlValue*>(val);
	}

	v->type = column_type(i);
	switch( v->type ) {
		case jq::SqlInt:     v->data.int_value   = column_int(i);   break;
		case jq::SqlFloat:   v->data.float_value = column_float(i); break;
		case jq::SqlText:    v->data.text_value  = new jq::String(column_text(i));  break;
		case jq::SqlBlob:    v->data.blob_value  = const_cast<void*>(column_blob(i));  break;
		case jq::SqlNull:
		default: v->data.blob_value  = 0; break;
	}
	return dynamic_cast<jq::SqlValue*>(v);
}

int SqlStatement::column_bytes(int i)
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);
	return sqlite3_column_bytes(m_stmt, i);
}

jq::SqlType SqlStatement::column_type(int i)
{
	CHECK_FOR_STATEMENT;
	CHECK_FOR_RANGE(i);

	switch(sqlite3_column_type(m_stmt, i)) {
		case SQLITE_INTEGER: return jq::SqlInt;
		case SQLITE_FLOAT:   return jq::SqlFloat;
		case SQLITE_TEXT:    return jq::SqlText;
		case SQLITE_BLOB:    return jq::SqlBlob;
		case SQLITE_NULL:    return jq::SqlNull;
	}

	return jq::SqlUnknown;
}


int SqlStatement::column_count()
{
	CHECK_FOR_STATEMENT;
	return sqlite3_column_count(m_stmt);
}


bool SqlStatement::exec()
{
	CHECK_FOR_STATEMENT;
	int rc;
	int nretries = MAX_RETRY_COUNT;

	sqlite3_reset( m_stmt );
	do {
		rc = sqlite3_step( m_stmt );

		if( rc == SQLITE_BUSY ) {
			if( nretries++ > MAX_RETRY_COUNT ) {
				break;
			} else {
				sqlite3_sleep(MAX_RETRY_SLEEP);
			}
		}
	} while ( rc == SQLITE_BUSY );

	if( rc == SQLITE_DONE ) {
		return true;
	}
	if( rc == SQLITE_ROW ) {
		return true;
	}
	throw jq::SqlException(SQL_ERROR(rc), sqlite3_errmsg(m_dbh));
	return false;
}


bool SqlStatement::fetchRow()
{
	CHECK_FOR_STATEMENT;

	int rc;
	int nretries = MAX_RETRY_COUNT;

	do {
		rc = sqlite3_step( m_stmt );

		if( rc == SQLITE_BUSY ) {
			if( nretries++ > MAX_RETRY_COUNT ) {
				break;
			} else {
				sqlite3_sleep(MAX_RETRY_SLEEP);
			}
		}
	} while ( rc == SQLITE_BUSY );

	if( rc == SQLITE_ROW ) {
		return true;
	}

	if( rc == SQLITE_DONE ) {
		return false;
	}

	throw jq::SqlException(SQL_ERROR(rc), sqlite3_errmsg(m_dbh));
	return false;
}


SqlHandler::~SqlHandler()
{
	disconnect();
/*	if( m_hdlib ) {
		dlclose(m_hdlib);
		m_hdlib = 0;
	}
*/}



jq::SqlStatement* SqlHandler::prepare( const jq::String &stmt )
{
	CHECK_FOR_CONNECTION;

	SqlStatement* st = new SqlStatement(m_dbh);
	this->prepare(stmt, st);
	return dynamic_cast<jq::SqlStatement*>(st);
}


jq::SqlResult SqlHandler::prepare( const jq::String &sql, jq::SqlStatement* st )
{
	CHECK_FOR_CONNECTION;
	if( !st ) {
		throw jq::SqlException(jq::DBI::SQL_NULLPTR, _Tr("statement is null"));
	}

	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare_v2( m_dbh, sql, -1, &stmt, 0 );
	if( rc != SQLITE_OK ) {
		throw jq::SqlException(SQL_ERROR(rc), sqlite3_errmsg( m_dbh ));
	}

	SqlStatement* _st = dynamic_cast<SqlStatement*>(st);
	_st->finish();
	_st->m_dbh = m_dbh;
	_st->m_stmt = stmt;
	return jq::DBI::SQL_SUCCESS;
}



bool SqlHandler::tableExists( const jq::String &name )
{
	CHECK_FOR_CONNECTION;

	jq::String sql;
	sql.sprintf("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = '%s'", (const char*)name);
	SqlStatement stmt(m_dbh);
	prepare(sql, &stmt);
	stmt.exec();
	int c = stmt.column_int(0);
	return ( c > 0 );
}


bool SqlHandler::info( jq::String* dsn, jq::String* dbname )
{
	if( dsn ) *dsn = m_path;
	if( dbname) *dbname = m_path;
	return true;
}

static bool __transaction_stage(sqlite3* dbh, const char* sql )
{
    char* errstr;
	int rc = sqlite3_exec(dbh, sql, 0, 0, &errstr);
	if( rc != SQLITE_OK ) {
		jq::String ex;
		ex.sprintf("transaction %s failed: %s", sql, errstr);
		sqlite3_free(errstr);
		throw jq::SqlException(SQL_ERROR(rc), ex);
	}
	sqlite3_free(errstr);
	return true;
}


bool SqlHandler::begin()
{
	CHECK_FOR_CONNECTION;
	return __transaction_stage(m_dbh, "BEGIN" );
}


bool SqlHandler::end()
{
	CHECK_FOR_CONNECTION;
	return __transaction_stage(m_dbh, "END" );}


bool SqlHandler::commit()
{
	CHECK_FOR_CONNECTION;
	return __transaction_stage(m_dbh, "COMMIT" );}


bool SqlHandler::rollback()
{
	CHECK_FOR_CONNECTION;
	return __transaction_stage(m_dbh, "ROLLBACK" );
}


jq::SqlResult SqlHandler::exec(const jq::String &sql)
{
	char* errstr;
	int rc = sqlite3_exec( m_dbh, sql, 0, 0, &errstr );
	if( rc != SQLITE_OK ) {
		jq::String ex;
		ex.sprintf(_Tr("sql execution failed: result: %d: %s"), rc, errstr);
		sqlite3_free(errstr);
		throw jq::SqlException(SQL_ERROR(rc), ex);
	}
	sqlite3_free(errstr);
	return jq::DBI::SQL_SUCCESS;
}

void SqlHandler::free( jq::SqlStatement *stmt )
{
	if( stmt )
		delete dynamic_cast<SqlStatement*>(stmt);
}


jq::SqlHandler* New( const jq::String &dsn )
{
	SqlHandler* dbh = new SqlHandler();
	dbh->connect(dsn, true);
//	DEBUG("connect(): DSN: %s", (const char*)dsn);
	return dynamic_cast<jq::SqlHandler*>(dbh);
}

bool Free( jq::SqlHandler *dbh )
{
	if( dbh ) {
		dbh->disconnect();
		delete dynamic_cast<SqlHandler*>(dbh);
	}
	return true;
}
#endif
