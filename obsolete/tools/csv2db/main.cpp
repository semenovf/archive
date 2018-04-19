#include <cwt/app.hpp>
#include <cwt/logger.hpp>
#include <iostream>
#include "proc.hpp"

static void __usage(const char * exeName)
{
	std::cout << _Tr("Usage") << ':' << std::endl
			<< '\t' << exeName << " --table=NAME --db-uri=URI --policy=FILE [--policy-type=POLICY_TYPE] SOURCE" << std::endl
			<< std::endl
			<< "POLICY_TYPE = json (default)" << std::endl;
}


int main(int argc, char * argv[])
{
	cwt::Option optset[] = {
//          longname      shortname  has_arg  settings xpath   default value   desc
//        ------------------------------------------------------------------------------------
          { "table",      "t",       true,    "/table",         cwt::String(),  _Tr("table name") }
		, { "source",     "",        true,    "/source",        cwt::String(),  _Tr("path to source CSV file") }
		, { "policy",     "",        true,    "/policy",        cwt::String(),  _Tr("path to policy file") }
		, { "policy-type","",        true,    "/policy-type",   "json",         _Tr("policy type, default is 'json'") }
		, { "db-uri",     "",        true,    "/db-uri",        cwt::String(),  _Tr("URI to destination database") }
	};

	cwt::App app;
	cwt::OptionsContext optctx;
	cwt::Vector<cwt::String> args;

	if (!optctx.parse(app.settings(), argc, argv, sizeof(optset)/sizeof(optset[0]), optset, args)) {
		__usage(argv[0]);
		return -1;
	}

	if (args.size() == 0) {
		cwt::Logger::error(_Tr("Source must be specified."));
		__usage(argv[0]);
		return -1;
	}

    Csv2DbProc proc(args[0]);
    return app.exec(proc);
}
