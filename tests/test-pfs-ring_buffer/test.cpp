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
#include <vector>

#include "pfs/ring_buffer.hpp"

using std::cout;
using std::endl;

struct A
{
	int    a;
	char   b;
	double c;
};

template <typename buffer_type>
void test_pod (buffer_type & rbuffer, size_t count)
{
	ADD_TESTS(4 + count * 3);

	TEST_OK(rbuffer.empty());

	for (int i = 0; i < count; ++i) {
		A x;
		x.a = i;
		x.b = 'A' + i;
		x.c = 3.14f + i;
		rbuffer.push(x);
	}

	TEST_OK(rbuffer.size() == count);
	TEST_OK(!rbuffer.can_push());

	for (int i = 0; i < count; ++i) {
		A x;
		rbuffer.pull_and_pop(& x);

		TEST_OK(x.a == i);
		TEST_OK(x.b == 'A' + i);
		TEST_OK(x.c == 3.14f + i);
	}

	TEST_FAIL(rbuffer.empty());
}

class D
{
public:
	int _a;

	static int alloc_counter;

public:
	D() : _a() { ++alloc_counter; }

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
	E () : D(), _b(0) { ++alloc_counter; }

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

	{
	E buffer[5];
	typedef pfs::ring_buffer<E *> non_pod_rbuffer;

	non_pod_rbuffer * prbuffer = new non_pod_rbuffer(buffer, buffer + 5);
	TEST_OK(prbuffer->empty());

	prbuffer->push(E(0, 'A'));
	prbuffer->push(E(1, 'B'));
	prbuffer->push(E(2, 'C'));
	prbuffer->push(E(3, 'D'));
	prbuffer->push(E(4, 'E'));

	TEST_OK(prbuffer->size() == 5);
	TEST_OK(E::alloc_counter == 5);

	delete prbuffer;
	}

	TEST_OK(E::alloc_counter == 0);
//	cout << "D::alloc_counter=" << D::alloc_counter << endl;
//	cout << "E::alloc_counter=" << E::alloc_counter << endl;
}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

	static const size_t SIZE = 2;

	{
		A buffer[SIZE];
		pfs::ring_buffer<A *> rbuffer(buffer, buffer + SIZE);
		test_pod(rbuffer, SIZE);
	}

	{
		std::vector<A> buffer;
		buffer.resize(SIZE);
		pfs::ring_buffer<std::vector<A>::iterator> rbuffer(buffer.begin(), buffer.end());
		test_pod(rbuffer, SIZE);
	}

	test_non_pod();

	END_TESTS;
}

