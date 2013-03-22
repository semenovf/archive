/*
 * string.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/array.hpp>

using namespace cwt;

#define _ARR_SIZE 256

void test_int_array(void)
{
	Array arr(_ARR_SIZE);
	int *pint_data;
	int i;

	arr = array_ns->allocInt(_ARR_SIZE);
	for(i = 0; i < _ARR_SIZE; i++)
		array_ns->put(arr, i, &i);

	for(i = 0; i < _ARR_SIZE; i++) {
		CWT_TEST_FAIL(pint_data = (int*)array_ns->at(arr, i));
		CWT_TEST_OK(*pint_data == i);
	}

	pint_data = array_ns->data(arr);
	for(i = 0; i < _ARR_SIZE; i++) {
		CWT_TEST_OK(pint_data[i] == i);
	}

	CWT_TEST_OK(array_ns->size(arr) == _ARR_SIZE);

	CWT_TEST_OK(array_ns->at(arr, _ARR_SIZE) == NULL);
	CWT_TEST_OK(array_ns->at(arr, -1) == NULL);

	arr = array_ns->realloc(arr, _ARR_SIZE/2);
	CWT_TEST_OK(array_ns->size(arr) == _ARR_SIZE/2);
	CWT_TEST_OK(array_ns->at(arr, _ARR_SIZE/2) == NULL);
	CWT_TEST_OK(array_ns->at(arr, _ARR_SIZE/2-1) != NULL);
	CWT_TEST_OK(array_ns->at(arr, -1) == NULL);

	arr = array_ns->realloc(arr, _ARR_SIZE);
	CWT_TEST_OK(array_ns->size(arr) == _ARR_SIZE);
	CWT_TEST_OK(array_ns->at(arr, _ARR_SIZE) == NULL);
	CWT_TEST_OK(array_ns->at(arr, -1) == NULL);

	array_ns->free(arr);
}


void test_long_array(void)
{
	CwtArrayNS *array_ns = cwt_array_ns();
	CwtArray   *arr;
	long *plong_data;
	const long *plong;
	int i;

	arr = array_ns->allocLong(_ARR_SIZE);

	plong_data = (long*)array_ns->data(arr);
	plong_data[0] = CWT_LONG_MIN;
	plong_data[_ARR_SIZE-1] = CWT_LONG_MAX;

	for(i = 1; i < _ARR_SIZE-1; i++) {
		plong = array_ns->at(arr, i);
		CWT_TEST_FAIL(plong);
		CWT_TEST_OK(*plong == 0L);
	}

	plong = array_ns->at(arr, 0);
	CWT_TEST_FAIL(plong);
	CWT_TEST_OK(*plong == CWT_LONG_MIN);

	plong = array_ns->at(arr, _ARR_SIZE-1);
	CWT_TEST_FAIL(plong);
	CWT_TEST_OK(*plong == CWT_LONG_MAX);

	array_ns->free(arr);
}


void test_char_array(void)
{
	CwtArrayNS *array_ns = cwt_array_ns();
	CwtArray   *arr, *clone;
	const char* loremipsum  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
	int sz;
	int i;

	sz = strlen(loremipsum) + 1;
	arr = array_ns->allocChar(sz);
	for (i = 0; i < sz; i++)
		array_ns->put(arr, i, &loremipsum[i]);

	clone = array_ns->clone(arr);
	CWT_TEST_OK(strcmp(array_ns->data(arr), loremipsum) == 0);
	CWT_TEST_OK(strcmp(array_ns->data(clone), loremipsum) == 0);
	CWT_TEST_OK(array_ns->eq(arr, clone));

	array_ns->free(clone);
	array_ns->free(arr);
}

void test_move_array(void)
{
	CwtArrayNS *array_ns = cwt_array_ns();
	int *pint_data;

	CWT_STATIC_ARRAY_BEGIN(arr, int, 3)
		  CWT_INT_MAX
		, CWT_INT_MIN
		, 12345
	CWT_STATIC_ARRAY_END(arr)

	pint_data = array_ns->data(arr);
	CWT_TEST_OK(pint_data[0] == CWT_INT_MAX);
	CWT_TEST_OK(pint_data[1] == CWT_INT_MIN);
	CWT_TEST_OK(pint_data[2] == 12345);

	array_ns->move(arr, 0, 1, 1);
	CWT_TEST_OK(pint_data[0] == CWT_INT_MIN);
	CWT_TEST_OK(pint_data[2] == 12345);
}




int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(1298);

	test_int_array();
	test_long_array();
	test_char_array();
	test_move_array();

    CWT_END_TESTS;
}
