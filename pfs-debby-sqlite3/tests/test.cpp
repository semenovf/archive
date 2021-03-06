#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/fs.hpp>
#include <pfs/debby/dbd.hpp>
#include <pfs/debby/database.hpp>
#include <pfs/debby/statement.hpp>

inline pfs::string __db_uri ()
{
	pfs::string r;
	pfs::fs fs;
	r << "sqlite3://"
		<< fs.tempDirectory()
		<< "/cwt-debby-sqlite3.sqlite";
	return r;
}

void test_assignment()
{
	pfs::string dburi(__db_uri());

	int n = 1000;

	while (--n) {
		pfs::debby::database dbh;

		if (!dbh.open(dburi)) {
			dbh.logErrors();
		}

		TEST_FAIL(dbh.opened());
		dbh.close();
		TEST_FAIL(!dbh.opened());
		TEST_FAIL(dbh.open(dburi));
		TEST_FAIL(dbh.opened());
	}
}

void test_base()
{
	static pfs::string sql_create_table_0(
			"CREATE TABLE IF NOT EXISTS test_table_0 (col1 INT,"
			"col2 VARCHAR(100),"
			"col3 SMALLINT,"
			"col4 TIMESTAMP)");

	static pfs::string sql_create_table_1(
			"CREATE TABLE IF NOT EXISTS test_table_1 (col1 INT,"
			"col2 VARCHAR(100),"
			"col3 SMALLINT,"
			"col4 TIMESTAMP)");

	pfs::debby::database dbh;
    TEST_FAIL(dbh.open(_l1("sqlite3:///tmp/test.db?mode=rwc")));
    TEST_FAIL(dbh.opened());

    TEST_OK(dbh.query(sql_create_table_0));
    TEST_OK(dbh.tableExists(_l1("test_table_0")));

    TEST_OK(dbh.query(sql_create_table_1));
    TEST_OK(dbh.tableExists(_l1("test_table_1")));

    pfs::vector<pfs::string> tables = dbh.tables();
    TEST_FAIL(tables.size() == 2);
    TEST_OK(tables[0] == "test_table_0" || tables[0] == "test_table_1");
    TEST_OK(tables[1] == "test_table_0" || tables[1] == "test_table_1");

    TEST_OK(dbh.query(_l1("DROP TABLE IF EXISTS test_table_0")));
    TEST_OK(dbh.query(_l1("DROP TABLE IF EXISTS test_table_1")));
    TEST_OK(dbh.tables().size() == 0);

    dbh.close(); // Not necessary, automatically called from destructor.
}

// source: http://www.sqlite.org/datatype3.html
void test_sqlite3_collation()
{
	pfs::debby::database dbh;
	TEST_FAIL_X(dbh.open(__db_uri()), dbh.logErrors());

	if(dbh.tableExists(_l1("t1"))) {
		dbh.query(_l1("DROP TABLE IF EXISTS t1"));
	}

	TEST_FAIL_X(dbh.query(_l1(
			"CREATE TABLE t1("
			"    x INTEGER PRIMARY KEY,"
			"    a,"                 /* collating sequence BINARY */
			"    b COLLATE BINARY,"  /* collating sequence BINARY */
			"    c COLLATE RTRIM,"   /* collating sequence RTRIM  */
			"    d COLLATE NOCASE"   /* collating sequence NOCASE */
			")")), dbh.logErrors());

	pfs::debby::statement sth = dbh.prepare(_l1("INSERT INTO t1 VALUES(?,?,?,?,?)"));

	sth.bind(int(1)).bind(_l1("abc")).bind(_l1("abc")).bind(_l1("abc  ")).bind(_l1("abc"));
	TEST_OK(sth.exec());

	sth.bind(int(2)).bind(_l1("abc")).bind(_l1("abc")).bind(_l1("abc")).bind(_l1("ABC"));
	TEST_OK(sth.exec());

	sth.bind(int(3)).bind(_l1("abc")).bind(_l1("abc")).bind(_l1("abc ")).bind(_l1("Abc"));
	TEST_OK(sth.exec());

	sth.bind(int(4)).bind(_l1("abc")).bind(_l1("abc ")).bind(_l1("ABC")).bind(_l1("abc"));
	TEST_OK(sth.exec());

	/* Text comparison a=b is performed using the BINARY collating sequence. */
	// SELECT x FROM t1 WHERE a = b ORDER BY x;
	// --result 1 2 3
	sth = dbh.prepare(_l1("SELECT x FROM t1 WHERE a = b ORDER BY x"));
	TEST_OK(sth.exec());

	pfs::vector<pfs::unitype> result;
	pfs::vector<pfs::unitype> row;

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 3);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 2);
	TEST_OK(result[2]->toInteger() == 3);

	/* Text comparison a=b is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 WHERE a = b COLLATE RTRIM ORDER BY x;
	// --result 1 2 3 4
	sth = dbh.prepare(_l1("SELECT x FROM t1 WHERE a = b COLLATE RTRIM ORDER BY x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 4);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 2);
	TEST_OK(result[2]->toInteger() == 3);
	TEST_OK(result[3]->toInteger() == 4);

	/* Text comparison d=a is performed using the NOCASE collating sequence. */
	// SELECT x FROM t1 WHERE d = a ORDER BY x;
	// --result 1 2 3 4
	sth = dbh.prepare(_l1("SELECT x FROM t1 WHERE d = a ORDER BY x"));
//	TEST_FAIL(sth.get());
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 4);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 2);
	TEST_OK(result[2]->toInteger() == 3);
	TEST_OK(result[3]->toInteger() == 4);

	/* Text comparison a=d is performed using the BINARY collating sequence. */
	// SELECT x FROM t1 WHERE a = d ORDER BY x;
	// --result 1 4
	sth = dbh.prepare(_l1("SELECT x FROM t1 WHERE a = d ORDER BY x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 2);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 4);

	/* Text comparison 'abc'=c is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 WHERE 'abc' = c ORDER BY x;
	// --result 1 2 3
	sth = dbh.prepare(_l1("SELECT x FROM t1 WHERE 'abc' = c ORDER BY x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 3);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 2);
	TEST_OK(result[2]->toInteger() == 3);

	/* Text comparison c='abc' is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 WHERE c = 'abc' ORDER BY x;
	// --result 1 2 3
	sth = dbh.prepare(_l1("SELECT x FROM t1 WHERE c = 'abc' ORDER BY x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 3);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 2);
	TEST_OK(result[2]->toInteger() == 3);

	/* Grouping is performed using the NOCASE collating sequence (Values
	 ** 'abc', 'ABC', and 'Abc' are placed in the same group). */
	// SELECT count(*) FROM t1 GROUP BY d ORDER BY 1;
	//--result 4
	sth = dbh.prepare(_l1("SELECT count(*) FROM t1 GROUP BY d ORDER BY 1"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 1);
	TEST_OK(result[0]->toInteger() == 4);

	/* Grouping is performed using the BINARY collating sequence.  'abc' and
	** 'ABC' and 'Abc' form different groups */
	// SELECT count(*) FROM t1 GROUP BY (d || '') ORDER BY 1;
	// --result 1 1 2
	sth = dbh.prepare(_l1("SELECT count(*) FROM t1 GROUP BY (d || '') ORDER BY 1"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 3);
	TEST_OK(result[0]->toInteger() == 1);
	TEST_OK(result[1]->toInteger() == 1);
	TEST_OK(result[2]->toInteger() == 2);

	/* Sorting or column c is performed using the RTRIM collating sequence. */
	// SELECT x FROM t1 ORDER BY c, x;
	// --result 4 1 2 3
	sth = dbh.prepare(_l1("SELECT x FROM t1 ORDER BY c, x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 4);
	TEST_OK(result[0]->toInteger() == 4);
	TEST_OK(result[1]->toInteger() == 1);
	TEST_OK(result[2]->toInteger() == 2);
	TEST_OK(result[3]->toInteger() == 3);

	/* Sorting of (c||'') is performed using the BINARY collating sequence. */
	// SELECT x FROM t1 ORDER BY (c||''), x;
	// --result 4 2 3 1
	sth = dbh.prepare(_l1("SELECT x FROM t1 ORDER BY (c||''), x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 4);
	TEST_OK(result[0]->toInteger() == 4);
	TEST_OK(result[1]->toInteger() == 2);
	TEST_OK(result[2]->toInteger() == 3);
	TEST_OK(result[3]->toInteger() == 1);

	/* Sorting of column c is performed using the NOCASE collating sequence. */
	// SELECT x FROM t1 ORDER BY c COLLATE NOCASE, x;
	// --result 2 4 3 1
	sth = dbh.prepare(_l1("SELECT x FROM t1 ORDER BY c COLLATE NOCASE, x"));
	TEST_OK(sth.exec());

	result.clear();
	while (!(row = sth.fetchRowArray()).isEmpty())
		result.append(row[0]);

	TEST_FAIL(result.size() == 4);
	TEST_OK(result[0]->toInteger() == 2);
	TEST_OK(result[1]->toInteger() == 4);
	TEST_OK(result[2]->toInteger() == 3);
	TEST_OK(result[3]->toInteger() == 1);

	TEST_OK_X(dbh.query(_l1("DROP TABLE IF EXISTS t1")), dbh.logErrors());
}


void test_columns()
{
	pfs::debby::database dbh;

	TEST_FAIL(dbh.open(__db_uri()));

	if (dbh.tableExists(_l1("t2"))) {
		dbh.query(_l1("DROP TABLE IF EXISTS t2"));
	}

	TEST_OK(dbh.query(_l1(
			"CREATE TABLE t2("
			"    x INTEGER PRIMARY KEY,"
			"    a TEXT,"
			"    b FLOAT,"
			"    c DOUBLE,"
			"    d NUMERIC,"
			"    e VARCHAR(10),"
			"    f NOTYPE,"
			"    g BOOLEAN,"
			"    h BLOB"
			")")));

	pfs::vector<pfs::debby::column_meta> meta;
	TEST_FAIL(dbh.meta(_l1("t2"), meta));
	TEST_FAIL(meta.size() == 9);

	for (size_t i = 0 ; i < meta.size(); ++i) {
		if (meta[i]->column_name == _l1("x")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Integer);
		} else if (meta[i]->column_name == _l1("a")) {
			TEST_OK(meta[i]->column_type == pfs::debby::String);
		} else if (meta[i]->column_name == _l1("b")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Float);
		} else if (meta[i]->column_name == _l1("c")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Double);
		} else if (meta[i]->column_name == _l1("d")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Double);
		} else if (meta[i]->column_name == _l1("e")) {
			TEST_OK(meta[i]->column_type == pfs::debby::String);
		} else if (meta[i]->column_name == _l1("f")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Null);
		} else if (meta[i]->column_name == _l1("g")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Bool);
		} else if (meta[i]->column_name == _l1("h")) {
			TEST_OK(meta[i]->column_type == pfs::debby::Blob);
		}

/*
		PFS_DEBUG(pfs::string(pfs::safeformat("SQL Type (%s) maps to pfs::unitype type (%s)")
				% meta[i]->native_type
				% pfs::unitype::typeToString(meta[i]->column_type)) . c_str() );
*/
	}

	TEST_OK(dbh.query(_l1("DROP TABLE IF EXISTS t2")));
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(4086);

	pfs::debby::database::addGlobalSearchPath(pfs::string("."));
	pfs::debby::database::addGlobalSearchPath(pfs::string(".."));
	test_assignment();
    test_base();
    test_sqlite3_collation();
    test_columns();

    END_TESTS;
}
