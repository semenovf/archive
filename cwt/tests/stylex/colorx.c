#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/stylex/stylex.h>
#include <cwt/stylex/colorx.h>

int main(int argc, char *argv[])
{
	JQ_UNUSED(argc);
	JQ_UNUSED(argv);

	CWT_BEGIN_TESTS(54);

	init_loggers();
	cwt_stylex_init();

	JQ_TEST_OK(CWT_InvalidColor == cwt_stylex_colorByName("AbraCadabra"));
	JQ_TEST_OK(CWT_Transparent  == cwt_stylex_colorByName("TrAnSpArEnT"));
	JQ_TEST_OK(CWT_Beige    == cwt_stylex_colorByName("Beige"));
	JQ_TEST_OK(CWT_Black    == cwt_stylex_colorByName("Black"));
	JQ_TEST_OK(CWT_Blue     == cwt_stylex_colorByName("Blue"));
	JQ_TEST_OK(CWT_Brown    == cwt_stylex_colorByName("Brown"));
	JQ_TEST_OK(CWT_Cyan     == cwt_stylex_colorByName("Cyan"));
	JQ_TEST_OK(CWT_DarkGray == cwt_stylex_colorByName("DarkGray"));
	JQ_TEST_OK(CWT_DarkGrey == cwt_stylex_colorByName("DarkGrey"));
	JQ_TEST_OK(CWT_Green    == cwt_stylex_colorByName("Green"));
	JQ_TEST_OK(CWT_Indigo   == cwt_stylex_colorByName("Indigo"));
	JQ_TEST_OK(CWT_Magenta  == cwt_stylex_colorByName("Magenta"));
	JQ_TEST_OK(CWT_Navy     == cwt_stylex_colorByName("Navy"));
	JQ_TEST_OK(CWT_Orange   == cwt_stylex_colorByName("Orange"));
	JQ_TEST_OK(CWT_Pink     == cwt_stylex_colorByName("Pink"));
	JQ_TEST_OK(CWT_Red      == cwt_stylex_colorByName("Red"));
	JQ_TEST_OK(CWT_Silver   == cwt_stylex_colorByName("Silver"));
	JQ_TEST_OK(CWT_White    == cwt_stylex_colorByName("White"));
	JQ_TEST_OK(CWT_Yellow   == cwt_stylex_colorByName("Yellow"));

	cwt_stylex_test_parse_color();
	cwt_stylex_test_parse_attrs();

	cwt_stylex_release();
	set_default_printers();

	JQ_END_TESTS;
}


