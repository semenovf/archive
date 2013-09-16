/*
 * scheme.cpp
 *
 *  Created on: Sep 11, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/safeformat.hpp>
#include <cwt/filesystem.hpp>
#include <cwt/debby/scheme.hpp>

/*
#include <cwt/dbh.hpp>
#include <cwt/sth.hpp>
#include <cwt/safeformat.hpp>

#include <cwt/shared_ptr.hpp>
*/
using namespace cwt;

void test_deploy()
{
	const String dbname("test_scheme");
	const String dbpath(_F("/tmp/%s.db") % dbname);

	String uri(_F("sqlite3:%s?mode=rwc") % dbpath);
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



