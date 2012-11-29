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
static const CWT_CHAR *__dsn_with_flags = _T("DBI:mysql:host=127.0.0.1;port=3306;mysql_flags=COMPRESS,FOUND_ROWS,IGNORE_SIGPIPE,IGNORE_SPACE,INTERACTIVE,LOCAL_FILES,MULTI_RESULTS,MULTI_STATEMENTS");
/*static const CWT_CHAR *__dsn      = _T("DBI:mysql:host=127.0.0.1;port=3306");*/

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

#define TABLE_NAME      _T("cwt_test_table")
#define TABLE0_NAME     _T("cwt_test_table0")
#define TABLE_DATE      _T("cwt_test_date")
#define TABLE_TIME      _T("cwt_test_time")
#define TABLE_INT       _T("cwt_test_int")
#define TABLE_TEXT      _T("cwt_test_text")

static const CWT_CHAR *__sql_use_db       = _T("USE cwt_test_db");
static const CWT_CHAR *__sql_drop_table   = _T("DROP TABLE IF EXISTS ") TABLE_NAME;
static const CWT_CHAR *__sql_create_table0 = _T("CREATE TABLE ") TABLE0_NAME _T("(col1 INT,\
                                                 col2 VARCHAR(100),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)");


static const CWT_CHAR *__sql_create_table_date =
	_T("CREATE TABLE ") TABLE_DATE
	_T("(date_val DATE)");

static const CWT_CHAR *__sql_create_table_time =
	_T("CREATE TABLE ") TABLE_TIME
	_T("(time_val TIME)");

static const CWT_CHAR *__sql_create_table_int =
	_T("CREATE TABLE ") TABLE_INT
	_T("(bool_val TINYINT\
	, sbyte_val TINYINT\
	, byte_val    TINYINT UNSIGNED\
    , short_val   SMALLINT\
	, ushort_val  SMALLINT UNSIGNED\
	, int_val     INT\
	, uint_val    INT UNSIGNED\
	, long_val    INT\
	, ulong_val   INT UNSIGNED\
	, llong_val   BIGINT\
	, ullong_val  BIGINT UNSIGNED)");


static const CWT_CHAR *__sql_create_table_text =
	_T("CREATE TABLE ") TABLE_TEXT
	_T("(vchar_val  VARCHAR(100))");

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

/*
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
*/

/*static const CWT_CHAR *__sql_select =
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
	from `cwt_test_table`");*/

int main(int argc, char *argv[])
{
	CwtDBI     *dbi   = cwt_dbi_ns();
	CwtStrNS   *strNS = cwt_str_ns();
	CwtStdioNS *stdioNS = cwt_stdio_ns();
	CwtUniTypeNS *utNS = cwt_unitype_ns();

	CwtDBHandler *dbh;
	CwtStatement *sth;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(58);

	dbh = dbi->connect(__dsn_with_flags, __username, __password, NULL);
	CWT_TEST_FAIL2(dbh, _T("May be you forgot to start MySQL service?"));

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
	CWT_TEST_FAIL(dbi->query(dbh, __sql_create_table_date));
	CWT_TEST_FAIL(dbi->query(dbh, __sql_create_table_time));
	CWT_TEST_FAIL(dbi->query(dbh, __sql_create_table_int));
	CWT_TEST_FAIL(dbi->query(dbh, __sql_create_table_text));

	/* Show tables */
	{
		CwtStrListNS *strlistNS = cwt_strList_ns();
		CwtStrList *tables;
		size_t i;

		tables = strlistNS->create();

		CWT_TEST_OK(dbi->tables(dbh, tables));

		CWT_TEST_FAIL(tables->count == 6);

		for( i = 0; i < tables->count; i++ ) {
			stdioNS->printf(_T("Table [%d]: %s\n"), i, strlistNS->at(tables, 0));
		}

		CWT_TEST_OK(strlistNS->find(tables, TABLE_NAME, NULL));
		CWT_TEST_OK(strlistNS->find(tables, TABLE0_NAME, NULL));
		CWT_TEST_OK(strlistNS->find(tables, TABLE_DATE, NULL));
		CWT_TEST_OK(strlistNS->find(tables, TABLE_TIME, NULL));
		CWT_TEST_OK(strlistNS->find(tables, TABLE_INT, NULL));
		CWT_TEST_OK(strlistNS->find(tables, TABLE_TEXT, NULL));
		strlistNS->free(tables);
	}

	{
		CwtUniType *ut;
		const CWT_CHAR *sql_insert =
			_T("INSERT INTO ") TABLE_TEXT
			_T("(vchar_val) VALUES(?)");
		const CWT_CHAR *text0 = _T("The quick brown fox jumps over the lazy dog");
		const CWT_CHAR *text1 = _T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog")
		_T("The quick brown fox jumps over the lazy dog");
		const CWT_CHAR* text_ru = _T("Съешь ещё этих мягких французских булок, да выпей чаю");

		CWT_TEST_FAIL((sth = dbi->prepare(dbh, sql_insert)));
		ut = dbi->bindTextByIndex(sth, 0, 100);

		CWT_TEST_FAIL(dbi->setTEXT(sth, ut, text0, strNS->strLen(text0)));
		CWT_TEST_FAIL(dbh->execute(sth));

		CWT_TEST_FAIL(!dbi->setTEXT(sth, ut, text1, strNS->strLen(text1)));

		CWT_TEST_FAIL(dbi->setTEXT(sth, ut, text_ru, strNS->strLen(text_ru)));
		CWT_TEST_FAIL(dbh->execute(sth));

		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_TEST_OK(dbi->err(dbh) == 0);
		dbi->close(sth);
	}

	{
		CwtUniType *ut;
		CWT_TIME cwtm;
		const CWT_CHAR *sql_insert =
			_T("INSERT INTO ") TABLE_DATE
			_T("(date_val) VALUES(?)");

		CWT_TEST_FAIL((sth = dbi->prepare(dbh, sql_insert)));
		ut = dbi->bindByIndex(sth, 0, CwtType_DATE);

		cwt_utils_ns()->now(&cwtm);
		CWT_TEST_FAIL(dbi->setDATE(sth, ut, &cwtm));

		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_TEST_OK(dbi->err(dbh) == 0);
		dbi->close(sth);
	}



	{
		const CWT_CHAR *sql_insert =
			_T("INSERT INTO ") TABLE_INT
			_T(" (bool_val, sbyte_val, byte_val, short_val, ushort_val, int_val, uint_val, long_val, ulong_val, llong_val, ullong_val)")
            _T(" VALUES(?,?,?,?,?,?,?,?,?,?,?)");

		int i;
		CwtUniType *ut[11];

		CWT_TEST_FAIL((sth = dbi->prepare(dbh, sql_insert)));
		ut[0]  = dbi->bindByIndex(sth, 0, CwtType_BOOL);
		ut[1]  = dbi->bindByIndex(sth, 1, CwtType_SBYTE);
		ut[2]  = dbi->bindByIndex(sth, 2, CwtType_BYTE);
		ut[3]  = dbi->bindByIndex(sth, 3, CwtType_SHORT);
		ut[4]  = dbi->bindByIndex(sth, 4, CwtType_USHORT);
		ut[5]  = dbi->bindByIndex(sth, 5, CwtType_INT);
		ut[6]  = dbi->bindByIndex(sth, 6, CwtType_UINT);
		ut[7]  = dbi->bindByIndex(sth, 7, CwtType_LONG);
		ut[8]  = dbi->bindByIndex(sth, 8, CwtType_ULONG);
		ut[9]  = dbi->bindByIndex(sth, 9, CwtType_LONGLONG);
		ut[10]  = dbi->bindByIndex(sth, 10, CwtType_ULONGLONG);

		dbi->setBOOL     (sth, ut[0],  TRUE);
		dbi->setSBYTE    (sth, ut[1],  CWT_SBYTE_MAX);
		dbi->setBYTE     (sth, ut[2],  CWT_BYTE_MAX);
		dbi->setSHORT    (sth, ut[3],  CWT_SHORT_MAX);
		dbi->setUSHORT   (sth, ut[4],  CWT_USHORT_MAX);
		dbi->setINT      (sth, ut[5],  CWT_INT_MAX);
		dbi->setUINT     (sth, ut[6],  CWT_UINT_MAX);
		dbi->setLONG     (sth, ut[7],  CWT_LONG_MAX);
		dbi->setULONG    (sth, ut[8],  CWT_ULONG_MAX);
		dbi->setLONGLONG (sth, ut[9],  CWT_LONGLONG_MAX);
		dbi->setULONGLONG(sth, ut[10], CWT_ULONGLONG_MAX);

		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_TEST_OK(dbi->err(dbh) == 0);

		cwt_logger_ns()->info(_T("inserting 1000 records, please wait ..."));

		for( i = 0; i < 1000; i++ ) {
			if( !dbh->execute(sth) )
				break;

			if( i % 50 == 0 ) {
				printf("%4d records inserted\r", i);
			}
		}
		printf("%4d records inserted\n", i);

		CWT_TEST_OK2(i == 1000, _T("1000 records inserted"));

		CWT_TEST_OK(dbi->err(dbh) == 0);
		dbi->close(sth);
	}

	{
		CwtUniType *ut;
		CWT_TIME cwtm;
		CWT_TIME res;
		BOOL ok;
		const CWT_CHAR *sql_insert =
			_T("INSERT INTO ") TABLE_TIME
			_T("(time_val) VALUES(?)");
		const CWT_CHAR *sql_select = _T("SELECT time_val FROM ") TABLE_TIME;

		CWT_TEST_FAIL((sth = dbi->prepare(dbh, sql_insert)));
		ut = dbi->bindByIndex(sth, 0, CwtType_TIME);

		cwt_utils_ns()->now(&cwtm);
		CWT_TEST_FAIL(dbi->setTIME(sth, ut, &cwtm));

		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->rows(sth) == 1UL);
		CWT_TEST_OK(dbi->err(dbh) == 0);
		dbi->close(sth);


		/* Fetching time */
		CWT_TEST_FAIL((sth = dbi->prepare(dbh, sql_select)));
		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->size(sth) == 1UL);

		ut = utNS->create();
		CWT_TEST_FAIL( dbi->fetchNext(sth) );
		CWT_TEST_FAIL( dbi->fetchColumn(sth, _T("time_val"), ut) );

		CWT_TEST_OK(utNS->toTIME(ut, &res, &ok) && ok);
		CWT_TEST_OK(res.hour == cwtm.hour);
		CWT_TEST_OK(res.min  == cwtm.min);
		CWT_TEST_OK(res.sec  == cwtm.sec);

		utNS->free(ut);

	}

	if( 0 ) {
#ifdef __COMMENT__
		/* Make selection */
		CWT_TEST_FAIL((sth = dbd->prepare(dbh, __sql_select)));
		CWT_TEST_FAIL(dbh->execute(sth));
		CWT_TEST_OK(dbh->size(sth) == 1002UL);
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
#endif
	}

	CWT_TEST_OK(dbi->err(dbh) == 0);

	dbi->disconnect(dbh);

	CWT_END_TESTS;
}

