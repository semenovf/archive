#include <cwt/test.h>
#include <cwt/dbd.hpp>
#include <cwt/dbh.hpp>
#include <cwt/sth.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/filesystem.hpp>
#include <cwt/shared_ptr.hpp>

using namespace cwt;

void test_base()
{
	static const char *sql_create_table_0 =
			"CREATE TABLE IF NOT EXISTS test_table_0 (col1 INT,"
			"col2 VARCHAR(100),"
			"col3 SMALLINT,"
			"col4 TIMESTAMP)";

	static const char *sql_create_table_1 =
			"CREATE TABLE IF NOT EXISTS test_table_1 (col1 INT,"
			"col2 VARCHAR(100),"
			"col3 SMALLINT,"
			"col4 TIMESTAMP)";

    shared_ptr<DbHandler> dbh(DbHandler::open("sqlite3:///tmp/test.db?mode=rwc"));
    CWT_TEST_FAIL(dbh.get());

    CWT_TEST_OK(dbh->query(sql_create_table_0));
    CWT_TEST_OK(dbh->tableExists("test_table_0"));

    CWT_TEST_OK(dbh->query(sql_create_table_1));
    CWT_TEST_OK(dbh->tableExists("test_table_1"));

    Vector<String> tables = dbh->tables();
    CWT_TEST_FAIL(tables.size() == 2);
    CWT_TEST_OK(tables[0] == String("test_table_0") || tables[0] == String("test_table_1"));
    CWT_TEST_OK(tables[1] == String("test_table_0") || tables[1] == String("test_table_1"));

    CWT_TEST_OK(dbh->query("DROP TABLE IF EXISTS test_table_0"));
    CWT_TEST_OK(dbh->query("DROP TABLE IF EXISTS test_table_1"));
    CWT_TEST_OK(dbh->tables().size() == 0);

    dbh->close(); // Not necessary, automatically called from destructor.
}

// source: http://www.sqlite.org/datatype3.html
void test_sqlite3_collation()
{
	shared_ptr<DbHandler> dbh(DbHandler::open("sqlite3:///tmp/test.db?mode=rwc"));

	if(dbh->tableExists("t1")) {
		dbh->query("DROP TABLE IF EXISTS t1");
	}

	CWT_TEST_OK(dbh->query(
			"CREATE TABLE t1("
			"    x INTEGER PRIMARY KEY,"
			"    a,"                 /* collating sequence BINARY */
			"    b COLLATE BINARY,"  /* collating sequence BINARY */
			"    c COLLATE RTRIM,"   /* collating sequence RTRIM  */
			"    d COLLATE NOCASE"   /* collating sequence NOCASE */
			")"));

	shared_ptr<DbStatement> sth(dbh->prepare("INSERT INTO t1 VALUES(?,?,?,?,?)"));
	CWT_TEST_FAIL(sth.get());

	sth->bind(int(1)).bind(String("abc")).bind(String("abc")).bind(String("abc  ")).bind(String("abc"));
	CWT_TEST_OK(sth->exec());

	sth->bind(int(2)).bind(String("abc")).bind(String("abc")).bind(String("abc")).bind(String("ABC"));
	CWT_TEST_OK(sth->exec());

	sth->bind(int(3)).bind(String("abc")).bind(String("abc")).bind(String("abc ")).bind(String("Abc"));
	CWT_TEST_OK(sth->exec());

	sth->bind(int(4)).bind(String("abc")).bind(String("abc ")).bind(String("ABC")).bind(String("abc"));
	CWT_TEST_OK(sth->exec());

	/* Text comparison a=b is performed using the BINARY collating sequence. */
	// SELECT x FROM t1 WHERE a = b ORDER BY x;
	// --result 1 2 3
	sth.reset(dbh->prepare("SELECT x FROM t1 WHERE a = b ORDER BY x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	Vector<UniType> result;
	while (sth->fetchRowArray(result))
		;

	CWT_TEST_FAIL(result.size() == 3);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 2);
	CWT_TEST_OK(result[2].toInt() == 3);

	/* Text comparison a=b is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 WHERE a = b COLLATE RTRIM ORDER BY x;
	// --result 1 2 3 4
	sth.reset(dbh->prepare("SELECT x FROM t1 WHERE a = b COLLATE RTRIM ORDER BY x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;

	CWT_TEST_FAIL(result.size() == 4);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 2);
	CWT_TEST_OK(result[2].toInt() == 3);
	CWT_TEST_OK(result[3].toInt() == 4);

	/* Text comparison d=a is performed using the NOCASE collating sequence. */
	// SELECT x FROM t1 WHERE d = a ORDER BY x;
	// --result 1 2 3 4
	sth.reset(dbh->prepare("SELECT x FROM t1 WHERE d = a ORDER BY x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;

	CWT_TEST_FAIL(result.size() == 4);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 2);
	CWT_TEST_OK(result[2].toInt() == 3);
	CWT_TEST_OK(result[3].toInt() == 4);

	/* Text comparison a=d is performed using the BINARY collating sequence. */
	// SELECT x FROM t1 WHERE a = d ORDER BY x;
	// --result 1 4
	sth.reset(dbh->prepare("SELECT x FROM t1 WHERE a = d ORDER BY x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;

	CWT_TEST_FAIL(result.size() == 2);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 4);

	/* Text comparison 'abc'=c is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 WHERE 'abc' = c ORDER BY x;
	// --result 1 2 3
	sth.reset(dbh->prepare("SELECT x FROM t1 WHERE 'abc' = c ORDER BY x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;

	CWT_TEST_FAIL(result.size() == 3);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 2);
	CWT_TEST_OK(result[2].toInt() == 3);

	/* Text comparison c='abc' is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 WHERE c = 'abc' ORDER BY x;
	// --result 1 2 3
	sth.reset(dbh->prepare("SELECT x FROM t1 WHERE c = 'abc' ORDER BY x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;
	CWT_TEST_FAIL(result.size() == 3);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 2);
	CWT_TEST_OK(result[2].toInt() == 3);

	/* Grouping is performed using the NOCASE collating sequence (Values
	 ** 'abc', 'ABC', and 'Abc' are placed in the same group). */
	// SELECT count(*) FROM t1 GROUP BY d ORDER BY 1;
	//--result 4
	sth.reset(dbh->prepare("SELECT count(*) FROM t1 GROUP BY d ORDER BY 1"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;
	CWT_TEST_FAIL(result.size() == 1);
	CWT_TEST_OK(result[0].toInt() == 4);

	/* Grouping is performed using the BINARY collating sequence.  'abc' and
	** 'ABC' and 'Abc' form different groups */
	// SELECT count(*) FROM t1 GROUP BY (d || '') ORDER BY 1;
	// --result 1 1 2
	sth.reset(dbh->prepare("SELECT count(*) FROM t1 GROUP BY (d || '') ORDER BY 1"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;
	CWT_TEST_FAIL(result.size() == 3);
	CWT_TEST_OK(result[0].toInt() == 1);
	CWT_TEST_OK(result[1].toInt() == 1);
	CWT_TEST_OK(result[2].toInt() == 2);

	/* Sorting or column c is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 ORDER BY c, x;
	// --result 4 1 2 3
	sth.reset(dbh->prepare("SELECT x FROM t1 ORDER BY c, x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;
	CWT_TEST_FAIL(result.size() == 4);
	CWT_TEST_OK(result[0].toInt() == 4);
	CWT_TEST_OK(result[1].toInt() == 1);
	CWT_TEST_OK(result[2].toInt() == 2);
	CWT_TEST_OK(result[3].toInt() == 3);

	/* Sorting of (c||'') is performed using the BINARY collating sequence. */
	// SELECT x FROM t1 ORDER BY (c||''), x;
	// --result 4 2 3 1
	sth.reset(dbh->prepare("SELECT x FROM t1 ORDER BY (c||''), x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;
	CWT_TEST_FAIL(result.size() == 4);
	CWT_TEST_OK(result[0].toInt() == 4);
	CWT_TEST_OK(result[1].toInt() == 2);
	CWT_TEST_OK(result[2].toInt() == 3);
	CWT_TEST_OK(result[3].toInt() == 1);

	/* Sorting of column c is performed using the NOCASE collating sequence. */
	// SELECT x FROM t1 ORDER BY c COLLATE NOCASE, x;
	// --result 2 4 3 1
	sth.reset(dbh->prepare("SELECT x FROM t1 ORDER BY c COLLATE NOCASE, x"));
	CWT_TEST_FAIL(sth.get());
	CWT_TEST_OK(sth->exec());

	result.clear();
	while (sth->fetchRowArray(result))
		;
	CWT_TEST_FAIL(result.size() == 4);
	CWT_TEST_OK(result[0].toInt() == 2);
	CWT_TEST_OK(result[1].toInt() == 4);
	CWT_TEST_OK(result[2].toInt() == 3);
	CWT_TEST_OK(result[3].toInt() == 1);

	CWT_TEST_OK(dbh->query("DROP TABLE IF EXISTS t1"));
}


void test_columns()
{
	shared_ptr<DbHandler> dbh(DbHandler::open("sqlite3:///tmp/test.db?mode=rwc"));

	if(dbh->tableExists("t2")) {
		dbh->query("DROP TABLE IF EXISTS t2");
	}

	CWT_TEST_OK(dbh->query(
			"CREATE TABLE t2("
			"    x INTEGER PRIMARY KEY,"
			"    a TEXT,"
			"    b FLOAT,"
			"    c DOUBLE,"
			"    d NUMERIC,"
			"    e VARCHAR(10),"
			"    f NOTYPE,"
			"    g BOOLEAN"
			")"));

	Vector<DbColumnMeta> meta;
	CWT_TEST_FAIL(dbh->meta("t2", meta));
	CWT_TEST_FAIL(meta.size() == 8);

	for (size_t i = 0 ; i < meta.size(); ++i) {
		if (meta[i].column_name == String("x")) {
			CWT_TEST_OK(meta[i].column_type == UniType::LongValue);
		} else if (meta[i].column_name == String("a")) {
			CWT_TEST_OK(meta[i].column_type == UniType::StringValue);
		} else if (meta[i].column_name == String("b")) {
			CWT_TEST_OK(meta[i].column_type == UniType::DoubleValue);
		} else if (meta[i].column_name == String("c")) {
			CWT_TEST_OK(meta[i].column_type == UniType::DoubleValue);
		} else if (meta[i].column_name == String("d")) {
			CWT_TEST_OK(meta[i].column_type == UniType::DoubleValue);
		} else if (meta[i].column_name == String("e")) {
			CWT_TEST_OK(meta[i].column_type == UniType::StringValue);
		} else if (meta[i].column_name == String("f")) {
			CWT_TEST_OK(meta[i].column_type == UniType::BlobValue);
		} else if (meta[i].column_name == String("g")) {
			CWT_TEST_OK(meta[i].column_type == UniType::BoolValue);
		}

		CWT_DEBUG(String(SafeFormat("SQL Type (%s) maps to UniType type (%s)")
				% meta[i].native_type
				% UniType::toStringType(meta[i].column_type)) . c_str() );
	}

	CWT_TEST_OK(dbh->query("DROP TABLE IF EXISTS t2"));
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(98);

    FileSystem::unlink("/tmp/test.db");
   	test_base();
    test_sqlite3_collation();
    test_columns();

    CWT_END_TESTS;
    return 0;
}
