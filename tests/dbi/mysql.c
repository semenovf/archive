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
#include <cwt/stdio.h>
#include <cwt/utils.h>
#include <cwt/dbi/dbi.h>
#include <string.h>

static const CWT_CHAR *__username = _T("root");
static const CWT_CHAR *__password = _T("rdflhfnehf");
static const CWT_CHAR *__dsn_with_flags = _T("DBI:mysql:database=stk;host=127.0.0.1;port=3306;mysql_flags=COMPRESS,FOUND_ROWS,IGNORE_SIGPIPE,IGNORE_SPACE,INTERACTIVE,LOCAL_FILES,MULTI_RESULTS,MULTI_STATEMENTS");
static const CWT_CHAR *__dsn      = _T("DBI:mysql:host=127.0.0.1;port=3306");

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

#define TABLE_NAME  _T("cwt_test_table")
#define TABLE0_NAME _T("cwt_test_table0")

static const CWT_CHAR *__sql_use_db       = _T("USE cwt_test_db");
static const CWT_CHAR *__sql_drop_table   = _T("DROP TABLE IF EXISTS ") TABLE_NAME;
static const CWT_CHAR *__sql_create_table0 = _T("CREATE TABLE ") TABLE0_NAME _T("(col1 INT,\
                                                 col2 VARCHAR(100),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)");

static const CWT_CHAR *__sql_create_table =
	_T("CREATE TABLE ") TABLE_NAME
	_T("(sbyte_val TINYINT\
	, byte_val    TINYINT UNSIGNED\
    , short_val   SMALLINT\
	, ushort_val  SMALLINT UNSIGNED\
	, int_val     INT\
	, uint_val    INT UNSIGNED\
	, long_val    INT\
	, ulong_val   INT UNSIGNED\
	, llong_val   BIGINT\
	, ullong_val  BIGINT UNSIGNED\
	, date_val    DATE\
	, time_val    TIME\
    , dt_val      DATETIME\
    , ts_val      TIMESTAMP\
	, vchar_val   VARCHAR(100))");

static const CWT_CHAR *__sql_insert
	= _T("INSERT INTO\
     cwt_test_table(\
	  sbyte_val\
	, byte_val\
	, short_val\
	, ushort_val\
	, int_val\
	, uint_val\
	, long_val\
	, ulong_val\
	, llong_val\
	, ullong_val\
	, date_val\
	, time_val\
	, dt_val\
	, vchar_val)\
    VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

static const CWT_CHAR *__sql_select =
	_T("SELECT\
	  sbyte_val\
	, byte_val\
	, short_val\
	, ushort_val\
	, int_val\
	, uint_val\
	, long_val\
	, ulong_val\
	, llong_val\
	, ullong_val\
	, date_val\
	, time_val\
	, dt_val\
	, vchar_val\
	from `cwt_test_table`");

int main(int argc, char *argv[])
{
	CwtDBI     *dbi   = cwtDBI();
	CwtStrNS   *strNS = cwtStrNS();
	CwtStdioNS *stdioNS = cwtStdioNS();

	CwtDBIDriver *dbd;
	CwtDBHandler *dbh;
	CwtStatement *sth;
	CWT_CHAR *scheme;
	CWT_CHAR *driver;
	CWT_CHAR *driverDSN;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(107);

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
	CWT_TEST_FAIL(dbd->query(dbh, __sql_create_table0));


	/* Show tables */
	{
		CwtStrListNS *strlistNS = cwtStrListNS();
		CwtStrList tables;

		strlistNS->init(&tables);

		CWT_TEST_OK(dbd->tables(dbh, &tables));

		CWT_TEST_FAIL(tables.count == 2);

		stdioNS->printf(_T("Table [0]: %s\n"),  strlistNS->at(&tables, 0));
		stdioNS->printf(_T("Table [1]: %s\n"),  strlistNS->at(&tables, 1));

		CWT_TEST_OK(
				strNS->streq(TABLE_NAME, strlistNS->at(&tables, 0))
			||  strNS->streq(TABLE0_NAME, strlistNS->at(&tables, 0)));

		CWT_TEST_OK(strNS->streq(TABLE_NAME, strlistNS->at(&tables, 1))
			|| strNS->streq(TABLE0_NAME, strlistNS->at(&tables, 1)));
		strlistNS->destroy(&tables);
	}


	{
		SBYTE     sbyte_val;
		BYTE      byte_val;
		SHORT     short_val;
		USHORT    ushort_val;
		INT       int_val;
		UINT      uint_val;
		LONG      long_val;
		ULONG     ulong_val;
		LONGLONG  llong_val;
		ULONGLONG ullong_val;

		CWT_TIME *cwtDate;
		CWT_TIME *cwtTime;
		CWT_TIME *cwtDateTime;

		char     *str_data = dbd->encode(dbh, _T("The quick brown fox jumps over the lazy dog"));
		size_t    str_length = strlen(str_data);

		CWT_TEST_FAIL((sth = dbd->prepare(dbh, __sql_insert)));

		cwtDate = dbd->createTime();
		cwtTime = dbd->createTime();
		cwtDateTime = dbd->createTime();

		/* Times, dates, blobs and texts must be initialized before they are binding */
		cwtUtilsNS()->now(cwtDate);
		cwtUtilsNS()->now(cwtTime);
		cwtUtilsNS()->now(cwtDateTime);

		CWT_TEST_FAIL(dbh->bindScalar(sth, 0, CwtType_SBYTE, &sbyte_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 1, CwtType_BYTE,  &byte_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 2, CwtType_SHORT, &short_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 3, CwtType_USHORT, &ushort_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 4, CwtType_INT, &int_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 5, CwtType_UINT,  &uint_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 6, CwtType_LONG, &long_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 7, CwtType_ULONG, &ulong_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 8, CwtType_LONGLONG, &llong_val));
		CWT_TEST_FAIL(dbh->bindScalar(sth, 9, CwtType_ULONGLONG, &ullong_val));

		CWT_TEST_FAIL(dbh->bindTime(sth, 10, CwtType_DATE, cwtDate));
		CWT_TEST_FAIL(dbh->bindTime(sth, 11, CwtType_TIME, cwtTime));
		CWT_TEST_FAIL(dbh->bindTime(sth, 12, CwtType_DATETIME, cwtDateTime));
		CWT_TEST_FAIL(dbh->bind(sth, 13, CwtType_TEXT, str_data, &str_length, FALSE));

		/* insert first row */
		sbyte_val   = CWT_SBYTE_MAX;
		byte_val    = CWT_BYTE_MAX;
		short_val   = CWT_SHORT_MAX;
		ushort_val  = CWT_USHORT_MAX;
		int_val     = CWT_INT_MAX;
		uint_val    = CWT_UINT_MAX;
		long_val    = CWT_LONG_MAX;
		ulong_val   = CWT_ULONG_MAX;
		llong_val   = CWT_LONGLONG_MAX;
		ullong_val  = CWT_ULONGLONG_MAX;

		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_FREE(str_data);

		CWT_TEST_OK(dbd->err(dbh) == 0);

		/* insert second row */
		str_data = dbd->encode(dbh, _T("Съешь ещё этих мягких французских булок, да выпей чаю"));
		str_length = strlen(str_data);

		sbyte_val   = CWT_SBYTE_MIN;
		byte_val    = 0;
		short_val   = CWT_SHORT_MIN;
		ushort_val  = 0;
		int_val     = CWT_INT_MIN;
		uint_val    = 0U;
		long_val    = CWT_LONG_MIN;
		ulong_val   = 0UL;
		llong_val   = CWT_LONGLONG_MIN;
		ullong_val  = 0ULL;

		CWT_TEST_FAIL(dbh->bind(sth, 13, CwtType_TEXT, str_data, &str_length, FALSE));

		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_FREE(str_data);

		CWT_TEST_OK(dbd->err(dbh) == 0);

		dbh->close(sth);

		/* Make selection */
		CWT_TEST_FAIL((sth = dbd->prepare(dbh, __sql_select)));
		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->size(sth) == 2UL);
		/*printf("Returned rows: %lu\n", dbh->size(sth));*/

		{
			CWT_UNITYPE sbyte_val;
			CWT_UNITYPE byte_val;
			CWT_UNITYPE short_val;
			CWT_UNITYPE ushort_val;
			CWT_UNITYPE int_val;
			CWT_UNITYPE uint_val;
			CWT_UNITYPE long_val;
			CWT_UNITYPE ulong_val;
			CWT_UNITYPE llong_val;
			CWT_UNITYPE ullong_val;
			CWT_UNITYPE date_val;
			CWT_UNITYPE time_val;
			CWT_UNITYPE dt_val;

			CWT_TIME cwtTimeTmp;

			/* fetch first row */
			CWT_TEST_FAIL( dbh->fetchNext(sth) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("sbyte_val"), &sbyte_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("byte_val"), &byte_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("short_val"), &short_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("ushort_val"), &ushort_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("int_val"), &int_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("uint_val"), &uint_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("long_val"), &long_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("ulong_val"), &ulong_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("llong_val"), &llong_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("ullong_val"), &ullong_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("date_val"), &date_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("time_val"), &time_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("dt_val"), &dt_val) );
			CWT_TEST_OK(sbyte_val.data.sbyte_val == CWT_SBYTE_MAX);
			CWT_TEST_OK(byte_val.data.byte_val == CWT_BYTE_MAX);
			CWT_TEST_OK(short_val.data.short_val == CWT_SHORT_MAX);
			CWT_TEST_OK(ushort_val.data.ushort_val == CWT_USHORT_MAX);
			CWT_TEST_OK(int_val.data.int_val == CWT_INT_MAX);
			CWT_TEST_OK(uint_val.data.uint_val == CWT_UINT_MAX);
			CWT_TEST_OK(long_val.data.long_val == CWT_LONG_MAX);
			CWT_TEST_OK(ulong_val.data.ulong_val == CWT_ULONG_MAX);
			CWT_TEST_OK(llong_val.data.llong_val == CWT_LONGLONG_MAX);
			CWT_TEST_OK(ullong_val.data.ullong_val == CWT_ULONGLONG_MAX);

			dbd->convertTime(&cwtTimeTmp, date_val.data.ptr);
			CWT_TEST_OK(cwtTimeTmp.year == cwtDate->year);
			CWT_TEST_OK(cwtTimeTmp.mon  == cwtDate->mon);
			CWT_TEST_OK(cwtTimeTmp.day  == cwtDate->day);

			dbd->convertTime(&cwtTimeTmp, time_val.data.ptr);
			CWT_TEST_OK(cwtTimeTmp.hour == cwtTime->hour);
			CWT_TEST_OK(cwtTimeTmp.min == cwtTime->min);
			CWT_TEST_OK(cwtTimeTmp.sec == cwtTime->sec);

			dbd->convertTime(&cwtTimeTmp, dt_val.data.ptr);
			CWT_TEST_OK(cwtTimeTmp.year == cwtDateTime->year);
			CWT_TEST_OK(cwtTimeTmp.mon  == cwtDateTime->mon);
			CWT_TEST_OK(cwtTimeTmp.day  == cwtDateTime->day);
			CWT_TEST_OK(cwtTimeTmp.hour == cwtDateTime->hour);
			CWT_TEST_OK(cwtTimeTmp.min == cwtDateTime->min);
			CWT_TEST_OK(cwtTimeTmp.sec == cwtDateTime->sec);

			/* fetch second row */
			CWT_TEST_FAIL( dbh->fetchNext(sth) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("sbyte_val"), &sbyte_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("byte_val"), &byte_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("short_val"), &short_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("ushort_val"), &ushort_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("int_val"), &int_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("uint_val"), &uint_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("long_val"), &long_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("ulong_val"), &ulong_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("llong_val"), &llong_val) );
			CWT_TEST_FAIL( dbh->fetchColumn(sth, _T("ullong_val"), &ullong_val) );
			CWT_TEST_OK(sbyte_val.data.sbyte_val == CWT_SBYTE_MIN);
			CWT_TEST_OK(byte_val.data.byte_val == 0);
			CWT_TEST_OK(short_val.data.short_val == CWT_SHORT_MIN);
			CWT_TEST_OK(ushort_val.data.ushort_val == 0);
			CWT_TEST_OK(int_val.data.int_val == CWT_INT_MIN);
			CWT_TEST_OK(uint_val.data.uint_val == 0);
			CWT_TEST_OK(long_val.data.long_val == CWT_LONG_MIN);
			CWT_TEST_OK(ulong_val.data.ulong_val == 0UL);
			CWT_TEST_OK(llong_val.data.llong_val == CWT_LONGLONG_MIN);
			CWT_TEST_OK(ullong_val.data.ullong_val == 0ULL);

			CWT_TEST_NOK( dbh->fetchNext(sth) ); /* no more records */
		}

		dbh->close(sth);

		dbd->freeTime(cwtTime);
		dbd->freeTime(cwtDate);
		dbd->freeTime(cwtDateTime);
	}

	CWT_TEST_OK(dbd->err(dbh) == 0);

	dbd->disconnect(dbh);

	CWT_END_TESTS;
}


