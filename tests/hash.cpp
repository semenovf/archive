/*
 * hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include "../include/cwt/test.h"
#include "../include/cwt/hash.hpp"
#include "../include/cwt/bytearray.hpp"
#include "../include/cwt/string.hpp"
#include "../include/cwt/logger.hpp"
#include "../include/cwt/safeformat.hpp"
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
    			, String(_Fr("[%s] == [%s]") % value % str).c_str());
    }
}

void test_string_hash()
{
    Hash<String, String> hash;
    for(int i = 0; i < 2000; i++) {
    	String str(_Fr("Test string %d") % i);
    	hash.insert(str, str);
    }

    for(int i = 0; i < 2000; i++) {
    	String str(_Fr("Test string %d") % i);
    	String sample(str);
    	String value(hash[str]);
    	CWT_TEST_OK2(sample == value
    			, String(_Fr("[%s] == [%s]") % value % sample).c_str());
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
