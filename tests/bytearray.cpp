/*
 * bytearray.cpp
 *
 *  Created on: May 16, 2013
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs/bytearray.hpp>
#include <cstring>
#include <iostream>

void test_append(void)
{
	pfs::bytearray ba("One");
	ba.append("Two");

	TEST_OK(ba.length() == 6);
	TEST_OK(strlen(ba.constData()) == ba.size());
	TEST_OK(ba == "OneTwo");
}

void test_convert_number()
{
	pfs::bytearray num("1234");
	bool ok;

	pfs::bytearray ba_double;
	ba_double.setNumber(double(13.14159));
	printf("13.14159 == %s\n", ba_double.constData());
	//CWT_TRACE(String(_Fr("13.14159 == %s") % ba_double.constData()).c_str());

	TEST_OK(num.toInt(&ok) == 1234 && ok);
}


void test_base64 ()
{
	pfs::bytearray text("Qt is great!");
	TEST_OK(text.toBase64() == "UXQgaXMgZ3JlYXQh");

	pfs::bytearray based64("UXQgaXMgZ3JlYXQh");
	TEST_OK(text == pfs::bytearray::fromBase64(based64));
}

void test_cow ()
{
	pfs::bytearray s1("Hello?");
	pfs::byteref c = s1[5]; // Non-const detachment does nothing here
	pfs::bytearray s2(s1);  // Lazy-copy, shared state
	c = '!';

	TEST_OK(s1[5] != s2[5]); // COW OK
	TEST_OK(s1[5] == '!');   // COW OK
	TEST_OK(s2[5] == '?');   // COW OK

	std::cout << "s1 =" << s1.c_str() << std::endl;
	std::cout << "s2 =" << s2.c_str() << std::endl;
}

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(9);

	test_append();
	test_convert_number();
	test_base64();
	test_cow();

    END_TESTS;
}
