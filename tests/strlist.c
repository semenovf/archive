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

const CWT_CHAR *loremipsum_doublequoted = _T(
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \
magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,  \
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
decima et quinta decima.\" Eodem modo typi, qui nunc nobis    \
videntur parum clari, fiant sollemnes in futurum.");


int main(int argc, char *argv[])
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *slNS = cwtStrListNS();
	CwtStrList *sl;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(8);

	sl = slNS->create();

	CWT_TEST_OK(slNS->splitAny(sl, loremipsum, _T(" "), NULL, 0) == 172);
	CWT_TEST_OK(strNS->strEq(_T("Lorem"), slNS->at(sl, 0)));
	CWT_TEST_OK(strNS->strEq(_T("ullamcorper"), slNS->at(sl, 31)));
	CWT_TEST_OK(strNS->strEq(_T("futurum."), slNS->at(sl, 171)));

	slNS->clear(sl);
	CWT_TEST_OK(slNS->size(sl) == 0);
	CWT_TEST_OK(slNS->splitAny(sl, loremipsum_doublequoted, _T(" "), CWT_QUOTES_DOUBLE, 0) == 35);
	CWT_TEST_OK(strNS->strEq(_T("Lorem"), slNS->at(sl, 0)));
	CWT_TEST_OK(strNS->strEq(_T("Eodem"), slNS->at(sl, 22)));

	slNS->free(sl);

	CWT_END_TESTS;
}


