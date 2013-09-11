/*
 * scheme.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/safeformat.hpp>
#include <cwt/debby/scheme.hpp>
#include <cwt/filesystem.hpp>
/*
#include <cwt/dbh.hpp>
#include <cwt/sth.hpp>
#include <cwt/safeformat.hpp>

#include <cwt/shared_ptr.hpp>
*/
using namespace cwt;
using namespace cwt::debby;

void test_deploy()
{
	const String dbname("test_scheme");
	const String dbpath(_F("/tmp/%s.db") % dbname);

	String uri(_F("sqlite3:%s?mode=rwc") % dbpath);
	//String uri("sqlite3:/tmp/test.db");
	Scheme scheme(uri);

	CWT_TEST_OK(FileSystem::exists(dbpath));

	Table * table_0 = scheme.addTable("TableName0");
	Table * table_1 = scheme.addTable("TableName1");
	Table * table_3 = scheme.addTable("TableName3");

	CWT_UNUSED3(table_0, table_1, table_3);

	Field * field_00 = table_0->addField("FieldName0");
	Field * field_01 = table_0->addField("FieldName1");
	Field * field_02 = table_0->addField("FieldName2");
	Field * field_03 = table_0->addField("FieldName3");
	Field * field_04 = table_0->addField("FieldName4");
	Field * field_05 = table_0->addField("FieldName5");
	Field * field_06 = table_0->addField("FieldName6");
	Field * field_07 = table_0->addField("FieldName7");
	Field * field_08 = table_0->addField("FieldName8");
	Field * field_09 = table_0->addField("FieldName9");

	field_00->setNumber(ulong_t(CWT_INT_MAX));
	field_01->setType(Debby::TypeBool);
	field_02->setType(Debby::TypeString);
	field_03->setType(Debby::TypeInteger);
	field_04->setType(Debby::TypeFloat);
	field_05->setType(Debby::TypeDouble);
	field_06->setType(Debby::TypeDate);
	field_07->setType(Debby::TypeTime);
	field_08->setType(Debby::TypeDateTime);
	field_09->setType(Debby::TypeTimeStamp);

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



