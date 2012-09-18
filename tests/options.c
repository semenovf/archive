/*
 * options.c
 *
 *  Created on: 18.09.2012
 *      Author: wladt
 */


#include <stdio.h>
#include <cwt/types.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/test.h>
#include <cwt/options.h>

static CwtOptionsNS *__optNS = NULL;

static const CWT_CHAR *argvTypeStr(CwtArgvType argvType)
{
    return CwtArgv_ShortOpt == argvType
		? _Tr("Short option")
		: CwtArgv_LongOpt == argvType
		? _Tr("Long option")
		: CwtArgv_LongOptWithArg == argvType
		? _Tr("Long option with argument")
		: CwtArgv_SingleDash == argvType
		? _Tr("Single dash")
		: CwtArgv_DoubleDash == argvType
		? _Tr("Double dash")
		: CwtArgv_Arg == argvType
		? _Tr("Argument")
		: _Tr("UNKNOWN option");
}


static void test_iterate_options(void)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStringNS *stringNS = cwtStringNS();
	CwtOptionIterator it;
	CwtString *desc;
	int i;
	char *argv[] = {
		  "-c"
		, "-S"
		, "-E"
		, "-std=standard"
		, "-g"
		, "-pg"
		, "--l"
		, "-"
		, "-o"
		, "outfile"
		, "arg1"
		, "arg2"
		, "-opt1=arg3"
		, "--"
	};

	struct {
		CwtArgvType argvType;
		CWT_CHAR *opt;
		CWT_CHAR *arg;
	} argv_check[] = {
		  { CwtArgv_ShortOpt, _T("c"), NULL }
		, { CwtArgv_ShortOpt, _T("S"), NULL }
		, { CwtArgv_ShortOpt, _T("E"), NULL }
		, { CwtArgv_LongOptWithArg, _T("std"), _T("standard") }
		, { CwtArgv_ShortOpt, _T("g"), NULL }
		, { CwtArgv_LongOpt,  _T("pg"), NULL }
		, { CwtArgv_LongOpt,  _T("l"), NULL }
		, { CwtArgv_SingleDash,  NULL, NULL }
		, { CwtArgv_ShortOpt, _T("o"), NULL }
		, { CwtArgv_Arg, NULL, _T("outfile") }
		, { CwtArgv_Arg, NULL, _T("arg1") }
		, { CwtArgv_Arg, NULL, _T("arg2") }
		, { CwtArgv_LongOptWithArg, _T("opt1"), _T("arg3") }
		, { CwtArgv_DoubleDash,  NULL, NULL }
	};

	int argc;

	desc = stringNS->create();
	argc = sizeof(argv)/sizeof(argv[0]);
	i = 0;
	__optNS->begin(&it, argc, argv);

	while(__optNS->hasMore(&it)) {
		CWT_CHAR *opt, *arg;
		CwtArgvType argvType;

		argvType = __optNS->next(&it, &opt, &arg);

		CWT_TEST_OK2(argvType == argv_check[i].argvType, argvTypeStr(argvType));

		if (argv_check[i].opt) {
			stringNS->sprintf(desc, _T("Option: '%s'"), opt);
			CWT_TEST_FAIL2(strNS->strEq(argv_check[i].opt, opt), stringNS->cstr(desc));
		}

		if (argv_check[i].arg) {
			stringNS->sprintf(desc, _T("Argument: '%s'"), arg);
			CWT_TEST_FAIL2(strNS->strEq(argv_check[i].arg, arg), stringNS->cstr(desc));
		}

		CWT_FREE(opt);
		CWT_FREE(arg);
		i++;
	}
	stringNS->free(desc);
}

int main(int argc, char *argv[])
{
	__optNS = cwtOptionsNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(28);

	test_iterate_options();

	CWT_END_TESTS;
}


