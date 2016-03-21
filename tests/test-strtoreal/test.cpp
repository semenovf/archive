/*
 * test.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <clocale>
#include <cerrno>
#include <cmath> // for isnan and ininf

using namespace std;
#define strtoreal(s,endptr) pfs::string_to_real(s.cbegin(), s.cend(), '.', endptr)

static char _MSG[512];

bool compare_with_strtold (const char * s)
{
	pfs::string::const_iterator endptr1;
	char * endptr2 = NULL;
	pfs::string str(s);
	real_t d1 = strtoreal(str, & endptr1);

#ifdef PFS_HAVE_STRTOLD
	real_t d2 = strtold(s, & endptr2);
#else
	real_t d2 = strtod(s, & endptr2);
#endif

	sprintf(_MSG,
#ifdef PFS_HAVE_LONG_DOUBLE
		"%s: %.30Lg %s %.30Lg%s"
#else
		"%s: %.30g %s %.30g%s"
#endif
			, s
			, d1
			, (d1 == d2 /*&& endptr1 == endptr2*/ ? "==" : "!=")
			, d2
			, (d1 != d2 && (s + std::distance(str.cbegin(), endptr1) != endptr2) ? " <= values and endptrs are different"
					: d1 != d2 ? " <= only values are different"
					: (s + std::distance(str.cbegin(), endptr1) != endptr2) ? " <= only endptrs are different"
					: ""));

	return d1 == d2 && (s + std::distance(str.cbegin(), endptr1) == endptr2);
}

bool compare_with_literal (const char * s, real_t d2)
{
	pfs::string str(s);
	real_t d1 = strtoreal(str, 0);

	sprintf(_MSG,
#ifdef PFS_HAVE_LONG_DOUBLE
		"%s: %.30Lg %s %.30Lg"
#else
		"%s: %.30g %s %.30g"
#endif
			, s
			, d1
			, (d1 == d2 /*&& endptr1 == endptr2*/ ? "==" : "!=")
			, d2);

	return d1 == d2;
}


#ifdef __COMMENT__
char * doubleToHex (real_t x)
{
    unsigned int i;
    static char buf[100];
    unsigned char const * p = (unsigned char const *) & x;

    for (i = 0; i < sizeof x; ++i)
    	sprintf(buf + 2*i, "%02x", *p++);

    return buf;
}


#endif

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(82);

	/* hack to get locale dependent decimal point char (spied at stackoverflow.com) */
//	setlocale(LC_NUMERIC, "C");
//	char fchars[10];
//	sprintf(fchars, "%f", 0.0f);
//	char decimalPoint = fchars[1];
//
//	TEST_FAIL2(decimalPoint == '.', "Decimal point character is a period");
	pfs::string::const_iterator endptr1;
	const char * endptr2;

//	TEST_OK(isinf(PFS_INFINITY));
//	TEST_OK(isinf(-PFS_INFINITY));
	// FIXME
#if __FIXME__
	TEST_OK(isinf(strtoreal(_u8("INFINITY"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("-INFINITY"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("+INFINITY"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("InFiNiTy"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("-infinity"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("+INFInity"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("INF"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("-INF"), & endptr1)));
	TEST_OK(isinf(strtoreal(_u8("+INF"), & endptr1)));
	TEST_OK(isnan(strtoreal(_u8("NAN"), & endptr1)));
	TEST_OK(isnan(strtoreal(_u8("-NAN"), & endptr1)));
	TEST_OK(isnan(strtoreal(_u8("+NAN"), & endptr1)));
	TEST_OK(isnan(strtoreal(_u8("nAN"), & endptr1)));
	TEST_OK(isnan(strtoreal(_u8("-nAn"), & endptr1)));
	TEST_OK(isnan(strtoreal(_u8("+nan"), & endptr1)));

	TEST_OK(isinf(strtoreal(_u8("INFINITY$%^"), & endptr1)) && strcmp(endptr1.base(), "$%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("-INFINITY$%^"), & endptr1)) && strcmp(endptr1.base(), "$%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("+INFINITY$%^"), & endptr1)) && strcmp(endptr1.base(), "$%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("InFiNiTy$%^"), & endptr1)) && strcmp(endptr1.base(), "$%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("-infinity $%^"), & endptr1)) && strcmp(endptr1.base(), " $%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("+INFInity $%^"), & endptr1)) && strcmp(endptr1.base(), " $%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("INF $%^"), & endptr1)) && strcmp(endptr1.base(), " $%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("-INF $%^"), & endptr1)) && strcmp(endptr1.base(), " $%^") == 0);
	TEST_OK(isinf(strtoreal(_u8("+INF$%^"), & endptr1)) && strcmp(endptr1.base(), "$%^") == 0);
	TEST_OK(isnan(strtoreal(_u8("NAN$%^"), & endptr1)) && strcmp(endptr1.base(), "$%^") == 0);
	TEST_OK(isnan(strtoreal(_u8("-NAN $%^"), & endptr1)) && strcmp(endptr1.base(), " $%^") == 0);
	TEST_OK(isnan(strtoreal(_u8("+NAN   $%^"), & endptr1)) && strcmp(endptr1.base(), "   $%^") == 0);
	TEST_OK(isnan(strtoreal(_u8("nAN $%^"), & endptr1)) && strcmp(endptr1.base(), " $%^") == 0);
	TEST_OK(isnan(strtoreal(_u8("-nAnNAN"), & endptr1)) && strcmp(endptr1.base(), "NAN") == 0);
	TEST_OK(isnan(strtoreal(_u8("+nannan"), & endptr1)) && strcmp(endptr1.base(), "nan") == 0);
#endif

// FIXME gcc ERROR: request for member ‘base’ in ‘endptr1’, which is of non-class type ‘pfs::utf::string<char, pfs::utf8::tag>::const_iterator {aka const char*}
#if __FIXME__
	TEST_OK(strtoreal(_u8("-"), & endptr1) == 0.0f && strcmp(endptr1.base(), "-") == 0);
	TEST_OK(strtoreal(_u8("+"), & endptr1) == 0.0f && strcmp(endptr1.base(), "+") == 0);
	TEST_OK(strtoreal(_u8("    "), & endptr1) == 0.0f && strcmp(endptr1.base(), "    ") == 0);
#endif

	TEST_OK2(compare_with_strtold("0"), _MSG);
	TEST_OK2(compare_with_strtold("00"), _MSG);
	TEST_OK2(compare_with_strtold("000"), _MSG);
	TEST_OK2(compare_with_strtold(".1"), _MSG);
	TEST_OK2(compare_with_strtold("0.1"), _MSG);
	TEST_OK2(compare_with_strtold(" ."), _MSG);
	TEST_OK2(compare_with_strtold(" 1.2e3"), _MSG);
	TEST_OK2(compare_with_strtold(" +1.2e3"), _MSG);
	TEST_OK2(compare_with_strtold("1.2e3"), _MSG);
	TEST_OK2(compare_with_strtold("+1.2e3"), _MSG);
	TEST_OK2(compare_with_strtold("+1.e3"), _MSG);
	TEST_OK2(compare_with_strtold("-1.2e3"), _MSG);
	TEST_OK2(compare_with_strtold("-1.2e3.5"), _MSG);
	TEST_OK2(compare_with_strtold("-1.2e"), _MSG);
	TEST_OK2(compare_with_strtold("--1.2e3.5"), _MSG);
	TEST_OK2(compare_with_strtold("--1-.2e3.5"), _MSG);
	TEST_OK2(compare_with_strtold("-a"), _MSG);
	TEST_OK2(compare_with_strtold("a"), _MSG);
	TEST_OK2(compare_with_strtold(".1e"), _MSG);
	TEST_OK2(compare_with_strtold(".1e3"), _MSG);
	TEST_OK2(compare_with_strtold(".1e-3"), _MSG);
	TEST_OK2(compare_with_strtold(".1e-"), _MSG);
	TEST_OK2(compare_with_strtold(" .e-"), _MSG);
	TEST_OK2(compare_with_strtold(" .e"), _MSG);
	TEST_OK2(compare_with_strtold(" e"), _MSG);
	TEST_OK2(compare_with_strtold(" e0"), _MSG);
	TEST_OK2(compare_with_strtold(" ee"), _MSG);
	TEST_OK2(compare_with_strtold(" -e"), _MSG);
	TEST_OK2(compare_with_strtold(" .9"), _MSG);
	TEST_OK2(compare_with_strtold(" 0.9"), _MSG);
	TEST_OK2(compare_with_strtold(" ..9"), _MSG);
	TEST_OK2(compare_with_strtold("9."), _MSG);
	TEST_OK2(compare_with_strtold("9"), _MSG);
	TEST_OK2(compare_with_strtold("9.0"), _MSG);
	TEST_OK2(compare_with_strtold("9.0000"), _MSG);
	TEST_OK2(compare_with_strtold("9.00001"), _MSG);
	TEST_OK2(compare_with_strtold("009"), _MSG);
	TEST_OK2(compare_with_strtold("0.09e02"), _MSG);
	TEST_OK2(compare_with_strtold("0.9999999999999999999999999999999999"), _MSG);
	TEST_OK2(compare_with_strtold("2.22e-308"), _MSG); // BUG
	TEST_OK2(compare_with_strtold("1.34"), _MSG);
	TEST_OK2(compare_with_strtold("12.34"), _MSG);
	TEST_OK2(compare_with_strtold("123.456"), _MSG);

	TEST_OK2(compare_with_strtold("2.22507385850720138309e-308"), _MSG);
	TEST_OK2(compare_with_strtold("1.79769313486231570815e+308"), _MSG);
	TEST_OK2(compare_with_strtold("3.36210314311209350626e-4932"), _MSG);
	TEST_OK2(compare_with_strtold("1.18973149535723176502e+4932"), _MSG);
	TEST_OK2(compare_with_strtold("1.18973149535723176502126385303e+4932"), _MSG);
	TEST_OK2(compare_with_strtold("18973149535723176502126385303"), _MSG);
	TEST_OK2(compare_with_strtold("12345678901234567890123456789"), _MSG);

	TEST_OK2(compare_with_literal("1", PFS_REAL_LITERAL(1.0)), _MSG);
	TEST_OK2(compare_with_literal("12", PFS_REAL_LITERAL(12.0)), _MSG);
	TEST_OK2(compare_with_literal("123", PFS_REAL_LITERAL(123.0)), _MSG);
	TEST_OK2(compare_with_literal("1234", PFS_REAL_LITERAL(1234.0)), _MSG);
	TEST_OK2(compare_with_literal("12345", PFS_REAL_LITERAL(12345.0)), _MSG);
	TEST_OK2(compare_with_literal("123456", PFS_REAL_LITERAL(123456.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567", PFS_REAL_LITERAL(1234567.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678", PFS_REAL_LITERAL(12345678.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789", PFS_REAL_LITERAL(123456789.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890", PFS_REAL_LITERAL(1234567890.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901", PFS_REAL_LITERAL(12345678901.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012", PFS_REAL_LITERAL(123456789012.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123", PFS_REAL_LITERAL(1234567890123.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234", PFS_REAL_LITERAL(12345678901234.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345", PFS_REAL_LITERAL(123456789012345.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456", PFS_REAL_LITERAL(1234567890123456.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567", PFS_REAL_LITERAL(12345678901234567.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678", PFS_REAL_LITERAL(123456789012345678.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789", PFS_REAL_LITERAL(1234567890123456789.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890", PFS_REAL_LITERAL(12345678901234567890.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678901", PFS_REAL_LITERAL(123456789012345678901.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789012", PFS_REAL_LITERAL(1234567890123456789012.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890123", PFS_REAL_LITERAL(12345678901234567890123.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678901234", PFS_REAL_LITERAL(123456789012345678901234.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789012345", PFS_REAL_LITERAL(1234567890123456789012345.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890123456", PFS_REAL_LITERAL(12345678901234567890123456.0)), _MSG);
	TEST_OK2(compare_with_literal("123456789012345678901234567", PFS_REAL_LITERAL(123456789012345678901234567.0)), _MSG);
	TEST_OK2(compare_with_literal("1234567890123456789012345678", PFS_REAL_LITERAL(1234567890123456789012345678.0)), _MSG);
	TEST_OK2(compare_with_literal("12345678901234567890123456789", PFS_REAL_LITERAL(12345678901234567890123456789.0)), _MSG);

	real_t d0 = real_t(123456789012345678901.0L);
	real_t d1 = strtoreal(_u8("123456789012345678901"), 0);
	real_t d2
	  = 1e0 * 123456789012345678901.0L;
	real_t d3
	  = 1e11 * 1234567890.0L + 1.e0 * 12345678901.0L;
	real_t d4
	  = 1e13 * 12345678.0L + 1e5 * 90123456.0L + 1e0 * 78901.0L;
	real_t d5
	  = 1e20 * 1.0L + 1e18 * 23.0L + 1e16 * 45.0L
	  + 1e14 * 67.0L + 1e12 * 89.0L + 1e10 * 01.0L
	  + 1e8 * 23.0L + 1e6 * 45.0L + 1e4 * 67.0L
	  + 1e2 * 89.0L + 1e0 * 01.0L;
	real_t d6
	  = 1e20 * 1.0 + 1e19 * 2.0 + 1e18 * 3.0
	  + 1e17 * 4.0 + 1e16 * 5.0 + 1e15 * 6.0
	  + 1e14 * 7.0 + 1e13 * 8.0 + 1e12 * 9.0
	  + 1e11 * 0.0 + 1e10 * 1.0 + 1e09 * 2.0
	  + 1e08 * 3.0 + 1e07 * 4.0 + 1e06 * 5.0
	  + 1e05 * 6.0 + 1e04 * 7.0 + 1e03 * 8.0
	  + 1e02 * 9.0 + 1e01 * 0.0 + 1e00 * 1.0;

	printf("  : 123456789012345678901 = 123456789012345678901\n");
	printf("d0: 123456789012345678901 = %.30Lg\n", d0);
	printf("d1: 123456789012345678901 = %.30Lg\n", d1);
	printf("d2: 123456789012345678901 = %.30Lg\n", d2);
	printf("d3: 123456789012345678901 = %.30Lg\n", d3);
	printf("d4: 123456789012345678901 = %.30Lg\n", d4);
	printf("d5: 123456789012345678901 = %.30Lg\n", d5);
	printf("d6: 123456789012345678901 = %.30Lg\n", d6);
//	TEST_OK2(compare_with_literal("123456789012345678901", PFS_REAL_LITERAL(123456789012345678901.0)), _MSG);

#ifdef __COMMENT__
	// PHP (slashdot.jp): http://opensource.slashdot.jp/story/11/01/08/0527259/PHP%E3%81%AE%E6%B5%AE%E5%8B%95%E5%B0%8F%E6%95%B0%E7%82%B9%E5%87%A6%E7%90%86%E3%81%AB%E7%84%A1%E9%99%90%E3%83%AB%E3%83%BC%E3%83%97%E3%81%AE%E3%83%90%E3%82%B0
	TEST_OK2(compare_with_strtold("2.2250738585072011e-308"), _MSG);
	// Gauche: http://blog.practical-scheme.net/gauche/20110203-bitten-by-floating-point-numbers-again
	TEST_OK2(compare_with_strtold("2.2250738585072012e-308"), _MSG);
	TEST_OK2(compare_with_strtold("2.2250738585072013e-308"), _MSG); // Hmm.
	TEST_OK2(compare_with_strtold("2.2250738585072014e-308"), _MSG); // Hmm.

	TEST_OK2(compare_with_strtold("123.456"), _MSG);

// TODO Tests not passed {
#ifdef PFS_HAVE_LONG_DOUBLE
	TEST_OK2(compare_with_strtold("3.36210314311209350626e-4932"), _MSG); // PFS_LONG_DOUBLE_MIN
	TEST_OK2(compare_with_strtold("1.18973149535723176502e+4932"), _MSG); // PFS_LONG_DOUBLE_MAX
#else
	TEST_OK2(compare_with_strtold("2.22507385850720138309e-308"), _MSG); // PFS_DOUBLE_MIN
	TEST_OK2(compare_with_strtold("1.79769313486231570815e+308"), _MSG); // PFS_DOUBLE_MAX
#endif
// }

	// MSVC supports strtold since VC 2013.
//	std::cout << "'" << strtold("123.456", NULL) << "'"
//			<< "=='" << double_t(123.456) << "'"
//			<< std::endl;
//	TEST_FAIL(strtold("123.456", NULL) == double_t(123.456));
#endif // __COMMENT__

    return END_TESTS;
}
