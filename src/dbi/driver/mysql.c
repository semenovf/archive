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


#define __LOG_PREFIX _T("mysql: ")
#define __DBH(dbh)  (((CwtMySqlDBHandler*)(dbh))->m_conn)
#define __STH(sth)  (((CwtMySqlStatement*)(sth))->m_stmt)

typedef struct CwtMySqlStatement {
	CwtStatement __base;
	MYSQL_STMT  *m_stmt;
	MYSQL_BIND  *m_bind_params;
	size_t       m_nbind_params;
	BOOL         m_is_bind; /* TRUE if parameters already bind */
} CwtMySqlStatement;


typedef struct CwtMySqlDBHandler
{
	CwtDBHandler __base;
	MYSQL       *m_conn;
	BOOL         m_auto_commit;
	CWT_CHAR    *m_csname;      /* character set name */
	CWT_CHAR    *m_errorstr;
	CWT_CHAR    *m_sqlstate;
} CwtMySqlDBHandler;


typedef struct CwtMySqlBindParam {
	enum enum_field_types field_type;
	my_bool               is_unsigned;
} CwtMySqlBindParam;


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

static void             __stmtClose(CwtStatement *sth);
static BOOL             __stmtExecute(CwtStatement *sth);
static BOOL             __stmtBind(CwtStatement *sth, size_t index, CwtTypeId type_id, void *value, size_t *plength);
static BOOL             __stmtBindArray(CwtStatement *sth, CwtBindEntry bindArray[]);
static ULONGLONG        __stmtAffectedRows(CwtStatement *sth);
static CwtDBI_RC        __stmtErr(CwtStatement *sth);
static const CWT_CHAR*  __stmtStrerror(CwtStatement *sth);



/* local helper functions */
static BOOL      __buildSqlCreateDB(CwtString *sql, CWT_CHAR *argv[]);
static BOOL      __buildSqlDropDB(CwtString *sql, CWT_CHAR *argv[]);
static char*     __encode(CwtMySqlDBHandler *dbh, const CWT_CHAR *s);
static CWT_CHAR* __decode(CwtMySqlDBHandler *dbh, const char *s);
static int       __realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, ULONG length);

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
};


DLL_API_EXPORT CwtDBIDriver* cwtDBIDriverImpl(void)
{
	return &__cwtDBIDriver;
}


static char* __encode(CwtMySqlDBHandler *dbh, CWT_CHAR *s)
{
	CwtStrNS *strNS = cwtStrNS();

	CWT_ASSERT(dbh);
	CWT_ASSERT(dbh->m_conn);

	if( !s )
		return NULL;

	if( !dbh->m_csname )
		dbh->m_csname = strNS->toLatin1(mysql_character_set_name(dbh->m_conn));

	return strNS->toMBCS(s, dbh->m_csname);
}

static CWT_CHAR* __decode(CwtMySqlDBHandler *dbh, const char *s)
{
	CwtStrNS *strNS = cwtStrNS();

	CWT_ASSERT(dbh);
	CWT_ASSERT(dbh->m_conn);

	if( !s )
		return NULL;

	if( !dbh->m_csname )
		dbh->m_csname = strNS->toLatin1(mysql_character_set_name(dbh->m_conn));

	return strNS->fromMBCS(s, dbh->m_csname);
}

static int __realQuery(CwtMySqlDBHandler *dbh, const CWT_CHAR *stmt_str, ULONG length)
{
	CwtStrNS *strNS = cwtStrNS();
	char *stmt_str_;
	int rc;

	CWT_ASSERT(dbh);
	CWT_ASSERT(dbh->m_conn);

	stmt_str_ = __encode(dbh, stmt_str);
	rc = mysql_real_query(dbh->m_conn, stmt_str_, strlen(stmt_str_));
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
	BOOL    ok = FALSE;
	char    *cs = NULL;

	if( !csname )
		csname = csname_default;

	if( strNS->streq(_T("latin1"), csname) || strNS->streq(_T("utf8"), csname) ) {
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
    dbh->__base.close     = __stmtClose;
    dbh->__base.execute   = __stmtExecute;
    dbh->__base.err       = __stmtErr;
    dbh->__base.errstr    = __stmtStrerror;
    dbh->__base.bind      = __stmtBind;
    dbh->__base.bindArray = __stmtBindArray;
    dbh->__base.rows      = __stmtAffectedRows;

    cs = strNS->toLatin1(csname);

    while( TRUE ) {
        /* initialize connection handler */
        dbh->m_conn = mysql_init(NULL);

        if( !dbh->m_conn == NULL ) {
        	printf_error(__LOG_PREFIX _Tr("unable to initialize MySQL connection handler (probably out of memory)"));
        	break;
        }

        /* Set character set */
        if( mysql_options(dbh->m_conn, MYSQL_SET_CHARSET_NAME, cs) != 0 ) {
        	printf_error(__LOG_PREFIX _Tr("failed to set character set: %s"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh));
        	break;
        }

        dbh->m_csname = strNS->strdup(csname); /* TODO need to free*/

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
        		host = __encode(dbh, &opt[5]);

        	} else if( strNS->strncmp(_T("database="), opt, 9) == 0) {

        		printf_debug(__LOG_PREFIX _T("database: %s"), &opt[9]);
        		dbname = __encode(dbh, &opt[9]);

        	} else if( strNS->strncmp(_T("port="), opt, 5) == 0) {
        		port = strNS->toUINT(&opt[5], 0, &ok);
        		if( !ok ) {
        			print_error(__LOG_PREFIX _Tr("bad port value"));
        			break;
        		}
        		printf_debug(__LOG_PREFIX _T("port:     %u"), port);

        	} else if( strNS->strncmp(_T("mysql_socket="), opt, 13) == 0) {

        		sockname = __encode(dbh, &opt[13]);
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

		if( mysql_real_connect(dbh->m_conn, host, username, password, dbname, port, sockname, flags) == NULL) {
			printf_error(__LOG_PREFIX _Tr("failed to connect the database: %s"), __cwtDBIDriver.strerror((CwtDBHandler*)dbh));
			break;
		}

		 if( mysql_autocommit(dbh->m_conn, 1) == 0 ) {
			 printf_error(__LOG_PREFIX _Tr("unable to set autocommit on"));
			 break;
		 }


        ok = TRUE;
    }

    CWT_FREE(cs);
	CWT_FREE(host);
	CWT_FREE(username);
	CWT_FREE(password);
	CWT_FREE(dbname);
	CWT_FREE(sockname);

    if( !ok ) {
    	/*TODO CWT_FREE(dbh);*/
    	if( conn ) {
    		mysql_close(conn);
    		conn = NULL;
    	}
    	__mysqlCleanup();
    } else {
        dbh->m_auto_commit = TRUE;

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

	if( mdbh->m_conn ) {

		CWT_ASSERT(__nConnections > 0 );

		mysql_close(mdbh->m_conn);
		mdbh->m_conn = NULL;
		CWT_FREE(mdbh);

		__nConnections--;
		__mysqlCleanup();
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
	BOOL usage = FALSE;

	CWT_ASSERT(dbh);
	CWT_ASSERT(argv);
	CWT_ASSERT(argv[0]);

	sql = stringNS->create();

	if( strNS->strieq(_T("admin"), func_name) ) {
		if( strNS->strieq(_T("createdb"), argv[0]) ) {
			if( __buildSqlCreateDB(sql, &argv[1])
					&& __realQuery(__DBH(dbh), stringNS->cstr(sql), stringNS->length(sql)) == 0 )
				rv = TRUE;
		} else if( strNS->strieq(_T("dropdb"), argv[0]) ) {
			if( __buildSqlDropDB(sql, &argv[1])
					&& __realQuery(__DBH(dbh), stringNS->cstr(sql), stringNS->length(sql)) == 0)
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

		*((const char**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_error(__DBH(dbh)));

	} else if(strNS->strieq(_T("hostinfo"), attr_name)) {

		*((const char**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_get_host_info(__DBH(dbh)));

	} else if(strNS->strieq(_T("info"), attr_name)) {

		*((const char**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_info(__DBH(dbh)));

	} else if(strNS->strieq(_T("insertid"), attr_name)) {

		*((ULONGLONG*)attr_value) = mysql_insert_id(__DBH(dbh));

	} else if(strNS->strieq(_T("protoinfo"), attr_name)) {

		*((UINT*)attr_value) = mysql_get_proto_info(__DBH(dbh));

	} else if(strNS->strieq(_T("serverinfo"), attr_name)) {

		*((const char**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_get_server_info(__DBH(dbh)));

	} else if(strNS->strieq(_T("serverversion"), attr_name)) {

		*((ULONG*)attr_value) = mysql_get_server_version(__DBH(dbh));

	} else if(strNS->strieq(_T("clientinfo"), attr_name)) {

		*((const char**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_get_client_info());

	} else if(strNS->strieq(_T("clientversion"), attr_name)) {

		*((ULONG*)attr_value) = mysql_get_client_version();

	} else if(strNS->strieq(_T("thread_id"), attr_name)) {

		*((ULONG*)attr_value) = mysql_thread_id(__DBH(dbh));

	} else if(strNS->strieq(_T("stat"), attr_name)) {

		*((const char**)attr_value) = CWT_STRING_OR_NULLSTR(mysql_stat(__DBH(dbh)));

	}
}

static BOOL __setAutoCommit(CwtDBHandler *dbh, BOOL on)
{
	CWT_ASSERT(dbh);
	if( mysql_autocommit(__DBH(dbh), on ? 1 : 0) == 0 ) {
		((CwtMySqlDBHandler*)dbh)->m_auto_commit = on;
		return TRUE;
	}
	print_error(__LOG_PREFIX _Tr("failed to change autocommit mode"));
	return FALSE;
}

static BOOL __autoCommit(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return ((CwtMySqlDBHandler*)dbh)->m_auto_commit;
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
	CWT_ASSERT(mdbh->m_conn);

	if( mdbh->m_errorstr ) {
		CWT_FREE(mdbh->m_errorstr);
		mdbh->m_errorstr = NULL;
	}

	mdbh->m_errorstr = __decode(mdbh, mysql_error(mdbh->m_conn));
	return mdbh->m_errorstr;
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
	CWT_ASSERT(mdbh->m_conn);

	if( mdbh->m_sqlstate ) {
		CWT_FREE(mdbh->m_sqlstate);
		mdbh->m_sqlstate = NULL;
	}

	mdbh->m_sqlstate = __decode(mdbh, mysql_sqlstate(mdbh->m_conn));
	return mdbh->m_sqlstate;

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

	if( __realQuery(__DBH(dbh), sql, cwtStrNS()->strlen(sql)) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to query: %s"), __mysqlStrerror(__DBH(dbh)));
		return FALSE;
	}
	return TRUE;
}

static BOOL __queryBin(CwtDBHandler *dbh, const CWT_CHAR *sql, size_t length)
{
	CWT_ASSERT(dbh);

	if( __realQuery(__DBH(dbh), sql, length) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to query: %s"), __mysqlStrerror(__DBH(dbh)));
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
static CwtStatement* __prepare(CwtDBHandler *dbh, const CWT_CHAR *stmt_str)
{
	CwtStrNS *strNS = cwtStrNS();

	MYSQL_STMT *stmt;
	CwtMySqlStatement *sth;
	size_t nbind_params;

	CWT_ASSERT(dbh);

	stmt = mysql_stmt_init(__DBH(dbh));

	if( !stmt ) {
		printf_error(__LOG_PREFIX _Tr("failed to prepare statement: %s"), mysql_error(__DBH(dbh)));
		return NULL;
	}

	if( mysql_stmt_prepare(stmt, stmt_str, (ULONG)strNS->strlen(stmt_str)) != 0 ) {
		printf_error(__LOG_PREFIX _Tr("failed to prepare statement: %s"), mysql_error(__DBH(dbh)));
		mysql_stmt_close(stmt);
		return NULL;
	}

	nbind_params = mysql_stmt_param_count(stmt);

	sth = CWT_MALLOC(CwtMySqlStatement);

	sth->m_stmt = stmt;
	sth->m_bind_params  = NULL;
	sth->m_nbind_params = nbind_params;
	sth->m_is_bind      = FALSE;

	if( nbind_params > 0UL ) {
		sth->m_bind_params = CWT_MALLOCA(MYSQL_BIND, nbind_params);
		strNS->bzero(sth->m_bind_params, sizeof(MYSQL_BIND) * nbind_params);
	}


	return (CwtStatement*)sth;
}


static ULONGLONG __affectedRows(CwtDBHandler *dbh)
{
	CWT_ASSERT(dbh);
	return (ULONGLONG)mysql_affected_rows(__DBH(dbh));
}

static void __stmtClose(CwtStatement *sth)
{
	if( !sth )
		return;

	if( __STH(sth) ) {

		if( ((CwtMySqlStatement*)sth)->m_bind_params )
			CWT_FREE(((CwtMySqlStatement*)sth)->m_bind_params);

		if( mysql_stmt_close(__STH(sth)) != 0 ) {
			printf_error(__LOG_PREFIX _Tr("failed to close statement: %s"), mysql_stmt_error(__STH(sth)));
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
static BOOL __stmtBind(CwtStatement *sth, size_t index, CwtTypeId type_id, void *value, size_t *plength)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
	MYSQL_BIND *bind_param;

	CWT_ASSERT(sth);

	if( index >= msth->m_nbind_params ) {
		print_error(__LOG_PREFIX _Tr("bind parameter index is out of bounds"));
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


static BOOL __stmtBindArray(CwtStatement *sth, CwtBindEntry bindArray[])
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;
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

static BOOL __stmtExecute(CwtStatement *sth)
{
	CwtMySqlStatement *msth = (CwtMySqlStatement*)sth;

	CWT_ASSERT(sth);

	if( !msth->m_is_bind ) {
		if( mysql_stmt_bind_param(__STH(sth), msth->m_bind_params) != 0 ) {
			printf_error( __LOG_PREFIX _Tr("bind parameters failed: %s\n"), mysql_stmt_error(__STH(sth)));
			return FALSE;
		}
		msth->m_is_bind = TRUE;
	}

	if( mysql_stmt_execute(__STH(sth)) != 0 ) {
		printf_error( __LOG_PREFIX _Tr("executing statement error: %s\n"), mysql_stmt_error(__STH(sth)));
		return FALSE;
	}

	return TRUE;
}

static ULONGLONG __stmtAffectedRows(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (ULONGLONG)mysql_stmt_affected_rows(__STH(sth));
}

static CwtDBI_RC __stmtErr(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return (CwtDBI_RC)mysql_stmt_errno(__STH(sth));
}

static const CWT_CHAR* __stmtStrerror(CwtStatement *sth)
{
	CWT_ASSERT(sth);
	return mysql_stmt_error(__STH(sth));
}
