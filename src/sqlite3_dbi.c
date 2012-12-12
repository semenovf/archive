/**
 * @file   sqlite3_dbi.c
 * @author wladt
 * @date   Nov 28, 2012 9:59:57 AM
 *
 * @brief
 */


#include <cwt/txtcodec.h>
#include <cwt/dbi/dbi.h>
/*#include <cwt/hashtab.h>*/
/*
#include <cwt/logger.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
*/


#define __LOG_PREFIX _T("sqlite3: ")
/*
#define __DBH(dbh)  (((CwtMySqlDBHandler*)(dbh))->conn)
#define __STH(sth)  (((CwtMySqlStatement*)(sth))->stmt)
*/

typedef struct _CwtSqliteStatement {
	CwtStatement __base;
} CwtSqliteStatement;


typedef struct _CwtSqliteDBHandler
{
	CwtDBHandler __base;
} CwtSqliteDBHandler;


/*
typedef struct _CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;
*/

static CwtStrNS       *__strNS = NULL;
/*static CwtHashTableNS *__htNS = NULL;*/
static CwtUniTypeNS   *__utNS = NULL;

/* DBI API functions implementations */
/*
CwtDBHandler*           __dbd_connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void             __dbd_disconnect(CwtDBHandler *dbh);
static BOOL             __dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void             __dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL             __dbd_setAutoCommit(CwtDBHandler *dbh, BOOL on);
static BOOL             __dbd_autoCommit(CwtDBHandler *dbh);
static CwtDBI_RC        __dbd_err(CwtDBHandler *dbh);
static const CWT_CHAR*  __dbd_strerror(CwtDBHandler *dbh);
static const CWT_CHAR*  __dbd_state(CwtDBHandler *dbh);
static BOOL             __dbd_query(CwtDBHandler *dbh, const CWT_CHAR *sql);
static BOOL             __dbd_queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length);
static CwtStatement*    __dbd_prepare(CwtDBHandler *dbh, const CWT_CHAR *statement);
static ULONGLONG        __dbd_affectedRows(CwtDBHandler *dbh);
static BOOL             __dbd_tables(CwtDBHandler *dbh, CwtStrList *tables);
static BOOL             __dbd_tableExists(CwtDBHandler*, const CWT_CHAR *tname);
static char*            __dbd_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n);
static CWT_CHAR*        __dbd_decode_n(CwtDBHandler *dbh, const char *s, size_t n);
static char*            __dbd_encode(CwtDBHandler *dbh, const CWT_CHAR *s) { return __dbd_encode_n(dbh, s, __strNS->strLen(s)); }
static CWT_CHAR*        __dbd_decode(CwtDBHandler *dbh, const char *s) { return __dbd_decode_n(dbh, s, strlen(s)); }
*/

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
/*
static BOOL             __mysql_isBlob(int mysqltype);
static BOOL             __mysql_isInteger(int mysqltype);
static BOOL             __mysql_isTime(int mysqltype);
static enum enum_field_types
                        __mysql_toMysqlType(CwtTypeEnum cwtType);
static CwtTypeEnum      __mysql_fromMysqlType( enum enum_field_types mysqlType, UINT field_flags);
static BOOL             __mysql_isUnsigned(CwtTypeEnum cwtType);
static void             __mysql_destroy(CwtMySqlDBHandler *dbh);
static void             __mysql_stmtDestroy(CwtMySqlStatement *sth);
static BOOL             __mysql_buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[]);
static BOOL             __mysql_buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[]);
static int              __mysql_realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, size_t length);
*/

/*static BOOL __nConnections = 0;*/ /* number of connections */

static CwtDBIDriver __cwtDBIDriver = {
	  NULL /*__dbd_connect          */
	, NULL /*__dbd_disconnect       */
	, NULL /*__dbd_func             */
	, NULL /*__dbd_attr             */
	, NULL /*__dbd_setAutoCommit    */
	, NULL /*__dbd_autoCommit       */
	, NULL /*__dbd_err              */
	, NULL /*__dbd_strerror         */
	, NULL /*__dbd_state            */
	, NULL /*__dbd_query            */
	, NULL /*__dbd_queryBin         */
	, NULL /*__dbd_prepare          */
	, NULL /*__dbd_affectedRows     */
	, NULL /*__dbd_tables           */
	, NULL /*__dbd_tableExists      */
	, NULL /*__dbd_encode_n         */
	, NULL /*__dbd_decode_n         */
	, NULL /*__dbd_encode           */
	, NULL /*__dbd_decode           */
	, NULL /*__dbd_begin            */
    , NULL /*__dbd_commit           */
    , NULL /*__dbd_rollback         */

    , NULL /*__dbd_specForDeploy    */
    , NULL /*__dbd_specForRecall    */
};

DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	if( !__strNS ) {
		__strNS = cwt_str_ns();
		/*__htNS  = cwtHashTableNS();*/
		__utNS  = cwt_unitype_ns();
	}
	return &__cwtDBIDriver;
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


jq::SqlResult SqlHandler::connect( const jq::String &dsn, bool create )
{
	// Close already connected data base
	if( isConnected() ) {
		if( m_path == dsn )
			return jq::DBI::SQL_SUCCESS;
		sqlite3_close(m_dbh);
	}

	m_path = dsn;
	int rc;
	if( create )
		rc = sqlite3_open_v2( m_path, &m_dbh, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0 );
	else
		rc = sqlite3_open_v2( m_path, &m_dbh, SQLITE_OPEN_READWRITE, 0 );

	if( rc != SQLITE_OK ) {
		if( !m_dbh ) {
			throw jq::SqlException(jq::DBI::SQL_NOMEM, _Tr("unable to allocate memory for database handler"));
		} else {
			jq::String strerr = sqlite3_errmsg( m_dbh );
			switch( rc ) {
				case SQLITE_CANTOPEN:
					strerr += _Tr(". Try to check path: ");
					strerr += m_path;
				default: break;
			}
			sqlite3_close(m_dbh);
			m_dbh = 0;
			throw jq::SqlException(SQL_ERROR(rc), strerr);
		}
	}
	sqlite3_busy_timeout( m_dbh, MAX_SQL_TIMEOUT);
	return jq::DBI::SQL_SUCCESS;
}


jq::SqlResult SqlHandler::disconnect()
{
	sqlite3_stmt* stmt;

	if( m_dbh ) {
		while( (stmt = sqlite3_next_stmt(m_dbh, 0)) != 0 )
			sqlite3_finalize(stmt);

		sqlite3_close(m_dbh);
		m_dbh = 0;
	}
	return jq::DBI::SQL_SUCCESS;
}


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
