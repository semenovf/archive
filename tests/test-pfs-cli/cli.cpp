/*
 * options.cpp
 *
 *  Created on: 18.09.2012
 *      Author: wladt
 *  Modified on: 04.06.2013 Replaced with C++ implementation.
 */

#include <pfs/test/test.hpp>
#include <pfs/cli/cli.hpp>
#include <pfs/cli/result.hpp>

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

/**
 * 1. @c router contains zero or more @c actions:
 * @code
 *		command
 *		command action
 * 		command action1 action2 ...
 * @endcode
 *
 * 2. @c router contains zero or more @c options
 * @code
 * 		command
 * 		command --optname=optvalue
 * 		command --optname=optvalue -b -o value
 * @endcode
 *
 * 3. @c action identified by name and zero or more synonyms
 * @code
 * 		command { a | act | action } ...
 * @endcode
 *
 * 4. @c actions has an order
 * @code
 * 		command precendent_action action following_action ...
 * @endcode
 *
 * 5. @c option identified by name and zero or more synonyms
 * @code
 * 		command { --name=string | -n string | --my-name=string } ...
 * 		command { --help | -h | -? | --usage } ...
 * @endcode
 *
 * 6. @c options is unordered
 * Below code lines are equivalent
 * @code
 * 		command --bitrate=9600 --parity=even --server -i
 * 		command --server --bitrate=9600 -i --parity=even
 * 		command -i --parity=even --server --bitrate=9600
 * @endcode
 *
 */


#ifdef __COMMENT__
static void test_basic(void)
{
//	pfs::option optset[] = {
////          longname          shortname     has_arg  xpath                default value      desc
////        ------------------------------------------------------------------------------------------------------------------------------
//		  { _l1("help")     , _l1("h"),      false,   _l1("/help"),        _l1(""),           _u8(_Tr("output help info")) }
//		, { _l1("man")      , _l1(""),       false,   _l1("/man"),         _l1(""),           _u8(_Tr("output help info in man style")) }
//		, { _l1("")         , _l1("i"),      false,   _l1("/interactive"), _l1(""),           _u8(_Tr("interactive mode")) }
//		, { _l1("server")   , _l1(""),       false,   _l1("/server"),      _l1(""),           _u8(_Tr("start this application as server")) }
//		, { _l1("noserver") , _l1(""),       false,   _l1("/noserver"),    _l1(""),           _u8(_Tr("start this application as standalone")) }
//		, { _l1("speed")    , _l1("b"),      true,    _l1("/bitrate"),     _l1("9600"),       _u8(_Tr("speed (bitrate) for serial port")) }
//		, { _l1("db")       , _l1("d"),      true,    _l1("/databits"),    _l1("8"),          _u8(_Tr("number of data bits for serial port")) }
//		, { _l1("sb")       , _l1("s"),      true,    _l1("/stopbits"),    _l1("1"),          _u8(_Tr("number of stop bits for serial port")) }
//		, { _l1("parity")   , _l1("p"),      true,    _l1("/parity"),      _l1("none"),       _u8(_Tr("parity for serial port")) }
//		, { _l1("")         , _l1("num"),    true,    _l1("/number"),      _l1("1234.56789"), _u8(_Tr("number")) }
//	};

	/*
	 * void usage () { ... }
	 * void man ()   { ... }
	 * void finish ()   { exit(0); }
	 * void validateBitrate (integral_t)
	 */
	 int longopt_format = pfs::option_format::Any;
	 int shortopt_format = pfs::option_format::UnixShortOption;

	 // Apply all option formats
	 pfs::command_line cmdLine;
	 pfs::settings settings;

	 cmdLine.setOptionFormat(shortopt_format, longopt_format);
	 cmdLine.booleanOption("help", "h")
//	        .handle(usage)
//	        .handle(finish)
			;
	 cmdLine.booleanOption("man")
//			 .handle(man).handle(finish);
		;
	 cmdLine.booleanOption("i")
//	        .handle(settings, "/interactive");
		;
	 cmdLine.booleanOption("server")
//	        .handle(settings, "/server");
		;
	 cmdLine.integerOption(pfs::stringlist() << "speed" << "bitrate" << "b")
//	        .defaultValue(9600)
//	        .validate(validateBitrate)
//	        .description(_u8("speed (bitrate) for serial port"))
//	        .handle(settings, "/bitrate");

		;

//	pfs::stringlist args;
//	pfs::settings settings;
//
//	TEST_FAIL(settings.load("pfs-settings-json", pfs::stringlist() << "." << ".."));
//	TEST_FAIL(cmdLine.parse(settings, argc, const_cast<char**>(argv), sizeof(optset)/sizeof(optset[0]), optset, & args));
//
//	TEST_OK(settings.getBoolean("/help") == true);
//	TEST_OK(settings.getBoolean("/man") == true);
//	TEST_OK(settings.getBoolean("/interactive") == true);
//	TEST_OK(settings.getBoolean("/server") == true);
//	TEST_OK(settings.getBoolean("/noserver") == false);
//
//	TEST_OK(settings.getInteger("/bitrate") == integral_t(36600));
//	TEST_OK(settings.getInteger("/databits") == integral_t(7));
//	TEST_OK(settings.getInteger("/stopbits") == integral_t(2));
//	TEST_OK(settings.getString("/parity") == "even");
//	TEST_OK(settings.getString("/number") ==  "4321.9876");
//
//	bool ok = false;
//	TEST_OK(settings.getString("/number").toDouble(& ok) && ok);
//
//	TEST_FAIL(args.size() == 3);
//	TEST_OK(args[0] == _l1("arg1"));
//	TEST_OK(args[1] == _l1("arg2"));
//	TEST_OK(args[2] == _l1("arg3"));
}
#endif


void bool test_single_action (const pfs::cli::result & r)
{
	return r.actionAt(0) == "action";
}

void bool test_action_synonyms (const pfs::cli::result & r)
{
	return r.actionAt(0) == "a"
			|| r.actionAt(0) == "act"
			|| r.actionAt(0) == "action";
}

void test_cli_router (
	, const pfs::cli::composer & composer
	, int argc
	, char **argv)
{
	pfs::cli::cli cli;
	TEST_OK(cli.parse(argc, argv));
}

#define TEST_CLI_ROUTER(composer, argv_array) \
{
	char * argv[] = argv_array;
	int argc = sizeof(argv)/sizeof(argv[0]);
	test_cli_router(composer, argc, argv);
}

int main (int /*argc*/, char ** /*argv*/)
{
	BEGIN_TESTS(2);

	TEST_CLI_ROUTER(r().a("action").h(handler)
		, {"action"}
		, test_single_action)
//	{
//		pfs::cli::cli cli;
//		cli.r().a("action").h(test_single_action);
//		char * argv[] = { "action" };
//		int argc = sizeof(argv)/sizeof(argv[0]);
//		TEST_OK(cli.parse(argc, argv));
//	}

//	{
//		pfs::cli::cli cli;
//		cli.r().a("action").h(test_action_synonyms);
//		char * argv[] = { "action" };
//		int argc = sizeof(argv)/sizeof(argv[0]);
//		TEST_OK(cli.parse(argc, argv));
//	}

//    cli.r();
////        .b("verbose");
////        :h(function (r)
////                Settings:set("Verbose", true);
////                return true;
////           end)
////        :continue();
//
//    cli.r();
////        .b("dump");
////        :h(function (r)
////                print("Options: " .. cli:dumpOpts());
////                print("Free arguments: " .. cli:dumpArgs());
////           end);
////
//    cli.r()
//        .a("help")
//        .a("workspace").syn("ws")
//        	.syn("solution").syn("sln")
//        	.syn("project").syn("pro").syn("prj");
////        --:a({})
////        .h(function (r)
////                Settings:set("Domain", r:actionAt(1));
////                help_type:new(Settings):help();
////                return true;
////           end);
//
//    cli.r()
//        .a("help");
////        :h(function (r)
////                cli:guide():usage();
////                return true;
////           end);
////
//    cli.r()
//        .a("workspace").syn("ws");
////        :b("create")
////        :s("path")
////        :s("build-tool")
////        :s("target-platform", "")
////        :s("config", "debug")
////        :h(function (r)
////                Settings:set("WorkspacePath" , r:optArg("path"));
////                Settings:set("BuildTool"     , r:optArg("build-tool"));
////                Settings:set("TargetPlatform", r:optArg("target-platform"));
////                Settings:set("BuildConfig"   , r:optArg("config"));
////                return require("gbs.workspace"):new(Settings):create();
////           end);
////
//    cli.r()
//        .a("solution").syn("sln");
////        :b("create")
////        :s("name")
////        :b("git", false)
////        :h(function (r)
////                Settings:set("SolutionName", r:optArg("name"));
////                Settings:set("EnableGitRepo", r:optArg("git"));
////                return require("gbs.solution"):new(Settings):create();
////           end);
////
//    cli.r()
//        .a("project").syn("pro").syn("prj");
////        :b("create")
////        :s("name")
////        :s("type", "console-app")
////        :s("lang", "C++")
////        :s("depends", {})
////        :h(function (r)
////                Settings:set("SolutionName"       , solutionName());
////                Settings:set("ProjectName"        , r:optArg("name"));
////                Settings:set("ProjectType"        , r:optArg("type"));
////                Settings:set("ProjectLanguage"    , r:optArg("lang"));
////                Settings:set("ProjectDependencies", r:optArg("depends"));
////                return require("gbs.project"):new(Settings):create();
////           end);
////
//    cli.r()
//        .a("project").syn("pro").syn("prj");
////        :b("build")
////        :s("name", "")
////        :s("config", Settings:get("BuildConfig") or "")
////        :s("build-tool", Settings:get("BuildTool") or "")
////        :s("target-platform", Settings:get("TargetPlatform") or "")
////        :h(function (r)
////                Settings:set("ProjectName"   , _esn(r:optArg("name")));
////                Settings:set("BuildConfig"   , _esn(r:optArg("config")));
////                Settings:set("BuildTool"     , _esn(r:optArg("build-tool")));
////                Settings:set("TargetPlatform", _esn(r:optArg("target-platform")));
////                return require("gbs.project"):new(Settings):build();
////           end);
////
////    -- Synonym for `gbs project --build'
//    cli.r()
//        .a("all");
////        :h(function (r)
////                Settings:set("ProjectName"   , nil);
////                Settings:set("BuildConfig"   , Settings:get_or_throw("BuildConfig"));
////                Settings:set("BuildTool"     , Settings:get_or_throw("BuildTool"));
////                Settings:set("TargetPlatform", Settings:get_or_throw("TargetPlatform"));
////                return require("gbs.project"):new(Settings):build();
////           end);
////
//    cli.r()
//        .a("project").syn("pro").syn("prj");
////        :b("clean")
////        :s("name", "")
////        :s("config", Settings:get("BuildConfig") or "")
////        :s("build-tool", Settings:get("BuildTool") or "")
////        :s("target-platform", Settings:get("TargetPlatform") or "")
////        :h(function (r)
////                Settings:set("ProjectName"   , _esn(r:optArg("name")));
////                Settings:set("BuildConfig"   , _esn(r:optArg("config")));
////                Settings:set("BuildTool"     , _esn(r:optArg("build-tool")));
////                Settings:set("TargetPlatform", _esn(r:optArg("target-platform")));
////                return require("gbs.project"):new(Settings):clean();
////           end);
////
////    -- Synonym for `gbs project --clean'
//    cli.r()
//        .a("clean");
////        :h(function (r)
////                Settings:set("ProjectName"   , "");
////                Settings:set("BuildConfig"   , Settings:get_or_throw("BuildConfig"));
////                Settings:set("BuildTool"     , Settings:get_or_throw("BuildTool"));
////                Settings:set("TargetPlatform", Settings:get_or_throw("TargetPlatform"));
////                return require("gbs.project"):new(Settings):clean();
////           end);
////
//    cli.r();
////        :h(function (r)
////                print("Type `gbs help' for usage");
////           end);
////
    cli.parse(argc, argv);

	END_TESTS;
}


