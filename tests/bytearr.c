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

const char* loremipsum_orig      = " \t\n\v\f\rLorem ipsum dolor sit amet, consectetuer adipiscing elit \t\n\v\f\r";
const char* loremipsum_ltrimmed  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit \t\n\v\f\r";
const char* loremipsum_rtrimmed  = " \t\n\v\f\rLorem ipsum dolor sit amet, consectetuer adipiscing elit";
const char* loremipsum_trimmed   = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";
const char* hello_world          = "HELLO, WORLD!";
const char* hello_world_replaced = "Hello, World!";

int main(int argc, char *argv[])
{
	CwtByteArrayNS *ba_ns = cwt_bytearray_ns();
	CwtByteArray *ba;
	size_t i, n;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(7);

	ba = ba_ns->create();
	ba_ns->appendBytes(ba, (BYTE*)loremipsum_orig, strlen(loremipsum_orig));
	ba_ns->ltrim(ba);
	CWT_TEST_OK(strcmp(ba_ns->cstr(ba), loremipsum_ltrimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_ltrimmed) == ba_ns->size(ba) );

	ba_ns->clear(ba);
	ba_ns->appendBytes(ba, (BYTE*)loremipsum_orig, strlen(loremipsum_orig));
	ba_ns->rtrim(ba);
	CWT_TEST_OK(strcmp(ba_ns->cstr(ba), loremipsum_rtrimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_rtrimmed) == ba_ns->size(ba) );

	ba_ns->clear(ba);
	ba_ns->appendBytes(ba, (BYTE*)loremipsum_orig, strlen(loremipsum_orig));
	ba_ns->trim(ba);
	CWT_TEST_OK(strcmp(ba_ns->cstr(ba), loremipsum_trimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_trimmed) == ba_ns->size(ba) );

	ba_ns->clear(ba);
	ba_ns->appendBytes(ba, (BYTE*)hello_world, strlen(hello_world));
	n = strlen(hello_world_replaced);
	for (i = 0; i < n; ++i)
		ba_ns->replace(ba, (BYTE)hello_world_replaced[i], i);
	CWT_TEST_OK(strcmp(ba_ns->cstr(ba), hello_world_replaced) == 0);

	ba_ns->free(ba);

	CWT_END_TESTS;
}


