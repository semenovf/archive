/*
 * options.c
 *
 *  Created on: Sep 17, 2012
 *      Author: wladt
 */


/*
    !!! При присвоении переменной arg структуры Option указателя на переменную
        необходимо придерживаться следующего правила:
       - если argType == CwtOpt_BOOL, то arg - указатель на BOOL
       - если argType == CwtOpt_CHAR, то arg - указатель на CWT_CHAR
       - если argType == CwtOpt_TEXT, то arg - указатель на CWT_CHAR*
       - если argType == CwtOpt_INT,  то arg - указатель на long
       - если argType == CwtOpt_REAL, то arg - указатель на double

	Пример использования класса Options для разбора командной строки

	BOOL help           = FALSE;
	BOOL man            = FALSE;
	BOOL isInteractive  = FALSE;
	BOOL isServer       = FALSE;
	LONGLONG speed      = 9600L;
	LONGLONG databits   = 8L;
	LONGLONG stopbits   = 1L;
	CWT_CHAR *parityStr = _T("none");
	double num          = 1234.56789f;
	CwtOptionsHandler   *hopt;


	Option optset[] = {
          longname      shortname  hasArg     arg            validator          desc
        ------------------------------------------------------------------------------------
		{ _T("help",    _T('h'), CwtOpt_BOOL, &help,         NULL,              _Tr("output help info") },
		{ _T("man",     0,       CwtOpt_BOOL, &man,          NULL,              _Tr("output help info in man style") },
		{ NULL,         _T('i'), CwtOpt_BOOL, &isInteractive,NULL,              _Tr("interactive mode") },
		{ _T("server"), 0,       CwtOpt_BOOL, &isServer,     NULL,              _Tr("start this application as server") },
		{ _T("speed"),  _T('b'), CwtOpt_INT,  &speed,        __IsSpeedValid,    _Tr("speed (bitrate) for serial port") },
		{ _T("db"),     _T('d'), CwtOpt_INT,  &databits,     __IsDatabitsValid, _Tr("number of data bits for serial port") },
		{ _T("sb"),     _T('s'), CwtOpt_INT,  &stopbits,     __IsStopbitsValid, _Tr("number of stop bits for serial port") },
		{ _T("parity"), _T('p'), CwtOpt_TEXT, &parityStr,    __IsParityValid,   _Tr("parity for serial port") },
		{ _T("num"),    _T('n'), CwtOpt_REAL, &num,          NULL,              _Tr("number") },
		JQ_END_OPTIONS
	};

	hopt = cwtDefaultOptionsHandler();
    cwtOptionsNS()->parse(argc, argv, optset, hopt);
    hopt->free(hopt);

	if( help ) {
		// Help output
		// jq::Options::Help( Argv()[0], optset, std::cout );
	}

	if( man ) {
		// Manual page output
	}
 */

#include <cwt/options.h>
#include <string.h>
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/hashtab.h>

static void        __printUsage(const CWT_CHAR *progname, const CwtOption *optset, FILE *out);
static BOOL        __parse     (int argc, char **argv, CwtOption *options, CwtOptionsHandler *handler);
static void        __begin     (CwtOptionIterator *it, int argc, char **argv);
static BOOL        __hasMore   (CwtOptionIterator *it);
static CwtArgvType __next      (CwtOptionIterator *it, CWT_CHAR **opt, CWT_CHAR **arg);

static CwtOptionsNS __cwtOptionsNS = {
	  __printUsage
	, __parse
	, __begin
	, __hasMore
	, __next
};

/*
static void __debug_option     (CwtOption *option, const CWT_CHAR *arg);
static void __debug_doubleDash (void);
static void __debug_singleDash (void);
static void __debug_argument   (const CWT_CHAR *arg);

static void __default_option     (CwtOption *option, const CWT_CHAR *arg);
static void __default_doubleDash (void);
static void __default_singleDash (void);
static void __default_argument   (const CWT_CHAR *arg);

static void __on_error(const CWT_CHAR *errstr) { cwtLoggerNS()->error(errstr); }
*/

/* helper function */
/*static BOOL __assign_optarg(CwtOption *o, const CWT_CHAR *arg);*/

/*
static CwtOptionsHandler __debugOptionsHandler =
{
	  __debug_option
    , __debug_doubleDash
	, __debug_singleDash
	, __debug_argument
	, __on_error
};

static CwtOptionsHandler __defaultOptionsHandler =
{
	  __default_option
	, __default_doubleDash
	, __default_singleDash
	, __default_argument
	, __on_error
};

*/


DLL_API_EXPORT CwtOptionsNS* cwtOptionsNS(void)
{
	return &__cwtOptionsNS;
}
/*

DLL_API_EXPORT CwtOptionsHandler* cwtDebugOptionsHandler(void)
{
	return &__debugOptionsHandler;
}

DLL_API_EXPORT CwtOptionsHandler* cwtDefaultOptionsHandler(void)
{
	return &__defaultOptionsHandler;
}

DLL_API_EXPORT CwtOptionsHandler* cwtShortOptionsOnlyHandler(void);
DLL_API_EXPORT CwtOptionsHandler* cwtLongOptionsOnlyHandler(void);
*/


/*
static CWT_CHAR* __buildOptionAltName(const CwtOption *opt)
{
	CwtStringNS *stringNS = cwtStringNS();
	CwtString *s;
	CWT_CHAR *chars;

	s = stringNS->create();

	if( opt->shortname )
		stringNS->appendChar(s, opt->shortname);

	if( !opt->longname ) {
		if( stringNS->size(s) > 0 )
			stringNS->appendChar(s, _T('|'));
		stringNS->append(s, opt.longname);
	}

	chars = cwtStrNS()->strDup(stringNS->cstr(s));
	stringNS->free(s);
	return chars;
}
*/

static void __splitLongOptWithArg(const CWT_CHAR *token, CWT_CHAR **popt, CWT_CHAR **parg)
{
	CwtStrNS *strNS = cwtStrNS();
	CWT_CHAR *p;

	CWT_ASSERT(popt);
	CWT_ASSERT(parg);

	*popt = NULL;
	*parg = NULL;
	p = strNS->strChr(token, _T('='));
	CWT_ASSERT(p);

	*popt = strNS->strNdup(token, p-token);
	p++;
	*parg = strNS->strDup(p);
}


static void __begin (CwtOptionIterator *it, int argc, char **argv)
{
	CWT_ASSERT(it);
	it->argv = argv;
	it->argc = argc;
	it->argi = 0;
}

static BOOL __hasMore (CwtOptionIterator *it)
{
	CWT_ASSERT(it);
	return it->argi < it->argc ? TRUE : FALSE;
}



static CwtArgvType __next (CwtOptionIterator *it, CWT_CHAR **opt, CWT_CHAR **arg)
{
	CwtStrNS        *strNS   = cwtStrNS();
	CwtTextCodecNS  *codecNS = cwtTextCodecNS();
	CWT_CHAR        *optStr;
	CWT_CHAR        *popt, *parg;
	CwtArgvType     argvType;

	CWT_ASSERT(it);
	popt = NULL;
	parg = NULL;

	optStr = codecNS->fromUtf8(it->argv[it->argi], strlen(it->argv[it->argi]));

	if (strNS->startsWith(optStr, _T("-"))) {
		/* Single dash '-' */
		if (strNS->strLen(optStr) == 1)  {
			argvType = CwtArgv_SingleDash;
		}
		/* Double dash '--'*/
		else if (strNS->strLen(optStr) == 2 && optStr[1] == _T('-')) {
			argvType = CwtArgv_DoubleDash;
		}
		/* Short option */
		else if (strNS->strLen(optStr) == 2) {
			argvType = CwtArgv_ShortOpt;
			popt = strNS->strNdup(&optStr[1], 1);
		}
		/* Long option */
		else {
			popt = optStr+1;

			if (optStr[1] == _T('-'))
				++popt;

			if (strNS->strChr(optStr, _T('='))) {
				argvType = CwtArgv_LongOptWithArg;
				__splitLongOptWithArg(popt, &popt, &parg);
			} else {
				argvType = CwtArgv_LongOpt;
				popt = strNS->strDup(popt);
			}
		}
	} else {
		argvType = CwtArgv_Arg;
		parg = strNS->strDup(optStr);
	}

	CWT_FREE(optStr);

	if (opt) {
		*opt = popt;
	} else {
		if(popt)
			CWT_FREE(popt);
	}

	if (arg) {
		*arg = parg;
	} else {
		if(parg)
			CWT_FREE(parg);
	}

	it->argi++;
	return argvType;
}


/* helper function */
#ifdef __COMMENT__

static void __debug_option(CwtOption *option, const CWT_CHAR *arg)
{
	cwtLoggerNS()->debug(_Tr("option: %s"), __optionName(option));
}


static void __debug_doubleDash(void)
{
	cwtLoggerNS()->debug(_Tr("double dash"));
}

static void __debug_singleDash(void)
{
	cwtLoggerNS()->debug(_Tr("single dash"));
}

static void __debug_argument(const CWT_CHAR *arg)
{
	cwtLoggerNS()->debug(_Tr("argument: \"%s\""), arg);
}
#endif

static BOOL __getOptarg(CwtOptionIterator *it, CWT_CHAR **arg)
{
	CWT_ASSERT(it);
	CWT_ASSERT(arg);

	if (__hasMore(it)) {
		/*cwtStringNS()->sprintf(errStr, _T("'%s': argument expected"), optName);*/
		if (CwtArgv_Arg == __next(it, NULL, arg)) {
			return TRUE;
		}

		if (*arg) {
			CWT_FREE(*arg);
			*arg = NULL;
		}
	}
	return FALSE;
}

static BOOL __assignOptarg(CwtOption *popt, const CWT_CHAR *arg)
{
/*	BOOL good;*/
	BOOL ok;

	CWT_ASSERT(popt);
	CWT_ASSERT(arg);

	switch(popt->optType) {
	case CwtOpt_BOOL:
		*((BOOL*)popt->arg) = TRUE;
		break;
	case CwtOpt_INT: {
		LONGLONG v;

		v = cwtStrNS()->toLONGLONG(arg, 0, &ok);
		if (!ok)
			return FALSE;

		if(popt->validator && !popt->validator(&v))
			return FALSE;

		*((LONGLONG*)popt->arg) = v;
		}
		break;

	case CwtOpt_REAL: {
		double v;
		BOOL ok;
		v = cwtStrNS()->toDouble(arg, &ok);
		if (!ok)
			return FALSE;

		if(popt->validator && !popt->validator(&v))
			return FALSE;

		*((double*)popt->arg) = v;
		}
		break;

	case CwtOpt_TEXT:
	default:
		if(popt->validator && !popt->validator(arg))
			return FALSE;
		popt->arg = (void*)cwtStrNS()->strDup(arg);
		break;
	}

	return TRUE;
}

/**
 * - 						Single dash
 *
 * -- 						Double dash
 *
 * -s | -s arg     			Short option
 *
 * -long | --long
 * -long=arg | --long=arg |
 * -long arg | --long arg   Long option
 */
static BOOL __parse(int argc, char **argv, CwtOption *options, CwtOptionsHandler *handler)
{
	CwtHashTableNS  *htNS    = cwtHashTableNS();
	CwtStringNS     *stringNS= cwtStringNS();

	BOOL             rc;
    CwtHashTable    *shortOptsHash;
	CwtHashTable    *longOptsHash;
	CwtString       *errStr;
	CwtOption       *popt;
	CwtOptionIterator it;

	CWT_ASSERT(argc);
	CWT_ASSERT(argv);
	CWT_ASSERT(options);
	CWT_ASSERT(handler);

	longOptsHash = htNS->create(htNS->strHash, htNS->streq, NULL, NULL);
	shortOptsHash = htNS->create(htNS->charHash, htNS->chareq, NULL, NULL);
	errStr = stringNS->create();

	popt = &options[0];

	while (popt && !(!popt->longname && popt->shortname == (CWT_CHAR)0)) {
		if (popt->longname) {
			htNS->insert(longOptsHash, popt->longname, popt);
		}

		if (popt->shortname) {
			htNS->insert(shortOptsHash, &popt->shortname, popt);
		}
		popt++;
	}

	rc = TRUE;
	__begin(&it, argc, argv);

	while (__hasMore(&it)) {
		CwtArgvType argvType;
		CWT_CHAR *optStr = NULL;
		CWT_CHAR *argStr = NULL;

		popt = NULL;
		argvType = __next(&it, &optStr, &argStr);

		switch(argvType) {
		case CwtArgv_ShortOpt:
			popt = (CwtOption*)htNS->lookup(shortOptsHash, &optStr[0]);
			if (!popt) {
				stringNS->sprintf(errStr, _T("'%s': bad option"), optStr);
				rc = FALSE;
			}
			break;

		case CwtArgv_LongOpt:
		case CwtArgv_LongOptWithArg:
			popt = (CwtOption*)htNS->lookup(longOptsHash, optStr);
			if (!popt) {
				stringNS->sprintf(errStr, _T("'%s': bad option"), optStr);
				rc = FALSE;
			}
			break;

		case CwtArgv_SingleDash:
			if(handler->singleDash)
				handler->singleDash();
			break;

		case CwtArgv_DoubleDash:
			if(handler->doubleDash)
				handler->doubleDash();
			break;

		case CwtArgv_Arg:
		default:
			if (handler->argument)
				handler->argument(argStr);
			break;
		}

		if( rc
				&& (CwtArgv_ShortOpt == argvType
				|| CwtArgv_LongOpt == argvType)) {

			CWT_ASSERT(popt);
			if (CwtOpt_BOOL != popt->optType) {
				CWT_CHAR *parg = NULL;
				if (__getOptarg(&it, &parg)) {
					if (!__assignOptarg(popt, parg)) {
						cwtStringNS()->sprintf(errStr, _T("'%s': invalid value"), optStr);
						rc = FALSE;
					}
				} else {
					cwtStringNS()->sprintf(errStr, _T("'%s': argument expected"), optStr);
					rc = FALSE;
				}

				if (parg)
					CWT_FREE(parg);
			} else {
				__assignOptarg(popt, NULL);
			}
		}



		if (optStr) {
			CWT_FREE(optStr);
			optStr = NULL;
		}

		if (argStr) {
			CWT_FREE(argStr);
			argStr = NULL;
		}

		if (!rc) {
			break;
		}
	}

	stringNS->free(errStr);
	htNS->free(longOptsHash);
	htNS->free(shortOptsHash);

	return rc;
}

/**
 *
 * @param progname
 * @param optset
 * @param out
 */
static void __printUsage(const CWT_CHAR *progname, const CwtOption *optset, FILE *out)
{
	CwtStdioNS *stdioNS = cwtStdioNS();
	const CwtOption *popt = optset;

	stdioNS->fprintf(out, _Tr("Usage: %s OPTIONS\n\n"), progname);
	stdioNS->fprintf(out, _Tr("Options:\n"));

	while (popt && !(!popt->longname && popt->shortname == (CWT_CHAR)0)) {
		stdioNS->fprintf(out, _T("    ")); /* indent */

		if (popt->shortname)
			stdioNS->fprintf(out, _T("-%c"), popt->shortname);

		if (popt->longname) {
			if (popt->shortname)
				stdioNS->fprintf(out, _T(" | "));
			stdioNS->fprintf(out, _T("--%s"), popt->longname);
		}

		if (popt->optType != CwtOpt_BOOL) {
			if (popt->longname)
				stdioNS->fprintf(out, _Tr("=ARG"));
			else
				stdioNS->fprintf(out, _Tr(" ARG"));
		}

		stdioNS->fprintf(out, _T("\n\t")); /* indent */

		if (popt->desc)
			stdioNS->fprintf(out, _T("%s\n"), popt->desc);
		popt++;
	}

}
