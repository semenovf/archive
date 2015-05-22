/*
 * options.cpp
 *
 *  Created on: 18.09.2012
 *      Author: wladt
 *  Modified on: 04.06.2013 Replaced with C++ implementation.
 */

#include <pfs/test.hpp>
#include <pfs/app.hpp>

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

	pfs::option optset[] = {
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

	pfs::app app;
	pfs::vector<pfs::string> args;
	OptionsContext optctx;
	pfs::settings & settings = app.settings();

	TEST_FAIL(optctx.parse(settings, argc, const_cast<char**>(argv), sizeof(optset)/sizeof(optset[0]), optset, args));

	TEST_OK(settings.boolean("/help") == true);
	TEST_OK(settings.boolean("/man") == true);
	TEST_OK(settings.boolean("/interactive") == true);
	TEST_OK(settings.boolean("/server") == true);
	TEST_OK(settings.boolean("/noserver") == false);

	TEST_OK(settings.integer("/bitrate") == long_t(36600));
	TEST_OK(settings.integer("/databits") == long_t(7));
	TEST_OK(settings.integer("/stopbits") == long_t(2));
	TEST_OK(settings.string("/parity") == "even");
	TEST_OK(settings.string("/number") ==  "4321.9876");

	bool ok = false;
	TEST_OK(settings["/number"].toDouble(& ok) && ok);

	TEST_FAIL(args.size() == 3);
	TEST_OK(args[0] =="arg1");
	TEST_OK(args[1] =="arg2");
	TEST_OK(args[2] =="arg3");
}

int main(int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(16);

	test_basic();

	END_TESTS;
}


