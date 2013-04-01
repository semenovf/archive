/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/vector.hpp>
#include <cstring>

using namespace cwt;

#define _VECTOR_SIZE 256

void test_int_vector(void)
{
	Vector<int> v(_VECTOR_SIZE);
	int *pint;

	for(int i = 0; i < _VECTOR_SIZE; i++)
		v.append(i);

	for(int i = 0; i < _VECTOR_SIZE; i++)
		CWT_TEST_OK(v[i] == i);

	pint = v.data();
	for(int i = 0; i < _VECTOR_SIZE; i++)
		CWT_TEST_OK(pint[i] == i);
}


void test_long_vector(void)
{
	Vector<int> v(_VECTOR_SIZE, 0);
	int i;
	int *pint;

	pint = v.data();
	pint[0] = CWT_INT_MIN;
	pint[_VECTOR_SIZE-1] = CWT_INT_MAX;

	for(i = 1; i < _VECTOR_SIZE-1; i++) {
		CWT_TEST_OK(v.at(i) == 0L);
	}

	CWT_TEST_OK(v.at(0) == CWT_INT_MIN);
	CWT_TEST_OK(v.at(_VECTOR_SIZE-1) == CWT_INT_MAX);
}


void test_char_vector(void)
{
	const char* loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = strlen(loremipsum) + 1;
	Vector<char> v(sz);

	v.resize(sz);

	for (i = 0; i < sz; i++)
		v[i] = loremipsum[i];

	Vector<char> peer = v;
	CWT_TEST_OK(strcmp(v.data(), loremipsum) == 0);
	CWT_TEST_OK(strcmp(peer.data(), loremipsum) == 0);

	Vector<char>::const_iterator it = v.cbegin();
	Vector<char>::const_iterator itEnd = v.cend();

	for(int i = 0; it != itEnd; ++it, ++i) {
		CWT_TEST_OK(loremipsum[i] == *it);
	}

	it = v.cbegin();
	for(int i = 0; it != itEnd; it++, i++) {
		CWT_TEST_OK(loremipsum[i] == *it);
	}
}


struct Foo {
	int i;
	char ch;
	int j;
};

void test_vector_iterator(void)
{
	const char* loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = strlen(loremipsum) + 1;
	Vector<Foo> v(sz);

	v.resize(sz);

	for (i = 0; i < sz; i++) {
		v[i].i  = i;
		v[i].j  = i;
		v[i].ch = loremipsum[i];
	}

	{
		Vector<Foo> peer = v;

		Vector<Foo>::iterator it = peer.begin();
		Vector<Foo>::iterator itEnd = peer.end();

		for(int i = 0; it != itEnd; ++it, ++i) {
			CWT_TEST_OK(i == it->i);
			CWT_TEST_OK(i == it->j);
			CWT_TEST_OK(loremipsum[i] == it->ch);
		}
	}


	Vector<Foo>::const_iterator it = v.cbegin();
	Vector<Foo>::const_iterator itEnd = v.cend();

	for(int i = 0; it != itEnd; ++it, ++i) {
		CWT_TEST_OK(i == it->i);
		CWT_TEST_OK(i == it->j);
		CWT_TEST_OK(loremipsum[i] == it->ch);
	}

	it = v.cbegin();
	for(int i = 0; it != itEnd; it++, i++) {
		CWT_TEST_OK(i == it->i);
		CWT_TEST_OK(i == it->j);
		CWT_TEST_OK(loremipsum[i] == it->ch);
	}

}



int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(1397);

	test_int_vector();
	test_long_vector();
	test_char_vector();
	test_vector_iterator();

    CWT_END_TESTS;
}
