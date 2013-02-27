/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Feb 27, 2013 4:54:05 PM
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/textstream.hpp>

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

    FileReader r;
    TextStream ts(&r);
    ts.readAll();

    CWT_END_TESTS;
}

