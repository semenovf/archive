/*
 * bytearray.cpp
 *
 *  Created on: May 16, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/bytearray.hpp>
#include <cwt/logger.hpp>
#include <cstring>

using namespace cwt;

void test_append(void)
{
	ByteArray ba("One");
	ba.append("Two");

	CWT_TEST_OK(ba.length() == 6);
	CWT_TEST_OK(strlen(ba.constData()) == ba.size());
}

void test_convert_number()
{
	ByteArray num("1234");
	bool ok;

	ByteArray ba_double;
	ba_double.setNumber(double(13.14159));
	Logger::debug("13.14159 == %s", ba_double.constData());

	CWT_TEST_OK(num.toInt(&ok) == 1234 && ok);
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(2);

	test_append();
	test_convert_number();

    CWT_END_TESTS;
}
