/*
 * map_hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/map.hpp>
//#include <pfs/hash.hpp>
#include <string>
#include <iostream>

using namespace std;

template <typename collection_type>
void test_byte_string_collection()
{
	static const int ENTRY_COUNT  = 10000;
	collection_type collection;

    for (int i = 0; i < ENTRY_COUNT; ++i) {
    	typename collection_type::key_type key("Test byte string ");
    	typename collection_type::value_type value("Test byte string ");
    	key += pfs::byte_string::toString(i).c_str();
    	value += pfs::byte_string::toString(i).c_str();

    	typename collection_type::iterator it = collection.insert(key, value);
    }

    TEST_OK(collection.size() == ENTRY_COUNT);

    std::string ss;
    int i;
    for (i = 0; i < ENTRY_COUNT; ++i) {
    	typename collection_type::key_type key("Test byte string ");
    	key += pfs::byte_string::toString(i).c_str();
//    	typename collection_type::reference ref = collection.at(sample);
    	typename collection_type::value_type value = collection.valueAt(key);

    	ss.clear();
    	ss += "expected: `";
    	ss += key.c_str();
    	ss += "', got: `";
    	ss += value.c_str();
    	ss += '\'';
    	cout << i << '\r';
    	if (value != key)
    		break;
    }

    if (i != ENTRY_COUNT) {
    	cout << "Failed when: " << ss << endl;
    }
    TEST_FAIL2(i == ENTRY_COUNT, "Insertion and accession of pfs::byte_string type values");
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

template <typename collection_type>
void test_get_keys ()
{
	collection_type collection;

	collection.insert("One"   , "One Data");
	collection.insert("Two"   , "Two Data");
	collection.insert("Three" , "Three Data");
	collection.insert("Four"  , "Four Data");

	TEST_OK(collection.contains("One"));
	TEST_OK(collection.contains("Two"));
	TEST_OK(collection.contains("Three"));
	TEST_OK(collection.contains("Four"));

	// Default value
	TEST_OK(collection.valueAt("Five", "Not found") == "Not found");

	pfs::vector<typename collection_type::key_type> keys = collection.keys();
	TEST_OK(keys.contains("One"));
	TEST_OK(keys.contains("Two"));
	TEST_OK(keys.contains("Three"));
	TEST_OK(keys.contains("Four"));

	// Remove
	TEST_OK(collection.size() == 4);
	TEST_OK(collection.remove("One") == 1);
	TEST_OK(collection.remove("Five") == 0);
	TEST_OK(collection.remove("One") == 0);
	TEST_OK(!collection.contains("One"));
	TEST_OK(collection.size() == 3);

//	TEST_OK(collection.remove(collection.find("Two")) != collection.end());
//	TEST_OK(collection.remove(collection.find("Five")) == collection.end());
//	TEST_OK(!collection.contains("Two"));
//	TEST_OK(collection.size() == 2);
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(37);

	//
	// Iterator to const_iterator conversion check at compile time.
	// If no compile time error occurred so the conversion is Ok.
	//
	typedef pfs::map<int, int> map;
	map::iterator it;
	//map::const_iterator it1(it);
	//it1 = it;

	if (1) test_byte_string_collection<pfs::map<pfs::byte_string, pfs::byte_string> >();
	if (1) test_copy_pod_collection<pfs::map<int,int> >();
    if (1) test_copy_collection<pfs::map<std::string,std::string> >();
    if (1) test_get_keys<pfs::map<std::string,std::string> >();

    END_TESTS;
}
