/*
 * options.cpp
 *
 *  Created on: 18.09.2012
 *      Author: wladt
 *  Modified on: 04.06.2013 Replaced with C++ implementation.
 */

#include <pfs/test.hpp>
#include <pfs/app.hpp>
#include <pfs/command_line.hpp>

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
		  { _l1("help"),     _l1("h"),      false,   _l1("/help"),        _l1(""),           _u8(_Tr("output help info")) }
		, { _l1("man"),      _l1(""),       false,   _l1("/man"),         _l1(""),           _u8(_Tr("output help info in man style")) }
		, { _l1(""),         _l1("i"),      false,   _l1("/interactive"), _l1(""),           _u8(_Tr("interactive mode")) }
		, { _l1(""),         _l1("server"), false,   _l1("/server"),      _l1(""),           _u8(_Tr("start this application as server")) }
		, { _l1("noserver"), _l1(""),       false,   _l1("/noserver"),    _l1(""),           _u8(_Tr("start this application as standalone")) }
		, { _l1("speed"),    _l1("b"),      true,    _l1("/bitrate"),     _l1("9600"),       _u8(_Tr("speed (bitrate) for serial port")) }
		, { _l1("db"),       _l1("d"),      true,    _l1("/databits"),    _l1("8"),          _u8(_Tr("number of data bits for serial port")) }
		, { _l1("sb"),       _l1("s"),      true,    _l1("/stopbits"),    _l1("1"),          _u8(_Tr("number of stop bits for serial port")) }
		, { _l1("parity"),   _l1("p"),      true,    _l1("/parity"),      _l1("none"),       _u8(_Tr("parity for serial port")) }
		, { _l1(""),         _l1("num"),    true,    _l1("/number"),      _l1("1234.56789"), _u8(_Tr("number")) }
	};

	pfs::app app;
	pfs::stringlist args;
	pfs::command_line cmdLine;
	pfs::settings & settings = app.settings();
    pfs::stringlist dirs;

    dirs << "." << "..";
    TEST_FAIL(settings.load("pfs-settings-json", dirs));

	TEST_FAIL(cmdLine.parse(settings, argc, const_cast<char**>(argv), sizeof(optset)/sizeof(optset[0]), optset, args));

	TEST_OK(settings.getBoolean("/help") == true);
	TEST_OK(settings.getBoolean("/man") == true);
	TEST_OK(settings.getBoolean("/interactive") == true);
	TEST_OK(settings.getBoolean("/server") == true);
	TEST_OK(settings.getBoolean("/noserver") == false);

	TEST_OK(settings.getInteger("/bitrate") == integral_t(36600));
	TEST_OK(settings.getInteger("/databits") == integral_t(7));
	TEST_OK(settings.getInteger("/stopbits") == integral_t(2));
	TEST_OK(settings.getString("/parity") == "even");
	TEST_OK(settings.getString("/number") ==  "4321.9876");

	bool ok = false;
	TEST_OK(settings.getString("/number").toDouble(& ok) && ok);

	TEST_FAIL(args.size() == 3);
	TEST_OK(args[0] == _l1("arg1"));
	TEST_OK(args[1] == _l1("arg2"));
	TEST_OK(args[2] == _l1("arg3"));
}

int main(int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(17);

	test_basic();

	END_TESTS;
}


