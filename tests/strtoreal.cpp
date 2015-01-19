/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#include <cwt/test.hpp>
#include <pfs.hpp>
#include <pfs/bits/strtoreal.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <clocale>
#include <cerrno>
#include <cmath> // for isnan and ininf

using namespace std;
#define strtoreal(s,endptr) pfs::strtoreal<char, const char *>(s, s + strlen(s), '.', endptr)

#ifdef __COMMENT__
extern "C" real_t pfs_strtoreal (const char * nptr, char decimalPoint, char ** endptr);
#define pfs_strtodx(x) pfs_strtoreal(x, '.', NULL)

static char _MSG[512];

char * doubleToHex (real_t x)
{
    unsigned int i;
    static char buf[100];
    unsigned char const * p = (unsigned char const *) & x;

    for (i = 0; i < sizeof x; ++i)
    	sprintf(buf + 2*i, "%02x", *p++);

    return buf;
}

bool __compare_with_strtold (const char * s)
{
	char * endptr1 = NULL;
	char * endptr2 = NULL;
	real_t d1 = pfs_strtoreal(s, '.', & endptr1);
/*	pdh_strtold(s, & endptr1);*/
	real_t d2 = strtold(s, & endptr2);

	sprintf(_MSG,
#ifdef PFS_HAVE_LONG_DOUBLE
		"%s: %.20Lg %s %.20Lg (s=%p; endptr1=%p; endptr2=%p)%s"
#else
		"%s: %.20g %s %.20g (s=%p; endptr1=%p; endptr2=%p)%s"
#endif
			, s
			, d1
			, (d1 == d2 /*&& endptr1 == endptr2*/ ? "==" : "!=")
			, d2
			, s
			, endptr1
			, endptr2
			, (d1 != d2 && endptr1 != endptr2 ? " <= values and endptrs are different"
					: d1 != d2 ? " <= only values are different"
					: endptr1 != endptr2 ? " <= only endptrs are different"
					: ""));

	return d1 == d2 && endptr1 == endptr2;
}

#endif

int main(int argc, char *argv[])
{
    PFS_CHECK_SIZEOF_TYPES;
    PFS_UNUSED2(argc, argv);
    int ntests = 42;
	BEGIN_TESTS(ntests);

	/* hack to get locale dependent decimal point char (spied at stackoverflow.com) */
//	setlocale(LC_NUMERIC, "C");
//	char fchars[10];
//	sprintf(fchars, "%f", 0.0f);
//	char decimalPoint = fchars[1];
//
//	TEST_FAIL2(decimalPoint == '.', "Decimal point character is a period");

	TEST_OK(isinf(PFS_INFINITY));
	TEST_OK(isinf(-PFS_INFINITY));

	const char * endptr;

	TEST_OK(isinf(strtoreal("INFINITY", & endptr)));
	TEST_OK(isinf(strtoreal("-INFINITY", & endptr)));
	TEST_OK(isinf(strtoreal("+INFINITY", & endptr)));
	TEST_OK(isinf(strtoreal("InFiNiTy", & endptr)));
	TEST_OK(isinf(strtoreal("-infinity", & endptr)));
	TEST_OK(isinf(strtoreal("+INFInity", & endptr)));
	TEST_OK(isinf(strtoreal("INF", & endptr)));
	TEST_OK(isinf(strtoreal("-INF", & endptr)));
	TEST_OK(isinf(strtoreal("+INF", & endptr)));
	TEST_OK(isnan(strtoreal("NAN", & endptr)));
	TEST_OK(isnan(strtoreal("-NAN", & endptr)));
	TEST_OK(isnan(strtoreal("+NAN", & endptr)));
	TEST_OK(isnan(strtoreal("nAN", & endptr)));
	TEST_OK(isnan(strtoreal("-nAn", & endptr)));
	TEST_OK(isnan(strtoreal("+nan", & endptr)));

	TEST_OK(isinf(strtoreal("INFINITY$%^", & endptr)) && strcmp(endptr, "$%^") == 0);
	cout << endptr << endl;

	TEST_OK(isinf(strtoreal("-INFINITY$%^", & endptr)) && strcmp(endptr, "$%^") == 0);
	TEST_OK(isinf(strtoreal("+INFINITY$%^", & endptr)) && strcmp(endptr, "$%^") == 0);
	TEST_OK(isinf(strtoreal("InFiNiTy$%^", & endptr)) && strcmp(endptr, "$%^") == 0);
	TEST_OK(isinf(strtoreal("-infinity $%^", & endptr)) && strcmp(endptr, " $%^") == 0);
	TEST_OK(isinf(strtoreal("+INFInity $%^", & endptr)) && strcmp(endptr, " $%^") == 0);
	TEST_OK(isinf(strtoreal("INF $%^", & endptr)) && strcmp(endptr, " $%^") == 0);
	TEST_OK(isinf(strtoreal("-INF $%^", & endptr)) && strcmp(endptr, " $%^") == 0);
	TEST_OK(isinf(strtoreal("+INF$%^", & endptr)) && strcmp(endptr, "$%^") == 0);
	TEST_OK(isnan(strtoreal("NAN$%^", & endptr)) && strcmp(endptr, "$%^") == 0);
	TEST_OK(isnan(strtoreal("-NAN $%^", & endptr)) && strcmp(endptr, " $%^") == 0);
	TEST_OK(isnan(strtoreal("+NAN   $%^", & endptr)) && strcmp(endptr, "   $%^") == 0);
	TEST_OK(isnan(strtoreal("nAN $%^", & endptr)) && strcmp(endptr, " $%^") == 0);
	TEST_OK(isnan(strtoreal("-nAnNAN", & endptr)) && strcmp(endptr, "NAN") == 0);
	TEST_OK(isnan(strtoreal("+nannan", & endptr)) && strcmp(endptr, "nan") == 0);

#ifdef __COMMENT__

// TODO Tests not passed {
#ifdef PFS_HAVE_LONG_DOUBLE
	TEST_OK2(__compare_with_strtold("3.36210314311209350626e-4932"), _MSG); // PFS_LONG_DOUBLE_MIN
	TEST_OK2(__compare_with_strtold("1.18973149535723176502e+4932"), _MSG); // PFS_LONG_DOUBLE_MAX
#else
	TEST_OK2(__compare_with_strtold("2.22507385850720138309e-308"), _MSG); // PFS_DOUBLE_MIN
	TEST_OK2(__compare_with_strtold("1.79769313486231570815e+308"), _MSG); // PFS_DOUBLE_MAX
#endif
// }

	TEST_OK2(__compare_with_strtold(".1"), _MSG);
	TEST_OK2(__compare_with_strtold(" ."), _MSG);
	TEST_OK2(__compare_with_strtold(" 1.2e3"), _MSG);
	TEST_OK2(__compare_with_strtold(" +1.2e3"), _MSG);
	TEST_OK2(__compare_with_strtold("1.2e3"), _MSG);
	TEST_OK2(__compare_with_strtold("+1.2e3"), _MSG);
	TEST_OK2(__compare_with_strtold("+1.e3"), _MSG);
	TEST_OK2(__compare_with_strtold("-1.2e3"), _MSG);
	TEST_OK2(__compare_with_strtold("-1.2e3.5"), _MSG);
	TEST_OK2(__compare_with_strtold("-1.2e"), _MSG);
	TEST_OK2(__compare_with_strtold("--1.2e3.5"), _MSG);
	TEST_OK2(__compare_with_strtold("--1-.2e3.5"), _MSG);
	TEST_OK2(__compare_with_strtold("-a"), _MSG);
	TEST_OK2(__compare_with_strtold("a"), _MSG);
	TEST_OK2(__compare_with_strtold(".1e"), _MSG);
	TEST_OK2(__compare_with_strtold(".1e3"), _MSG);
	TEST_OK2(__compare_with_strtold(".1e-3"), _MSG);
	TEST_OK2(__compare_with_strtold(".1e-"), _MSG);
	TEST_OK2(__compare_with_strtold(" .e-"), _MSG);
	TEST_OK2(__compare_with_strtold(" .e"), _MSG);
	TEST_OK2(__compare_with_strtold(" e"), _MSG);
	TEST_OK2(__compare_with_strtold(" e0"), _MSG);
	TEST_OK2(__compare_with_strtold(" ee"), _MSG);
	TEST_OK2(__compare_with_strtold(" -e"), _MSG);
	TEST_OK2(__compare_with_strtold(" .9"), _MSG);
	TEST_OK2(__compare_with_strtold(" ..9"), _MSG);
	TEST_OK2(__compare_with_strtold("009"), _MSG);
	TEST_OK2(__compare_with_strtold("0.09e02"), _MSG);
	// http://thread.gmane.org/gmane.editors.vim.devel/19268/
	TEST_OK2(__compare_with_strtold("0.9999999999999999999999999999999999"), _MSG);
	TEST_OK2(__compare_with_strtold("2.2250738585072010e-308"), _MSG); // BUG
	// PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0
	TEST_OK2(__compare_with_strtold("2.2250738585072011e-308"), _MSG);
	// Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again
	TEST_OK2(__compare_with_strtold("2.2250738585072012e-308"), _MSG);
	TEST_OK2(__compare_with_strtold("2.2250738585072013e-308"), _MSG); // Hmm.
	TEST_OK2(__compare_with_strtold("2.2250738585072014e-308"), _MSG); // Hmm.

	TEST_OK2(__compare_with_strtold("123.456"), _MSG);

	// MSVC supports strtold since VC 2013.
//	std::cout << "'" << strtold("123.456", NULL) << "'"
//			<< "=='" << double_t(123.456) << "'"
//			<< std::endl;
//	TEST_FAIL(strtold("123.456", NULL) == double_t(123.456));
#endif // __COMMENT__

    END_TESTS;
}
