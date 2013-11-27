/*
 * vector.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/vector.hpp>
#include <cstring>
#include <iostream>

#define _VECTOR_SIZE 256

void test_int_vector(void)
{
	pfs::vector<int> v(_VECTOR_SIZE);
	const int *pint;

	for(int i = 0; i < _VECTOR_SIZE; i++)
		v.append(i);

	for(int i = 0; i < _VECTOR_SIZE; i++)
		TEST_OK(v[i] == i);

	pint = v.data();
	for(int i = 0; i < _VECTOR_SIZE; i++)
		TEST_OK(pint[i] == i);
}


void test_long_vector(void)
{
	pfs::vector<int> v(_VECTOR_SIZE, 0);
	int i;
	int *pint;

	pint = v.data();
	pint[0] = PFS_INT_MIN;
	pint[_VECTOR_SIZE-1] = PFS_INT_MAX;

	for(i = 1; i < _VECTOR_SIZE-1; i++) {
		TEST_OK(v.at(i) == 0L);
	}

	TEST_OK(v.at(0) == PFS_INT_MIN);
	TEST_OK(v.at(_VECTOR_SIZE-1) == PFS_INT_MAX);
}


void test_char_vector(void)
{
	const char* loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = strlen(loremipsum) + 1;
	pfs::vector<char> v(sz);

	v.resize(sz);

	for (i = 0; i < sz; i++)
		v[i] = loremipsum[i];

	pfs::vector<char> peer = v;
	TEST_OK(strcmp(v.data(), loremipsum) == 0);
	TEST_OK(strcmp(peer.data(), loremipsum) == 0);

	pfs::vector<char>::const_iterator it = v.cbegin();
	pfs::vector<char>::const_iterator itEnd = v.cend();

	for(int i = 0; it != itEnd; ++it, ++i) {
		TEST_OK(loremipsum[i] == *it);
	}

	it = v.cbegin();
	for(int i = 0; it != itEnd; it++, i++) {
		TEST_OK(loremipsum[i] == *it);
	}
}

struct Foo
{
	int i;
	char ch;
	int j;
	Foo() : i(0), ch(0), j(0) {}
};

void test_vector_iterator(void)
{
	const char* loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = strlen(loremipsum) + 1;
	pfs::vector<Foo> v(sz);

	v.resize(sz);

	for (i = 0; i < sz; i++) {
		v[i].i  = i;
		v[i].j  = i;
		v[i].ch = loremipsum[i];
	}

	{
		pfs::vector<Foo> peer = v;

		pfs::vector<Foo>::iterator it = peer.begin();
		pfs::vector<Foo>::iterator itEnd = peer.end();

		for(int i = 0; it != itEnd; ++it, ++i) {
			TEST_OK(i == it->i);
			TEST_OK(i == it->j);
			TEST_OK(loremipsum[i] == it->ch);
		}
	}


	pfs::vector<Foo>::const_iterator it = v.cbegin();
	pfs::vector<Foo>::const_iterator itEnd = v.cend();

	for(int i = 0; it != itEnd; ++it, ++i) {
		TEST_OK(i == it->i);
		TEST_OK(i == it->j);
		TEST_OK(loremipsum[i] == it->ch);
	}

	it = v.cbegin();
	for(int i = 0; it != itEnd; it++, i++) {
		TEST_OK(i == it->i);
		TEST_OK(i == it->j);
		TEST_OK(loremipsum[i] == it->ch);
	}
}


void test_swap()
{
	const char * lorem  = "Lorem";
	const char * ipsum  = "Ipsum";
	pfs::vector<char> v1(lorem, strlen(lorem));

	if (true) {
		pfs::vector<char> v2(ipsum, strlen(ipsum));

		TEST_OK(strncmp(v1.constData(), lorem, strlen(lorem)) == 0);
		TEST_OK(strncmp(v2.constData(), ipsum, strlen(ipsum)) == 0);

		v2.swap(v1);

		TEST_OK(strncmp(v2.constData(), lorem, strlen(lorem)) == 0);
		TEST_OK(strncmp(v1.constData(), ipsum, strlen(ipsum)) == 0);

		v2.clear();
		v2.append(ipsum, strlen(ipsum));
		TEST_OK(strncmp(v2.constData(), ipsum, strlen(ipsum)) == 0);
	}

	TEST_OK(strncmp(v1.constData(), ipsum, strlen(ipsum)) == 0);
}


void test_vector_vector()
{
	pfs::vector<pfs::vector<int> > records;

	for (int i = 0; size_t(i) < 33; ++i) {
		pfs::vector<int> record;
		for (int j = 0; size_t(j) < 33; ++j) {
			record.append(j);
		}
		records.append(record);
	}

	TEST_FAIL(records.size() == 33);
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(9);

	test_int_vector();
	test_long_vector();
	test_char_vector();
	test_vector_iterator();
	test_swap();
	test_vector_vector();

    END_TESTS;
}
