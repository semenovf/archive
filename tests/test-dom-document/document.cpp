#include <pfs/test/test.hpp>
#include <pfs/dom/document.hpp>

void test_basic ()
{
    pfs::dom::document doc;
    doc.createElement(_u8("html"));
    TEST_OK(true == true);
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(1);

    test_basic();

    END_TESTS;
}
