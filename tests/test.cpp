#include <cwt/test.h>
#include <cwt/dbd.hpp>
#include <cwt/dbh.hpp>
#include <cwt/sth.hpp>
#include <cwt/shared_ptr.hpp>

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    shared_ptr<DbHandler> dbh(DbHandler::open("sqlite3:///tmp/test.db?mode=rwc"));
    CWT_TEST_FAIL(dbh.get());

    CWT_END_TESTS;
    return 0;
}
