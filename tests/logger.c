/**
 * @file   logger.c
 * @author wladt
 * @date   Nov 30, 2012 11:48:54 AM
 *
 * @brief
 */

#include <cwt/logger.h>
#include <cwt/test.h>

const CWT_CHAR *loremipsum_0 = _T(
"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,    \
sed diam nonummy nibh euismod tincidunt ut laoreet dolore     \
magna aliquam erat volutpat. Ut wisi enim ad minim veniam");

const CWT_CHAR *loremipsum_1 = _T(
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
	int count = 100;
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(1);

	while(--count > 0) {
		cwt_logger_ns()->trace (loremipsum_0);
		cwt_logger_ns()->debug (loremipsum_0);
		cwt_logger_ns()->info  (loremipsum_0);
		cwt_logger_ns()->warn  (loremipsum_0);
		cwt_logger_ns()->error (loremipsum_0);

		cwt_logger_ns()->trace (loremipsum_1);
		cwt_logger_ns()->debug (loremipsum_1);
		cwt_logger_ns()->info  (loremipsum_1);
		cwt_logger_ns()->warn  (loremipsum_1);
		cwt_logger_ns()->error (loremipsum_1);

		cwt_logger_ns()->trace (loremipsum_0);
		cwt_logger_ns()->debug (loremipsum_0);
		cwt_logger_ns()->info  (loremipsum_0);
		cwt_logger_ns()->warn  (loremipsum_0);
		cwt_logger_ns()->error (loremipsum_0);

		cwt_logger_ns()->trace (_T("%s %s\n"), loremipsum_0, loremipsum_1);
		cwt_logger_ns()->debug (_T("%s %s\n"), loremipsum_0, loremipsum_1);
		cwt_logger_ns()->info  (_T("%s %s\n"), loremipsum_0, loremipsum_1);
		cwt_logger_ns()->warn  (_T("%s %s\n"), loremipsum_0, loremipsum_1);
		cwt_logger_ns()->error (_T("%s %s\n"), loremipsum_0, loremipsum_1);
	}

	CWT_TEST_OK2(argc == 1, _T("Successfully processed"));

	CWT_END_TESTS;
}

