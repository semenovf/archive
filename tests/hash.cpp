/*
 * hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/hash.hpp>
#include <cwt/bytearray.hpp>
#include <cstring>
#include <QtCore/QByteArray>

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(1000);

    Hash<ByteArray, ByteArray> hash;
    for(int i = 0; i < 1000; i++) {
    	char str[64];
    	sprintf(str, "Test string %d", i);
    	hash.insert(ByteArray(str), ByteArray(str));
    }

    for(int i = 0; i < 1000; i++) {
    	char str[64];
    	sprintf(str, "Test string %d", i);
    	ByteArray sample(str);
    	ByteArray value(hash[str]);
    	CWT_TEST_OK2(strcmp(value.data(),sample.data()) == 0
    			, String().sprintf("[%s] == [%s]", value.data(), str).toUtf8().data());
    }
    CWT_END_TESTS;
}
