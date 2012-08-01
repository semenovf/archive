/*
 * list.c
 *
 *  Created on: 01.08.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/list.h>

static CwtStrNS *__strNS = NULL;
static CwtListNS *__listNS = NULL;



static void test_00(void)
{
	CwtList *list;
	int i;
	int count = 10000;

	list = __listNS->create(sizeof(int), NULL);

	i = CWT_INT_MAX;
	__listNS->append(list, &i);
	CWT_TEST_OK( *((int*)__listNS->first(list)) == CWT_INT_MAX);

	i = CWT_INT_MAX - 1;
	__listNS->append(list, &i);
	CWT_TEST_OK( *((int*)__listNS->last(list)) == CWT_INT_MAX - 1);

	i = CWT_INT_MIN;
	__listNS->prepend(list, &i);
	CWT_TEST_OK( *((int*)__listNS->first(list)) == CWT_INT_MIN);

	CWT_TEST_OK(__listNS->size(list) == 3);

	__listNS->clear(list);

	CWT_TEST_OK(__listNS->size(list) == 0);

	for( i = 0; i < count; i++ ) {
		__listNS->append(list, &i);
	}

	CWT_TEST_OK(__listNS->size(list) == count);

	for( i = 0; i < count; i++ ) {
		__listNS->removeFirst(list);
	}

	CWT_TEST_OK(__listNS->size(list) == 0);

	__listNS->free(list);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	__strNS = cwtStrNS();
	__listNS = cwtListNS();

	CWT_BEGIN_TESTS(4);
	test_00();

	CWT_END_TESTS;
}


