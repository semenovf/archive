/*
 * map_hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/map.hpp>
//#include <pfs/hash.hpp>
#include <iostream>
#include <sstream>

template <typename collection_type>
void test_bytearray_collection()
{
	static const int __ENTRY_COUNT  = 10000;
	collection_type collection;
    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	std::stringstream str;
    	str << "Test byte array " << i;
    	collection.insert(pfs::bytearray(str.str()), pfs::bytearray(str.str()));
    }

    for(int i = 0; i < __ENTRY_COUNT; i++) {
    	std::stringstream str;
    	str << "Test byte array " << i;
    	pfs::bytearray sample(str.str());
    	pfs::bytearray value(collection[str.str()]);

    	std::stringstream ss;
    	ss << '[' << value << "] == [" << sample << ']';

    	TEST_OK2(value == sample, ss.str());
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

	TEST_OK(collection[1] == 11);
	TEST_OK(collection[2] == 22);
	TEST_OK(collection[3] == 33);
	TEST_OK(collection[4] == 44);

	TEST_OK(collectionCopy[1] == 111);
	TEST_OK(collectionCopy[2] == 22);
	TEST_OK(collectionCopy[3] == 33);
	TEST_OK(collectionCopy[4] == 444);
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

	TEST_OK(constCollectionCopy["One"]   == "One Data");
	TEST_OK(constCollectionCopy["Two"]   == "Two Data");
	TEST_OK(constCollectionCopy["Three"] == "Three Data");
	TEST_OK(constCollectionCopy["Four"]  == "Four Data");

	collection_type collectionCopy(collection);

	collectionCopy["Two"]  = "Copy of Two Data";
	collectionCopy["Four"] = "Copy of Four Data";

	TEST_OK(collection["One"]   == "One Data");
	TEST_OK(collection["Two"]   == "Two Data");
	TEST_OK(collection["Three"] == "Three Data");
	TEST_OK(collection["Four"]  == "Four Data");

	TEST_OK(collectionCopy["One"]   == "One Data");
	TEST_OK(collectionCopy["Two"]   == "Copy of Two Data");
	TEST_OK(collectionCopy["Three"] == "Three Data");
	TEST_OK(collectionCopy["Four"]  == "Copy of Four Data");
}


int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(10020);

//    test_bytearray_collection<Hash<pfs::bytearray, pfs::bytearray> >();
    test_bytearray_collection<pfs::map<pfs::bytearray, pfs::bytearray> >();
//    test_copy_pod_collection<Hash<int,int> >();
    test_copy_pod_collection<pfs::map<int,int> >();
//    test_copy_collection<Hash<pfs::string,pfs::string> >();
    test_copy_collection<pfs::map<pfs::bytearray,std::string> >();

    END_TESTS;
}
