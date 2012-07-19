/*
 * strlist.c
 *
 *  Created on: 19.07.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/str.h>
#include <cwt/strlist.h>
#include <cwt/logger.h>

const CWT_CHAR *loremipsum = _T(
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \
magna aliquam erat volutpat. Ut wisi enim ad minim veniam,    \
quis nostrud exerci tation ullamcorper suscipit lobortis      \
nisl ut aliquip ex ea commodo consequat. Duis autem vel eum   \
iriure dolor in hendrerit in vulputate velit esse molestie    \
consequat, vel illum dolore eu feugiat nulla facilisis at     \
vero eros et accumsan et iusto odio dignissim qui blandit     \
praesent luptatum zzril delenit augue duis dolore te feugait  \
nulla facilisi. Nam liber tempor cum soluta nobis eleifend    \
option congue nihil imperdiet doming id quod mazim placerat   \
facer possim assum. Typi non habent claritatem insitam; est   \
usus legentis in iis qui facit eorum claritatem.              \
Investigationes demonstraverunt lectores legere me lius quod  \
ii legunt saepius. Claritas est etiam processus dynamicus,    \
qui sequitur mutationem consuetudium lectorum. Mirum est      \
notare quam littera gothica, quam nunc putamus parum claram,  \
anteposuerit litterarum formas humanitatis per seacula quarta \
decima et quinta decima. Eodem modo typi, qui nunc nobis      \
videntur parum clari, fiant sollemnes in futurum.");


int main(int argc, char *argv[])
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *slNS = cwtStrListNS();
	CwtStrList strList;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	slNS->init(&strList);

	CWT_TEST_OK(slNS->splitAny(&strList, loremipsum, _T(" "), NULL) == 172);
	printf_debug(_T("%d"), slNS->size(&strList));
	printf_debug(_T("at[0]: %s"), slNS->at(&strList, 0));
	printf_debug(_T("at[1]: %s"), slNS->at(&strList, 1));
	printf_debug(_T("at[2]: %s"), slNS->at(&strList, 2));
	printf_debug(_T("at[8]: %s"), slNS->at(&strList, 8));

	slNS->destroy(&strList);
	CWT_TEST_OK(slNS->size(&strList) == 0);

/*
	CWT_TEST_OK(CWT_LONGLONG_MIN   == strNS->toLONGLONG(_T("-9223372036854775808"), 0, NULL));
	CWT_TEST_NOK(CWT_LONGLONG_MIN  == strNS->toLONGLONG(_T("-9223372036854775809"), 0, NULL));
	CWT_TEST_OK(CWT_LONGLONG_MAX   == strNS->toLONGLONG(_T("9223372036854775807"), 0, NULL));
	CWT_TEST_NOK(CWT_LONGLONG_MAX  == strNS->toLONGLONG(_T("9223372036854775808"), 0, NULL));
	CWT_TEST_OK(CWT_ULONGLONG_MAX  == strNS->toULONGLONG(_T("18446744073709551615"), 0, NULL));
	CWT_TEST_NOK(CWT_ULONGLONG_MAX == strNS->toULONGLONG(_T("18446744073709551616"), 0, NULL));
	CWT_TEST_OK(CWT_LONG_MIN       == strNS->toLONG(_T("-2147483648"), 0, NULL));
	CWT_TEST_NOK(CWT_LONG_MIN      == strNS->toLONG(_T("-2147483649"), 0, NULL));
	CWT_TEST_OK(CWT_LONG_MAX       == strNS->toLONG(_T("2147483647"), 0, NULL));
	CWT_TEST_NOK(CWT_LONG_MAX      == strNS->toLONG(_T("2147483648"), 0, NULL));
	CWT_TEST_OK(CWT_ULONG_MAX      == strNS->toULONG(_T("4294967295"), 0, NULL));
	CWT_TEST_NOK(CWT_ULONG_MAX     == strNS->toULONG(_T("4294967296"), 0, NULL));
*/


	CWT_END_TESTS;
}


