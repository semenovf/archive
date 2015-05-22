/*
 * tests/stack.cpp
 *
 *  Created on: Jan 09, 2015
 *      Author: wladt
 */

#include <pfs/test.hpp>
#include <pfs/stack.hpp>
#include <cstring>
#include <iostream>

void test_stack_reference ()
{
	pfs::stack<int> null_stack;
	TEST_OK(null_stack.isNull());
	TEST_OK(null_stack.isEmpty());
	TEST_OK(null_stack.size() == 0);

	pfs::stack<int> s1;

	s1.push(2);
	s1.push(6);
	s1.push(51);

	TEST_OK(s1.size() == 3);
	TEST_OK(s1.top() == 51);
	TEST_OK(s1.size() == 3);
	s1.pop();
	TEST_OK(s1.size() == 2);
	TEST_OK(s1.top() == 6);

	pfs::stack<int> s2(s1);
	s1.push(32);
	TEST_OK(s1.size() == 3);
	TEST_OK(s2.size() == 2)
	TEST_OK(s1.top() == 32);
	TEST_OK(s2.top() == 6);

	pfs::stack<int> s3;
	s3.push(10);
	s3.push(11);
	s3.push(12);
	s3.push(13);
	s3.push(14);

	pfs::stack<int>::reference r = s3.top();
	TEST_OK(r == 14);
	r = 32;
	r = s3.top();
	TEST_OK(r == 32);
	s3.pop();
	TEST_OK(s3.top() == 13);
}

void test_stack_swap ()
{
	pfs::stack<int> s1;
	pfs::stack<int> s2;

	s1 << 1 << 2 << 3;
	s2 << 4 << 5 << 6;

	TEST_OK(s1.top() == 3);
	TEST_OK(s2.top() == 6);
	s2.swap(s1);
	TEST_OK(s1.top() == 6);
	TEST_OK(s2.top() == 3);
}

int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(19);

	test_stack_reference();
	test_stack_swap();

	END_TESTS;
}
