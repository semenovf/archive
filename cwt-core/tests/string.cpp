/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/string.hpp>
#include <cstring>

using namespace cwt;


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1);

/*
    test_bytearray_hash();
    test_string_hash();
*/

    CWT_END_TESTS;
}
