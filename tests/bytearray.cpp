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

void test_length ()
{
	pfs::bytearray ba("Hello");
	TEST_OK(ba.length() == 5);
	TEST_OK(pfs::bytearray::length(ba.cbegin(), ba.cend()) == 5);
	TEST_OK(pfs::bytearray::length(ba.cend(), ba.cbegin()) == 5);
	TEST_OK(pfs::bytearray::length(ba.cbegin()+1, ba.cend()) == 4);
	TEST_OK(pfs::bytearray::length(ba.cbegin()+1, ba.cend()-1) == 3);
	TEST_OK(pfs::bytearray::length(ba.cbegin(), ba.cbegin()) == 0);
	TEST_OK(pfs::bytearray::length(ba.cend(), ba.cend()) == 0);
}

void test_append ()
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
	printf("13.14159 == %g\n", double(13.14159f));
	printf("13.1415912345678 == %e\n", 13.1415912345678f);

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
	BEGIN_TESTS(16);

	test_length();
	test_append();
	test_convert_number();
	test_base64();
	test_cow();

    END_TESTS;
}


// TODO Add this tests
/***

void
test(char* str)
{
double d1, d2;
char *e1, *e2;
int x1, x2;

printf("CASE: %s\n", str);

errno = 0;
e1 = NULL;
d1 = vim_strtod(str, &e1);
x1 = errno;

errno = 0;
e2 = NULL;
d2 = strtod(str, &e2);
x2 = errno;

if (d1 != d2 || e1 != e2 || x1 != x2) {
printf(" ERR: %s, %s\n", str, strerror(errno));
printf(" E1 %f, %g, %s, %d\n", d1, d1, e1 ? e1 : "", x1);
printf(" E2 %f, %g, %s, %d\n", d2, d2, e2 ? e2 : "", x2);
if (d1 != d2) puts("different value");
if (e1 != e2) puts("different end position");
if (x1 != x2) puts("different errno");
} else {
printf(" SUCCESS [%f][%s]: %s\n", d1, e1 ? e1 : "", strerror(errno));
}
printf("\n");
}

int
main()
{
test(".1");
test(" .");
test(" 1.2e3");
test(" +1.2e3");
test("1.2e3");
test("+1.2e3");
test("+1.e3");
test("-1.2e3");
test("-1.2e3.5");
test("-1.2e");
test("--1.2e3.5");
test("--1-.2e3.5");
test("-a");
test("a");
test(".1e");
test(".1e3");
test(".1e-3");
test(".1e-");
test(" .e-");
test(" .e");
test(" e");
test(" e0");
test(" ee");
test(" -e");
test(" .9");
test(" ..9");
test("009");
test("0.09e02");
// http://thread.gmane.org/gmane.editors.vim.devel/19268/
test("0.9999999999999999999999999999999999");
test("2.2250738585072010e-308"); // BUG
// PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0
test("2.2250738585072011e-308");
// Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again
test("2.2250738585072012e-308");
test("2.2250738585072013e-308"); // Hmm.
test("2.2250738585072014e-308"); // Hmm.
}
 ***/
