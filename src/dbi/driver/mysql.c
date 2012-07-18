/*
 * mysql.c
 *
 *  Created on: 03.07.2012
 *      Author: wladt
 */

#include <mysql/my_global.h>
#include <mysql/my_sys.h>
#include <mysql/mysql.h>
#include <cwt/dbi/dbi.h>
#include <cwt/algo/hash_tab.h>
#include <cwt/algo/hash_str.h>
#include <cwt/algo/cmp_str.h>


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

#include <cwt/logger.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/str.h>
#include <string.h>


#define __LOG_PREFIX _T("mysql: ")
#define __DBH(dbh)  (((CwtMySqlDBHandler*)(dbh))->conn)
#define __STH(sth)  (((CwtMySqlStatement*)(sth))->stmt)

typedef struct CwtMySqlStatement {
	CwtStatement __base;
	CwtDBHandler *dbh;
	MYSQL_STMT   *stmt;
	MYSQL_BIND   *bind_params;
	my_bool      *is_null;
	size_t        nbind_params;
	BOOL          is_bind; /* TRUE if parameters already bind for output */

	MYSQL_RES    *meta;
	MYSQL_BIND   *rbind_params;
	size_t        nrbind_params;
	HashTable    *rbind_map;
	HashTable    *meta_map;
	CWT_CHAR     *errorstr;
} CwtMySqlStatement;


typedef struct CwtMySqlDBHandler
{
	CwtDBHandler __base;
	MYSQL       *conn;
	BOOL         auto_commit;
	CWT_CHAR    *csname;      /* character set name */
	CWT_CHAR    *errorstr;
	CWT_CHAR    *sqlstate;
} CwtMySqlDBHandler;


typedef struct CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;


typedef struct CwtMySqlTime {
	CWT_TIME     __base;
	MYSQL_TIME   mysql_time;
} CwtMySqlTime;

/* DBI API functions implementations */
CwtDBHandler*           __connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password, const CWT_CHAR *csname);
static void             __disconnect(CwtDBHandler *dbh);
static BOOL             __func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void             __attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL             __setAutoCommit(CwtDBHandler *dbh, BOOL on);
static BOOL             __autoCommit(CwtDBHandler *dbh);
static CwtDBI_RC        __err(CwtDBHandler *dbh);
static const CWT_CHAR*  __strerror(CwtDBHandler *dbh);
static const CWT_CHAR*  __state(CwtDBHandler *dbh);
static BOOL             __query(CwtDBHandler *dbh, const CWT_CHAR *sql);
static BOOL             __queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length);
static CwtStatement*    __prepare(CwtDBHandler *dbh, const CWT_CHAR *statement);
static ULONGLONG        __affectedRows(CwtDBHandler *dbh);
static BOOL             __tables(CwtDBHandler *dbh, CwtStrList *tables);
static char*            __encode(CwtDBHandler *dbh, const CWT_CHAR *s);
static CWT_CHAR*        __decode(CwtDBHandler *dbh, const char *s);
static CWT_TIME*        __createTime(void);
static void             __freeTime(CWT_TIME*);
static void             __convertTime(CWT_TIME *cwtTime, void *nativeTime);
static BOOL             __begin(CwtDBHandler *dbh);
static BOOL             __commit(CwtDBHandler *dbh);
static BOOL             __rollback(CwtDBHandler *dbh);

static void             __stmtClose(CwtStatement *sth);
static BOOL             __stmtExecute(CwtStatement *sth);
static ULONGLONG        __stmtAffectedRows(CwtStatement *sth);
static ULONGLONG        __stmtNumRows(CwtStatement*);
static BOOL             __stmtFetchNext(CwtStatement*);
static BOOL             __stmtFetchColumn(CwtStatement *sth, CWT_CHAR *col, CWT_UNITYPE *value);
static CwtDBI_RC        __stmtErr(CwtStatement *sth);
static const CWT_CHAR*  __stmtStrerror(CwtStatement *sth);
static BOOL             __stmtBind(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, void *value, size_t *plength, BOOL is_null);
static BOOL             __stmtBindScalar(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, void *value);
static BOOL             __stmtBindTime(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, void *value);
static BOOL             __stmtBindNull(CwtStatement *sth, size_t index, CwtTypeEnum cwtType);

/* local helper functions */
static BOOL             __mysqlIsBlob(int mysqltype);
static BOOL             __mysqlIsInteger(int mysqltype);
static BOOL             __mysqlIsTime(int mysqltype);
static enum enum_field_types __toMysqlType(CwtTypeEnum cwtType);
static CwtTypeEnum      __fromMysqlType( enum enum_field_types mysqlType, UINT field_flags);
static BOOL             __isUnsigned(CwtTypeEnum cwtType);
static void             __destroy(CwtMySqlDBHandler *dbh);
static void             __stmtDestroy(CwtMySqlStatement *sth);
static BOOL             __buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[]);
static BOOL             __buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[]);
static int              __realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, size_t length);

static BOOL __nConnections = 0; /* number of connections */

static CwtDBIDriver __cwtDBIDriver = {
	  __connect
	, __disconnect
	, __func
	, __attr
	, __setAutoCommit
	, __autoCommit
	, __err
	, __strerror
	, __state
	, __query
	, __queryBin
	, __prepare
	, __affectedRows
	, __tables
	, __encode
	, __decode
	, __createTime
	, __freeTime
	, __convertTime
	, __begin
    , __commit
    , __rollback
};


DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	return &__cwtDBIDriver;
}


static BOOL __mysqlIsBlob(int t)
{
	return (t == MYSQL_TYPE_TINY_BLOB
		   || t == MYSQL_TYPE_BLOB
		   || t == MYSQL_TYPE_MEDIUM_BLOB
		   || t == MYSQL_TYPE_LONG_BLOB) ? TRUE : FALSE;
}

static BOOL __mysqlIsInteger(int t)
{
	return (t == MYSQL_TYPE_TINY
		   || t == MYSQL_TYPE_SHORT
		   || t == MYSQL_TYPE_LONG
		   || t == MYSQL_TYPE_LONGLONG
		   || t == MYSQL_TYPE_INT24) ? TRUE : FALSE;
}

static BOOL __mysqlIsTime(int t)
{
	return ( t == MYSQL_TYPE_TIMESTAMP
			|| t == MYSQL_TYPE_DATE
			|| t == MYSQL_TYPE_TIME
			|| t == MYSQL_TYPE_DATETIME
			|| t == MYSQL_TYPE_NEWDATE) ? TRUE : FALSE;
}


static enum enum_field_types __toMysqlType(CwtTypeEnum cwtType)
{
	enum enum_field_types mysqlType;

	switch( cwtType ) {
	case CwtType_CHAR:
	case CwtType_UCHAR:
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
		CWT_ASSERT(cwtType != cwtType );
		break;
	}

	return mysqlType;
}

static BOOL __isUnsigned(CwtTypeEnum cwtType)
{
	BOOL is_unsigned;

	switch(cwtType) {
	case CwtType_UCHAR:
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

static CwtTypeEnum __fromMysqlType( enum enum_field_types mysqlType, UINT field_flags )
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


static char* __encode(CwtDBHandler *dbh, const CWT_CHAR *s)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( !s )
		return NULL;

	if( !mdbh->csname )
		mdbh->csname = strNS->fromLatin1(mysql_character_set_name(mdbh->conn));

	return strNS->toMBCS(s, mdbh->csname);
}

static CWT_CHAR* __decode(CwtDBHandler *dbh, const char *s)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( !s )
		return NULL;

	if( !mdbh->csname )
		mdbh->csname = strNS->fromLatin1(mysql_character_set_name(mdbh->conn));

	return strNS->fromMBCS(s, mdbh->csname);
}

static CWT_TIME* __createTime(void)
{
	CwtMySqlTime *dt = CWT_MALLOC(CwtMySqlTime);
	cwtStrNS()->bzero(dt, sizeof(CwtMySqlTime));
	return (CWT_TIME*)dt;
}

static void __freeTime(CWT_TIME *dt)
{
	CWT_FREE(dt);
}


static void __convertTime(CWT_TIME *cwtTime, void *nativeTime)
{
	MYSQL_TIME *mysqlTime = (MYSQL_TIME*)nativeTime;

	cwtTime->year     = mysqlTime->year;
	cwtTime->mon      = mysqlTime->month;
	cwtTime->day      = mysqlTime->day;
	cwtTime->hour     = mysqlTime->hour;
	cwtTime->min      = mysqlTime->minute;
	cwtTime->sec      = mysqlTime->second;
	cwtTime->sec_part = mysqlTime->second_part;
}

static int __realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, size_t length)
{
	char *stmt_str_;
	int rc;

	CWT_ASSERT(dbh);
	CWT_ASSERT(dbh->conn);

	stmt_str_ = __cwtDBIDriver.encode((CwtDBHandler*)dbh, stmt_str);
	rc = mysql_real_query(dbh->conn, stmt_str_, (ULONG)length/*strlen(stmt_str_)*/);
	CWT_FREE(stmt_str_);

	return rc;
}


static void __mysqlCleanup(void)
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
CwtDBHandler* __connect(const CWT_CHAR *driverDSN
	, const CWT_CHAR *username
	, const CWT_CHAR *password
	, const CWT_CHAR *csname)
{
	CwtStrNS     *strNS     = cwtStrNS();
	CwtStrListNS *strlistNS = cwtStrListNS();

	static const CWT_CHAR *csname_default = _T("utf8");

	CwtMySqlDBHandler *dbh = NULL;
	BOOL ok = FALSE;
	CwtStrList opts;
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
    dbh->__base.close      = __stmtClose;
    dbh->__base.execute    = __stmtExecute;
    dbh->__base.err        = __stmtErr;
    dbh->__base.strerror   = __stmtStrerror;
	dbh->__base.bind       = __stmtBind;
	dbh->__base.bindScalar = __stmtBindScalar;
	dbh->__base.bindTime   = __stmtBindTime;
	dbh->__base.bindNull   = __stmtBindNull;
    dbh->__base.rows       = __stmtAffectedRows;
    dbh->__base.size       = __stmtNumRows;
    dbh->__base.fetchNext  = __stmtFetchNext;
    dbh->__base.fetchColumn= __stmtFetchColumn;
	dbh->auto_commit       = FALSE;
	dbh->csname            = NULL;
	dbh->errorstr          = NULL;
	dbh->sqlstate          = NULL;

    csname_   = strNS->toLatin1(csname);
    username_ = strNS->toLatin1(username);
    password_ = strNS->toLatin1(password);

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
        strlistNS->init(&opts);
        strlistNS->split(&opts, driverDSN, _T(";"));
        strlistNS->begin(&opts, &itOpts);

        ok = TRUE;

        while( strlistNS->hasMore(&itOpts) ) {
        	CWT_CHAR* opt = strlistNS->next(&itOpts);
        	printf_debug(__LOG_PREFIX _Tr("option: %s"), opt);

        	if( strNS->strncmp(_T("host="), opt, 5) == 0 ) {

        		printf_debug(__LOG_PREFIX _T("host: %s"), &opt[5]);
        		host = __cwtDBIDriver.encode((CwtDBHandler*)dbh, &opt[5]);

        	} else if( strNS->strncmp(_T("database="), opt, 9) == 0) {

        		printf_debug(__LOG_PREFIX _T("database: %s"), &opt[9]);
        		dbname = __cwtDBIDriver.encode((CwtDBHandler*)dbh, &opt[9]);

        	} else if( strNS->strncmp(_T("port="), opt, 5) == 0) {
        		port = strNS->toUINT(&opt[5], 0, &ok);
        		if( !ok ) {
        			print_error(__LOG_PREFIX _Tr("bad port value"));
        			break;
        		}
        		printf_debug(__LOG_PREFIX _T("port:     %u"), port);

        	} else if( strNS->strncmp(_T("mysql_socket="), opt, 13) == 0) {

        		sockname = __cwtDBIDriver.encode((CwtDBHandler*)dbh, &opt[13]);
        		printf_debug(__LOG_PREFIX _T("sockname: %s"), &opt[13]);

        	} else if( strNS->strncmp(_T("mysql_flags="), opt, 12) == 0) {
        	    CWT_CHAR *flagstr = &opt[12];

        	    flags |= CLIENT_REMEMBER_OPTIONS;
        	    if( strNS->strstr(flagstr, _T("COMPRESS")) )         flags |= CLIENT_COMPRESS;
        	    if( strNS->strstr(flagstr, _T("FOUND_ROWS")) )       flags |= CLIENT_FOUND_ROWS;
        	    if( strNS->strstr(flagstr, _T("IGNORE_SIGPIPE")) )   flags |= CLIENT_IGNORE_SIGPIPE;
        	    if( strNS->strstr(flagstr, _T("IGNORE_SPACE")) )     flags |= CLIENT_IGNORE_SPACE;
        	    if( strNS->strstr(flagstr, _T("INTERACTIVE")) )      flags |= CLIENT_INTERACTIVE;
        	    if( strNS->strstr(flagstr, _T("LOCAL_FILES")) )      flags |= CLIENT_LOCAL_FILES;
        	    if( strNS->strstr(flagstr, _T("MULTI_RESULTS")) )    flags |= CLIENT_MULTI_RESULTS;
        	    if( strNS->strstr(flagstr, _T("MULTI_STATEMENTS")) ) flags |= CLIENT_MULTI_STATEMENTS;

        	    printf_debug(__LOG_PREFIX _T("flags:    %lu (0x%X)"), flags, flags);
        	}
        }
        strlistNS->destroy(&opts);

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
    		__destroy(dbh);
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
static void __disconnect(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	if( !dbh )
		return;

	if( mdbh->conn ) {

		CWT_ASSERT(__nConnections > 0 );

		__destroy(mdbh);
		CWT_FREE(mdbh);

		__nConnections--;
		__mysqlCleanup();
	}
}


static void __destroy(CwtMySqlDBHandler *dbh)
{
	CWT_ASSERT(dbh);

	if( dbh->conn ) {
		mysql_close(dbh->conn);
		dbh->conn = NULL;
		CWT_FREE(dbh->csname);
		CWT_FREE(dbh->errorstr);
		CWT_FREE(dbh->sqlstate);
		/*CWT_FREE(dbh);*/
	}
}


static void __stmtDestroy(CwtMySqlStatement *sth)
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
			hash_table_free(sth->rbind_map);
			sth->rbind_map = NULL;
		}
		if( sth->meta_map ) {
			hash_table_free(sth->meta_map);
			sth->meta_map = NULL;
		}


		if( mysql_stmt_close(sth->stmt) != 0 ) {
			printf_error(__LOG_PREFIX _Tr("failed to close statement: %s"), mysql_stmt_error(__STH(sth)));
		}

		sth->stmt = NULL;
		CWT_FREE(sth->errorstr);
	}
}

static BOOL __buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[])
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


static BOOL __buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[])
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
static BOOL __func(CwtDBHandler *dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
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
			if( __buildSqlCreateDB(sql, &argv[1])
					&& __realQuery((CwtMySqlDBHandler*)dbh, stringNS->cstr(sql), stringNS->length(sql)) == 0 )
				rv = TRUE;
		} else if( strNS->strieq(_T("dropdb"), argv[0]) ) {
			if( __buildSqlDropDB(sql, &argv[1])
					&& __realQuery((CwtMySqlDBHandler*)dbh, stringNS->cstr(sql), stringNS->length(sql)) == 0)
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
static void __attr(CwtDBHandler *dbh, const CWT_CHAR *attr_name, void *attr_value)
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

static BOOL __setAutoCommit(CwtDBHandler *dbh, BOOL on)
{
	CWT_ASSERT(dbh);
	if( mysql_autocommit(__DBH(dbh), on ? 1 : 0) == 0 ) {
		((CwtMySqlDBHandler*)dbh)->auto_commit = on;
		return TRUE;
	}
	print_error(__LOG_PREFIX _Tr("failed to change autocommit mode"));
	return FALSE;
}

static BOOL __autoCommit(CwtDBHandler *dbh)
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
static CwtDBI_RC __err(CwtDBHandler *dbh)
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
static const CWT_CHAR*  __strerror(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( mdbh->errorstr ) {
		CWT_FREE(mdbh->errorstr);
		mdbh->errorstr = NULL;
	}

	mdbh->errorstr = __cwtDBIDriver.decode(dbh, mysql_error(mdbh->conn));
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
static const CWT_CHAR*  __state(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( mdbh->sqlstate ) {
		CWT_FREE(mdbh->sqlstate);
		mdbh->sqlstate = NULL;
	}

	mdbh->sqlstate = __cwtDBIDriver.decode(dbh, mysql_sqlstate(mdbh->conn));
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
static BOOL __query(CwtDBHandler *dbh, const CWT_CHAR *sql)
{
	CWT_ASSERT(dbh);

	if( __realQuery((CwtMySqlDBHandler*)dbh, sql, cwtStrNS()->strlen(sql)) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to query: %s"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh));
		return FALSE;
	}
	return TRUE;
}

static BOOL __queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length)
{
	CWT_ASSERT(dbh);

	if( __realQuery((CwtMySqlDBHandler*)dbh, sql, length) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to query: %s"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh));
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
static CwtStatement* __prepare(CwtDBHandler *dbh, const CWT_CHAR *stmt_str)
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
	sth->dbh           = dbh;
	sth->bind_params   = NULL;
	sth->is_null       = NULL;
	sth->nbind_params  = mysql_stmt_param_count(stmt);
	sth->is_bind       = FALSE;
	sth->meta          = NULL;
	sth->rbind_params  = NULL;
	sth->nrbind_params = 0;
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


static ULONGLONG __affectedRows(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return (ULONGLONG)mysql_affected_rows(__DBH(dbh));
}


static BOOL __tables(CwtDBHandler *dbh, CwtStrList *tables)
{
	CwtStrListNS *strlistNS = cwtStrListNS();

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
		mysql_data_seek(res, i);

		row = mysql_fetch_row(res);

		if( !row ) {
			break;
		}

		strlistNS->append( tables, __decode(dbh, row[0]) );

		i++;
	}

	mysql_free_result(res);

	return ok;
}



static BOOL __begin(CwtDBHandler *dbh)
{
	CwtMySqlDBHandler *mdbh = (CwtMySqlDBHandler*)dbh;

	CWT_ASSERT(mdbh);
	CWT_ASSERT(mdbh->conn);

	if( mdbh->auto_commit ) {
		if( ! __setAutoCommit(dbh, FALSE) )
			return FALSE;
	}

	return __query(dbh, _T("BEGIN WORK"));
}

static BOOL __commit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	CWT_ASSERT(((CwtMySqlDBHandler*)dbh)->conn);
	return __query(dbh, _T("COMMIT"));
}

static BOOL __rollback(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	CWT_ASSERT(((CwtMySqlDBHandler*)dbh)->conn);
	return __query(dbh, _T("ROLLBACK"));
}


static void __stmtClose(CwtStatement *sth)
{
	if( !sth )
		return;

	if( __STH(sth) ) {
		__stmtDestroy((CwtMySqlStatement*)sth);
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

static BOOL __stmtExecute(CwtStatement *sth)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	BOOL hasBlobs = FALSE;

	CWT_ASSERT(sth);
	CWT_ASSERT(msth->stmt);

	if( msth->nbind_params > 0 ) {
		if( !msth->is_bind ) {
			if( mysql_stmt_bind_param(__STH(sth), msth->bind_params) != 0 ) {
				printf_error( __LOG_PREFIX _Tr("bind parameters failed: %s\n"), msth->dbh->strerror(sth));
				return FALSE;
			}
			msth->is_bind = TRUE;
		} else {
			if( mysql_stmt_reset(msth->stmt) != 0 ) {
				printf_error( __LOG_PREFIX _Tr("unable to reset statement: %s\n"), msth->dbh->strerror(sth));
				return FALSE;
			}
		}
	}


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
		hash_table_free(msth->rbind_map);
		msth->rbind_map = NULL;
	}
	if( msth->meta_map ) {
		hash_table_free(msth->meta_map);
		msth->meta_map = NULL;
	}


	if( mysql_stmt_execute(__STH(sth)) != 0 ) {
		printf_error( __LOG_PREFIX _Tr("executing statement error: %s\n"), msth->dbh->strerror(sth));
		return FALSE;
	}

	if( msth->nrbind_params > 0 ) {
		UINT i;

		msth->rbind_map = hash_table_new(string_hash, string_equal);
		msth->meta_map = hash_table_new(string_hash, string_equal);
		hash_table_register_free_functions(msth->rbind_map, NULL/*cwtFree*/, NULL);
		hash_table_register_free_functions(msth->meta_map, NULL/*cwtFree*/, NULL);

		for( i = 0; i < msth->nrbind_params; i++ ) {
			MYSQL_FIELD *field = mysql_fetch_field_direct(msth->meta, i);

			msth->rbind_params[i].buffer = NULL;

			if( __mysqlIsBlob(field->type) ) {
				hasBlobs = TRUE;
				field->length = 0;
			} else if( __mysqlIsTime(field->type) ) {
				field->length = sizeof(MYSQL_TIME);
			} else if( __mysqlIsInteger(field->type) ) {
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
				strNS->bzero(msth->rbind_params[i].buffer, 0);

				/*printf_debug(_T("Buffer allocated: 0x%p [i=%u][sz=%lu]"), msth->rbind_params[i].buffer, i, field->length + 1);*/
			}

			CWT_ASSERT(hash_table_insert(msth->rbind_map, field->name, &msth->rbind_params[i]));
			CWT_ASSERT(hash_table_insert(msth->meta_map,  field->name, field));

		}

		if( mysql_stmt_bind_result(__STH(sth), msth->rbind_params) != 0 ) {
			printf_error( __LOG_PREFIX _Tr("failed to bind result: %s\n"), msth->dbh->strerror(sth));
			return FALSE;
		}


		if( hasBlobs ) {
			my_bool update_max_length = 1;
			mysql_stmt_attr_set(__STH(sth), STMT_ATTR_UPDATE_MAX_LENGTH, &update_max_length);
		}

		if( mysql_stmt_store_result(__STH(sth)) != 0 ) {
		    printf_error( __LOG_PREFIX _Tr("failed to cache (buffer) the result: %s\n"), msth->dbh->strerror(sth));
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

				if( __mysqlIsBlob(field->type) ) {
					msth->rbind_params[i].buffer_length = field->max_length;
					msth->rbind_params[i].buffer = CWT_MALLOCA(char, field->max_length);
				}
			}

			if( mysql_stmt_bind_result(__STH(sth), msth->rbind_params) != 0 ) {
				printf_error( __LOG_PREFIX _Tr("unable to bind BLOB value(s): %s\n"), msth->dbh->strerror(sth));
				return FALSE;
			}
	     }
	}

	return TRUE;
}

static ULONGLONG __stmtAffectedRows(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)mysql_stmt_affected_rows(__STH(sth));
}

static ULONGLONG __stmtNumRows(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)mysql_stmt_num_rows(__STH(sth));
}


static BOOL __stmtFetchNext(CwtStatement *sth)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	int rv;

	CWT_ASSERT(msth);
	CWT_ASSERT(msth->stmt);
	CWT_ASSERT(msth->dbh);

	rv = mysql_stmt_fetch(msth->stmt);

	if( rv != 0 ) {
		switch(rv) {
		case 1: /*error*/
			printf_error( __LOG_PREFIX _Tr("failed to fetch result: %s"), msth->dbh->strerror(sth));
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

static BOOL __stmtFetchColumn(CwtStatement *sth, CWT_CHAR *col, CWT_UNITYPE *value)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	CwtTypeEnum cwtType;
	MYSQL_BIND *rbind;
	MYSQL_FIELD *field;
	char *col_;

	CWT_ASSERT(msth);
	CWT_ASSERT(msth->stmt);
	CWT_ASSERT(msth->rbind_map);
	CWT_ASSERT(msth->meta_map);
	CWT_ASSERT(value);

	col_ = __encode(msth->dbh, col);
	rbind = (MYSQL_BIND*)hash_table_lookup(msth->rbind_map, col_);
	field = (MYSQL_FIELD*)hash_table_lookup(msth->meta_map, col_);
	CWT_FREE(col_);

	if( !rbind ) {
		printf_error(_T("column '%s' not found"), col);
		return FALSE;
	}
	CWT_ASSERT(field);


	if( rbind->is_null && *rbind->is_null ) {
		value->is_null = 1;
		return TRUE;
	} else {
		value->is_null = 0;
	}


	cwtType  = __fromMysqlType(rbind->buffer_type, field->flags);
	value->type = cwtType;

	switch(cwtType) {
		case CwtType_CHAR:
			value->data.sbyte_val = *((char*)rbind->buffer);
			break;
		case CwtType_UCHAR:
			value->data.byte_val = *((UCHAR*)rbind->buffer);
			break;
		case CwtType_SHORT:
			value->data.short_val = *((SHORT*)rbind->buffer);
			break;
		case CwtType_USHORT:
			value->data.ushort_val = *((USHORT*)rbind->buffer);
			break;
		case CwtType_INT:
			value->data.int_val = *((INT*)rbind->buffer);
			break;
		case CwtType_UINT:
			value->data.uint_val = *((UINT*)rbind->buffer);
			break;
		case CwtType_LONG:
			value->data.long_val = *((LONG*)rbind->buffer);
			break;
		case CwtType_ULONG:
			value->data.ulong_val = *((ULONG*)rbind->buffer);
			break;
		case CwtType_LONGLONG:
			value->data.llong_val = *((LONGLONG*)rbind->buffer);
			break;
		case CwtType_ULONGLONG:
			value->data.ullong_val = *((ULONGLONG*)rbind->buffer);
			break;
		case CwtType_FLOAT:
			value->data.float_val = *((float*)rbind->buffer);
			break;
		case CwtType_DOUBLE:
			value->data.double_val = *((double*)rbind->buffer);
			break;

		/* must be converted by convertTime function */
		case CwtType_TIME:
		case CwtType_DATE:
		case CwtType_DATETIME:
			value->data.ptr = rbind->buffer;
			value->length   = *rbind->length;
			break;

		case CwtType_BLOB:
			CWT_ASSERT(rbind->length);
			value->data.ptr = rbind->buffer;
			value->length   = *rbind->length;
			break;

		default:
		case CwtType_TEXT:
			CWT_ASSERT(rbind->length);
			value->data.ptr = rbind->buffer;
			value->length   = *rbind->length;
			((char*)value->data.ptr)[value->length] = (char)0;
			break;
	}

#ifdef __COMMENT__

	switch(cwtType) {
	case CwtType_SBYTE:
	case CwtType_BYTE:
		*((SBYTE*)value) = *((SBYTE*)rbind->buffer);
		break;
	case CwtType_SHORT:
	case CwtType_USHORT:
		*((SHORT*)value) = *((SHORT*)rbind->buffer);
		break;
	case CwtType_INT:
	case CwtType_UINT:
		*((INT*)value) = *((INT*)rbind->buffer);
		break;
	case CwtType_LONG:
	case CwtType_ULONG:
		*((LONG*)value) = *((LONG*)rbind->buffer);
		break;
    case CwtType_LONGLONG:
    case CwtType_ULONGLONG:
    	*((LONGLONG*)value) = *((LONGLONG*)rbind->buffer);
        break;
    case CwtType_DOUBLE:
    	*((double*)value) = *((double*)rbind->buffer);
        break;

    case CwtType_DATE:
    case CwtType_TIME:
    case CwtType_DATETIME: {

    case CwtType_BLOB:
    	break;

    case CwtType_TEXT:
    	break;

    default:
    case CwtType_TEXT:
        break;
	}
#endif
	return TRUE;
}

static CwtDBI_RC __stmtErr(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (CwtDBI_RC)mysql_stmt_errno(__STH(sth));
}

static const CWT_CHAR* __stmtStrerror(CwtStatement *sth)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;

	CWT_ASSERT(msth);
	CWT_ASSERT(msth->stmt);
	CWT_ASSERT(msth->dbh);

	if( msth->errorstr ) {
		CWT_FREE(msth->errorstr);
		msth->errorstr = NULL;
	}

	msth->errorstr = __cwtDBIDriver.decode(msth->dbh, mysql_stmt_error(msth->stmt));
	return msth->errorstr;
}


static BOOL __stmtBind(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, void *value, size_t *plength, BOOL is_null)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	MYSQL_BIND *bind_param;

	CWT_ASSERT(sth);

	if( index >= msth->nbind_params ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
		return FALSE;
	}

	bind_param = (MYSQL_BIND*)&msth->bind_params[index];
	bind_param->buffer_type   = __toMysqlType(cwtType);
	bind_param->buffer        = value;
	bind_param->buffer_length = plength ? (ULONG)*plength : (ULONG)0;  /*FIXME type cast problems*/
	bind_param->length        = (ULONG*)plength;  /*FIXME type cast problems*/
	bind_param->is_null       = &msth->is_null[index];
	bind_param->is_unsigned   = __isUnsigned(cwtType) ? 1 : 0;

	*bind_param->is_null      = is_null;

	if( cwtType == CwtType_TIME || cwtType == CwtType_DATE || cwtType == CwtType_DATETIME ) {
		CwtMySqlTime *tm = (CwtMySqlTime*)value;

		if( cwtType == CwtType_DATE || cwtType == CwtType_DATETIME ) {
			tm->mysql_time.year  = tm->__base.year;
			tm->mysql_time.month = tm->__base.mon;
			tm->mysql_time.day   = tm->__base.day;
		}

		if( cwtType == CwtType_TIME || cwtType == CwtType_DATETIME ) {
			tm->mysql_time.hour  = tm->__base.hour;
			tm->mysql_time.minute= tm->__base.min;
			tm->mysql_time.second= tm->__base.sec;
			tm->mysql_time.second_part = tm->__base.sec_part;
		}

		bind_param->buffer        = &tm->mysql_time;
		bind_param->buffer_length = sizeof(MYSQL_TIME);
		bind_param->length        = 0;
	}

	msth->is_bind = FALSE;
	return TRUE;
}

static BOOL __stmtBindScalar(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, void *value)
{
	CWT_ASSERT(CWT_IS_SCALAR(cwtType));
	return __stmtBind(sth, index, cwtType, value, NULL, FALSE);
}

static BOOL __stmtBindNull(CwtStatement *sth, size_t index, CwtTypeEnum cwtType)
{
	return __stmtBind(sth, index, cwtType, NULL, NULL, FALSE);
}

static BOOL __stmtBindTime(CwtStatement *sth, size_t index, CwtTypeEnum cwtType, void *value)
{
	return __stmtBind(sth, index, cwtType, value, NULL, FALSE);
}

