/*
 * hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/hash.hpp>
#include <cwt/bytearray.hpp>
#include <cwt/string.hpp>
#include <cwt/logger.hpp>
#include <cstring>

using namespace cwt;

void test_bytearray_hash()
{
    Hash<ByteArray, ByteArray> hash;
    for(int i = 0; i < 10000; i++) {
    	char str[64];
    	sprintf(str, "Test byte array %d", i);
    	hash.insert(ByteArray(str), ByteArray(str));
    }

    for(int i = 0; i < 10000; i++) {
    	char str[64];
    	sprintf(str, "Test byte array %d", i);
    	ByteArray sample(str);
    	ByteArray value(hash[str]);
    	CWT_TEST_OK2(strcmp(value.data(),sample.data()) == 0
    			, String().sprintf("[%s] == [%s]", value.c_str(), str).c_str());
    }
}

void test_string_hash()
{
    Hash<String, String> hash;
    for(int i = 0; i < 2000; i++) {
    	String str;
    	str.sprintf("Test string %d", i);
    	hash.insert(str, str);
    }

    for(int i = 0; i < 2000; i++) {
    	String str;
    	str.sprintf("Test string %d", i);
    	String sample(str);
    	String value(hash[str]);
    	CWT_TEST_OK2(sample == value
    			, String().sprintf("[%s] == [%s]", value.c_str(), sample.c_str()).c_str());
    }
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(12000);

    test_bytearray_hash();
    test_string_hash();

    CWT_END_TESTS;
}
