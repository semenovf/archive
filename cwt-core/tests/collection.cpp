/*
 * hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include "../include/cwt/test.h"
#include "../include/cwt/hash.hpp"
#include "../include/cwt/map.hpp"
#include "../include/cwt/bytearray.hpp"
#include "../include/cwt/string.hpp"
#include "../include/cwt/safeformat.hpp"
#include <cstring>

using namespace cwt;

template <typename collection_type>
void test_bytearray_collection()
{
	static const int __ENTRY_COUNT  = 10000;
	collection_type collection;
    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	char str[64];
    	sprintf(str, "Test byte array %d", i);
    	collection.insert(ByteArray(str), ByteArray(str));
    }

    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	char str[64];
    	sprintf(str, "Test byte array %d", i);
    	ByteArray sample(str);
    	ByteArray value(collection[str]);
    	CWT_TEST_OK2(strcmp(value.data(),sample.data()) == 0
    			, String(_Fr("[%s] == [%s]") % value % str).c_str());
    }
}


template <typename collection_type>
void test_string_collection()
{
	static const int __ENTRY_COUNT  = 2000;
    collection_type collection;
    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	String str(_Fr("Test string %d") % i);
    	collection.insert(str, str);
    }

    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	String str(_Fr("Test string %d") % i);
    	String sample(str);
    	String value(collection[str]);
    	CWT_TEST_OK2(sample == value
    			, String(_Fr("[%s] == [%s]") % value % sample).c_str());
    }
}

template <typename collection_type>
void test_copy_pod_collection()
{
	collection_type collection;

	collection.insert(1 , 11);
	collection.insert(2 , 22);
	collection.insert(3 , 33);
	collection.insert(4 , 44);

	collection_type collectionCopy(collection);

	collectionCopy[1] = 111;
	collectionCopy[4] = 444;

	CWT_TEST_OK(collection[1] == 11);
	CWT_TEST_OK(collection[2] == 22);
	CWT_TEST_OK(collection[3] == 33);
	CWT_TEST_OK(collection[4] == 44);

	CWT_TEST_OK(collectionCopy[1] == 111);
	CWT_TEST_OK(collectionCopy[2] == 22);
	CWT_TEST_OK(collectionCopy[3] == 33);
	CWT_TEST_OK(collectionCopy[4] == 444);
}

template <typename collection_type>
void test_copy_collection()
{
	collection_type collection;

	collection.insert("One"   , "One Data");
	collection.insert("Two"   , "Two Data");
	collection.insert("Three" , "Three Data");
	collection.insert("Four"  , "Four Data");

	const collection_type constCollectionCopy(collection);

	CWT_TEST_OK(constCollectionCopy["One"]   == String("One Data"));
	CWT_TEST_OK(constCollectionCopy["Two"]   == String("Two Data"));
	CWT_TEST_OK(constCollectionCopy["Three"] == String("Three Data"));
	CWT_TEST_OK(constCollectionCopy["Four"]  == String("Four Data"));

	collection_type collectionCopy(collection);

	collectionCopy["Two"]  = String("Copy of Two Data");
	collectionCopy["Four"] = String("Copy of Four Data");

	CWT_TEST_OK(collection["One"]   == String("One Data"));
	CWT_TEST_OK(collection["Two"]   == String("Two Data"));
	CWT_TEST_OK(collection["Three"] == String("Three Data"));
	CWT_TEST_OK(collection["Four"]  == String("Four Data"));

	CWT_TEST_OK(collectionCopy["One"] == String("One Data"));
	CWT_TEST_OK(collectionCopy["Two"] == String("Copy of Two Data"));
	CWT_TEST_OK(collectionCopy["Three"] == String("Three Data"));
	CWT_TEST_OK(collectionCopy["Four"] == String("Copy of Four Data"));
}


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(24040);

    test_bytearray_collection<Hash<ByteArray, ByteArray> >();
    test_bytearray_collection<Map<ByteArray, ByteArray> >();
    test_string_collection<Hash<String, String> >();
    test_string_collection<Map<String, String> >();
    test_copy_pod_collection<Hash<int,int> >();
    test_copy_pod_collection<Map<int,int> >();
    test_copy_collection<Hash<String,String> >();
    test_copy_collection<Map<String,String> >();

    CWT_END_TESTS;
}
