#include <cwt/test.hpp>
#include <cwt/dom.hpp>

void test_basic ()
{
	cwt::dom::document doc;
	doc.createElement(_l1("html"));
	TEST_OK(true == true);
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(1);

    test_basic();

    END_TESTS;
}
