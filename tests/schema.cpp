/*
 * scheme.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/safeformat.hpp>
#include <cwt/debby/schema.hpp>
#include <iostream>

using namespace cwt;
using namespace cwt::debby;

void test_deploy()
{
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


	//CWT_TEST_OK2(schema.drop(uri), cwt::String(_Fr("Dropping '%s' ... ") % uri).utf8());


#ifdef __COMMENT__
	//String uri("sqlite3:/tmp/test.db");
	DebbyScheme scheme(uri);

	CWT_TEST_OK(FileSystem::exists(dbpath));

	DebbyTable * table_0 = scheme.addTable("TableName0");
	DebbyTable * table_1 = scheme.addTable("TableName1");
	DebbyTable * table_3 = scheme.addTable("TableName3");

	CWT_UNUSED2(table_1, table_3);

	DebbyField * field_00 = table_0->addNumberField    ("FieldName0", ulong_t(CWT_INT_MAX));
/*
	DebbyField * field_01 = table_0->addBooleanField   ("FieldName1");
	DebbyField * field_02 = table_0->addStringField    ("FieldName2", 256);
	DebbyField * field_03 = table_0->addNumberField    ("FieldName3", CWT_UINT_MAX);
	DebbyField * field_04 = table_0->addNumberField    ("FieldName4", 0.0f, 1.0f);
	DebbyField * field_05 = table_0->addNumberField    ("FieldName5", CWT_DOUBLE_MIN, CWT_DOUBLE_MAX);
	DebbyField * field_06 = table_0->addDateField      ("FieldName6");
	DebbyField * field_07 = table_0->addTimeField      ("FieldName7");
	DebbyField * field_08 = table_0->addDateTimeField  ("FieldName8");
	DebbyField * field_09 = table_0->addTimeStampField ("FieldName9");
*/

	field_00->setPk(true);
	field_00->setAutoinc(1);

	if (!scheme.deploy()) {
		// Error
	}

	scheme.drop();

	CWT_TEST_OK(!FileSystem::exists(dbpath));

	scheme.close(); // unnecessary call after drop()
/*
	...

	scheme.open(uri);
	Table table_M = scheme.addTable("TableNameM");

	...

	scheme.drop();
	scheme.close();
*/
#endif
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    test_deploy();

    CWT_END_TESTS;
    return 0;
}



