#include <pfs/test/test.hpp>
#include <pfs/dom/domimpl.hpp>

void test_feature ()
{
    pfs::dom::dom_implementation dom_impl;
    TEST_OK(dom_impl.hasFeature("XML"));
    TEST_OK(dom_impl.hasFeature("XML", "1.0"));
    TEST_OK(!dom_impl.hasFeature("UnavailableFeature"));
    TEST_OK(!dom_impl.hasFeature("UnavailableFeature", "1.0"));
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(4);

    test_feature();

    END_TESTS;
}
