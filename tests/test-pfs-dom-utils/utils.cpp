#include <pfs/test/test.hpp>
#include "../../src/pfs-dom/utils.hpp"

void test_split_namespace ()
{
	pfs::string prefix;
	pfs::string name;

	pfs::string qName("prefix:name");
	pfs::dom::split_namespace(prefix, name, qName);
	TEST_OK(prefix == "prefix");
	TEST_OK(name == "name");

	qName = pfs::string(":name");
	pfs::dom::split_namespace(prefix, name, qName);
	TEST_OK(prefix.isEmpty());
	TEST_OK(name == "name");

	qName = pfs::string("prefix:");
	pfs::dom::split_namespace(prefix, name, qName);
	TEST_OK(prefix == "prefix");
	TEST_OK(name.isEmpty());

	qName = pfs::string(":");
	pfs::dom::split_namespace(prefix, name, qName);
	TEST_OK(prefix.isEmpty());
	TEST_OK(name.isEmpty());

	qName = pfs::string("");
	pfs::dom::split_namespace(prefix, name, qName);
	TEST_OK(prefix.isEmpty());
	TEST_OK(name.isEmpty());

	qName = pfs::string();
	pfs::dom::split_namespace(prefix, name, qName);
	TEST_OK(prefix.isEmpty());
	TEST_OK(name.isEmpty());
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(12);

    test_split_namespace();

    END_TESTS;
}
