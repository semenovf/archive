#include <cwt/test.h>

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    CWT_TEST_OK(true == true);

    CWT_END_TESTS;
    return 0;
}
