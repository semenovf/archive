/*
 * bytearray.cpp
 *
 *  Created on: May 16, 2013
 *      Author: wladt
 */

#include "../include/cwt/test.h"
#include "../include/cwt/bytearray.hpp"
#include "../include/cwt/logger.hpp"
#include "../include/cwt/safeformat.hpp"
#include <cstring>

using namespace cwt;

void test_append(void)
{
	ByteArray ba("One");
	ba.append("Two");

	CWT_TEST_OK(ba.length() == 6);
	CWT_TEST_OK(strlen(ba.constData()) == ba.size());
	CWT_TEST_OK(ba == "OneTwo");

}

void test_convert_number()
{
	ByteArray num("1234");
	bool ok;

	ByteArray ba_double;
	ba_double.setNumber(double(13.14159));
	printf("13.14159 == %s\n", ba_double.constData());
	Logger::trace(_Fr("13.14159 == %s") % ba_double.constData());

	CWT_TEST_OK(num.toInt(&ok) == 1234 && ok);
}


void test_base64()
{
	ByteArray text("Qt is great!");
	CWT_TEST_OK(text.toBase64() == "UXQgaXMgZ3JlYXQh");

	ByteArray based64("UXQgaXMgZ3JlYXQh");
	CWT_TEST_OK(text == ByteArray::fromBase64(based64));
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(6);

	test_append();
	test_convert_number();
	test_base64();

    CWT_END_TESTS;
}
