/*
 * map_hash.cpp
 *
 *  Created on: Feb 23, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
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
    	string key("Test byte string ");
    	string value("Test byte string ");
    	key += pfs::byte_string::toString(i).c_str();
    	value += pfs::byte_string::toString(i).c_str();

    	std::pair<typename collection_type::iterator, bool> it = collection.insert(key, value);
    	TEST_OK(it.second == true);

    	typename collection_type::pointer p = it.first.base();
    	TEST_OK((*p).first == key);
    }

    TEST_OK(collection.size() == ENTRY_COUNT);

    for (int i = 0; i < ENTRY_COUNT; ++i) {
    	string sample("Test byte array ");
    	sample += pfs::byte_string::toString(i).c_str();
    	typename collection_type::reference ref = collection.at(sample);
//    	typename collection_type::value_type value = collection.valueAt(sample);
    	typename collection_type::value_type value = (*(& ref));

    	string ss;
    	ss += '[' + value.second + "] == [" + sample + ']';

    	TEST_OK2(value.second == sample, ss.c_str());
    }
}


#ifdef __COMMENT__
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
#endif


int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(10020);

	typedef pfs::map<string, string> map;
	map m;
	m.insert("First", "Hello");
	m.insert("Second", "World");

	TEST_OK(m.size() == 2);

	map::reference ref = m.at("First");
	map::value_type value = (*(& ref));
	TEST_OK(value.second == "Hello");

	map::reference ref1 = m.at("Second");
	map::value_type value1 = (*(& ref1));
	TEST_OK(value1.second == "World");

	std::pair<const string, string> a("A","B");
	std::pair<const string, string> b;

	b = a;
	a = b;

//	std::pair<const string, string> & std::pair<const string, string>::operator = (const std::pair<const string, string> &)

	if (0) {
		test_byte_string_collection<pfs::map<string, string> >();
	}
//    test_copy_pod_collection<pfs::map<int,int> >();
//    test_copy_collection<pfs::map<pfs::byte_string,std::string> >();

    END_TESTS;
}
