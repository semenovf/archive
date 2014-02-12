/*
 * model.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/string.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/vector.hpp>
#include <cwt/debby/schema.hpp>

// XXX Insertion in Sqlite3 is very slow (one transaction for each insertion), so it is good idea
//     to wrap series of insertions with transaction.


struct A
{
	char ch;
	short sh;
	int i;
};

struct B
{
	pfs::string text;
	pfs::bytearray blob;
};

struct C
{
	A a;
	pfs::vector<B> b;
};

void create_drop_schema ()
{
	pfs::string sql_a, sql_b, sql_c;

	// sqlite3:///tmp/test.db?mode=rwc
	cwt::debby::database db(pfs::string("sqlite3:///tmp/test_schema.sqlite3"));

	TEST_FAIL_X(db.opened(), db.logErrors());

	db.begin();
	// drop schema
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS a")), db.logErrors());
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS b")), db.logErrors());
	TEST_OK_X(db.query(_l1("DROP TABLE IF EXISTS c")), db.logErrors());


	pfs::string pk("id INTEGER PRIMARY KEY NOT NULL"); // mandatory implicit field
	pfs::string table_name;

	table_name = pfs::string("a");
	sql_a << "CREATE TABLE IF NOT EXISTS " << table_name
			<< "(" << pk
			<< ", ch INTEGER NOT NULL"
			<< ", sh INTEGER NOT NULL"
			<< ", i  INTEGER NOT NULL"
			<< ")";

	table_name = pfs::string("b");
	sql_b << "CREATE TABLE IF NOT EXISTS " << table_name
			<< "(" << pk
			<< ", text TEXT NOT NULL"
			<< ", blob BLOB NOT NULL"
			<< ", c_id INTEGER NOT NULL"
			<< ", CONSTRAINT c_fk FOREIGN KEY (c_id) REFERENCES c (id)"
			<< ");";

	table_name = pfs::string("c");
	sql_c << "CREATE TABLE IF NOT EXISTS " << table_name
			<< "(" << pk
			<< ", a_id INTEGER NOT NULL"
			<< ", CONSTRAINT a_fk FOREIGN KEY (a_id) REFERENCES a (id)"
			<< ");";

	TEST_OK_X(db.query(sql_a), db.logErrors());
	TEST_OK_X(db.query(sql_b), db.logErrors());
	TEST_OK_X(db.query(sql_c), db.logErrors());
	db.commit();
}


#ifdef __COMMENT__

static bool populate_currency (const Vector<String> & columns, Record currency)
{
	// ingore fits column 'id'
	currency["code"]    = columns[1];
	currency["name"]    = columns[2];
	currency["symbol"]  = columns[3];
	currency["country"] = columns[4];
	return true;
}

static bool populate_country (const Vector<String> & columns, Record country)
{
	// ingore fits column 'id'
	country["code"]      = columns[1];
	country["name"]      = columns[2];
/*
	country["latitude"]  = columns[3];
	country["longitude"] = columns[4];
*/
	country["currency"]  = columns[5];
	country["timezone"]  = columns[6];

	return true;
}


static bool traverse (DbHandler & dbh, )
{
	File csvFile(currencyCsvPath);
	io::TextReader reader(csvFile);

	if (!reader.device()->opened())
		return false;

	CsvReader csvReader(reader, UChar(','));

	Vector<String> csvRecord;
	int line = 0;

	csvReader.skipLines(2); // ignore header
	line += 2;

	dbh.begin();

	// id, code, name, symbol, country
	while (!(csvRecord = csvReader.readRecord()).isEmpty()) {
		++line;
		if (csvRecord.size() != 5) {
			Logger::error(_Fr("%s: Incomplete number of columns (expected 5) at %d")
					% currencyCsvPath
					% line);
			return false;
		}

		currency["code"]    = csvRecord[1];
		currency["name"]    = csvRecord[2];
		currency["symbol"]  = csvRecord[3];
		currency["country"] = csvRecord[4];

		if (!currency.create(dbh)) {
			dbh.rollback();
			return false;
		}

	}
	dbh.commit();
	Logger::info(_Fr("%d lines added") % (line - 2));

	return true;
}


static bool __traverse_countries (DbHandler & dbh, Record & country)
{
	File csvFile(countryCsvPath);
	io::TextReader reader(csvFile);

	if (!reader.device()->opened())
		return false;

	CsvReader csvReader(reader, UChar(','));
	csvReader.setIgnoreLeadingWS(false);
	csvReader.setIgnoreTrailingWS(false);
	csvReader.useEndLine(String(1, '\n'));

	Vector<String> csvRecord;
	int line = 0;

	csvReader.skipLines(2); // ignore header
	line += 2;

	dbh.begin();
	// id,code,name,latitude,longitude,currency,timezone
	while (!(csvRecord = csvReader.readRecord()).isEmpty()) {
		++line;
		if (csvRecord.size() != 7) {
			Logger::error(_Fr("%s: Incomplete number of columns (expected 7) at %d")
					% currencyCsvPath
					% line);
			return false;
		}

		country["code"]      = csvRecord[1];
		country["name"]      = csvRecord[2];
/*
		country["latitude"]  = csvRecord[3];
		country["longitude"] = csvRecord[4];
*/
		country["currency"]  = csvRecord[5];
		country["timezone"]  = csvRecord[6];

		if (!country.create(dbh)) {
			dbh.rollback();
			return false;
		}

	}
	dbh.commit();
	Logger::info(_Fr("%d lines added") % (line - 2));

	return true;
}

void test_deploy_drop ()
{
	Schema schema;
	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	DbHandlerPtr dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());
	__prepare_schema(schema);
	CWT_TEST_FAIL2(schema.deploy(*dbh), String(_Fr("Deploying '%s' ... ") % uri).utf8());
	CWT_TEST_OK2(schema.drop(*dbh), String(_Fr("Dropping '%s' ... ") % uri).utf8());
}

void test_create ()
{
	Schema schema;
	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	DbHandlerPtr dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());
	__prepare_schema(schema);
	CWT_TEST_FAIL2(schema.deploy(*dbh), String(_Fr("Deploying '%s' ... ") % uri).utf8());
	CWT_TEST_FAIL(schema.contains("currency"));
	CWT_TEST_FAIL(schema.contains("country"));
	CWT_TEST_OK2(__traverse_currencies(*dbh, schema["currency"]), _Tr("Populating currencies ... "));
	CWT_TEST_OK2(__traverse_countries(*dbh, schema["country"]), _Tr("Populating countries ... "));
}


void test_destroy ()
{
	Schema schema;
	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	DbHandlerPtr dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());
	__prepare_schema(schema);
	CWT_TEST_FAIL2(schema.deploy(*dbh), String(_Fr("Deploying '%s' ... ") % uri).utf8());
	CWT_TEST_FAIL(schema.contains("currency"));
	CWT_TEST_FAIL(schema.contains("country"));
	CWT_TEST_OK2(__traverse_currencies(*dbh, schema["currency"]), _Tr("Populating currencies ... "));
	CWT_TEST_OK2(__traverse_countries(*dbh, schema["country"]), _Tr("Populating countries ... "));
}

void test_load ()
{
	Schema schema;
	__prepare_schema(schema);

	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	CWT_TEST_FAIL2(schema.create(uri), String(_Fr("Deploying '%s' ... ") % uri).utf8());

	DbHandlerPtr dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());

	Schema peer;
	CWT_TEST_FAIL(peer.load(*dbh));

	CWT_TEST_OK2(peer.drop(*dbh), cwt::String(_Fr("Dropping '%s' ... ") % uri).utf8());
}

#endif

int main (int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(1);

	create_drop_schema();

    END_TESTS;
}



