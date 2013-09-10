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


static const int __ENTRY_COUNT  = 1; //2000
void test_string_hash()
{
    Hash<String, String> hash;
    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	String str(_Fr("Test string %d") % i);
    	hash.insert(str, str);
    }

    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	String str(_Fr("Test string %d") % i);
    	String sample(str);
    	String value(hash[str]);
    	CWT_TEST_OK2(sample == value
    			, String(_Fr("[%s] == [%s]") % value % sample).c_str());
    }
}

void test_copy_pod_hash()
{
	Hash<int, int> hash;

	hash.insert(1 , 11);
	hash.insert(2 , 22);
	hash.insert(3 , 33);
	hash.insert(4 , 44);

	Hash<int, int> hashCopy(hash);

	hashCopy[1] = 111;
	hashCopy[4] = 444;

	CWT_TEST_OK(hash[1] == 11);
	CWT_TEST_OK(hash[2] == 22);
	CWT_TEST_OK(hash[3] == 33);
	CWT_TEST_OK(hash[4] == 44);

	CWT_TEST_OK(hashCopy[1] == 111);
	CWT_TEST_OK(hashCopy[2] == 22);
	CWT_TEST_OK(hashCopy[3] == 33);
	CWT_TEST_OK(hashCopy[4] == 444);
}


void test_copy_hash()
{
	Hash<String, String> hash;

	hash.insert("One"   , "One Data");
	hash.insert("Two"   , "Two Data");
	hash.insert("Three" , "Three Data");
	hash.insert("Four"  , "Four Data");

	const Hash<String, String> constHashCopy(hash);

	CWT_TEST_OK(constHashCopy["One"]   == String("One Data"));
	CWT_TEST_OK(constHashCopy["Two"]   == String("Two Data"));
	CWT_TEST_OK(constHashCopy["Three"] == String("Three Data"));
	CWT_TEST_OK(constHashCopy["Four"]  == String("Four Data"));

	Hash<String, String> hashCopy(hash);

	hashCopy["Two"]  = String("Copy of Two Data");
	hashCopy["Four"] = String("Copy of Four Data");

	CWT_TEST_OK(hash["One"]   == String("One Data"));
	CWT_TEST_OK(hash["Two"]   == String("Two Data"));
	CWT_TEST_OK(hash["Three"] == String("Three Data"));
	CWT_TEST_OK(hash["Four"]  == String("Four Data"));

	CWT_TEST_OK(hashCopy["One"] == String("One Data"));
	CWT_TEST_OK(hashCopy["Two"] == String("Copy of Two Data"));
	CWT_TEST_OK(hashCopy["Three"] == String("Three Data"));
	CWT_TEST_OK(hashCopy["Four"] == String("Copy of Four Data"));
}


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(12000);

    if(false) {
    test_bytearray_hash();
    }
    test_string_hash();
    test_copy_pod_hash();
    test_copy_hash();

    CWT_END_TESTS;
}
