/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>

extern void test_basic ();
extern void test_poll ();

const char * loremipsum [] = {
	  "1.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,"
	, "2.sed diam nonummy nibh euismod tincidunt ut laoreet dolore"
	, "3.magna aliquam erat volutpat. Ut wisi enim ad minim veniam,"
	, "4.quis nostrud exerci tation ullamcorper suscipit lobortis"
	, "5.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum"
	, "6.iriure dolor in hendrerit in vulputate velit esse molestie"
	, "7.consequat, vel illum dolore eu feugiat nulla facilisis at"
	, "8.vero eros et accumsan et iusto odio dignissim qui blandit"
	, "9.praesent luptatum zzril delenit augue duis dolore te feugait"
	, "10.nulla facilisi. Nam liber tempor cum soluta nobis eleifend"
	, "11.option congue nihil imperdiet doming id quod mazim placerat"
	, "12.facer possim assum. Typi non habent claritatem insitam; est"
	, "13.usus legentis in iis qui facit eorum claritatem."
	, "14.Investigationes demonstraverunt lectores legere me lius quod"
	, "15.ii legunt saepius. Claritas est etiam processus dynamicus,"
	, "16.qui sequitur mutationem consuetudium lectorum. Mirum est"
	, "17.notare quam littera gothica, quam nunc putamus parum claram,"
	, "18.anteposuerit litterarum formas humanitatis per seacula quarta"
	, "19.decima et quinta decima. Eodem modo typi, qui nunc nobis"
	, "20.videntur parum clari, fiant sollemnes in futurum."
	, "21.Lorem ipsum dolor sit amet, consectetuer adipiscing elit,"
	, "22.sed diam nonummy nibh euismod tincidunt ut laoreet dolore"
	, "23.magna aliquam erat volutpat. \"Ut wisi enim ad minim veniam,"
	, "24.quis nostrud exerci tation ullamcorper suscipit lobortis"
	, "25.nisl ut aliquip ex ea commodo consequat. Duis autem vel eum"
	, "26.iriure dolor in hendrerit in vulputate velit esse molestie"
	, "27.consequat, vel illum dolore eu feugiat nulla facilisis at"
	, "28.vero eros et accumsan et iusto odio dignissim qui blandit"
	, "29.praesent luptatum zzril delenit augue duis dolore te feugait"
	, "30.nulla facilisi. Nam liber tempor cum soluta nobis eleifend"
	, "31.option congue nihil imperdiet doming id quod mazim placerat"
	, "32.facer possim assum. Typi non habent claritatem insitam; est"
	, "33.usus legentis in iis qui facit eorum claritatem."
	, "34.Investigationes demonstraverunt lectores legere me lius quod"
	, "35.ii legunt saepius. Claritas est etiam processus dynamicus,"
	, "36.qui sequitur mutationem consuetudium lectorum. Mirum est"
	, "37.notare quam littera gothica, quam nunc putamus parum claram,"
	, "38.anteposuerit litterarum formas humanitatis per seacula quarta"
	, "39.decima et quinta decima.\" Eodem modo typi, qui nunc nobis"
	, "40.videntur parum clari, fiant sollemnes in futurum."
};

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

	test_basic();
	test_poll();

	END_TESTS;
}

