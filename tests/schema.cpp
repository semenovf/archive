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

static void __prepare_schema (Schema & schema)
{
	Record & currency = schema.add("currency");
	currency.addString("code").setPk();
	currency.addString("name");
	currency.addString("symbol");

	Record & account = schema.add("account");
	account.addInteger("id").setPk().setAutoinc(1);
	account.addString("name").setDefault(String("default name"));
	account.addFromAttr("currency", currency["code"]);
}

void test_deploy_drop()
{
	Schema schema;
	__prepare_schema(schema);

	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	CWT_TEST_OK2(schema.deploy(uri), String(_Fr("Deploying '%s' ... ") % uri).utf8());
	CWT_TEST_OK2(schema.drop(uri), cwt::String(_Fr("Dropping '%s' ... ") % uri).utf8());
}

void test_load()
{
	Schema schema;
	__prepare_schema(schema);

	String uri = buildSqlite3Uri("test_schema", "/tmp", "mode=rwc");
	CWT_TEST_FAIL2(schema.deploy(uri), String(_Fr("Deploying '%s' ... ") % uri).utf8());

	DbHandlerPtr dbh(DbHandler::open(uri));
	CWT_TEST_FAIL(dbh.get());

	Schema peer;
	CWT_TEST_FAIL(peer.load(*dbh));



	CWT_TEST_OK2(peer.drop(*dbh), cwt::String(_Fr("Dropping '%s' ... ") % uri).utf8());
}


/*
shared_ptr<DbHandler> dbh(DbHandler::open(uri));
CWT_TEST_FAIL(dbh.get());

currency["code"] = String("RUB"); // pk
currency["name"] = String("Russian ruble");
currency["symbol"] = String::fromUtf8("руб (RUB)");
currency.create(*dbh);
*/

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(6);

    test_deploy_drop();
    test_load();

    CWT_END_TESTS;
    return 0;
}



