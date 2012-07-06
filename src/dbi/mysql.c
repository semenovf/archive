/*
 * mysql.c
 *
 *  Created on: 03.07.2012
 *      Author: wladt
 */

#include <cwt/dbi/dbi.h>
#include <mysql/my_global.h>
#include <mysql/my_sys.h>
#include <mysql/mysql.h>
#include <cwt/logger.h>
#include <cwt/string.h>
#include <cwt/strlist.h>
#include <cwt/strutils.h>
#include <cwt/strbuf.h>

#define __LOG_PREFIX "mysql: "
#define __DBH(dbh)  (((CwtMySqlDBHandler)(dbh))->m_conn)
#define __STH(sth)  (((CwtMySqlStatement)(sth))->m_stmt)

typedef struct CwtMySqlStatement {
	struct CwtStatementBase __base;
	MYSQL_STMT *m_stmt;
	MYSQL_BIND *m_bind_params;
	size_t      m_nbind_params;
	BOOL        m_is_bind; /* TRUE if parameters already bind */
} *CwtMySqlStatement;


typedef struct CwtMySqlDBHandler
{
	struct CwtDBHandlerBase __base;
	MYSQL *m_conn;
	BOOL   m_auto_commit;
} *CwtMySqlDBHandler;


typedef struct CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;


/* DBI API functions implementations */
CwtDBHandler        __connect(const CWT_CHAR *dsn, const CWT_CHAR *username, const CWT_CHAR *password);
static void         __disconnect(CwtDBHandler dbh);
static BOOL         __func(CwtDBHandler dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[]);
static void         __attr(CwtDBHandler dbh, const CWT_CHAR *attr_name, void *attr_value);
static BOOL         __setAutoCommit(CwtDBHandler dbh, BOOL on);
static BOOL         __autoCommit(CwtDBHandler dbh);
static CwtDBI_RC    __err(CwtDBHandler dbh);
static const CWT_CHAR*  __errstr(CwtDBHandler dbh);
static const CWT_CHAR*  __state(CwtDBHandler dbh);
static BOOL         __query(CwtDBHandler dbh, const CWT_CHAR *sql);
static BOOL         __queryBin(CwtDBHandler dbh, const CWT_CHAR *sql, size_t length);
static CwtStatement __prepare(CwtDBHandler dbh, const CWT_CHAR *statement);
static ULONGLONG    __affectedRows(CwtDBHandler dbh);

static void         __stmtClose(CwtStatement sth);
static BOOL         __stmtExecute(CwtStatement sth);
static BOOL         __stmtBind(CwtStatement sth, size_t index, CwtTypeId type_id, void *value, size_t *plength);
static BOOL         __stmtBindArray(CwtStatement sth, CwtBindEntry bindArray[]);
static ULONGLONG    __stmtAffectedRows(CwtStatement sth);
static CwtDBI_RC    __stmtErr(CwtStatement sth);
static const CWT_CHAR*  __stmtErrstr(CwtStatement sth);



/* local helper functions */
static BOOL __buildSqlCreateDB(CwtStringBufferPtr sql, CWT_CHAR *argv[]);
static BOOL __buildSqlDropDB(CwtStringBufferPtr sql, CWT_CHAR *argv[]);


static BOOL __nConnections = 0; /* number of connections */


static CwtMySqlBindParam __mySqlTypesMapping[] = {
	  { MYSQL_TYPE_NULL,       0 }   /* CwtType_NULL */
	, { MYSQL_TYPE_TINY,       0 }   /* CwtType_CHAR */
	, { MYSQL_TYPE_TINY,       0 }   /* CwtType_UCHAR */
	, { MYSQL_TYPE_SHORT,      0 }   /* CwtType_SHORT */
	, { MYSQL_TYPE_SHORT,      1 }   /* CwtType_USHORT */
	, { MYSQL_TYPE_LONG,       0 }   /* CwtType_INT */
	, { MYSQL_TYPE_LONG,       1 }   /* CwtType_UINT */
	, { MYSQL_TYPE_LONG,       0 }   /* CwtType_LONG */
	, { MYSQL_TYPE_LONG,       1 }   /* CwtType_ULONG */
	, { MYSQL_TYPE_LONGLONG,   0 }   /* CwtType_LONGLONG */
	, { MYSQL_TYPE_LONGLONG,   1 }   /* CwtType_ULONGLONG */
	, { MYSQL_TYPE_FLOAT,      0 }   /* CwtType_FLOAT */
	, { MYSQL_TYPE_DOUBLE,     0 }   /* CwtType_DOUBLE */
	, { MYSQL_TYPE_VAR_STRING, 0 }   /* CwtType_STRING */
	, { MYSQL_TYPE_BLOB,       0 }   /* CwtType_TEXT */
	, { MYSQL_TYPE_BLOB,       0 }   /* CwtType_BLOB */
	, { MYSQL_TYPE_TIME,       0 }   /* CwtType_TIME */
	, { MYSQL_TYPE_DATE,       0 }   /* CwtType_DATE */
	, { MYSQL_TYPE_TIMESTAMP,  0 }   /* CwtType_TIMESTAMP */
};


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
CwtDBHandler __connect(const CWT_CHAR *driverDSN, const CWT_CHAR *username, const CWT_CHAR *password)
{
	CwtMySqlDBHandler dbh = NULL;
	CwtStringListPtr opts = NULL;
	CwtStringListIterator itOpts;
	BOOL ok = FALSE;
	CWT_CHAR *host = NULL;
	CWT_CHAR *dbname = NULL;
	UINT port = 0;
	CWT_CHAR *sockname = NULL;
	ULONG flags = 0UL;
	MYSQL *conn;

	while( TRUE ) {

		if( !__nConnections ) {
		    /* initialize client library */
		    if( mysql_library_init(0, NULL, NULL) ) {
		        printf_error(__LOG_PREFIX "failed to initialize MySQL library");
		        break;
		    }
		}
	    /* initialize connection handler */
	    conn = mysql_init(NULL);

	    if( conn == NULL ) {
	    	printf_error(__LOG_PREFIX "unable to initialize MySQL connection handler (probably out of memory)");
	    	break;
	    }

	    opts = cwtNewStringList();
	    cwtStringListSplit(opts, driverDSN, ";");
	    cwtStringListBegin(opts, &itOpts);

	    ok = TRUE;

	    while( cwtStringListHasMore(&itOpts) ) {
	    	CWT_CHAR* opt = cwtStringListNext(&itOpts);
	    	printf_debug(__LOG_PREFIX "option: %s", opt);
	    	if( cwtStrNcmp("host=", opt, 5) == 0 ) {
	    		host = &opt[5];
	    	} else if(cwtStrNcmp("database=", opt, 9) == 0) {
	    		dbname = &opt[9];
	    	} else if(cwtStrNcmp("port=", opt, 5) == 0) {
	    		port = cwtStrToUINT(&opt[5], 0, &ok);
	    		if( !ok ) {
	    			print_error(__LOG_PREFIX "bad port value");
	    			break;
	    		}
	    	} else if(cwtStrNcmp("mysql_socket=", opt, 13) == 0) {
	    		sockname = &opt[13];
	    	} else if(cwtStrNcmp("mysql_flags=", opt, 12) == 0) {
	    	    CWT_CHAR *flagstr = &opt[12];

	    	    if( cwtStrStr(flagstr, "COMPRESS") )         flags |= CLIENT_COMPRESS;
	    	    if( cwtStrStr(flagstr, "FOUND_ROWS") )       flags |= CLIENT_FOUND_ROWS;
	    	    if( cwtStrStr(flagstr, "IGNORE_SIGPIPE") )   flags |= CLIENT_IGNORE_SIGPIPE;
	    	    if( cwtStrStr(flagstr, "IGNORE_SPACE") )     flags |= CLIENT_IGNORE_SPACE;
	    	    if( cwtStrStr(flagstr, "INTERACTIVE") )      flags |= CLIENT_INTERACTIVE;
	    	    if( cwtStrStr(flagstr, "LOCAL_FILES") )      flags |= CLIENT_LOCAL_FILES;
	    	    if( cwtStrStr(flagstr, "MULTI_RESULTS") )    flags |= CLIENT_MULTI_RESULTS;
	    	    if( cwtStrStr(flagstr, "MULTI_STATEMENTS") ) flags |= CLIENT_MULTI_STATEMENTS;
	    	}
	    }

	    if( !ok )
	    	break;

	    ok = FALSE;

	    print_trace(__LOG_PREFIX "Connecting...");
	    printf_debug(__LOG_PREFIX "host:     %s", CWT_STRING_OR_NULLSTR(host));
	    printf_debug(__LOG_PREFIX "dbname:   %s", CWT_STRING_OR_NULLSTR(dbname));
	    printf_debug(__LOG_PREFIX "port:     %u", port);
	    printf_debug(__LOG_PREFIX "sockname: %s", CWT_STRING_OR_NULLSTR(sockname));
	    printf_debug(__LOG_PREFIX "flags:    %lu (0x%X)", flags, flags);

	    if( mysql_real_connect(
	    		  conn
	    		, host
	    		, username
	    		, password
	    		, dbname
	    		, port
	    		, sockname
	    		, flags) == NULL) {
	    	printf_error(__LOG_PREFIX "failed to connect the database: %s", mysql_error(conn));
	        break;
	    }

	    if( mysql_autocommit(conn, 1) != 0 ) {
	    	printf_error(__LOG_PREFIX "unable to set autocommit on");
	        break;
	    }


		ok = TRUE;
		break;
	}

    if( opts )
    	cwtDeleteStringList(opts);

    if( !ok ) {
    	if( conn ) {
    		mysql_close(conn);
    		conn = NULL;
    	}
    	__mysqlCleanup();
    } else {
        dbh = CWT_MALLOC(struct CwtMySqlDBHandler);
        dbh->__base.disconnect     = __disconnect;
        dbh->__base.func           = __func;
        dbh->__base.attr           = __attr;
        dbh->__base.setAutoCommit  = __setAutoCommit;
        dbh->__base.autoCommit     = __autoCommit;
        dbh->__base.err            = __err;
        dbh->__base.errstr         = __errstr;
        dbh->__base.state          = __state;
        dbh->__base.query          = __query;
        dbh->__base.queryBin       = __queryBin;
        dbh->__base.prepare        = __prepare;
        dbh->__base.rows          = __affectedRows;

        dbh->m_conn = conn;
        dbh->m_auto_commit = TRUE;

    	__nConnections++;
    }

	return (CwtDBHandler)dbh;
}


/**
 * @brief Closes connection, deallocates the connection handle.
 *
 * @param dbh DBI handle
 */
static void __disconnect(CwtDBHandler dbh)
{
	CwtMySqlDBHandler mdbh = (CwtMySqlDBHandler)dbh;

	if( !dbh )
		return;

	if( mdbh->m_conn ) {

		CWT_ASSERT(__nConnections > 0 );

		mysql_close(mdbh->m_conn);
		mdbh->m_conn = NULL;
		CWT_FREE(mdbh);

		__nConnections--;
		__mysqlCleanup();
	}
}



static BOOL __buildSqlCreateDB(CwtStringBufferPtr sql, CWT_CHAR *argv[])
{
	int i = 0;
	BOOL usage = FALSE;

	cwtStringBufferAppend(sql, "CREATE DATABASE IF NOT EXISTS ");

	if( argv[i] ) {
		cwtStringBufferAppend(sql, argv[i]);
		i++;

		while( argv[i] )  {
			if( cwtStrEqi("CHARACTER SET", argv[i]) && argv[i+1] ) {
				cwtStringBufferAppendChar(sql, ' ');
				cwtStringBufferAppend(sql, "CHARACTER SET");
				cwtStringBufferAppendChar(sql, ' ');
				cwtStringBufferAppend(sql, argv[i+1]);
				i += 2;
			} else if( cwtStrEqi("COLLATE", argv[i]) && argv[i+1] ) {
				cwtStringBufferAppendChar(sql, ' ');
				cwtStringBufferAppend(sql, "COLLATE");
				cwtStringBufferAppendChar(sql, ' ');
				cwtStringBufferAppend(sql, argv[i+1]);
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
		printf_warn(__LOG_PREFIX "usage: dbh->func: createdb dbname ['CHARACTER SET' charset_name] ['COLLATE' collation_name]");
		return FALSE;
	}

	return TRUE;
}


static BOOL __buildSqlDropDB(CwtStringBufferPtr sql, CWT_CHAR *argv[])
{
	int i = 0;
	BOOL usage = FALSE;

	cwtStringBufferAppend(sql, "DROP DATABASE IF EXISTS ");

	if( argv[i] ) {
		cwtStringBufferAppendChar(sql, '`');
		cwtStringBufferAppend(sql, argv[i]);
		cwtStringBufferAppendChar(sql, '`');
		i++;
	} else {
		usage = TRUE;
	}

	if( usage ) {
		printf_warn(__LOG_PREFIX "usage: dbh->func: dropdb dbname");
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
static BOOL __func(CwtDBHandler dbh, const CWT_CHAR *func_name, CWT_CHAR *argv[])
{
	CwtStringBufferPtr sql;
	BOOL rv = FALSE;
	BOOL usage = FALSE;

	CWT_ASSERT(dbh);
	CWT_ASSERT(argv);
	CWT_ASSERT(argv[0]);

	sql = cwtNewStringBuffer();

	if( cwtStrEqi("admin", func_name) ) {
		if( cwtStrEqi("createdb", argv[0]) ) {
			if( __buildSqlCreateDB(sql, &argv[1])
					&& mysql_real_query(__DBH(dbh), cwtStringBufferCstr(sql), cwtStringBufferSize(sql)) == 0 )
				rv = TRUE;
		} else if( cwtStrEqi("dropdb", argv[0]) ) {
			if( __buildSqlDropDB(sql, &argv[1])
					&& mysql_real_query(__DBH(dbh), cwtStringBufferCstr(sql), cwtStringBufferSize(sql)) == 0)
				rv = TRUE;
		}
	} else {
		printf_warn(__LOG_PREFIX "unsupported function name: %s", CWT_STRING_OR_NULLSTR(func_name));
	}

	cwtDeleteStringBuffer(sql);
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
 *
 */
static void __attr(CwtDBHandler dbh, const CWT_CHAR *attr_name, void *attr_value)
{
	CWT_ASSERT(dbh);
	CWT_ASSERT(attr_name);
	CWT_ASSERT(attr_value);

	if( cwtStrEqi("errno", attr_name) ) {

		*((UINT*)attr_value) = mysql_errno(((CwtMySqlDBHandler)dbh)->m_conn);

	} else if(cwtStrEqi("error", attr_name)) {

		*((const CWT_CHAR**)attr_value)
			= CWT_STRING_OR_NULLSTR(mysql_error(((CwtMySqlDBHandler)dbh)->m_conn));

	} else if(cwtStrEqi("hostinfo", attr_name)) {

		*((const CWT_CHAR**)attr_value)
			= CWT_STRING_OR_NULLSTR(mysql_get_host_info(((CwtMySqlDBHandler)dbh)->m_conn));

	} else if(cwtStrEqi("info", attr_name)) {

		*((const CWT_CHAR**)attr_value)
			= CWT_STRING_OR_NULLSTR(mysql_info(((CwtMySqlDBHandler)dbh)->m_conn));

	} else if(cwtStrEqi("insertid", attr_name)) {

		*((ULONGLONG*)attr_value) = mysql_insert_id(((CwtMySqlDBHandler)dbh)->m_conn);

	} else if(cwtStrEqi("protoinfo", attr_name)) {

		*((UINT*)attr_value) = mysql_get_proto_info(((CwtMySqlDBHandler)dbh)->m_conn);

	} else if(cwtStrEqi("serverinfo", attr_name)) {

		*((const CWT_CHAR**)attr_value)
			= CWT_STRING_OR_NULLSTR(mysql_get_server_info(((CwtMySqlDBHandler)dbh)->m_conn));

	} else if(cwtStrEqi("serverversion", attr_name)) {

		*((ULONG*)attr_value) = mysql_get_server_version(((CwtMySqlDBHandler)dbh)->m_conn);

	} else if(cwtStrEqi("clientinfo", attr_name)) {

		*((const CWT_CHAR**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_get_client_info());

	} else if(cwtStrEqi("clientversion", attr_name)) {

		*((ULONG*)attr_value) = mysql_get_client_version();

	} else if(cwtStrEqi("thread_id", attr_name)) {

		*((ULONG*)attr_value) = mysql_thread_id(((CwtMySqlDBHandler)dbh)->m_conn);

	} else if(cwtStrEqi("stat", attr_name)) {

		*((const CWT_CHAR**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_stat(((CwtMySqlDBHandler)dbh)->m_conn));

	}
}

static BOOL __setAutoCommit(CwtDBHandler dbh, BOOL on)
{
	CWT_ASSERT(dbh);
	if( mysql_autocommit(((CwtMySqlDBHandler)dbh)->m_conn, on ? 1 : 0) == 0 ) {
		((CwtMySqlDBHandler)dbh)->m_auto_commit = on;
		return TRUE;
	}
	print_error(__LOG_PREFIX "failed to change autocommit mode");
	return FALSE;
}

static BOOL __autoCommit(CwtDBHandler dbh)
{
	CWT_ASSERT(dbh);
	return ((CwtMySqlDBHandler)dbh)->m_auto_commit;
}


/**
 *
 * @param dbh Database handle
 * @return
 *
 * @see mysql_errno()
 */
static CwtDBI_RC __err(CwtDBHandler dbh)
{
	CWT_ASSERT(dbh);
	return (CwtDBI_RC)mysql_errno(((CwtMySqlDBHandler)dbh)->m_conn);
}

/**
 *
 * @param dbh Database handle
 * @return
 *
 * @see mysql_error()
 */
static const CWT_CHAR*  __errstr(CwtDBHandler dbh)
{
	CWT_ASSERT(dbh);
	return mysql_error(((CwtMySqlDBHandler)dbh)->m_conn);
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
static const CWT_CHAR*  __state(CwtDBHandler dbh)
{
	CWT_ASSERT(dbh);
	return mysql_sqlstate(((CwtMySqlDBHandler)dbh)->m_conn);
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
static BOOL __query(CwtDBHandler dbh, const CWT_CHAR *sql)
{
	CWT_ASSERT(dbh);

	if( mysql_real_query(__DBH(dbh), sql, cwtStrLen(sql)) != 0 ) {
		printf_error(__LOG_PREFIX "failed to query: %s", mysql_error(__DBH(dbh)));
		return FALSE;
	}
	return TRUE;
}

static BOOL __queryBin(CwtDBHandler dbh, const CWT_CHAR *sql, size_t length)
{
	CWT_ASSERT(dbh);

	if( mysql_real_query(__DBH(dbh), sql, length) != 0 ) {
		printf_error(__LOG_PREFIX "failed to query: %s", mysql_error(__DBH(dbh)));
		return FALSE;
	}
	return TRUE;
}

/**
 *
 * @param dbh
 * @param statement
 * @return
 */
static CwtStatement __prepare(CwtDBHandler dbh, const CWT_CHAR *stmt_str)
{
	MYSQL_STMT *stmt;
	CwtMySqlStatement sth;
	size_t nbind_params;

	CWT_ASSERT(dbh);

	stmt = mysql_stmt_init(__DBH(dbh));

	if( !stmt ) {
		printf_error(__LOG_PREFIX "failed to prepare statement: %s", mysql_error(__DBH(dbh)));
		return NULL;
	}

	if( mysql_stmt_prepare(stmt, stmt_str, (ULONG)cwtStrLen(stmt_str)) != 0 ) {
		printf_error(__LOG_PREFIX "failed to prepare statement: %s", mysql_error(__DBH(dbh)));
		mysql_stmt_close(stmt);
		return NULL;
	}

	nbind_params = mysql_stmt_param_count(stmt);

	sth = CWT_MALLOC(struct CwtMySqlStatement);
	sth->__base.close     = __stmtClose;
	sth->__base.execute   = __stmtExecute;
	sth->__base.err       = __stmtErr;
	sth->__base.errstr    = __stmtErrstr;
	sth->__base.bind      = __stmtBind;
	sth->__base.bindArray = __stmtBindArray;
	sth->__base.rows      = __stmtAffectedRows;

	sth->m_stmt = stmt;
	sth->m_bind_params  = NULL;
	sth->m_nbind_params = nbind_params;
	sth->m_is_bind      = FALSE;

	if( nbind_params > 0UL ) {
		sth->m_bind_params = CWT_MALLOCA(MYSQL_BIND, nbind_params);
		cwtMemZero(sth->m_bind_params, sizeof(MYSQL_BIND) * nbind_params);
	}


	return (CwtStatement)sth;
}


static ULONGLONG __affectedRows(CwtDBHandler dbh)
{
	CWT_ASSERT(dbh);
	return (ULONGLONG)mysql_affected_rows(__DBH(dbh));
}

static void __stmtClose(CwtStatement sth)
{
	if( !sth )
		return;

	if( __STH(sth) ) {

		if( ((CwtMySqlStatement)sth)->m_bind_params )
			CWT_FREE(((CwtMySqlStatement)sth)->m_bind_params);

		if( mysql_stmt_close(__STH(sth)) != 0 ) {
			printf_error(__LOG_PREFIX "failed to close statement: %s", mysql_stmt_error(__STH(sth)));
		}

		__STH(sth) = NULL;
		CWT_FREE(sth);
	}
}


/**
 * @brief Used to prepare bind input data for the parameter marker in the SQL statement.
 *
 * @param mysql_bind Target MYSQL_BIND value
 * @param type_id Value type
 * @param value Pointer to value
 * @param len Value buffer length. Not matter if value is a numeric.
 */

/* TODO need is_null support */
/* TODO need TIME/DATE/TIMESTAMP support */
static BOOL __stmtBind(CwtStatement sth, size_t index, CwtTypeId type_id, void *value, size_t *plength)
{
	CwtMySqlStatement msth = (CwtMySqlStatement)sth;
	MYSQL_BIND *bind_param;

	CWT_ASSERT(sth);

	if( index >= msth->m_nbind_params ) {
		print_error(__LOG_PREFIX "bind parameter index is out of bounds");
		return FALSE;
	}

	bind_param = (MYSQL_BIND*)&msth->m_bind_params[index];
	bind_param->buffer_type = __mySqlTypesMapping[type_id].field_type;
	/*bind_param->is_null    = 0;*/

	if( cwtIsNumberTypeId(type_id) ) {
		/* This is a number type, so there is no need to specify buffer_length */
		bind_param->buffer  = value;
		bind_param->length  = 0;
		bind_param->is_unsigned  = __mySqlTypesMapping[type_id].is_unsigned;
	} else if ( type_id == CwtType_TIME || type_id == CwtType_DATE || type_id == CwtType_TIMESTAMP ) {

	} else { /* STRING, TEXT or BLOB */
		bind_param->buffer        = value;
		bind_param->buffer_length = *plength;
		bind_param->length        = plength;
	}

	return TRUE;
}


static BOOL __stmtBindArray(CwtStatement sth, CwtBindEntry bindArray[])
{
	CwtMySqlStatement msth = (CwtMySqlStatement)sth;
	size_t i;

	CWT_ASSERT(sth);

	for( i = 0; i < msth->m_nbind_params; i++ ) {
		if( !__stmtBind(sth, i, bindArray[i].type_id, bindArray[i].value, bindArray[i].plength) )
			return FALSE;
	}

	return TRUE;
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

static BOOL __stmtExecute(CwtStatement sth)
{
	CwtMySqlStatement msth = (CwtMySqlStatement)sth;

	CWT_ASSERT(sth);

	if( !msth->m_is_bind ) {
		if( mysql_stmt_bind_param(__STH(sth), msth->m_bind_params) != 0 ) {
			printf_error( __LOG_PREFIX "bind parameters failed: %s\n", mysql_stmt_error(__STH(sth)));
			return FALSE;
		}
		msth->m_is_bind = TRUE;
	}

	if( mysql_stmt_execute(__STH(sth)) != 0 ) {
		printf_error( __LOG_PREFIX "executing statement error: %s\n", mysql_stmt_error(__STH(sth)));
		return FALSE;
	}

	return TRUE;
}

static ULONGLONG __stmtAffectedRows(CwtStatement sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)mysql_stmt_affected_rows(__STH(sth));
}

static CwtDBI_RC __stmtErr(CwtStatement sth)
{
	CWT_ASSERT(sth);
	return (CwtDBI_RC)mysql_stmt_errno(__STH(sth));
}

static const CWT_CHAR* __stmtErrstr(CwtStatement sth)
{
	CWT_ASSERT(sth);
	return mysql_stmt_error(__STH(sth));
}
