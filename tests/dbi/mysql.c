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
	CwtUniTypeNS *utNS = cwtUniTypeNS();

	CwtDBHandler *dbh;
	CwtStatement *sth;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(107);

	dbh = dbi->connect(__dsn_with_flags, __username, __password, NULL);
	CWT_TEST_FAIL2(dbh, "May be you forgot to start MySQL service?");

/* Attributes of database handle */
	dbi->attr(dbh, _T("hostinfo"),      (void*)&__hostinfo);
	dbi->attr(dbh, _T("info"),          (void*)&__info);
	dbi->attr(dbh, _T("protoinfo"),     (void*)&__protoinfo);
	dbi->attr(dbh, _T("serverinfo"),    (void*)&__serverinfo);
	dbi->attr(dbh, _T("serverversion"), (void*)&__serverversion);
	dbi->attr(dbh, _T("clientinfo"),    (void*)&__clientinfo);
	dbi->attr(dbh, _T("clientversion"), (void*)&__clientversion);
	dbi->attr(dbh, _T("thread_id"),     (void*)&__thread_id);
	dbi->attr(dbh, _T("stat"),          (void*)&__stat);
	dbi->attr(dbh, _T("charset"),       (void*)&__charset);

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

	CWT_TEST_OK(dbi->autoCommit(dbh));
	CWT_TEST_OK(dbi->setAutoCommit(dbh, FALSE));
	CWT_TEST_NOK(dbi->autoCommit(dbh));
	CWT_TEST_OK(dbi->setAutoCommit(dbh, TRUE));
	CWT_TEST_OK(dbi->autoCommit(dbh));
	CWT_TEST_FAIL(dbi->func(dbh, __admin_func, __dropdb_argv));
	CWT_TEST_FAIL(dbi->func(dbh, __admin_func, __createdb_argv));

	CWT_TEST_FAIL(dbi->query(dbh, __sql_use_db));
	CWT_TEST_FAIL(dbi->query(dbh, __sql_drop_table));
	CWT_TEST_FAIL(dbi->query(dbh, __sql_create_table));
	CWT_TEST_FAIL(dbi->query(dbh, __sql_create_table0));


	/* Show tables */
	{
		CwtStrListNS *strlistNS = cwtStrListNS();
		CwtStrList *tables;

		tables = strlistNS->create();

		CWT_TEST_OK(dbi->tables(dbh, tables));

		CWT_TEST_FAIL(tables->count == 2);

		stdioNS->printf(_T("Table [0]: %s\n"),  strlistNS->at(tables, 0));
		stdioNS->printf(_T("Table [1]: %s\n"),  strlistNS->at(tables, 1));

		CWT_TEST_OK(
				strNS->streq(TABLE_NAME, strlistNS->at(tables, 0))
			||  strNS->streq(TABLE0_NAME, strlistNS->at(tables, 0)));

		CWT_TEST_OK(strNS->streq(TABLE_NAME, strlistNS->at(tables, 1))
			|| strNS->streq(TABLE0_NAME, strlistNS->at(tables, 1)));
		strlistNS->free(tables);
	}


	{
		CwtUniType *ut[14];
		CWT_TIME cwtDate;
		CWT_TIME cwtTime;
		CWT_TIME cwtDateTime;

/*
		cwtDate = dbi->createTime(dbh);
		cwtTime = dbi->createTime(dbh);
		cwtDateTime = dbi->createTime(dbh);
*/

		/* Times, dates, blobs and texts must be initialized before they are binding */
		cwtUtilsNS()->now(&cwtDate);
		cwtUtilsNS()->now(&cwtTime);
		cwtUtilsNS()->now(&cwtDateTime);

		/* insert first row */
		utNS->setSByte    (&ut[0], CWT_SBYTE_MAX);
		utNS->setByte     (&ut[1], CWT_BYTE_MAX);
		utNS->setShort    (&ut[2], CWT_SHORT_MAX);
		utNS->setUShort   (&ut[3], CWT_USHORT_MAX);
		utNS->setInt      (&ut[4], CWT_INT_MAX);
		utNS->setUInt     (&ut[5], CWT_UINT_MAX);
		utNS->setLong     (&ut[6], CWT_LONG_MAX);
		utNS->setULong    (&ut[7], CWT_ULONG_MAX);
		utNS->setLongLong (&ut[8], CWT_LONGLONG_MAX);
		utNS->setULongLong(&ut[9], CWT_ULONGLONG_MAX);
		utNS->setTime     (&ut[10], cwtDate);
		utNS->setTime     (&ut[11], cwtTime);
		utNS->setTime     (&ut[12], cwtDateTime);
		utNS->setText     (&ut[13], _T("The quick brown fox jumps over the lazy dog"));

		CWT_CHAR *str_data  = _T("The quick brown fox jumps over the lazy dog"); /*dbi->encode(dbh, _T("The quick brown fox jumps over the lazy dog"));*/
		size_t   str_length = strNS->strlen(str_data);
		CWT_CHAR *str_data1 = _T("Съешь ещё этих мягких французских булок, да выпей чаю");

		CWT_TEST_FAIL((sth = dbi->prepare(dbh, __sql_insert)));
		dbi->startBindings(sth);
		ut[0] = dbi->bind(sth, CwtType_SBYTE);
		ut[1] = dbi->bind(sth, CwtType_BYTE);
		ut[2] = dbi->bind(sth, CwtType_SHORT);
		ut[3] = dbi->bind(sth, CwtType_USHORT);
		ut[4] = dbi->bind(sth, CwtType_INT);
		ut[5] = dbi->bind(sth, CwtType_UINT);
		ut[6] = dbi->bind(sth, CwtType_LONG);
		ut[7] = dbi->bind(sth, CwtType_ULONG);
		ut[8] = dbi->bind(sth, CwtType_LONGLONG);
		ut[9] = dbi->bind(sth, CwtType_ULONGLONG);
		ut[10] = dbi->bind(sth, CwtType_DATE);
		ut[11] = dbi->bind(sth, CwtType_TIME);
		ut[12] = dbi->bind(sth, CwtType_DATETIME);
		ut[13] = dbi->bindText(sth, 255);
		dbi->finishBindings(sth);

		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);

		/*...*/
		dbi->cleanupBindings(sth);


/*
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
*/

		CWT_FREE(str_data);

		CWT_TEST_OK(dbd->err(dbh) == 0);

		/* insert second row */
		str_data   = str_data1;
		str_length = strNS->strlen(str_data);

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
			CwtUniType sbyte_val;
			CwtUniType byte_val;
			CwtUniType short_val;
			CwtUniType ushort_val;
			CwtUniType int_val;
			CwtUniType uint_val;
			CwtUniType long_val;
			CwtUniType ulong_val;
			CwtUniType llong_val;
			CwtUniType ullong_val;
			CwtUniType date_val;
			CwtUniType time_val;
			CwtUniType dt_val;

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
			CWT_TEST_OK(sbyte_val.value.sbyte_val == CWT_SBYTE_MAX);
			CWT_TEST_OK(byte_val.value.byte_val == CWT_BYTE_MAX);
			CWT_TEST_OK(short_val.value.short_val == CWT_SHORT_MAX);
			CWT_TEST_OK(ushort_val.value.ushort_val == CWT_USHORT_MAX);
			CWT_TEST_OK(int_val.value.int_val == CWT_INT_MAX);
			CWT_TEST_OK(uint_val.value.uint_val == CWT_UINT_MAX);
			CWT_TEST_OK(long_val.value.long_val == CWT_LONG_MAX);
			CWT_TEST_OK(ulong_val.value.ulong_val == CWT_ULONG_MAX);
			CWT_TEST_OK(llong_val.value.llong_val == CWT_LONGLONG_MAX);
			CWT_TEST_OK(ullong_val.value.ullong_val == CWT_ULONGLONG_MAX);

			dbd->convertTime(&cwtTimeTmp, date_val.value.ptr);
			CWT_TEST_OK(cwtTimeTmp.year == cwtDate->year);
			CWT_TEST_OK(cwtTimeTmp.mon  == cwtDate->mon);
			CWT_TEST_OK(cwtTimeTmp.day  == cwtDate->day);

			dbd->convertTime(&cwtTimeTmp, time_val.value.ptr);
			CWT_TEST_OK(cwtTimeTmp.hour == cwtTime->hour);
			CWT_TEST_OK(cwtTimeTmp.min == cwtTime->min);
			CWT_TEST_OK(cwtTimeTmp.sec == cwtTime->sec);

			dbd->convertTime(&cwtTimeTmp, dt_val.value.ptr);
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
			CWT_TEST_OK(sbyte_val.value.sbyte_val == CWT_SBYTE_MIN);
			CWT_TEST_OK(byte_val.value.byte_val == 0);
			CWT_TEST_OK(short_val.value.short_val == CWT_SHORT_MIN);
			CWT_TEST_OK(ushort_val.value.ushort_val == 0);
			CWT_TEST_OK(int_val.value.int_val == CWT_INT_MIN);
			CWT_TEST_OK(uint_val.value.uint_val == 0);
			CWT_TEST_OK(long_val.value.long_val == CWT_LONG_MIN);
			CWT_TEST_OK(ulong_val.value.ulong_val == 0UL);
			CWT_TEST_OK(llong_val.value.llong_val == CWT_LONGLONG_MIN);
			CWT_TEST_OK(ullong_val.value.ullong_val == 0ULL);

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


