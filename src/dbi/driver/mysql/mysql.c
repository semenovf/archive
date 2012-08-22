/*
 * mysql.c
 *
 *  Created on: 03.07.2012
 *      Author: wladt
 */


#include <mysql/my_global.h>
/*to avoid warning C4005: 'ETIMEDOUT' : macro redefinition*/
#ifdef ETIMEDOUT
#	undef ETIMEDOUT
#endif
#include <mysql/my_sys.h>
#include <mysql/mysql.h>

/* Macros INT8 INT16 and INT32 defined in my_global.h are conflicts with same data types
 * Avoid it.
 * */
#if defined(INT8)
#	undef INT8
#endif

#if defined(INT16)
#	undef INT16
#endif

#if defined(INT32)
#	undef INT32
#endif

#include <cwt/txtcodec.h>
#include <cwt/dbi/dbi.h>
#include <cwt/hashtab.h>
#include <cwt/logger.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
/*#include <cwt/str.h>*/
/*#include <string.h>*/


#define __LOG_PREFIX _T("mysql: ")
#define __DBH(dbh)  (((CwtMySqlDBHandler*)(dbh))->conn)
#define __STH(sth)  (((CwtMySqlStatement*)(sth))->stmt)

typedef struct _CwtMySqlStatement {
	CwtStatement __base;
	MYSQL_STMT   *stmt;
	MYSQL_BIND   *bind_params;
	my_bool      *is_null;
	size_t        nbind_params;
	BOOL          is_bind; /* TRUE if parameters already bind for output */

	MYSQL_RES    *meta;
	MYSQL_BIND   *rbind_params;
	size_t        nrbind_params;
	CwtHashTable *rbind_map;
	BOOL          is_rbind;
	CwtHashTable *meta_map;

	CWT_CHAR     *errorstr;
} CwtMySqlStatement;


typedef struct _CwtMySqlDBHandler
{
	CwtDBHandler __base;
	MYSQL       *conn;
	BOOL         auto_commit;
	CWT_CHAR    *csname;      /* character set name */
	CWT_CHAR    *errorstr;
	CWT_CHAR    *sqlstate;
} CwtMySqlDBHandler;


typedef struct _CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;

static CwtStrNS       *__strNS = NULL;
static CwtHashTableNS *__htNS = NULL;
static CwtUniTypeNS   *__utNS = NULL;

/* DBI API functions implementations */
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
static char*            __dbd_encode(CwtDBHandler *dbh, const CWT_CHAR *s) { return __dbd_encode_n(dbh, s, __strNS->strlen(s)); }
static CWT_CHAR*        __dbd_decode(CwtDBHandler *dbh, const char *s) { return __dbd_decode_n(dbh, s, strlen(s)); }

static BOOL             __dbd_begin(CwtDBHandler *dbh);
static BOOL             __dbd_commit(CwtDBHandler *dbh);
static BOOL             __dbd_rollback(CwtDBHandler *dbh);

extern CwtStrList*      __dbd_specForDeploy(CwtDDI *ddi, int flags);
extern CwtStrList*      __dbd_specForRecall(CwtDDI *ddi, int flags);

static void             __stmt_close       (CwtStatement *sth);
static BOOL             __stmt_execute     (CwtStatement *sth);
static ULONGLONG        __stmt_affectedRows(CwtStatement *sth);
static ULONGLONG        __stmt_numRows     (CwtStatement*);
static BOOL             __stmt_fetchNext   (CwtStatement*);
static BOOL             __stmt_fetchColumn (CwtStatement *sth, CWT_CHAR *col, CwtUniType *ut);
static CwtDBI_RC        __stmt_err         (CwtStatement *sth);
static const CWT_CHAR*  __stmt_strerror    (CwtStatement *sth);
static BOOL             __stmt_bindByIndex (CwtStatement *sth, size_t index, CwtUniType *ut);
static inline size_t    __stmt_bindParmsCount(CwtStatement *sth) { CWT_ASSERT(sth); return ((CwtMySqlStatement*)sth)->nbind_params; }
static BOOL             __stmt_setParm     (CwtStatement *sth, CwtUniType *ut, const void *copy, size_t sz);

/* local helper functions */
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

static BOOL __nConnections = 0; /* number of connections */

static CwtDBIDriver __cwtDBIDriver = {
	  __dbd_connect
	, __dbd_disconnect
	, __dbd_func
	, __dbd_attr
	, __dbd_setAutoCommit
	, __dbd_autoCommit
	, __dbd_err
	, __dbd_strerror
	, __dbd_state
	, __dbd_query
	, __dbd_queryBin
	, __dbd_prepare
	, __dbd_affectedRows
	, __dbd_tables
	, __dbd_tableExists
	, __dbd_encode_n
	, __dbd_decode_n
	, __dbd_encode
	, __dbd_decode
	, __dbd_begin
    , __dbd_commit
    , __dbd_rollback

    , __dbd_specForDeploy
    , __dbd_specForRecall
};

DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	if( !__strNS ) {
		__strNS = cwtStrNS();
		__htNS  = cwtHashTableNS();
		__utNS  = cwtUniTypeNS();
	}
	return &__cwtDBIDriver;
}


static BOOL __mysql_isBlob(int t)
{
	return (t == MYSQL_TYPE_TINY_BLOB
		   || t == MYSQL_TYPE_BLOB
		   || t == MYSQL_TYPE_MEDIUM_BLOB
		   || t == MYSQL_TYPE_LONG_BLOB) ? TRUE : FALSE;
}

static BOOL __mysql_isInteger(int t)
{
	return (t == MYSQL_TYPE_TINY
		   || t == MYSQL_TYPE_SHORT
		   || t == MYSQL_TYPE_LONG
		   || t == MYSQL_TYPE_LONGLONG
		   || t == MYSQL_TYPE_INT24) ? TRUE : FALSE;
}

static BOOL __mysql_isTime(int t)
{
	return ( t == MYSQL_TYPE_TIMESTAMP
			|| t == MYSQL_TYPE_DATE
			|| t == MYSQL_TYPE_TIME
			|| t == MYSQL_TYPE_DATETIME
			|| t == MYSQL_TYPE_NEWDATE) ? TRUE : FALSE;
}


static enum enum_field_types __mysql_toMysqlType(CwtTypeEnum cwtType)
{
	enum enum_field_types mysqlType;

	switch( cwtType ) {
	case CwtType_BOOL:
	case CwtType_CHAR:
	case CwtType_SBYTE:
	case CwtType_BYTE:
		mysqlType = MYSQL_TYPE_TINY;
		break;
	case CwtType_SHORT:
	case CwtType_USHORT:
		mysqlType = MYSQL_TYPE_SHORT;
		break;
	case CwtType_INT:
	case CwtType_UINT:
	case CwtType_LONG:
	case CwtType_ULONG:
		mysqlType = MYSQL_TYPE_LONG;
		break;

	case CwtType_LONGLONG:
	case CwtType_ULONGLONG:
		mysqlType = MYSQL_TYPE_LONGLONG;
		break;

	case CwtType_FLOAT:
		mysqlType = MYSQL_TYPE_FLOAT;
		break;

	case CwtType_DOUBLE:
		mysqlType = MYSQL_TYPE_DOUBLE;
		break;

	case CwtType_BLOB:
		mysqlType = MYSQL_TYPE_BLOB;
		break;

	case CwtType_TIME:
		mysqlType = MYSQL_TYPE_TIME;
		break;

	case CwtType_DATE:
		mysqlType = MYSQL_TYPE_DATE;
		break;

	case CwtType_DATETIME:
		mysqlType = MYSQL_TYPE_DATETIME;
		break;

	case CwtType_TEXT:
		mysqlType = MYSQL_TYPE_STRING;
		break;

	default:
		CWT_ASSERT(cwtType != cwtType);
		break;
	}

	return mysqlType;
}

static BOOL __mysql_isUnsigned(CwtTypeEnum cwtType)
{
	BOOL is_unsigned;

	switch(cwtType) {
	case CwtType_BYTE:
	case CwtType_USHORT:
	case CwtType_UINT:
	case CwtType_ULONG:
	case CwtType_ULONGLONG:
		is_unsigned = TRUE;
		break;
	default:
		is_unsigned = FALSE;
		break;
	}

	return is_unsigned;
}

static CwtTypeEnum __mysql_fromMysqlType( enum enum_field_types mysqlType, UINT field_flags )
{
	CwtTypeEnum cwtType;

	switch( mysqlType ) {
	case MYSQL_TYPE_TINY:
		cwtType = field_flags & UNSIGNED_FLAG ? CwtType_BYTE : CwtType_SBYTE;
		break;
	case MYSQL_TYPE_SHORT:
		cwtType = field_flags & UNSIGNED_FLAG ? CwtType_USHORT : CwtType_SHORT;
		break;
	case MYSQL_TYPE_INT24:
		cwtType = field_flags & UNSIGNED_FLAG ? CwtType_UINT : CwtType_INT;
		break;
	case MYSQL_TYPE_LONG:
		cwtType = field_flags & UNSIGNED_FLAG ? CwtType_ULONG : CwtType_LONG;
		break;
    case MYSQL_TYPE_YEAR:
    	cwtType = CwtType_INT;
        break;
    case MYSQL_TYPE_LONGLONG:
    	cwtType = field_flags & UNSIGNED_FLAG ? CwtType_ULONGLONG : CwtType_LONGLONG;
        break;
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
        cwtType = CwtType_DOUBLE;
        break;
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_NEWDATE:
        cwtType = CwtType_DATE;
        break;
    case MYSQL_TYPE_TIME:
    	cwtType = CwtType_TIME;
        break;
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_TIMESTAMP:
    	cwtType = CwtType_DATETIME;
        break;
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    	cwtType = field_flags & BINARY_FLAG ? CwtType_BLOB : CwtType_TEXT;
        break;

    default:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_BIT:
    case MYSQL_TYPE_GEOMETRY:
    	cwtType = CwtType_TEXT;
        break;
	}

	return cwtType;
}


static char* __dbd_encode_n(CwtDBHandler *dbh, const CWT_CHAR *s, size_t n)
{
	CwtTextCodecNS *codecNS = cwtTextCodecNS();
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( !s )
		return NULL;

	if( !mdbh->csname ) {
		const char *csname = mysql_character_set_name(mdbh->conn);
		mdbh->csname = codecNS->fromLatin1(csname, strlen(csname));
	}

	return codecNS->toMBCS(s, mdbh->csname, n);
}

static CWT_CHAR* __dbd_decode_n(CwtDBHandler *dbh, const char *s, size_t n)
{
	CwtTextCodecNS *codecNS = cwtTextCodecNS();
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( !s )
		return NULL;

	if( !mdbh->csname ) {
		const char *csname = mysql_character_set_name(mdbh->conn);
		mdbh->csname = codecNS->fromLatin1(csname, strlen(csname));
	}

	return codecNS->fromMBCS(s, mdbh->csname, n);
}


static int __mysql_realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, size_t length)
{
	char *stmt_str_;
	int rc;

	CWT_ASSERT(dbh);
	CWT_ASSERT(dbh->conn);

	stmt_str_ = __dbd_encode((CwtDBHandler*)dbh, stmt_str);
	rc = mysql_real_query(dbh->conn, stmt_str_, (ULONG)length/*strlen(stmt_str_)*/);
	CWT_FREE(stmt_str_);

	return rc;
}


static void __mysql_cleanup(void)
{
	/* terminate client library */
	if( !__nConnections ) {
		mysql_library_end();
	}
}


/**
 *
 * @param dsn data source in one of the format:
 * 		"DBI:mysql:DBNAME[;OPTIONS]"
 *    	"DBI:mysql:database=DBNAME;host=HOST[;OPTIONS]"
 *   	"DBI:mysql:database=DBNAME;host=HOST;port=PORT[;OPTIONS]"
 *
 *   	OPTIONS:
 *   	mysql_socket=/dev/mysql - Unix socket that is used for connecting to the server.
 *   	mysql_flag=[COMPRESS][,FOUND_ROWS][,IGNORE_SIGPIPE][,IGNORE_SPACE][,INTERACTIVE]
 *   		[,LOCAL_FILES][,MULTI_RESULTS][,MULTI_STATEMENTS]
 *
 *   	COMPRESS 			Use compression protocol.
 *   	FOUND_ROWS 			Return the number of found (matched) rows, not the number of changed rows.
 *   	IGNORE_SIGPIPE 		Prevents the client library from installing a SIGPIPE signal handler.
 *   						This can be used to avoid conflicts with a handler that the application has already installed.
 *   	IGNORE_SPACE 		Permit spaces after function names. Makes all functions names reserved words.
 *   	INTERACTIVE 		Permit interactive_timeout seconds (instead of wait_timeout seconds)
 *   						of inactivity before closing the connection. The client's session wait_timeout
 *   						variable is set to the value of the session interactive_timeout variable.
 *   	LOCAL_FILES 		Enable LOAD DATA LOCAL handling.
 *   	MULTI_RESULTS 	    Tell the server that the client can handle multiple result sets from
 *   						multiple-statement executions or stored procedures.
 *   						This flag is automatically enabled if CLIENT_MULTI_STATEMENTS is enabled.
 *   						See the note following this table for more information about this flag.
 *   	MULTI_STATEMENTS 	Tell the server that the client may send multiple statements in a single string (separated by ';').
 *   						If this flag is not set, multiple-statement execution is disabled.
 *   						See the note following this table for more information about this flag.
 *
 * @param username user name, can be NULL or empty string. In latter case the current user is assumed
 * @param password user password
 * @return DBI handle or NULL if error
 */
CwtDBHandler* __dbd_connect(const CWT_CHAR *driverDSN
	, const CWT_CHAR *username
	, const CWT_CHAR *password
	, const CWT_CHAR *csname)
{
	CwtStrNS     *strNS     = cwtStrNS();
	CwtTextCodecNS *codecNS = cwtTextCodecNS();
	CwtStrListNS *strlistNS = cwtStrListNS();

	static const CWT_CHAR *csname_default = _T("utf8");

	CwtMySqlDBHandler *dbh = NULL;
	BOOL ok = FALSE;
	CwtStrList *opts;
	CwtStrListIterator itOpts;

	char   *host = NULL;
	char   *dbname = NULL;
	UINT    port = 0;
	char   *sockname = NULL;
	ULONG   flags = 0UL;
	char    *csname_ = NULL;
	char    *username_ = NULL;
	char    *password_ = NULL;

	if( !csname )
		csname = csname_default;

	if( !(strNS->streq(_T("latin1"), csname) || strNS->streq(_T("utf8"), csname)) ) {
		print_error(__LOG_PREFIX _Tr("only Latin1 or UTF-8 character set is supported now"));
		return NULL;
	}

	if( !__nConnections ) {
	    /* initialize client library */
	    if( mysql_library_init(0, NULL, NULL) ) {
	        printf_error(__LOG_PREFIX _Tr("failed to initialize MySQL library"));
	        return NULL;
	    }
	}

    dbh = CWT_MALLOC(CwtMySqlDBHandler);
    dbh->__base.close      = __stmt_close;
    dbh->__base.execute    = __stmt_execute;
    dbh->__base.err        = __stmt_err;
    dbh->__base.strerror   = __stmt_strerror;
	dbh->__base.bindByIndex     = __stmt_bindByIndex;
	dbh->__base.bindParmsCount = __stmt_bindParmsCount;
	dbh->__base.setParm    = __stmt_setParm;
    dbh->__base.rows       = __stmt_affectedRows;
    dbh->__base.size       = __stmt_numRows;
    dbh->__base.fetchNext  = __stmt_fetchNext;
    dbh->__base.fetchColumn= __stmt_fetchColumn;
    dbh->__base.driver     = cwtDBIDriverImpl;
	dbh->auto_commit       = FALSE;
	dbh->csname            = NULL;
	dbh->errorstr          = NULL;
	dbh->sqlstate          = NULL;

    csname_   = codecNS->toLatin1(csname, __strNS->strlen(csname));
    username_ = codecNS->toLatin1(username, __strNS->strlen(username));
    password_ = codecNS->toLatin1(password, __strNS->strlen(password));

    while( TRUE ) {
        /* initialize connection handler */
        dbh->conn = mysql_init(NULL);

        if( dbh->conn == NULL ) {
        	printf_error(__LOG_PREFIX _Tr("unable to initialize MySQL connection handler (probably out of memory)"));
        	break;
        }

        /* Set character set */
        if( mysql_options(dbh->conn, MYSQL_SET_CHARSET_NAME, csname_) != 0 ) {
        	printf_error(__LOG_PREFIX _Tr("failed to set character set: %s"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh));
        	break;
        }

        dbh->csname = strNS->strdup(csname); /* TODO need to free*/

        /* Parse driver DSN */
        opts = strlistNS->create();
        strlistNS->split(opts, driverDSN, _T(";"), NULL, 0);
        strlistNS->begin(opts, &itOpts);

        ok = TRUE;

        while( strlistNS->hasMore(&itOpts) ) {
        	const CWT_CHAR* opt = strlistNS->next(&itOpts);

        	if( strNS->strncmp(_T("host="), opt, 5) == 0 ) {

        		host = __cwtDBIDriver.encode((CwtDBHandler*)dbh, &opt[5]);

        	} else if( strNS->strncmp(_T("database="), opt, 9) == 0) {

        		dbname = __cwtDBIDriver.encode((CwtDBHandler*)dbh, &opt[9]);

        	} else if( strNS->strncmp(_T("port="), opt, 5) == 0) {
        		port = strNS->toUINT(&opt[5], 0, &ok);
        		if( !ok ) {
        			print_error(__LOG_PREFIX _Tr("bad port value"));
        			break;
        		}

        	} else if( strNS->strncmp(_T("mysql_socket="), opt, 13) == 0) {

        		sockname = __cwtDBIDriver.encode((CwtDBHandler*)dbh, &opt[13]);

        	} else if( strNS->strncmp(_T("mysql_flags="), opt, 12) == 0) {
        	    const CWT_CHAR *flagstr = &opt[12];

        	    flags |= CLIENT_REMEMBER_OPTIONS;
        	    if( strNS->strstr(flagstr, _T("COMPRESS")) )         flags |= CLIENT_COMPRESS;
        	    if( strNS->strstr(flagstr, _T("FOUND_ROWS")) )       flags |= CLIENT_FOUND_ROWS;
        	    if( strNS->strstr(flagstr, _T("IGNORE_SIGPIPE")) )   flags |= CLIENT_IGNORE_SIGPIPE;
        	    if( strNS->strstr(flagstr, _T("IGNORE_SPACE")) )     flags |= CLIENT_IGNORE_SPACE;
        	    if( strNS->strstr(flagstr, _T("INTERACTIVE")) )      flags |= CLIENT_INTERACTIVE;
        	    if( strNS->strstr(flagstr, _T("LOCAL_FILES")) )      flags |= CLIENT_LOCAL_FILES;
        	    if( strNS->strstr(flagstr, _T("MULTI_RESULTS")) )    flags |= CLIENT_MULTI_RESULTS;
        	    if( strNS->strstr(flagstr, _T("MULTI_STATEMENTS")) ) flags |= CLIENT_MULTI_STATEMENTS;

        	    /*printf_debug(__LOG_PREFIX _T("flags:    %lu (0x%X)"), flags, flags);*/
        	}
        }
        strlistNS->free(opts);

        if( !ok )
        	break;

        ok = FALSE;

		if( mysql_real_connect(dbh->conn, host, username_, password_, dbname, port, sockname, flags) == NULL) {
			printf_error(__LOG_PREFIX _Tr("failed to connect the database: %s"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh));
			break;
		}

		 if( mysql_autocommit(dbh->conn, 1) != 0 ) {
			 printf_error(__LOG_PREFIX _Tr("unable to set autocommit on"));
			 break;
		 }

        ok = TRUE;
        break;
    }

    CWT_FREE(csname_);
	CWT_FREE(username_);
	CWT_FREE(password_);
	CWT_FREE(host);
	CWT_FREE(dbname);
	CWT_FREE(sockname);

    if( !ok ) {
    	if( dbh->conn ) {
    		__mysql_destroy(dbh);
    	}
    	CWT_FREE(dbh);
    	dbh = NULL;
    } else {
        dbh->auto_commit = TRUE;

    	__nConnections++;
    }

	return (CwtDBHandler*)dbh;
}


/**
 * @brief Closes connection, deallocates the connection handle.
 *
 * @param dbh DBI handle
 */
static void __dbd_disconnect(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	if( !dbh )
		return;

	if( mdbh->conn ) {

		CWT_ASSERT(__nConnections > 0 );

		__mysql_destroy(mdbh);
		CWT_FREE(mdbh);

		__nConnections--;
		__mysql_cleanup();
	}
}


static void __mysql_destroy(CwtMySqlDBHandler *dbh)
{
	CWT_ASSERT(dbh);

	if( dbh->conn ) {
		mysql_close(dbh->conn);
		dbh->conn = NULL;
		CWT_FREE(dbh->csname);
		CWT_FREE(dbh->errorstr);
		CWT_FREE(dbh->sqlstate);
	}
}


static void __mysql_stmtDestroy(CwtMySqlStatement *sth)
{
	CWT_ASSERT(sth);
	if( sth->stmt ) {
		if( sth->bind_params ) {
			CWT_FREE(sth->bind_params);
			sth->bind_params = NULL;
		}

		if( sth->is_null ) {
			CWT_FREE(sth->is_null);
			sth->is_null = NULL;
		}

		if( sth->rbind_params ) {
			size_t i;
			for( i = 0; i < sth->nrbind_params; i++ ) {
				CWT_FREE(sth->rbind_params[i].is_null);
				CWT_FREE(sth->rbind_params[i].length);
				CWT_FREE(sth->rbind_params[i].error);

				/*printf_debug(_T("Buffer dealocation: 0x%p [i=%u]"), sth->rbind_params[i].buffer, i);*/
				CWT_FREE(sth->rbind_params[i].buffer);
			}
			CWT_FREE(sth->rbind_params);
			sth->rbind_params = NULL;
		}

		if( sth->meta ) {
			/* Free previously prepared result metadata */
			mysql_free_result(sth->meta);
			sth->meta = NULL;
		}

		if( sth->rbind_map ) {
			__htNS->free(sth->rbind_map);
			sth->rbind_map = NULL;
		}
		if( sth->meta_map ) {
			__htNS->free(sth->meta_map);
			sth->meta_map = NULL;
		}


		if( mysql_stmt_close(sth->stmt) != 0 ) {
			printf_error(__LOG_PREFIX _Tr("failed to close statement: %s"), mysql_stmt_error(__STH(sth)));
		}

		sth->stmt = NULL;
		CWT_FREE(sth->errorstr);
	}
}

static BOOL __mysql_buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[])
{
	CwtStringNS *stringNS = cwtStringNS();
	CwtStrNS *strNS = cwtStrNS();
	int i = 0;
	BOOL usage = FALSE;

	stringNS->append(sql, _T("CREATE DATABASE IF NOT EXISTS "));

	if( argv[i] ) {
		stringNS->append(sql, argv[i]);
		i++;

		while( argv[i] )  {
			if( strNS->strieq(_T("CHARACTER SET"), argv[i]) && argv[i+1] ) {
				stringNS->appendChar(sql, _T(' '));
				stringNS->append(sql, _T("CHARACTER SET"));
				stringNS->appendChar(sql, _T(' '));
				stringNS->append(sql, argv[i+1]);
				i += 2;
			} else if( strNS->strieq(_T("COLLATE"), argv[i]) && argv[i+1] ) {
				stringNS->appendChar(sql, _T(' '));
				stringNS->append(sql, _T("COLLATE"));
				stringNS->appendChar(sql, _T(' '));
				stringNS->append(sql, argv[i+1]);
				i += 2;
			} else {
				usage = TRUE;
				break;
			}
		}
	} else {
		usage = TRUE;
	}

	if( usage ) {
		printf_warn(__LOG_PREFIX _Tr("usage: dbh->func: createdb dbname ['CHARACTER SET' charset_name] ['COLLATE' collation_name]"));
		return FALSE;
	}

	return TRUE;
}


static BOOL __mysql_buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[])
{
	CwtStringNS *stringNS = cwtStringNS();
	int i = 0;
	BOOL usage = FALSE;

	stringNS->append(sql, _T("DROP DATABASE IF EXISTS "));

	if( argv[i] ) {
		stringNS->appendChar(sql, _T('`'));
		stringNS->append(sql, argv[i]);
		stringNS->appendChar(sql, _T('`'));
		i++;
	} else {
		usage = TRUE;
	}

	if( usage ) {
		printf_warn(__LOG_PREFIX _Tr("usage: dbh->func: dropdb dbname"));
		return FALSE;
	}

	return TRUE;
}

/**
 *
 * @param dbh database handle
 * @param func_name function name
 * @param argv NULL-terminated array of function arguments
 * @return Zero if the database was created successfully. Nonzero if an error occurred.
 *
 * Function names:
 * 	admin
 * 		createdb dbname ['CHARACTER SET' charset_name] ['COLLATE' collation_name]
 * 	 	dropdb dbname
 * 	 	shutdown
 *
 */
static BOOL __dbd_func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStringNS *stringNS = cwtStringNS();

	CwtString *sql;
	BOOL rv = FALSE;
	/*BOOL usage = FALSE;*/

	CWT_ASSERT(dbh);
	CWT_ASSERT(argv);
	CWT_ASSERT(argv[0]);

	sql = stringNS->create();

	if( strNS->strieq(_T("admin"), func_name) ) {
		if( strNS->strieq(_T("createdb"), argv[0]) ) {
			if( __mysql_buildSqlCreateDB(sql, &argv[1])
					&& __mysql_realQuery((CwtMySqlDBHandler*)dbh, stringNS->cstr(sql), stringNS->length(sql)) == 0 )
				rv = TRUE;
		} else if( strNS->strieq(_T("dropdb"), argv[0]) ) {
			if( __mysql_buildSqlDropDB(sql, &argv[1])
					&& __mysql_realQuery((CwtMySqlDBHandler*)dbh, stringNS->cstr(sql), stringNS->length(sql)) == 0)
				rv = TRUE;
		}
	} else {
		printf_warn(__LOG_PREFIX _Tr("unsupported function name: %s"), CWT_STRING_OR_NULLSTR(func_name));
	}

	stringNS->free(sql);
	return rv;
}

/**
 * @brief Retreives the attributes of database handles.
 *
 * @param dbh database handle
 * @param attr_name attribute name
 * @param attr_value place to store attribute value
 *
 * Acceptable attributes and corresponding value types:
 *		@arg errno:UINT              The error code for the most recently invoked API function that can succeed or fail.
 *								     A return value of zero means that no error occurred.
 *								     (@see mysql_errno())
 *	    @arg error:const CHAR*       A null-terminated string containing the error message for the most recently invoked API
 *	                                 function that failed. If a function didn't fail, the return value
 *	                                 may be the previous error or an empty string to indicate no error.
 *	                                 (@see mysql_error())
 *		@arg hostinfo:const CHAR*    String describing the type of connection in use, including the server host name.
 *		                             (@see mysql_get_host_info())
 *		@arg info:const CHAR*        String providing information about the most recently executed statement,
 *		                             but only for the statements listed here. For other statements, returns NULL.
 *		                             The format of the string varies depending on the type of statement, as described here.
 *		                             The numbers are illustrative only; the string contains values appropriate for the statement.
 *		                             (@see mysql_info())
 *		@arg insertid:ULONGLONG      The value generated for an AUTO_INCREMENT column by the previous INSERT or UPDATE statement.
 *		                             Use it after you have performed an INSERT statement into a table that contains
 *		                             an AUTO_INCREMENT field, or have used INSERT or UPDATE to set a column value with LAST_INSERT_ID(expr).
 *		                             (@see mysql_insert_id())
 *	    @arg protoinfo:UINT          The protocol version used by current connection.
 *	                                 (@see mysql_get_proto_info())
 *	    @arg serverinfo:const CHAR*  A string that represents the server version number.
 *	                                 (@see mysql_get_server_info())
 *	    @arg serverversion:ULONG     The version number of the server as an integer in format
 *	                                 major_version*10000 + minor_version *100 + sub_version. For example, 5.1.5 is returned as 50105.
 *	                                 (@see mysql_get_server_version())
 *	    @arg clientinfo:const CHAR*  A string that represents the client library version.
 *	                                 (@see mysql_get_client_info())
 *	    @arg clientversion:ULONG     An integer that represents the client library version.
 *	                                 The value has the format XYYZZ where X is the major version,
 *	                                 YY is the release level, and ZZ is the version number within the release level.
 *	                                 For example, a value of 40102 represents a client library version of 4.1.2.
 *	                                 (@see mysql_get_client_version())
 *	    @arg thread_id:ULONG         The thread ID of the current connection.
 *	                                 (@see mysql_thread_id())
 *	    @arg stat:const CHAR*        A character string containing information similar to that provided
 *	                                 by the mysqladmin status command. This includes uptime in seconds and
 *	                                 the number of running threads, questions, reloads, and open tables.
 *	                                 (@see mysql_stat())
 *	    @arg charset:const CHAR*     The default character set name for the current connection.
 *	                                 (@see mysql_character_set_name())
 *
 */
static void __dbd_attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CwtStrNS *strNS = cwtStrNS();

	CWT_ASSERT(dbh);
	CWT_ASSERT(attr_name);
	CWT_ASSERT(attr_value);

	if( strNS->strieq(_T("errno"), attr_name) ) {

		*((UINT*)attr_value) = mysql_errno(__DBH(dbh));

	} else if(strNS->strieq(_T("error"), attr_name)) {

		*((const char**)attr_value) = mysql_error(__DBH(dbh));

	} else if(strNS->strieq(_T("hostinfo"), attr_name)) {

		*((const char**)attr_value) = mysql_get_host_info(__DBH(dbh));

	} else if(strNS->strieq(_T("info"), attr_name)) {

		*((const char**)attr_value) = mysql_info(__DBH(dbh));

	} else if(strNS->strieq(_T("insertid"), attr_name)) {

		*((ULONGLONG*)attr_value) = mysql_insert_id(__DBH(dbh));

	} else if(strNS->strieq(_T("protoinfo"), attr_name)) {

		*((UINT*)attr_value) = mysql_get_proto_info(__DBH(dbh));

	} else if(strNS->strieq(_T("serverinfo"), attr_name)) {

		*((const char**)attr_value) = mysql_get_server_info(__DBH(dbh));

	} else if(strNS->strieq(_T("serverversion"), attr_name)) {

		*((ULONG*)attr_value) = mysql_get_server_version(__DBH(dbh));

	} else if(strNS->strieq(_T("clientinfo"), attr_name)) {

		*((const char**)attr_value) = mysql_get_client_info();

	} else if(strNS->strieq(_T("clientversion"), attr_name)) {

		*((ULONG*)attr_value) = mysql_get_client_version();

	} else if(strNS->strieq(_T("thread_id"), attr_name)) {

		*((ULONG*)attr_value) = mysql_thread_id(__DBH(dbh));

	} else if(strNS->strieq(_T("stat"), attr_name)) {

		*((const char**)attr_value) = mysql_stat(__DBH(dbh));

	} else if(strNS->strieq(_T("charset"), attr_name)) {

		*((const char**)attr_value) = mysql_character_set_name(__DBH(dbh));

	}
}

static BOOL __dbd_setAutoCommit(CwtDBHandler *dbh, BOOL on)
{
	CWT_ASSERT(dbh);
	if( mysql_autocommit(__DBH(dbh), on ? 1 : 0) == 0 ) {
		((CwtMySqlDBHandler*)dbh)->auto_commit = on;
		return TRUE;
	}
	print_error(__LOG_PREFIX _Tr("failed to change autocommit mode"));
	return FALSE;
}

static BOOL __dbd_autoCommit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return ((CwtMySqlDBHandler*)dbh)->auto_commit;
}


/**
 *
 * @param dbh Database handle
 * @return
 *
 * @see mysql_errno()
 */
static CwtDBI_RC __dbd_err(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return (CwtDBI_RC)mysql_errno(__DBH(dbh));
}

/**
 *
 * @param dbh Database handle
 * @return
 *
 * @see mysql_error()
 */
static const CWT_CHAR*  __dbd_strerror(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( mdbh->errorstr ) {
		CWT_FREE(mdbh->errorstr);
		mdbh->errorstr = NULL;
	}

	mdbh->errorstr = __dbd_decode(dbh, mysql_error(mdbh->conn));
	return mdbh->errorstr;
}


/**
 * @brief Returns a null-terminated string containing the SQLSTATE error code.
 *
 * Returns a null-terminated string containing the SQLSTATE error code for the most recently executed SQL statement.
 * The error code consists of five characters. '00000' means "no error".
 *
 * @param dbh Database handle
 * @return a null-terminated string containing the SQLSTATE error code.
 *
 * @see mysql_sqlstate()
 */
static const CWT_CHAR*  __dbd_state(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( mdbh->sqlstate ) {
		CWT_FREE(mdbh->sqlstate);
		mdbh->sqlstate = NULL;
	}

	mdbh->sqlstate = __dbd_decode(dbh, mysql_sqlstate(mdbh->conn));
	return mdbh->sqlstate;

}


/**
 *
 * Executes the SQL statement pointed to by @c sql, a string length bytes long.
 * If multiple-statement execution has been enabled, the string can contain several
 * statements separated by semicolons. Cannot be used for statements
 * that contain binary data.
 *
 * @param dbh Database handle
 * @param sql SQL statement
 * @return Zero if the statement was successful. Nonzero if an error occurred.
 */
static BOOL __dbd_query(CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	CWT_ASSERT(dbh);

	if( __mysql_realQuery((CwtMySqlDBHandler*)dbh, sql, cwtStrNS()->strlen(sql)) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to query: %s [%s]"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh), sql);
		return FALSE;
	}
	return TRUE;
}

static BOOL __dbd_queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length)
{
	CWT_ASSERT(dbh);

	if( __mysql_realQuery((CwtMySqlDBHandler*)dbh, sql, length) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to query: %s [%s]"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh), sql);
		return FALSE;
	}
	return TRUE;
}

/**
 * @brief Prepares statement for execution
 *
 *
 * @param dbh Database handle
 * @param stmt_str
 * @return
 */
static CwtStatement* __dbd_prepare(CwtDBHandler *dbh, const CWT_CHAR *stmt_str)
{
	CwtStrNS *strNS = cwtStrNS();

	MYSQL_STMT *stmt;
	CwtMySqlStatement *sth;
	char *stmt_str_ = NULL;

	CWT_ASSERT(dbh);

	stmt = mysql_stmt_init(__DBH(dbh));

	if( !stmt ) {
		printf_error(__LOG_PREFIX _Tr("failed to initialize statement: %s"), __cwtDBIDriver.strerror(dbh));
		return NULL;
	}

	stmt_str_ = __cwtDBIDriver.encode(dbh, stmt_str);
	if( mysql_stmt_prepare(stmt, stmt_str_, (ULONG)strlen(stmt_str_)) != 0 ) { /*FIXME type cast problems*/
		printf_error(__LOG_PREFIX _Tr("failed to prepare statement: %s"), __cwtDBIDriver.strerror(dbh));
		mysql_stmt_close(stmt);
		CWT_FREE(stmt_str_);
		return NULL;
	}
	CWT_FREE(stmt_str_);

	sth = CWT_MALLOC(CwtMySqlStatement);

	sth->stmt          = stmt;
	sth->bind_params   = NULL;
	sth->is_null       = NULL;
	sth->nbind_params  = mysql_stmt_param_count(stmt);
	sth->is_bind       = FALSE;
	sth->meta          = NULL;
	sth->rbind_params  = NULL;
	sth->nrbind_params = 0;
	sth->is_rbind      = FALSE;
	sth->errorstr      = NULL;
	sth->rbind_map     = NULL;
	sth->meta_map      = NULL;

	if( sth->nbind_params > 0UL ) {
		sth->bind_params = CWT_MALLOCA(MYSQL_BIND, sth->nbind_params);
		strNS->bzero(sth->bind_params, sizeof(MYSQL_BIND) * sth->nbind_params);

		sth->is_null = CWT_MALLOCA(my_bool, sth->nbind_params);
		strNS->bzero(sth->is_null, sizeof(my_bool) * sth->nbind_params);
	}

	return (CwtStatement*)sth;
}


static ULONGLONG __dbd_affectedRows(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return (ULONGLONG)mysql_affected_rows(__DBH(dbh));
}


static BOOL __dbd_tables(CwtDBHandler *dbh, CwtStrList *tables)
{
	CwtStrNS     *strNS = cwtStrNS();
	CwtStrListNS *slNS  = cwtStrListNS();

	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;
	MYSQL_RES* res;
	MYSQL_ROW row;
	int i = 0;
	BOOL ok = TRUE;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);
	CWT_ASSERT(tables);

	res = mysql_list_tables(mdbh->conn, NULL);

	if( !res ) {
		printf_error(__LOG_PREFIX _Tr("failed to list tables: %s"), __cwtDBIDriver.strerror(dbh));
		return FALSE;
	}

	while( res ) {
		CWT_CHAR* decoded;

		mysql_data_seek(res, i);

		row = mysql_fetch_row(res);

		if( !row ) {
			break;
		}

		decoded = __dbd_decode(dbh, row[0]);
		slNS->append( tables, decoded, strNS->strlen(decoded) );

		i++;
	}

	mysql_free_result(res);

	return ok;
}


static BOOL __dbd_tableExists(CwtDBHandler *dbh, const CWT_CHAR *tname)
{
	CwtStrNS     *strNS = cwtStrNS();

	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;
	MYSQL_RES* res;
	MYSQL_ROW row;
	int i = 0;
	BOOL ok;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	res = mysql_list_tables(mdbh->conn, NULL);

	if( !res ) {
		printf_error(__LOG_PREFIX _Tr("failed to list tables: %s"), __cwtDBIDriver.strerror(dbh));
		return FALSE;
	}

	ok = FALSE;

	while( res ) {
		CWT_CHAR* decoded;

		mysql_data_seek(res, i);

		row = mysql_fetch_row(res);

		if( !row ) {
			break;
		}

		decoded = __dbd_decode(dbh, row[0]);
		if( strNS->strieq( tname, decoded ) ) {
			ok = TRUE;
			break;
		}


		i++;
	}

	mysql_free_result(res);

	return ok;
}


static BOOL __dbd_begin(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( mdbh->auto_commit ) {
		if( ! __dbd_setAutoCommit(dbh, FALSE) )
			return FALSE;
	}

	return __dbd_query(dbh, _T("BEGIN WORK"));
}

static BOOL __dbd_commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	CWT_ASSERT(((CwtMySqlDBHandler*)dbh)->conn);
	return __dbd_query(dbh, _T("COMMIT"));
}

static BOOL __dbd_rollback(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	CWT_ASSERT(((CwtMySqlDBHandler*)dbh)->conn);
	return __dbd_query(dbh, _T("ROLLBACK"));
}


static void __stmt_close(CwtStatement *sth)
{
	if( !sth )
		return;

	if( __STH(sth) ) {
		__mysql_stmtDestroy((CwtMySqlStatement*)sth);
		CWT_FREE(sth);
	}
}


/*
 * TODO
 *
 * In case when between statement executions occurred lost connection with server,
 * the connection will recovered gracefully, but stmt will no-longer
 * be valid after the call - most likely causing you a segfault
 * if you try to do anything with it. The code below resolves this problem
 *
 * block
 *   int __dbi_stmt_execute_ntries__ = 5;
 *   	while( __dbi_stmt_execute_ntries__-- )
 *   		sth = dbh->prepare();
 *   		sth->bind()
 *   		sth->bind()
 *   		...
 *   		sth->bind()
 *
 *   		if( !sth->execute() && stmt->err == CR_SERVER_LOST ) {
 *   			sth->close();
 *   			continue;
 *   		}
 *
 *   		break;
 *   	end while
 *   end block
 *
 * Source: http://dev.mysql.com/doc/refman/5.6/en/mysql-stmt-execute.html
 *
 */

static BOOL __stmt_execute(CwtStatement *sth)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	BOOL hasBlobs = FALSE;

	CWT_ASSERT(sth);
	CWT_ASSERT(msth->stmt);

	if( msth->nbind_params > 0 ) {
		if( !msth->is_bind ) {
			if( mysql_stmt_bind_param(__STH(sth), msth->bind_params) != 0 ) {
				printf_error( __LOG_PREFIX _Tr("bind parameters failed: %s\n"), msth->__base.dbh->strerror(sth));
				return FALSE;
			}

			msth->is_bind = TRUE;
		} else { /* TODO when need to use mysql_stmt_reset ? */
/*
			if( mysql_stmt_reset(msth->stmt) != 0 ) {
				printf_error( __LOG_PREFIX _Tr("unable to reset statement: %s\n"), msth->__base.dbh->strerror(sth));
				return FALSE;
			}
*/
		}
	}

	if( !msth->is_rbind ) {
		if( msth->meta ) {
			/* Free previously prepared result metadata */
			mysql_free_result(msth->meta);
		}

		msth->meta          = mysql_stmt_result_metadata(__STH(msth));
		msth->nrbind_params = msth->meta != NULL ? mysql_num_fields(msth->meta) : 0;

		if( msth->nrbind_params > 0UL && msth->rbind_params == NULL ) {
			msth->rbind_params = CWT_MALLOCA(MYSQL_BIND, msth->nrbind_params);
			strNS->bzero(msth->rbind_params, sizeof(MYSQL_BIND) * msth->nrbind_params);
		}

		if( msth->rbind_map ) {
			__htNS->free(msth->rbind_map);
			msth->rbind_map = NULL;
		}
		if( msth->meta_map ) {
			__htNS->free(msth->meta_map);
			msth->meta_map = NULL;
		}

		msth->is_rbind = TRUE;
	}

	if( mysql_stmt_execute(__STH(sth)) != 0 ) {
		printf_error( __LOG_PREFIX _Tr("executing statement error: %s\n"), msth->__base.dbh->strerror(sth));
		return FALSE;
	}

	if( msth->nrbind_params > 0 ) {
		UINT i;

		msth->rbind_map = __htNS->create(__htNS->strHash, __htNS->streq, NULL/*cwtFree*/, NULL);
		msth->meta_map = __htNS->create(__htNS->strHash, __htNS->streq, NULL/*cwtFree*/, NULL);

		for( i = 0; i < msth->nrbind_params; i++ ) {
			MYSQL_FIELD *field = mysql_fetch_field_direct(msth->meta, i);

			msth->rbind_params[i].buffer = NULL;

			if( __mysql_isBlob(field->type) ) {
				hasBlobs = TRUE;
				field->length = 0;
			} else if( __mysql_isTime(field->type) ) {
				field->length = sizeof(MYSQL_TIME);
			} else if( __mysql_isInteger(field->type) ) {
				field->length = MAX_BIGINT_WIDTH;
			} else if( field->type == MYSQL_TYPE_NULL ) {
				field->length = 0;
			}

			msth->rbind_params[i].buffer_type = field->type;
			msth->rbind_params[i].is_null = CWT_MALLOC(my_bool);
			msth->rbind_params[i].length  = CWT_MALLOC(ULONG);
			msth->rbind_params[i].error   = CWT_MALLOC(my_bool);

			if( field->length > 0 ) {
				msth->rbind_params[i].buffer_length = field->length + 1;
				msth->rbind_params[i].buffer = CWT_MALLOCA(char, field->length + 1);
				strNS->bzero(msth->rbind_params[i].buffer, sizeof(char) * (field->length + 1));

				/*printf_debug(_T("Buffer allocated: 0x%p [i=%u][sz=%lu]"), msth->rbind_params[i].buffer, i, field->length + 1);*/
			}

			CWT_ASSERT(__htNS->insert(msth->rbind_map, field->name, &msth->rbind_params[i]));
			CWT_ASSERT(__htNS->insert(msth->meta_map,  field->name, field));

		}

		if( mysql_stmt_bind_result(__STH(sth), msth->rbind_params) != 0 ) {
			printf_error( __LOG_PREFIX _Tr("failed to bind result: %s\n"), msth->__base.dbh->strerror(sth));
			return FALSE;
		}


		if( hasBlobs ) {
			my_bool update_max_length = 1;
			mysql_stmt_attr_set(__STH(sth), STMT_ATTR_UPDATE_MAX_LENGTH, &update_max_length);
		}

		if( mysql_stmt_store_result(__STH(sth)) != 0 ) {
		    printf_error( __LOG_PREFIX _Tr("failed to cache (buffer) the result: %s\n"), msth->__base.dbh->strerror(sth));
			return FALSE;
		}

		/* From qt4/src/sql/drivers/mysql/qsql_mysql.cpp */
	    if( hasBlobs ) {
			/* mysql_stmt_store_result() with STMT_ATTR_UPDATE_MAX_LENGTH set to true crashes
			 when called without a preceding call to mysql_stmt_bind_result()
			 in versions < 4.1.8 */

	    	/* Bind BLOBs */
			for( i = 0; i < msth->nrbind_params; i++ ) {
				MYSQL_FIELD *field = mysql_fetch_field_direct(msth->meta, i);

				if( __mysql_isBlob(field->type) ) {
					msth->rbind_params[i].buffer_length = field->max_length;
					msth->rbind_params[i].buffer = CWT_MALLOCA(char, field->max_length);
				}
			}

			if( mysql_stmt_bind_result(__STH(sth), msth->rbind_params) != 0 ) {
				printf_error( __LOG_PREFIX _Tr("unable to bind BLOB value(s): %s\n"), msth->__base.dbh->strerror(sth));
				return FALSE;
			}
	     }
	}

	return TRUE;
}

static ULONGLONG __stmt_affectedRows(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)mysql_stmt_affected_rows(__STH(sth));
}

static ULONGLONG __stmt_numRows(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)mysql_stmt_num_rows(__STH(sth));
}


static BOOL __stmt_fetchNext(CwtStatement *sth)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	int rv;

	CWT_ASSERT(sth);
	CWT_ASSERT(sth->dbh);
	CWT_ASSERT(msth->stmt);

	rv = mysql_stmt_fetch(msth->stmt);

	if( rv != 0 ) {
		switch(rv) {
		case 1: /*error*/
			printf_error( __LOG_PREFIX _Tr("failed to fetch result: %s"), msth->__base.dbh->strerror(sth));
			return FALSE;
		case MYSQL_NO_DATA:
			return FALSE;
		case MYSQL_DATA_TRUNCATED:
			print_warn( __LOG_PREFIX _Tr("data truncated"));
			break;
		}
	}

	return TRUE;
}


static BOOL __stmt_fetchColumn(CwtStatement *sth, CWT_CHAR *col, CwtUniType *ut)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	MYSQL_BIND *rbind;
	MYSQL_FIELD *field;
	CwtTypeEnum cwtType;
	char *colname;
	BOOL ok = FALSE;

	CWT_ASSERT(msth);
	CWT_ASSERT(msth->stmt);
	CWT_ASSERT(msth->rbind_map);
	CWT_ASSERT(msth->meta_map);
	CWT_ASSERT(ut);

	colname = __dbd_encode(msth->__base.dbh, col);
	rbind = (MYSQL_BIND*)__htNS->lookup(msth->rbind_map, colname);
	field = (MYSQL_FIELD*)__htNS->lookup(msth->meta_map, colname);
	CWT_FREE(colname);

	if( !rbind ) {
		printf_error(_T("column '%s' not found"), col);
		return FALSE;
	}
	CWT_ASSERT(field);


/*
	if( rbind->is_null && *rbind->is_null ) {
		ut->is_null = 1;
		return TRUE;
	} else {
		ut->is_null = 0;
	}
*/

	cwtType  = __mysql_fromMysqlType(rbind->buffer_type, field->flags);
	CWT_ASSERT(rbind->length);

	if( CwtType_TIME == cwtType || CwtType_DATE == cwtType || CwtType_DATETIME == cwtType ) {
		CWT_TIME cwtm;
		MYSQL_TIME *mytm = (MYSQL_TIME*)rbind->buffer;

		cwtm.hour     = mytm->hour;
		cwtm.min      = mytm->minute;
		cwtm.sec      = mytm->second;
		cwtm.sec_part = mytm->second_part;
		cwtm.year     = mytm->year;
		cwtm.mon      = mytm->month;
		cwtm.day      = mytm->day;
		ok = __utNS->set(ut, cwtType, &cwtm, 0);
	} else if( CwtType_TEXT == cwtType ) {
		CWT_CHAR *s = sth->dbh->driver()->decode_n(sth->dbh, (const char*)rbind->buffer, *rbind->length);
		ok = __utNS->setTEXT(ut, s, *rbind->length);
		CWT_FREE(s);
	} else {
		ok = __utNS->set(ut, cwtType, rbind->buffer, *rbind->length);
	}

	return ok;
}

static CwtDBI_RC __stmt_err(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (CwtDBI_RC)mysql_stmt_errno(__STH(sth));
}

static const CWT_CHAR* __stmt_strerror(CwtStatement *sth)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;

	CWT_ASSERT(msth);
	CWT_ASSERT(msth->stmt);
	CWT_ASSERT(msth->__base.dbh);

	if( msth->errorstr ) {
		CWT_FREE(msth->errorstr);
		msth->errorstr = NULL;
	}

	msth->errorstr = __dbd_decode(msth->__base.dbh, mysql_stmt_error(msth->stmt));
	return msth->errorstr;
}


static BOOL __stmt_bindByIndex (CwtStatement *sth, size_t index, CwtUniType *ut)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	MYSQL_BIND *bind_param;

	CWT_ASSERT(sth);
	CWT_ASSERT(ut);

	if( index >= msth->nbind_params ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
		return FALSE;
	}

	bind_param = (MYSQL_BIND*)&msth->bind_params[index];

	if( ut->type == CwtType_TIME || ut->type == CwtType_DATE || ut->type == CwtType_DATETIME ) {
		/* need to correct size of date/time value buffer */
		__utNS->set(ut, ut->type, NULL, CWT_MAX(sizeof(CWT_TIME), sizeof(MYSQL_TIME)));
		bind_param->buffer_length = sizeof(MYSQL_TIME);

	} else {
		bind_param->buffer_length = CWT_TYPE_IS_SCALAR(ut->type) ? (ULONG)0 : ut->capacity;
	}

	bind_param->length        = &ut->length;
	bind_param->buffer_type   = __mysql_toMysqlType(ut->type);
	bind_param->buffer        = CWT_TYPE_IS_SCALAR(ut->type)
		? (char*)&ut->value.llong_val
		: (char*)ut->value.ptr;

	bind_param->is_null       = &msth->is_null[index];
	bind_param->is_unsigned   = __mysql_isUnsigned(ut->type) ? 1 : 0;

	*bind_param->is_null      = ut->type == CwtType_UNKNOWN ? 1 : 0;

	msth->is_bind  = FALSE;
	msth->is_rbind = FALSE;
	return TRUE;
}


static BOOL __stmt_setParm(CwtStatement *sth, CwtUniType *ut, const void *copy, size_t sz)
{
	BOOL ok = FALSE;

	CWT_ASSERT(ut);

	switch( ut->type ) {
	case CwtType_TIME:
	case CwtType_DATE:
	case CwtType_DATETIME: {
			CWT_TIME *cwtm   = (CWT_TIME*)copy;
			MYSQL_TIME *mytm = (MYSQL_TIME*)ut->value.ptr;

			__strNS->bzero(mytm, sizeof(MYSQL_TIME));

			mytm->time_type = MYSQL_TIMESTAMP_DATETIME;

			if( CwtType_TIME == ut->type ||  CwtType_DATETIME == ut->type ) {
				mytm->hour = cwtm->hour;
				mytm->minute = cwtm->min;
				mytm->second = cwtm->sec;
				mytm->second_part = cwtm->sec_part;

				if( CwtType_TIME == ut->type )
					mytm->time_type = MYSQL_TIMESTAMP_TIME;
			}
			if( CwtType_DATE == ut->type ||  CwtType_DATETIME == ut->type ) {
				mytm->year = cwtm->year;
				mytm->month = cwtm->mon;
				mytm->day = cwtm->day;

				if( CwtType_DATE == ut->type )
					mytm->time_type = MYSQL_TIMESTAMP_DATE;
			}
		}
		ok = TRUE;
		break;

	case CwtType_TEXT: {
			CwtDBIDriver *dbd = sth->dbh->driver();
			const CWT_CHAR *text = (const CWT_CHAR*)copy;
			char *encoded;
			size_t encoded_len;

			encoded = dbd->encode_n(sth->dbh, text, sz);
			encoded_len = strlen(encoded);

			if( encoded_len > ut->capacity ) {
				printf_error(_T("string too long"));
				ok = FALSE;
			} else {
				strncpy((char*)ut->value.ptr, encoded, encoded_len);
				ut->length = encoded_len;
				ok = TRUE;
			}

			CWT_FREE(encoded);
	    }
		break;

	case CwtType_CHAR:
	case CwtType_BLOB:
	default:
		ok = __utNS->set(ut, ut->type, copy, sz);
		break;
	}

	return ok;
}
