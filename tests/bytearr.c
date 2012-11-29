/*
 * bytearr.c
 *
 *  Created on: 23.07.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <string.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/bytearr.h>
#include <cwt/logger.h>

const char* loremipsum_orig     = " \t\n\v\f\rLorem ipsum dolor sit amet, consectetuer adipiscing elit \t\n\v\f\r";
const char* loremipsum_ltrimmed = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit \t\n\v\f\r";
const char* loremipsum_rtrimmed = " \t\n\v\f\rLorem ipsum dolor sit amet, consectetuer adipiscing elit";
const char* loremipsum_trimmed  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
const char* hello_world         = "HELLO, WORLD!";
const char* hello_world_replaced= "Hello, World!";
const char* hello_world_replaced_extended= "Hello, World!HELLO, WORLD!";


int main(int argc, char *argv[])
{
	CwtByteArrayNS *baNS = cwt_bytearray_ns();
	CwtByteArray *ba;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(8);

	ba = baNS->create();
	baNS->appendElems(ba, (BYTE*)loremipsum_orig, strlen(loremipsum_orig));
	baNS->ltrim(ba);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum_ltrimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_ltrimmed) == baNS->size(ba) );

	baNS->clear(ba);
	baNS->appendElems(ba, (BYTE*)loremipsum_orig, strlen(loremipsum_orig));
	baNS->rtrim(ba);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum_rtrimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_rtrimmed) == baNS->size(ba) );

	baNS->clear(ba);
	baNS->appendElems(ba, (BYTE*)loremipsum_orig, strlen(loremipsum_orig));
	baNS->trim(ba);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum_trimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_trimmed) == baNS->size(ba) );

	baNS->clear(ba);
	baNS->appendElems(ba, (BYTE*)hello_world, strlen(hello_world));
	baNS->replace(ba, (BYTE*)hello_world_replaced, strlen(hello_world_replaced), 0);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), hello_world_replaced) == 0);

	baNS->replace(ba, (BYTE*)hello_world, strlen(hello_world), baNS->size(ba));
	CWT_TEST_OK(strcmp(baNS->cstr(ba), hello_world_replaced_extended) == 0);

	baNS->free(ba);

	CWT_END_TESTS;
}


