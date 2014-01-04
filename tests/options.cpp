/*
 * options.cpp
 *
 *  Created on: 18.09.2012
 *      Author: wladt
 *  Modified on: 04.06.2013 Replaced with C++ implementation.
 */

#include <cwt/test.hpp>
#include <cwt/app.hpp>

using namespace cwt;

static void test_basic(void)
{
	const char * argv[] = {
		"prog-name"
		, "-h"
		, "--man"
		, "-i"
		, "-server"
		, "--speed=36600"
		, "-db"
		, "7"
		, "arg1"
		, "--sb=2"
		, "--parity=\"even\""
		, "arg2"
		, "-num"
		, "4321.9876"
		, "arg3"
	};

	int argc = sizeof(argv)/sizeof(argv[0]);

	cwt::option optset[] = {
//          longname  shortname has_arg  xpath     default value   desc
//        ------------------------------------------------------------------------------------
		  { "help",   "h",      false,   "/help",        "",           _Tr("output help info") }
		, { "man",    "",       false,   "/man",         "",           _Tr("output help info in man style") }
		, { "",       "i",      false,   "/interactive", "",           _Tr("interactive mode") }
		, { "",       "server", false,   "/server",      "",           _Tr("start this application as server") }
		, { "noserver","",      false,   "/noserver",    "",           _Tr("start this application as standalone") }
		, { "speed",  "b",      true,    "/bitrate",     "9600",       _Tr("speed (bitrate) for serial port") }
		, { "db",     "d",      true,    "/databits",    "8",          _Tr("number of data bits for serial port") }
		, { "sb",     "s",      true,    "/stopbits",    "1",          _Tr("number of stop bits for serial port") }
		, { "parity", "p",      true,    "/parity",      "none",       _Tr("parity for serial port") }
		, { "",       "num",    true,    "/number",      "1234.56789", _Tr("number") }
	};

	cwt::App app;
	cwt::Vector<cwt::String> args;
	OptionsContext optctx;
	cwt::Settings & settings = app.settings();

	CWT_TEST_FAIL(optctx.parse(settings, argc, const_cast<char**>(argv), sizeof(optset)/sizeof(optset[0]), optset, args));

	CWT_TEST_OK(settings.boolean("/help") == true);
	CWT_TEST_OK(settings.boolean("/man") == true);
	CWT_TEST_OK(settings.boolean("/interactive") == true);
	CWT_TEST_OK(settings.boolean("/server") == true);
	CWT_TEST_OK(settings.boolean("/noserver") == false);

	CWT_TEST_OK(settings.integer("/bitrate") == long_t(36600));
	CWT_TEST_OK(settings.integer("/databits") == long_t(7));
	CWT_TEST_OK(settings.integer("/stopbits") == long_t(2));
	CWT_TEST_OK(settings.string("/parity") == "even");
	CWT_TEST_OK(settings.string("/number") ==  "4321.9876");

	bool ok = false;
	CWT_TEST_OK(settings["/number"].toDouble(& ok) && ok);

	CWT_TEST_FAIL(args.size() == 3);
	CWT_TEST_OK(args[0] =="arg1");
	CWT_TEST_OK(args[1] =="arg2");
	CWT_TEST_OK(args[2] =="arg3");
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_BEGIN_TESTS(16);

	test_basic();

	CWT_END_TESTS;
}


