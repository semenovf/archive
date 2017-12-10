/*
 * async_qt.cpp
 *
 *  Created on: May 24, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/async.hpp>
#include <cstring>

int int_hello_void()
{
	CWT_TRACE(">>int_hello_void()");
	return 10;
}

int int_hello_int(int i)
{
	CWT_TRACE(">>int_hello_int()");
	return i;
}

int int_hello_int_char(int i, char c)
{
	CWT_TRACE(">>int_hello_int_char()");
	printf(">>int_hello_int_char(%d, '%c')\n", i, c);
	return i;
}

int int_hello_int2(int i, int a1, int a2)
{
	CWT_TRACE(">>int_hello_int2()");
	printf(">>int_hello_int2(%d, %d, %d)\n", i, a1, a2);
	return i;
}

using namespace cwt;

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(4);

	cwt::future<int> future = cwt::async(int_hello_void);
	cwt::future<int> future_int = cwt::async(int_hello_int, 12);
	cwt::future<int> future_int_char = cwt::async(int_hello_int_char, 13, 'a');
	cwt::future<int> future_int2 = cwt::async(int_hello_int2, 14, 15, 16);
	CWT_TEST_OK(future.result() == 10);
	CWT_TEST_OK(future_int.result() == 12);
	CWT_TEST_OK(future_int_char.result() == 13);
	CWT_TEST_OK(future_int2.result() == 14);

    CWT_END_TESTS;
}
