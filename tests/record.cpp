/*
 * scheme.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/safeformat.hpp>
#include <cwt/debby/record.hpp>
#include <cwt/debby/schema.hpp>
#include <cwt/debby/utils.hpp>
#include <iostream>

using namespace cwt;
using namespace cwt::debby;

void test_deploy()
{
	Record currency("currency", cwt::UniType::StringValue);
//	currency.addString("code");
	currency.addString("name");
	currency.addString("symbol");

	Record account("account");
	account.addString("name").setDefault(String("default name"));
	account.setAutoinc();
	account.addRef("currency", currency);

	Schema schema;
	schema << currency << account;

	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	CWT_TEST_OK2(schema.deploy(uri), String(_Fr("Deploying '%s' ... ") % uri).utf8());

	shared_ptr<DbHandler> dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());

	currency[Record::PkName] = String("RUB");
	currency["name"] = String("Russian ruble");
	currency["symbol"] = String::fromUtf8("руб (RUB)");
	currency.create(*dbh);

#ifdef __COMMENT__
	Schema schema;
	Table table_0 = schema.add("Table0");
	Table table_1 = schema.add("Table1");

	Field id_0 = table_0.addInteger("id");
	id_0.setNullable(false);
	id_0.setUnique(true);
	id_0.setAutoinc();
	Field f1 = table_0.addString("first_name");
	Field f2 = table_0.addString("last_name");
	Field f3 = table_0.addInteger("age", Field::Unsigned);
	Field f4 = table_0.addInteger("birth_year", Field::Unsigned);

	table_0.addPrimaryKey("id");
/*
	table_0.addPrimaryKey("pk1");
	table_0.addPrimaryKey("pk2");
*/

	Field id_1  = table_1.addInteger("id");
	Field fk_10 = table_1.addRef("fk_10", "Table0");
	id_1.setNullable(false);
	id_1.setUnique(true);

	std::cout << schema.json().toString() << std::endl;

	const String dbname("test_schema");
	const String dbpath(_F("/tmp/%s.db") % dbname);
	String uri(_F("sqlite3:%s?mode=rwc") % dbpath);

	CWT_TEST_OK2(schema.deploy(uri), String(_Fr("Deploying '%s' ... ") % uri).utf8());

	shared_ptr<DbHandler> dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());

	Insert qins("Table0");
	qins["first_name"] = String("Fedor");
	qins["last_name"] = String("Semenov");
	qins["age"] = 14;
	qins["birth_year"] = 1999;
	qins.save(*dbh);

/*
	cwt::debby::Select query;
	query.distinct().limit(2);
	query.first();
*/


#endif
	//CWT_TEST_OK2(schema.drop(uri), cwt::String(_Fr("Dropping '%s' ... ") % uri).utf8());
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(2);

    test_deploy();

    CWT_END_TESTS;
    return 0;
}



