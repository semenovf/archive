/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/array.hpp>
#include <cstring>

using namespace cwt;

#define _ARR_SIZE 256

void test_int_array(void)
{
	Array<int> arr(_ARR_SIZE);
	int i;
	int *pint;

	for(i = 0; i < _ARR_SIZE; i++)
		arr[i] = i;

	for(i = 0; i < _ARR_SIZE; i++)
		CWT_TEST_OK(arr[i] == i);

	pint = arr.data();
	for(i = 0; i < _ARR_SIZE; i++)
		CWT_TEST_OK(pint[i] == i);

	CWT_TEST_OK(arr.size() == _ARR_SIZE);
	arr.realloc(_ARR_SIZE/2);
	CWT_TEST_OK(arr.size() == _ARR_SIZE/2);

	arr.realloc(_ARR_SIZE);
	CWT_TEST_OK(arr.size() == _ARR_SIZE);
}


void test_long_array(void)
{
	Array<int> arr(_ARR_SIZE, true);
	int i;
	int *pint;

	pint = arr.data();
	pint[0] = CWT_INT_MIN;
	pint[_ARR_SIZE-1] = CWT_INT_MAX;

	for(i = 1; i < _ARR_SIZE-1; i++) {
		CWT_TEST_OK(arr.at(i) == 0L);
	}

	CWT_TEST_OK(arr.at(0) == CWT_INT_MIN);
	CWT_TEST_OK(arr.at(_ARR_SIZE-1) == CWT_INT_MAX);
}


void test_char_array(void)
{
	const char* loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = strlen(loremipsum) + 1;
	Array<char> arr(sz), *clone;

	for (i = 0; i < sz; i++)
		arr[i] = loremipsum[i];

	clone = arr.clone();
	CWT_TEST_OK(strcmp(arr.data(), loremipsum) == 0);
	CWT_TEST_OK(strcmp(clone->data(), loremipsum) == 0);
	CWT_TEST_OK(arr.eq(*clone));
	delete clone;
}

void test_move_array(void)
{
	int int_arr[] = {
		  CWT_INT_MAX
		, CWT_INT_MIN
		, 12345 };

	Array<int> arr(int_arr, 3);
	CWT_TEST_FAIL(arr.size() == sizeof(int_arr)/sizeof(int_arr[0]));
	CWT_TEST_OK(arr[0] == CWT_INT_MAX);
	CWT_TEST_OK(arr[1] == CWT_INT_MIN);
	CWT_TEST_OK(arr[2] == 12345);

	arr.move(0, 1, 1);
	CWT_TEST_OK(arr[0] == CWT_INT_MIN);
	CWT_TEST_OK(arr[2] == 12345);
}


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(780);

	char *p = 0;
	if (__builtin_object_size (p, 0) == sizeof (p)) {
		;
	}

	test_int_array();
	test_long_array();
	test_char_array();
	test_move_array();

    CWT_END_TESTS;
}
