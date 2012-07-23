/*
 * bytearr.c
 *
 *  Created on: 23.07.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/bytearr.h>
#include <cwt/logger.h>

const char* loremipsum_orig     = " \t\n\v\f\rLorem ipsum dolor sit amet, consectetuer adipiscing elit \t\n\v\f\r";
const char* loremipsum_ltrimmed = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit \t\n\v\f\r";
const char* loremipsum_rtrimmed = " \t\n\v\f\rLorem ipsum dolor sit amet, consectetuer adipiscing elit";
const char* loremipsum_trimmed  = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit";


int main(int argc, char *argv[])
{
	CwtByteArrayNS *baNS = cwtByteArrayNS();
	CwtByteArray *ba;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(6);

	ba = baNS->create();
	baNS->appendElems(ba, loremipsum_orig, strlen(loremipsum_orig));
	baNS->ltrim(ba);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum_ltrimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_ltrimmed) == baNS->size(ba) );

	baNS->clear(ba);
	baNS->appendElems(ba, loremipsum_orig, strlen(loremipsum_orig));
	baNS->rtrim(ba);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum_rtrimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_rtrimmed) == baNS->size(ba) );

	baNS->clear(ba);
	baNS->appendElems(ba, loremipsum_orig, strlen(loremipsum_orig));
	baNS->trim(ba);
	CWT_TEST_OK(strcmp(baNS->cstr(ba), loremipsum_trimmed) == 0);
	CWT_TEST_OK(strlen(loremipsum_trimmed) == baNS->size(ba) );

	baNS->free(ba);

	CWT_END_TESTS;
}


