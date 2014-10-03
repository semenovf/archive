/*
 * vector.cpp
 *
 *  Created on: Nov 27, 2013
 *      Author: wladt
 */

#include <cstring>
#include <iostream>
#include <cwt/test.hpp>
#include <cwt/stopwatch.hpp>
#include "vector.hpp"
#include "bits/pointer.hpp"
#include "bits/iterator.hpp"

//#include <iostream>

#define _VECTOR_SIZE 256

void test_vector_reference ()
{
	pfs::vector<int> null_vector;
	TEST_OK(null_vector.isNull());
	TEST_OK(null_vector.isEmpty());
	TEST_OK(null_vector.size() == 0);

	// This code predictable crashes the program through 'assert'
	//
	// pfs::vector<int>::reference r(null_vector[0]);

	pfs::vector<char> v1(10, 'W');
	pfs::vector<char> v2(v1);
	pfs::vector<char> v3(v1);

	TEST_OK(v1.size() == 10);
	TEST_OK(v2.size() == 10);
	TEST_OK(v3.size() == 10);

	pfs::vector<char>::reference a1(v1[5]);
	pfs::vector<char>::reference a2(v2[5]);
	pfs::vector<char>::reference a3(v3[5]);

	pfs::vector<char>::reference r3(a3);

	a1 = 'Z';
	TEST_OK(a1 == 'Z');
	TEST_OK(a2 == 'W');
	TEST_OK(a3 == 'W');

	a2 = 'Y';
	TEST_OK(a1 == 'Z');
	TEST_OK(a2 == 'Y');
	TEST_OK(a3 == 'W');

	a3 = 'X';
	TEST_OK(a1 == 'Z');
	TEST_OK(a2 == 'Y');
	TEST_OK(a3 == 'X');

	pfs::vector<char>::reference r1(a1);
	TEST_OK(r1 == 'Z');

	// Predictable compile error
	//
	// const pfs::vector<char>::reference r2(a2);
	// r2 = 'A';

	// r3 is a copy of a3 but a3 was changed. What will with r3
	// Right, it will be equals to new value of a3
	TEST_OK(r3 == 'X');

	v3[0] = 'A';
	v3[9] = 'Z';

	TEST_OK(v3.front() == v3.first() && v3.first() == 'A');
	TEST_OK(v3.back() == v3.last() && v3.last() == 'Z');
}

void test_vector_of_vectors ()
{
	typedef std::vector<int> std_int_vector;
	typedef pfs::vector<int> pfs_int_vector;
	using cwt::test::stopwatch;

	stopwatch sw;
	double ellapsed;

	// std::string
	{
		sw.start();
		std::vector<std_int_vector> std_vector;
		std_vector.resize(10000);
		ellapsed = sw.ellapsed();
		std::cout << "std_vector capacity = " << std_vector.capacity() << std::endl;
		std::cout << "std_vector test ellapsed = " << ellapsed << std::endl;
	}

	// pfs::string
	//
	{
		sw.start();
		pfs::vector<pfs_int_vector> pfs_vector;
		pfs_vector.resize(10000);
		ellapsed = sw.ellapsed();
		std::cout << "pfs_vector capacity = " << pfs_vector.capacity() << std::endl;
		std::cout << "pfs_vector test ellapsed = " << ellapsed << std::endl;
	}
}

struct Foo
{
	int i;
	char ch;
	Foo() : i(0), ch(0) {}
	Foo(int a, char b) : i(a), ch(b) {}
};

void test_vector_iterator ()
{
	const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = std::strlen(loremipsum);
	pfs::vector<Foo> v(sz);

	v.resize(sz);

	for (i = 0; i < sz; i++) {
		Foo f;
		f.i = i;
		f.ch = loremipsum[i];
		v[i] = f;
	}

	{
		pfs::vector<Foo>::iterator it = v.begin();
		pfs::vector<Foo>::iterator itEnd = v.end();

		TEST_OK(it != itEnd);
		TEST_OK(it < itEnd);
		TEST_OK(it <= itEnd);
		TEST_OK(itEnd > it);
		TEST_OK(itEnd >= it);
		TEST_OK(itEnd != it);

		bool ok = true;
		for(int i = 0; it != itEnd; ++it, ++i) {
			if (!(i == it->i
					&& loremipsum[i] == it->ch)) {
				ok = false;
				break;
			}
		}
		TEST_OK2(ok, "Walk through Foo type elements from begin to end using pfs::vector::iterator");
	}


	{
		pfs::vector<Foo>::iterator it = v.end();
		pfs::vector<Foo>::iterator itBegin = v.begin();
		char firstChar = 0;
		char lastChar = 0;

		bool ok = true;
		--it;
		lastChar = it->ch;
		for (int i = sz - 1; it >= itBegin; --it, --i) {
			firstChar = it->ch;
			if (!(i == it->i
					&& loremipsum[i] == it->ch)) {
				ok = false;
				break;
			}
		}

		TEST_OK(firstChar == 'L');
		TEST_OK(lastChar == 't');
		TEST_OK2(ok, "Walk through Foo type elements from end to begin using pfs::vector::iterator");
	}


	{
		pfs::vector<Foo>::const_iterator it = v.cbegin();
		pfs::vector<Foo>::const_iterator itEnd = v.cend();

		bool ok = true;
		for(int i = 0; it != itEnd; ++it, ++i) {
			if (!(i == it->i
					&& loremipsum[i] == it->ch)) {
				ok = false;
				break;
			}
		}
		TEST_OK2(ok, "Walk through Foo type elements from begin to end using pfs::vector::const_iterator");
	}


	{
		pfs::vector<Foo>::const_iterator it = v.cend();
		pfs::vector<Foo>::const_iterator itBegin = v.cbegin();
		char firstChar = 0;
		char lastChar = 0;

		bool ok = true;
		--it;
		lastChar = it->ch;
		for (int i = sz - 1; it >= itBegin; --it, --i) {
			firstChar = it->ch;
			if (!(i == it->i
					&& loremipsum[i] == it->ch)) {
				ok = false;
				break;
			}
		}

		TEST_OK(firstChar == 'L');
		TEST_OK(lastChar == 't');
		TEST_OK2(ok, "Walk through Foo type elements from end to begin using pfs::vector::const_iterator");
	}


	{
		pfs::vector<Foo>::reverse_iterator it = v.rbegin();
		pfs::vector<Foo>::reverse_iterator itEnd = v.rend();

		bool ok = true;
		for(int i = v.size() - 1; it != itEnd; ++it, --i) {
			if (!(i == it->i
					&& loremipsum[i] == it->ch)) {
				ok = false;
				break;
			}
		}
		TEST_OK2(ok, "Walk through Foo type elements using pfs::vector::reverse_iterator");
	}

	{
		pfs::vector<Foo>::const_reverse_iterator it = v.crbegin();
		pfs::vector<Foo>::const_reverse_iterator itEnd = v.crend();

		bool ok = true;

		for(int i = v.size() - 1; it != itEnd; ++it, --i) {
			if (!(i == it->i
					&& loremipsum[i] == it->ch)) {
				ok = false;
				break;
			}
		}

		TEST_OK2(ok, "Walk through Foo type elements using pfs::vector::const_reverse_iterator");
	}
}

void test_vector_insert ()
{
	pfs::vector<Foo> v;
	pfs::vector<Foo>::iterator it = v.insert(v.cbegin(), 3, Foo(int('A'), 'A'));

	TEST_OK(it->i == int('A'));
	TEST_OK(it->ch == 'A');
	TEST_OK((it + 1)->i == int('A'));
	TEST_OK((it + 1)->ch == 'A');
	TEST_OK((it + 2)->i == int('A'));
	TEST_OK((it + 2)->ch == 'A');

	it = v.insert(v.cend(), Foo(int('Z'), 'Z'));

	TEST_OK((v.cbegin() + (v.size() - 1))->i == int('Z'));
	TEST_OK((v.cbegin() + (v.size() - 1))->ch == 'Z');
	TEST_OK((v.cbegin() + (v.size() - 2))->i == int('A'));
	TEST_OK((v.cbegin() + (v.size() - 2))->ch == 'A');


	pfs::vector<char> v1;
	pfs::vector<char> v2;

	const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	const char * p = loremipsum;

	while (*p) {
		v1.insert(v1.cend(), *p);
		++p;
	}

	v2.insert(v2.cbegin(), v1.cbegin(), v1.cend());

	TEST_OK(v1.size() == strlen(loremipsum));
	TEST_OK(v2.size() == strlen(loremipsum));

	p = loremipsum;
	bool ok = true;
	for (pfs::vector<char>::const_iterator it = v1.cbegin()
			; it != v1.cend()
			; ++it, ++p) {
		if (*it != *p) {
			ok = false;
			break;
		}
	}

	TEST_OK2(ok, "Vector instance content equals to source string");

	ok = true;
	pfs::vector<char>::const_iterator it1 = v1.cbegin();
	pfs::vector<char>::const_iterator it2 = v2.cbegin();

	for (; it1 != v1.cend(); ++it1, ++it2) {
		if (*it1 != *it2) {
			ok = false;
			break;
		}
	}

	TEST_OK2(ok, "Vector instance equals to other vector instance");
	TEST_OK(it2 == v2.cend());
}


struct Bar {
	static int counter;
	int i;
	Bar (int a) : i(a)
	{
		++counter;
//		std::cout << "Bar(" << i << ")::counter=" << Bar::counter << std::endl;
	}
	~Bar ()
	{
		--counter;
//		std::cout << "~Bar(" << i << ")::counter=" << Bar::counter << std::endl;
	}

	Bar (const Bar & o)
	{
		i = o.i;
		++counter;
//		std::cout << "Bar(other." << i << ")::counter=" << Bar::counter << std::endl;

	}
	Bar & operator == (const Bar & o)
	{
		i = o.i;
		++counter;
//		std::cout << "operator = (" << i << ")::counter=" << Bar::counter << std::endl;
		return *this;
	}
};

int Bar::counter = 0;

void test_vector_erase ()
{
	{
		pfs::vector<char> v;

		const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
		const char * p = loremipsum;

		while (*p) {
			v.insert(v.cend(), *p);
			++p;
		}

		pfs::vector<char>::iterator it = v.begin();
		TEST_OK((*it) == 'L');

		it = v.erase(v.cbegin());
		TEST_OK((*it) == 'o');
		TEST_OK((*v.cbegin()) == 'o');

		it = v.erase(v.cend() - 1);
		TEST_OK(it == v.cend());

		it = v.erase(v.cbegin() + 2, v.cbegin() + 39);
		TEST_OK(*it == ' ');

		TEST_OK(strncmp("or adipiscing eli", v.data(), v.size()) == 0);

		it = v.erase(v.cbegin(), v.cend());
		TEST_OK(it == v.cend());
		TEST_OK(v.size() == 0);
	}


	{
		std::vector<Bar> v;
		int i = 1;
		v.insert(v.begin(), Bar(i++));
		v.insert(v.begin(), Bar(i++));
		v.insert(v.begin(), Bar(i++));
		v.insert(v.begin(), Bar(i++));
		v.insert(v.begin(), Bar(i++));

		TEST_OK(Bar::counter == i - 1);
//		std::cout << Bar::counter << std::endl;
//		std::cout << v.capacity() << std::endl;
	}
}


void test_vector_append ()
{
	{
		pfs::vector<char> v1;

		const char * loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
		const char * loremipsum1  = "Lorem ipsum dolor sit amet,";
		const char * loremipsum2  = " consectetuer adipiscing elit";

		const char * p = loremipsum1;
		while (*p) {
			v1.insert(v1.cend(), *p);
			++p;
		}

		pfs::vector<char> v2;
		v2.append(v1);

		v1.clear();
		p = loremipsum2;
		while (*p) {
			v1.insert(v1.cend(), *p);
			++p;
		}

		v2.append(v1);
		TEST_OK(strncmp(loremipsum, v2.data(), strlen(loremipsum)) == 0);
	}
}

void test_vector_compare ()
{
	// ==
	{
		pfs::vector<int> v1;
		pfs::vector<int> v2;

		// Empty vectors are equals
		TEST_OK(v1 == v2);

		v1 << 1 << 2 << 3 << 4;
		v2 << 1 << 2 << 3 << 4;

		TEST_OK(v1 == v2);
	}

	// !=
	{
		pfs::vector<int> v1;
		pfs::vector<int> v2;

		v1 << 1 << 2 << 3;
		v2 << 1 << 2 << 3 << 4;

		TEST_OK(v1 != v2);

		v1 << 4 << 5;

		TEST_OK(v1 != v2);
	}

	// <
	{
		pfs::vector<int> v1;
		pfs::vector<int> v2;

		v1 << 1 << 2 << 3;
		v2 << 1 << 2 << 4;

		TEST_OK(v1 < v2);

		v2.clear();
		// Empty vector less than not empty
		TEST_OK(v2 < v1);
		v2 << 1 << 3;

		TEST_OK(v1 < v2);
	}

	// >
	{
		pfs::vector<int> v1;
		pfs::vector<int> v2;

		v1 << 1 << 2 << 3;
		v2 << 1 << 2 << 4;

		TEST_OK(v2 > v1);

		v2.clear();
		// Not empty vector more than empty
		TEST_OK(v1 > v2);
		v2 << 1 << 3;

		TEST_OK(v2 > v1);
	}
}

void test_vector_search ()
{
	pfs::vector<int> v1;
	pfs::vector<int> v2;
	pfs::vector<int> v3;

	v1 << 1 << 2 << 3 << 4 << 5 << 6;
	v2 << 3 << 4;
	v3 << 7;

	pfs::vector<int>::const_iterator it = pfs::search(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend());

	TEST_OK(it != v1.cend());
	TEST_OK(*it == 3);

	it = pfs::search(v1.cbegin(), v1.cend(), v3.cbegin(), v3.cend());

	TEST_OK(it == v1.cend());
}

void test_vector_find ()
{
	pfs::vector<int> v;

	v << 1 << 2 << 3 << 4 << 5 << 6;

	pfs::vector<int>::const_iterator it = pfs::find(v.cbegin(), v.cend(), 4);

	TEST_OK(it != v.cend());
	TEST_OK(*it == 4);

	it = pfs::find(v.cbegin(), v.cend(), 7);
	TEST_OK(it == v.cend());

	TEST_OK(v.contains(1));
	TEST_OK(v.contains(2));
	TEST_OK(v.contains(3));
	TEST_OK(v.contains(4));
	TEST_OK(v.contains(5));
	TEST_OK(v.contains(6));
	TEST_OK(!v.contains(0));
	TEST_OK(!v.contains(7));

	TEST_OK(v.startsWith(1));
	TEST_OK(!v.startsWith(2));

	TEST_OK(v.endsWith(6));
	TEST_OK(!v.endsWith(5));

	pfs::vector<int>::iterator it1 = v.find(6);
	TEST_OK(*it1 == 6);

	pfs::vector<int> v1;
	pfs::vector<int> v2;
	pfs::vector<int> v3;
	pfs::vector<int> v4;
	pfs::vector<int> v5;

	v1 << 1 << 2 << 3;
	v2 << 4 << 5 << 6;
	v3 << 1 << 2 << 3 << 5;
	v4 << 4 << 5 << 6 << 7;

	TEST_OK(v.startsWith(v1));
	TEST_OK(!v.startsWith(v2))
	TEST_OK(!v.endsWith(v1));;
	TEST_OK(v.endsWith(v2));

	TEST_OK(!v.startsWith(v3));
	TEST_OK(!v.endsWith(v4));

	// Starts with null (empty) vector
	TEST_OK(v.startsWith(v5));

	// Ends with null (empty) vector
	TEST_OK(v.endsWith(v5));

	//pfs::vector<int>::const_iterator it2(it1); // FIXME need iterator to const_iterator conversion
}

void test_swap ()
{
	pfs::vector<int> v1;
	pfs::vector<int> v2;

	v1 << 1 << 2 << 3;
	v2 << 4 << 5 << 6;

	v2.swap(v1);

	TEST_OK(v1[0] == 4 && v1[1] == 5 && v1[2] == 6);
	TEST_OK(v2[0] == 1 && v2[1] == 2 && v2[2] == 3);
}


int main(int argc, char * argv[])
{
//    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(99);

	if (1) {
	test_vector_reference();
	test_vector_of_vectors();
	test_vector_iterator();
	test_vector_insert();
	test_vector_erase();
	test_vector_append();
	test_vector_compare();
	test_vector_search();
	test_vector_find();
	test_swap();
	}
    END_TESTS;
}
