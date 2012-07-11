/*
 * utils.c
 *
 *  Created on: 29.06.2012
 *      Author: user
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/dbi/dbi.h>

static const CWT_CHAR *__username = _T("root");
static const CWT_CHAR *__password = _T("rdflhfnehf");
static const CWT_CHAR *__dsn_with_flags = _T("DBI:mysql:database=stk;host=localhost;port=3306;mysql_flags=COMPRESS,FOUND_ROWS,IGNORE_SIGPIPE,IGNORE_SPACE,INTERACTIVE,LOCAL_FILES,MULTI_RESULTS,MULTI_STATEMENTS");
static const CWT_CHAR *__dsn      = _T("DBI:mysql:host=localhost;port=3306");

static const CWT_CHAR *__admin_func = _T("admin");
static CWT_CHAR       *__createdb_argv[] = {_T("createdb"), _T("cwt_test_db"), NULL};
static CWT_CHAR       *__dropdb_argv[]   = {_T("dropdb"), _T("cwt_test_db"), NULL};

static const char* __hostinfo;
static const char* __info;
static UINT        __protoinfo;
static const char* __serverinfo;
static ULONG       __serverversion;
static const char* __clientinfo;
static ULONG       __clientversion;
static ULONG       __thread_id;
static const char* __stat;
static const char* __charset;


static const CWT_CHAR *__sql_use_db       = _T("USE cwt_test_db");
static const CWT_CHAR *__sql_drop_table   = _T("DROP TABLE IF EXISTS test_table");
static const CWT_CHAR *__sql_create_table = _T("CREATE TABLE cwt_test_table(col1 INT,\
                                                 col2 VARCHAR(100),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)");
static const CWT_CHAR *__sql_insert = _T("INSERT INTO \
                       cwt_test_table(col1,col2,col3) \
                       VALUES(?,?,?)");

static const CWT_CHAR *__sql_select = _T("SELECT * from `cwt_test_table`");

int main(int argc, char *argv[])
{
	CwtDBI   *dbi   = cwtDBI();
	CwtStrNS *strNS = cwtStrNS();

	CwtDBIDriver *dbd;
	CwtDBHandler *dbh;
	CwtStatement *sth;
	CWT_CHAR *scheme;
	CWT_CHAR *driver;
	CWT_CHAR *driverDSN;

	INT    int_data;
	char   str_data[100];
	size_t str_length = 100;
	SHORT  short_data;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(25);

	dbi->parseDSN(__dsn_with_flags, &scheme, &driver, &driverDSN);
	CWT_TEST_OK(strNS->streq(_T("DBI"), scheme));
	CWT_TEST_OK(strNS->streq(_T("mysql"), driver));
	CWT_TEST_OK(strNS->streq(&__dsn_with_flags[10], driverDSN));
	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	dbi->parseDSN(__dsn, &scheme, &driver, &driverDSN);
	CWT_TEST_OK(strNS->streq(_T("DBI"), scheme));
	CWT_TEST_OK(strNS->streq(_T("mysql"), driver));
	CWT_TEST_OK(strNS->streq(&__dsn[10], driverDSN));
	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	dbd = dbi->load(__dsn_with_flags);
	CWT_TEST_FAIL2(dbd, "Unable to load driver represented by DSN");

	dbi->parseDSN(__dsn_with_flags, NULL, NULL, &driverDSN);
	dbh = dbd->connect(driverDSN, __username, __password, NULL);
	CWT_FREE(driverDSN);
	CWT_TEST_FAIL2(dbh, "May be you forgot to start MySQL service?");

/* Attributes of database handle */
	dbd->attr(dbh, _T("hostinfo"),      (void*)&__hostinfo);
	dbd->attr(dbh, _T("info"),          (void*)&__info);
	dbd->attr(dbh, _T("protoinfo"),     (void*)&__protoinfo);
	dbd->attr(dbh, _T("serverinfo"),    (void*)&__serverinfo);
	dbd->attr(dbh, _T("serverversion"), (void*)&__serverversion);
	dbd->attr(dbh, _T("clientinfo"),    (void*)&__clientinfo);
	dbd->attr(dbh, _T("clientversion"), (void*)&__clientversion);
	dbd->attr(dbh, _T("thread_id"),     (void*)&__thread_id);
	dbd->attr(dbh, _T("stat"),          (void*)&__stat);
	dbd->attr(dbh, _T("charset"),       (void*)&__charset);

	printf("Attributes of database handle---\n");
	printf("Host info:      %s\n",  __hostinfo);
	printf("Stmt info:      %s\n",  __info);
	printf("Proto info:     %u\n",  __protoinfo);
	printf("Server info:    %s\n",  __serverinfo);
	printf("Server Version: %lu\n", __serverversion);
	printf("Client info:    %s\n",  __clientinfo);
	printf("Client Version: %lu\n", __clientversion);
	printf("Thread ID:      %lu\n", __thread_id);
	printf("Stat:           %s\n",  __stat);
	printf("Character set:  %s\n",  __charset);
	printf("---\n");

	CWT_TEST_OK(dbd->autoCommit(dbh));
	CWT_TEST_OK(dbd->setAutoCommit(dbh, FALSE));
	CWT_TEST_NOK(dbd->autoCommit(dbh));
	CWT_TEST_OK(dbd->setAutoCommit(dbh, TRUE));
	CWT_TEST_OK(dbd->autoCommit(dbh));
	CWT_TEST_FAIL(dbd->func(dbh, __admin_func, __dropdb_argv));
	CWT_TEST_FAIL(dbd->func(dbh, __admin_func, __createdb_argv));

	CWT_TEST_FAIL(dbd->query(dbh, __sql_use_db));
	CWT_TEST_FAIL(dbd->query(dbh, __sql_drop_table));
	CWT_TEST_FAIL(dbd->query(dbh, __sql_create_table));

	CWT_TEST_FAIL((sth = dbd->prepare(dbh, __sql_insert)));
	CWT_TEST_FAIL(dbh->bind(sth, 0, CwtType_INT, &int_data, NULL))
	CWT_TEST_FAIL(dbh->bind(sth, 1, CwtType_STRING, str_data, &str_length));
	CWT_TEST_FAIL(dbh->bind(sth, 2, CwtType_SHORT, &short_data, NULL));


	{
		char *str_data_ = dbd->encode(dbh, _T("The quick brown fox jumps over the lazy dog"));
		int_data = CWT_INT_MAX;
		short_data = CWT_SHORT_MAX;
		strNS->bzero(str_data, 100);
		strncpy(str_data, str_data_, strlen(str_data_));
		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_FREE(str_data_);
	}

	CWT_TEST_OK(dbd->err(dbh) == 0);

	{
		char *str_data_ = dbd->encode(dbh, _T("—ъешь ещЄ этих м€гких французских булок"));
		strNS->bzero(str_data, 100);
		strncpy(str_data, str_data_, strlen(str_data_));
		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_FREE(str_data_);
	}

	CWT_TEST_OK(dbd->err(dbh) == 0);

	{

	}




	dbd->disconnect(dbh);

	CWT_END_TESTS;
}


