/*
 * test_orderedhash.cpp
 *
 *  Created on: Sep 10, 2013
 *      Author: wladt
 */


#include <cwt/test.h>
#include <cwt/string.hpp>
#include "../include/cwt/orderedhash.hpp"

using namespace cwt;


void test_base()
{
	OrderedHash<String, String> hash;

	hash.insert("One"   , "One Data");
	hash.insert("Two"   , "Two Data");
	hash.insert("Three" , "Three Data");
	hash.insert("Four"  , "Four Data");

	OrderedHash<String, String>::ordered_iterator it = hash.beginOrdered();
	OrderedHash<String, String>::ordered_iterator itEnd = hash.endOrdered();

	CWT_TEST_OK(it++.key() == String("One"));
	CWT_TEST_OK(it++.key() == String("Two"));
	CWT_TEST_OK(it++.key() == String("Three"));
	CWT_TEST_OK(it++.key() == String("Four"));
	CWT_TEST_OK(it == itEnd);

	it = hash.beginOrdered();
	CWT_TEST_OK(*it++ == String("One Data"));
	CWT_TEST_OK(*it++ == String("Two Data"));
	CWT_TEST_OK(*it++ == String("Three Data"));
	CWT_TEST_OK(*it++ == String("Four Data"));
	CWT_TEST_OK(it == itEnd);

	OrderedHash<String, String> hashCopy(hash);

	hashCopy["Two"]  = String("Copy of Two Data");
	hashCopy["Four"] = String("Copy of Four Data");
	it = hashCopy.beginOrdered();
	CWT_TEST_OK(*it++ == String("One Data"));
	CWT_TEST_OK(*it++ == String("Copy of Two Data"));
	CWT_TEST_OK(*it++ == String("Three Data"));
	CWT_TEST_OK(*it++ == String("Copy of Four Data"));
	CWT_TEST_OK(it == hashCopy.endOrdered());
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
    CWT_BEGIN_TESTS(10);

   	test_base();

    CWT_END_TESTS;
    return 0;
}


