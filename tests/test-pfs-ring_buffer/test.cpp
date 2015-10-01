/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include <pfs/utility.hpp>
#include <iostream>

#include "pfs/ring_buffer.hpp"

using std::cout;
using std::endl;

struct A
{
	int a;
};


struct B
{
	int  a;
	char b;
};

struct C
{
	int    a;
	char   b;
	double c;
};

//#define ITEM_SIZE 20 // must be enough to store A, B and C structures

void test_pod ()
{
	ADD_TESTS(44);

	pfs::ring_buffer<C> rbuffer(12);
	TEST_OK(rbuffer.isEmpty());

	for (int i = 0; i < 4; ++i) {
		A x;
		x.a = i;
		rbuffer.push(x);
	}

	TEST_OK(rbuffer.size() == 4);
	TEST_FAIL(rbuffer.canPush(4));

	for (int i = 0; i < 4; ++i) {
		B x;
		x.a = i;
		x.b = 'W';
		rbuffer.push(x);
	}

	TEST_OK(rbuffer.size() == 8);
	TEST_FAIL(rbuffer.canPush(4));

	for (int i = 0; i < 4; ++i) {
		C x;
		x.a = i;
		x.b = 'W';
		x.c = 3.14;
		rbuffer.push(x);
	}

	TEST_OK(rbuffer.size() == 12);
	TEST_OK(!rbuffer.canPush());

	for (int i = 0; i < 4; ++i) {
		TEST_FAIL(!rbuffer.isEmpty());

		A x;
		rbuffer.pop<A>(x);
		TEST_OK(x.a == i);
	}

	for (int i = 0; i < 4; ++i) {
		TEST_FAIL(!rbuffer.isEmpty());

		B x;
		rbuffer.pop<B>(x);

		TEST_OK(x.a == i);
		TEST_OK(x.b == 'W');
	}

	for (int i = 0; i < 4; ++i) {
		TEST_FAIL(!rbuffer.isEmpty());

		C x;
		rbuffer.pop<C>(x);

		TEST_OK(x.a == i);
		TEST_OK(x.b == 'W');
		TEST_OK(x.c == 3.14);
	}

	TEST_FAIL(rbuffer.isEmpty());
}

class D
{
public:
	int _a;

	static int alloc_counter;

public:
	D (int a) : _a(a)
	{
		++alloc_counter;
	}

	D (const D & other)
		: _a(other._a)
	{
		++alloc_counter;
	}

	virtual ~D ()
	{
//		cout << "~D()" << endl;
		--alloc_counter;
	}

};

class E : public D
{
public:
	char _b;

	static int alloc_counter;

public:
	E (int a, char b)
		: D(a)
		, _b(b)
	{
//		cout << "E(int,int)" << endl;
		++alloc_counter;
	}

	E (const E & other)
		: D(other._a)
		, _b(other._b)
	{
//		cout << "E(const E &)" << endl;
		++alloc_counter;
	}

	virtual ~E ()
	{
//		cout << "~E()" << endl;
		--alloc_counter;
	}
};

int D::alloc_counter = 0;
int E::alloc_counter = 0;

void test_non_pod ()
{
	ADD_TESTS(4);

	typedef pfs::ring_buffer<E> non_pod_rbuffer;

	non_pod_rbuffer * prbuffer = new non_pod_rbuffer(5);
	TEST_OK(prbuffer->isEmpty());

	prbuffer->push(E(0, 'A'));
	prbuffer->push(E(1, 'B'));
	prbuffer->push(E(2, 'C'));
	prbuffer->push(E(3, 'D'));
	prbuffer->push(E(4, 'E'));

	TEST_OK(prbuffer->size() == 5);
	TEST_OK(E::alloc_counter == 5);

	delete prbuffer;
	TEST_OK(E::alloc_counter == 0);

//	cout << "D::alloc_counter=" << D::alloc_counter << endl;
//	cout << "E::alloc_counter=" << E::alloc_counter << endl;
}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

	test_pod();
	test_non_pod();

	END_TESTS;
}

