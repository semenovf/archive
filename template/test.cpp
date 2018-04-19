#include <pfs/test.hpp>

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
    BEGIN_TESTS(1);

    TEST_OK(true == true);

    return END_TESTS;
}
