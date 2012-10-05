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
    return Cwt_Argv_ShortOpt == argvType
		? _Tr("Short option")
		: Cwt_Argv_LongOpt == argvType
		? _Tr("Long option")
		: Cwt_Argv_LongOptWithArg == argvType
		? _Tr("Long option with argument")
		: Cwt_Argv_SingleDash == argvType
		? _Tr("Single dash")
		: Cwt_Argv_DoubleDash == argvType
		? _Tr("Double dash")
		: Cwt_Argv_Arg == argvType
		? _Tr("Argument")
		: _Tr("UNKNOWN option");
}


static void test_iterate_options(void)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStringNS *stringNS = cwtStringNS();
	CwtOptionIterator *it;
	CwtString *desc;
	int i;
	char *argv[] = {
		  "prog-name"
		, "-c"
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
		, "--opt2=arg4"
		, "--longopt"
	};

	struct {
		CwtArgvType argvType;
		CWT_CHAR *opt;
		CWT_CHAR *arg;
	} argv_check[] = {
		  { Cwt_Argv_ShortOpt, _T("c"), NULL }
		, { Cwt_Argv_ShortOpt, _T("S"), NULL }
		, { Cwt_Argv_ShortOpt, _T("E"), NULL }
		, { Cwt_Argv_LongOptWithArg, _T("std"), _T("standard") }
		, { Cwt_Argv_ShortOpt, _T("g"), NULL }
		, { Cwt_Argv_LongOpt,  _T("pg"), NULL }
		, { Cwt_Argv_LongOpt,  _T("l"), NULL }
		, { Cwt_Argv_SingleDash,  NULL, NULL }
		, { Cwt_Argv_ShortOpt, _T("o"), NULL }
		, { Cwt_Argv_Arg, NULL, _T("outfile") }
		, { Cwt_Argv_Arg, NULL, _T("arg1") }
		, { Cwt_Argv_Arg, NULL, _T("arg2") }
		, { Cwt_Argv_LongOptWithArg, _T("opt1"), _T("arg3") }
		, { Cwt_Argv_DoubleDash,  NULL, NULL }
		, { Cwt_Argv_LongOptWithArg, _T("opt2"), _T("arg4") }
		, { Cwt_Argv_LongOpt,  _T("longopt"), NULL }
	};

	int argc;
	int nopts; /* number of options */
	int nargs; /* number of arguments (optargs and free args)*/

	desc = stringNS->create();
	argc = sizeof(argv)/sizeof(argv[0]);
	i = 0;
	nopts = 0;
	nargs = 0;
	it = __optNS->createIterator(Cwt_OptionIterator_Default);
	it->begin(it, argc, argv);
	it->next(it, NULL, NULL); /* skip program name */

	while(it->hasMore(it)) {
		CWT_CHAR *opt, *arg;
		CwtArgvType argvType;

		argvType = it->next(it, &opt, &arg);

		CWT_TEST_OK2(argvType == argv_check[i].argvType, argvTypeStr(argvType));

		if (argv_check[i].opt) {
			stringNS->sprintf(desc, _T("Option: '%s'"), opt);
			CWT_TEST_FAIL2(strNS->strEq(argv_check[i].opt, opt), stringNS->cstr(desc));
			nopts++;
		}

		if (argv_check[i].arg) {
			stringNS->sprintf(desc, _T("Argument: '%s'"), arg);
			CWT_TEST_FAIL2(strNS->strEq(argv_check[i].arg, arg), stringNS->cstr(desc));
			nargs++;
		}

		CWT_FREE(opt);
		CWT_FREE(arg);
		i++;
	}

	CWT_TEST_OK(nopts==11);
	CWT_TEST_OK(nargs==6);
	it->free(it);
	stringNS->free(desc);
}


static void test_options_general(void)
{
	CwtStrNS *strNS = cwtStrNS();
	CwtStrListNS *slNS = cwtStrListNS();
	CwtOptionIterator *it;

	CwtStrList *args;
	BOOL help           = FALSE;
	BOOL man            = FALSE;
	BOOL isInteractive  = FALSE;
	BOOL isServer       = FALSE;
	LONGLONG speed      = 9600L;
	LONGLONG databits   = 8L;
	LONGLONG stopbits   = 1L;
	CWT_CHAR *parityStr = _T("none");
	double num          = 1234.56789f;
	CwtOption optset[]  = {
/*          longname      shortname  hasArg     arg            validator desc*/
/*        ------------------------------------------------------------------------------------*/
		  { _T("help"),   _T('h'), Cwt_Opt_BOOL, &help,         NULL,     _Tr("output help info") }
		, { _T("man"),     0,      Cwt_Opt_BOOL, &man,          NULL,     _Tr("output help info in man style") }
		, { NULL,         _T('i'), Cwt_Opt_BOOL, &isInteractive,NULL,     _Tr("interactive mode") }
		, { _T("server"),  0,      Cwt_Opt_BOOL, &isServer,     NULL,     _Tr("start this application as server") }
		, { _T("speed"),  _T('b'), Cwt_Opt_INT,  &speed,        NULL,     _Tr("speed (bitrate) for serial port") }
		, { _T("db"),     _T('d'), Cwt_Opt_INT,  &databits,     NULL,     _Tr("number of data bits for serial port") }
		, { _T("sb"),     _T('s'), Cwt_Opt_INT,  &stopbits,     NULL,     _Tr("number of stop bits for serial port") }
		, { _T("parity"), _T('p'), Cwt_Opt_TEXT, &parityStr,    NULL,     _Tr("parity for serial port") }
		, { _T("num"),    _T('n'), Cwt_Opt_REAL, &num,          NULL,     _Tr("number") }
		, CWT_END_OPTIONS
	};
	char *argv[] = {
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

	int argc;

	argc = sizeof(argv)/sizeof(argv[0]);
	args = slNS->create();

	it = __optNS->createIterator(Cwt_OptionIterator_Default);
	it->begin(it, argc, argv);
	it->next(it, NULL, NULL); /* skip program name */

	CWT_TEST_FAIL(__optNS->parseWithIterator(optset, args, it));
	CWT_TEST_OK(help == TRUE);
	CWT_TEST_OK(man == TRUE);
	CWT_TEST_OK(isInteractive == TRUE);
	CWT_TEST_OK(isServer == TRUE);
	CWT_TEST_OK(speed == 36600L);
	CWT_TEST_OK(databits == 7L);
	CWT_TEST_OK(stopbits == 2L);
	CWT_TEST_OK(strNS->strEq(parityStr, _T("\"even\"")));
	CWT_TEST_OK(num >= 4321.9875f && num <= 4321.9877f);

	CWT_TEST_FAIL(slNS->size(args) == 3);
	CWT_TEST_OK(strNS->strEq(slNS->at(args, 0), _T("arg1")));
	CWT_TEST_OK(strNS->strEq(slNS->at(args, 1), _T("arg2")));
	CWT_TEST_OK(strNS->strEq(slNS->at(args, 2), _T("arg3")));

	/* free memory allocated for text option argument */
	if( strNS->strEq(parityStr, _T("\"even\"")) )
		CWT_FREE(parityStr);

	it->free(it);
	slNS->free(args);
}


int main(int argc, char *argv[])
{
	__optNS = cwtOptionsNS();

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(49);

	test_iterate_options();
	test_options_general();
	/*test_options_with_validator();*/

	CWT_END_TESTS;
}


