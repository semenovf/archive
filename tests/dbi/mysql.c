/*
 * utils.c
 *
 *  Created on: 29.06.2012
 *      Author: user
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/string.h>
#include <cwt/dbi/dbi.h>

static const CWT_CHAR *__username = "root";
static const CWT_CHAR *__password = "rdflhfnehf";
static const CWT_CHAR *__dsn_with_flags = "DBI:mysql:database=stk;host=localhost;port=3306;mysql_flags=COMPRESS,FOUND_ROWS,IGNORE_SIGPIPE,IGNORE_SPACE,INTERACTIVE,LOCAL_FILES,MULTI_RESULTS,MULTI_STATEMENTS";
static const CWT_CHAR *__dsn      = "DBI:mysql:host=localhost;port=3306";

static const CWT_CHAR *__admin_func = "admin";
static CWT_CHAR       *__createdb_argv[] = {"createdb", "cwt_test_db", NULL};
static CWT_CHAR       *__dropdb_argv[]   = {"dropdb", "cwt_test_db", NULL};

static const CWT_CHAR* __hostinfo;
static const CWT_CHAR* __info;
static UINT        __protoinfo;
static const CWT_CHAR* __serverinfo;
static ULONG       __serverversion;
static const CWT_CHAR* __clientinfo;
static ULONG       __clientversion;
static ULONG       __thread_id;
static const CWT_CHAR* __stat;


static const CWT_CHAR *__sql_use_db       = "USE cwt_test_db";
static const CWT_CHAR *__sql_drop_table   = "DROP TABLE IF EXISTS test_table";
static const CWT_CHAR *__sql_create_table = "CREATE TABLE cwt_test_table(col1 INT,\
                                                 col2 VARCHAR(40),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)";
static const CWT_CHAR *__sql_insert = "INSERT INTO \
                       cwt_test_table(col1,col2,col3) \
                       VALUES(?,?,?)";

int main(int argc, char *argv[])
{
	CwtDBHandler dbh;
	CwtStatement sth;
	CWT_CHAR *scheme;
	CWT_CHAR *driver;
	CWT_CHAR *driverDSN;

	INT int_data;
	CWT_CHAR str_data[50];
	size_t str_length;
	SHORT short_data;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(25);

	cwtDBIParseDSN(__dsn_with_flags, &scheme, &driver, &driverDSN);
	CWT_TEST_OK(cwtStrEq("DBI", scheme));
	CWT_TEST_OK(cwtStrEq("mysql", driver));
	CWT_TEST_OK(cwtStrEq(&__dsn_with_flags[10], driverDSN));
	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	cwtDBIParseDSN(__dsn, &scheme, &driver, &driverDSN);
	CWT_TEST_OK(cwtStrEq("DBI", scheme));
	CWT_TEST_OK(cwtStrEq("mysql", driver));
	CWT_TEST_OK(cwtStrEq(&__dsn[10], driverDSN));
	CWT_FREE(scheme);
	CWT_FREE(driver);
	CWT_FREE(driverDSN);

	dbh = cwtDBIConnect(__dsn_with_flags, __username, __password);
	CWT_TEST_FAIL2(dbh, "May be you forgot to start MySQL service?");

/* Attributes of database handle */
	dbh->attr(dbh, "hostinfo",      (void*)&__hostinfo);
	dbh->attr(dbh, "info",          (void*)&__info);
	dbh->attr(dbh, "protoinfo",     (void*)&__protoinfo);
	dbh->attr(dbh, "serverinfo",    (void*)&__serverinfo);
	dbh->attr(dbh, "serverversion", (void*)&__serverversion);
	dbh->attr(dbh, "clientinfo",    (void*)&__clientinfo);
	dbh->attr(dbh, "clientversion", (void*)&__clientversion);
	dbh->attr(dbh, "thread_id",     (void*)&__thread_id);
	dbh->attr(dbh, "stat",          (void*)&__stat);

	print_debug("Attributes of database handle---");
	printf_debug("Host info:      %s",  __hostinfo);
	printf_debug("Stmt info:      %s",  __info);
	printf_debug("Proto info:     %u",  __protoinfo);
	printf_debug("Server info:    %s",  __serverinfo);
	printf_debug("Server Version: %lu", __serverversion);
	printf_debug("Client info:    %s",  __clientinfo);
	printf_debug("Client Version: %lu", __clientversion);
	printf_debug("Thread ID:      %lu", __thread_id);
	printf_debug("Stat:           %s",  __stat);
	print_debug("---");

	CWT_TEST_OK(dbh->autoCommit(dbh));
	CWT_TEST_OK(dbh->setAutoCommit(dbh, FALSE));
	CWT_TEST_NOK(dbh->autoCommit(dbh));
	CWT_TEST_OK(dbh->setAutoCommit(dbh, TRUE));
	CWT_TEST_OK(dbh->autoCommit(dbh));
	CWT_TEST_FAIL(dbh->func(dbh, __admin_func, __dropdb_argv));
	CWT_TEST_FAIL(dbh->func(dbh, __admin_func, __createdb_argv));

	CWT_TEST_FAIL(dbh->query(dbh, __sql_use_db));
	CWT_TEST_FAIL(dbh->query(dbh, __sql_drop_table));
	CWT_TEST_FAIL(dbh->query(dbh, __sql_create_table));

	CWT_TEST_FAIL((sth = dbh->prepare(dbh, __sql_insert)));
	CWT_TEST_FAIL(sth->bind(sth, 0, CwtType_INT, &int_data, NULL))
	CWT_TEST_FAIL(sth->bind(sth, 1, CwtType_STRING, str_data, &str_length));
	CWT_TEST_FAIL(sth->bind(sth, 2, CwtType_SHORT, &short_data, NULL));

	int_data = CWT_INT_MAX;
	short_data = CWT_SHORT_MAX;
	cwtStrNcpy(str_data, _Tr("MySQL is the most popular database"), 50);
	str_length = cwtStrLen(str_data);

	CWT_TEST_FAIL(sth->execute(sth));
	CWT_TEST_OK(sth->rows(sth) == 1UL);

	CWT_TEST_OK(dbh->err(dbh) == 0);

	dbh->disconnect(dbh);

	dbh = cwtDBIConnect(__dsn, __username, __password);
	CWT_TEST_OK(dbh);
	dbh->disconnect(dbh);

	CWT_END_TESTS;
}


