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

	For example see tests/options.c
 */

#include <cwt/options.h>
#include <string.h>
#include <cwt/logger.h>
#include <cwt/str.h>
#include <cwt/string.h>
#include <cwt/txtcodec.h>
#include <cwt/hashtab.h>

static CwtOptionIterator* __createIterator (CwtOptionIteratorType itType);
static void  __printUsage(const CWT_CHAR *progname
		, const CWT_CHAR *copyright
		, const CWT_CHAR *usageSummary
		, const CwtOption *optset
		, FILE *out);
static BOOL  __parse      (int argc, char **argv, CwtOption *options, CwtStrList *args, CwtOptionIteratorType itType);
static BOOL  __parseWithIterator (CwtOption *options, CwtStrList *args, CwtOptionIterator *it);

/*BOOL (*parseWithIterator) (CwtOption *options, CwtOptionIterator *it);*/

static void        __it_default_begin     (CwtOptionIterator *it, int argc, char **argv);
static BOOL        __it_default_hasMore   (CwtOptionIterator *it);
static CwtArgvType __it_default_next      (CwtOptionIterator *it, CWT_CHAR **opt, CWT_CHAR **arg);
static void        __it_default_free      (CwtOptionIterator *it) { if (it) CWT_FREE(it); }

static void        __it_on_error(const CWT_CHAR *errstr) { cwtLoggerNS()->error(errstr); }

static CwtOptionsNS __cwtOptionsNS = {
	  __createIterator
	, __printUsage
	, __parse
	, __parseWithIterator
};

typedef struct _CwtDefaultOptionIterator {
	struct _CwtOptionIterator __base;
	char **argv;
	int    argc;
	int    argi;
} CwtDefaultOptionIterator;


/* helper functions */
static BOOL __getOptarg(CwtOptionIterator *it, CWT_CHAR **arg);
static BOOL __assignOptarg(CwtOption *popt, const CWT_CHAR *arg);


DLL_API_EXPORT CwtOptionsNS* cwtOptionsNS(void)
{
	return &__cwtOptionsNS;
}

static CwtOptionIterator* __createIterator (CwtOptionIteratorType itType)
{
	switch(itType) {
	case Cwt_OptIt_Default:
	default: {
		CwtDefaultOptionIterator *it;
		it = CWT_MALLOC(CwtDefaultOptionIterator);
		it->__base.begin        = __it_default_begin;
		it->__base.hasMore      = __it_default_hasMore;
		it->__base.next         = __it_default_next;
		it->__base.free         = __it_default_free;
		it->__base.onError      = __it_on_error;
		it->__base.onOption     = NULL;
		it->__base.onSingleDash = NULL;
		it->__base.onDoubleDash = NULL;
		it->__base.onArgument   = NULL;
		it->argv = NULL;
		it->argc = 0;
		it->argi = 0;

		return (CwtOptionIterator *)it;
	}
		break;
	}

	return NULL;
}


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


static void __it_default_begin (CwtOptionIterator *it, int argc, char **argv)
{
	CwtDefaultOptionIterator *it_ = (CwtDefaultOptionIterator *)it;
	CWT_ASSERT(it);
	it_->argv = argv;
	it_->argc = argc;
	it_->argi = 0;
}

static BOOL __it_default_hasMore (CwtOptionIterator *it)
{
	CwtDefaultOptionIterator *it_ = (CwtDefaultOptionIterator *)it;
	CWT_ASSERT(it);
	return it_->argi < it_->argc ? TRUE : FALSE;
}



static CwtArgvType __it_default_next (CwtOptionIterator *it, CWT_CHAR **opt, CWT_CHAR **arg)
{
	CwtDefaultOptionIterator *it_ = (CwtDefaultOptionIterator *)it;
	CwtStrNS        *strNS   = cwtStrNS();
	CwtTextCodecNS  *codecNS = cwtTextCodecNS();
	CWT_CHAR        *optStr;
	CWT_CHAR        *popt, *parg;
	CwtArgvType     argvType;

	CWT_ASSERT(it);
	popt = NULL;
	parg = NULL;

	optStr = codecNS->fromUtf8(it_->argv[it_->argi], strlen(it_->argv[it_->argi]));

	if (strNS->startsWith(optStr, _T("-"))) {
		/* Single dash '-' */
		if (strNS->strLen(optStr) == 1)  {
			argvType = Cwt_Argv_SingleDash;
		}
		/* Double dash '--'*/
		else if (strNS->strLen(optStr) == 2 && optStr[1] == _T('-')) {
			argvType = Cwt_Argv_DoubleDash;
		}
		/* Short option */
		else if (strNS->strLen(optStr) == 2) {
			argvType = Cwt_Argv_ShortOpt;
			popt = strNS->strNdup(&optStr[1], 1);
		}
		/* Long option */
		else {
			popt = optStr+1;

			if (optStr[1] == _T('-'))
				++popt;

			if (strNS->strChr(optStr, _T('='))) {
				argvType = Cwt_Argv_LongOptWithArg;
				__splitLongOptWithArg(popt, &popt, &parg);
			} else {
				argvType = Cwt_Argv_LongOpt;
				popt = strNS->strDup(popt);
			}
		}
	} else {
		argvType = Cwt_Argv_Arg;
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

	it_->argi++;
	return argvType;
}


static BOOL __getOptarg(CwtOptionIterator *it, CWT_CHAR **arg)
{
	CWT_ASSERT(it);
	CWT_ASSERT(arg);

	if (it->hasMore(it)) {
		if (Cwt_Argv_Arg == it->next(it, NULL, arg)) {
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
	BOOL ok;

	CWT_ASSERT(popt);

	if (!popt->arg)
		return TRUE;

	switch(popt->optType) {
	case Cwt_Opt_BOOL:
		*((BOOL*)popt->arg) = TRUE;
		break;
	case Cwt_Opt_INT: {
		LONGLONG v;
		CWT_ASSERT(arg);

		v = cwtStrNS()->toLONGLONG(arg, 0, &ok);
		if (!ok)
			return FALSE;

		if(popt->validator && !popt->validator(&v))
			return FALSE;

		*((LONGLONG*)popt->arg) = v;
		}
		break;

	case Cwt_Opt_REAL: {
		double v;
		BOOL ok;
		CWT_ASSERT(arg);

		v = cwtStrNS()->toDouble(arg, &ok);
		if (!ok)
			return FALSE;

		if(popt->validator && !popt->validator(&v))
			return FALSE;

		*((double*)popt->arg) = v;
		}
		break;

	case Cwt_Opt_TEXT:
	default:
		CWT_ASSERT(arg);
		if(popt->validator && !popt->validator(arg))
			return FALSE;
		*(CWT_CHAR**)popt->arg = cwtStrNS()->strDup(arg);
		break;
	}

	return TRUE;
}


static BOOL  __parse (int argc, char **argv, CwtOption *options, CwtStrList *args, CwtOptionIteratorType itType)
{
	CwtOptionIterator *it;
	BOOL ok;

	CWT_ASSERT(argv);
	CWT_ASSERT(options);

	it = __createIterator(itType);
	CWT_ASSERT(it);

	it->begin(it, argc, argv);
	it->next(it, NULL, NULL); /* skip program name */
	ok = __parseWithIterator(options, args, it);
	it->free(it);
	return ok;
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
static BOOL __parseWithIterator(CwtOption *options, CwtStrList *args, CwtOptionIterator *it)
{
	CwtHashTableNS  *htNS     = cwtHashTableNS();
	CwtStringNS     *stringNS = cwtStringNS();
	CwtStrListNS    *slNS     = cwtStrListNS();

	BOOL             rc;
    CwtHashTable    *shortOptsHash;
	CwtHashTable    *longOptsHash;
	CwtString       *errStr;
	CwtOption       *popt;

	CWT_ASSERT(options);
	CWT_ASSERT(it);

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

	while (it->hasMore(it)) {
		CwtArgvType argvType;
		CWT_CHAR *optStr = NULL;
		CWT_CHAR *argStr = NULL;

		popt = NULL;
		argvType = it->next(it, &optStr, &argStr);

		switch(argvType) {
		case Cwt_Argv_ShortOpt:
			popt = (CwtOption*)htNS->lookup(shortOptsHash, &optStr[0]);
			if (!popt) {
				stringNS->sprintf(errStr, _T("'%s': bad option"), optStr);
				rc = FALSE;
			}
			break;

		case Cwt_Argv_LongOpt:
		case Cwt_Argv_LongOptWithArg:
			popt = (CwtOption*)htNS->lookup(longOptsHash, optStr);
			if (!popt) {
				stringNS->sprintf(errStr, _T("'%s': bad option"), optStr);
				rc = FALSE;
			}
			break;

		case Cwt_Argv_SingleDash:
			if (it->onSingleDash)
				it->onSingleDash();
			break;

		case Cwt_Argv_DoubleDash:
			if (it->onDoubleDash)
				it->onDoubleDash();
			break;

		case Cwt_Argv_Arg:
		default:
			if (it->onArgument)
				it->onArgument(argStr);

			if (args)
				slNS->append(args, argStr);
			break;
		}

		if( rc
				&& (Cwt_Argv_ShortOpt == argvType
				|| Cwt_Argv_LongOpt == argvType
				|| Cwt_Argv_LongOptWithArg == argvType)) {

			CWT_ASSERT(popt);
			if (Cwt_Opt_BOOL != popt->optType) {

				if (!argStr)
					__getOptarg(it, &argStr);

				if (argStr) {
					if (!__assignOptarg(popt, argStr)) {
						cwtStringNS()->sprintf(errStr, _T("'%s': invalid value"), optStr);
						rc = FALSE;
					} else {
						if (it->onOption)
							it->onOption(popt);
					}
				} else {
					cwtStringNS()->sprintf(errStr, _T("'%s': argument expected"), optStr);
					rc = FALSE;
				}
			} else {
				__assignOptarg(popt, NULL);
				if (it->onOption)
					it->onOption(popt);
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

		if (!rc)
			break;
	}

	if (!rc) {
		if (it->onError)
			it->onError(stringNS->cstr(errStr));
	}


	stringNS->free(errStr);
	htNS->free(longOptsHash);
	htNS->free(shortOptsHash);

	return rc;
}

/**
 * @fn void CwtOptionsNS::printUsage(const CWT_CHAR *progname
 * 		, const CWT_CHAR *copyright
 * 		, const CWT_CHAR *usageSummary
 * 		, const CwtOption *optset
 * 		, FILE *out)
 *
 * @brief Prints out program usage.
 *
 * @param progname Program name. If NULL nothing will be printed out.
 * @param copyright Copyright text. May be NULL.
 * @param usageSummary Usage summary. May be NULL.
 * @param optset List of options terminated by @c CWT_END_OPTIONS.
 * @param out Output stream to print out the usage.
 *
 * @code{.c}
 * optionsNS->printUsage(_T("amazing-program")
 *		, _Tr("Copyright (R) 2012 CherrySW")
 *		, _Tr("[OPTIONS] file1 [file2] ...")
 *		, optset
 *		, stdout);
 * @endcode
 *
 * will print out to standard output stream:
 *
 * @verbatim
 * Copyright (R) 2012 CherrySW
 *
 * Usage: amazing-program [OPTIONS] file1 [file2] ...
 *
 * Options:
 * ...
 * <options and descriptions>
 * ...
 * @endverbatim
 */
static void __printUsage(const CWT_CHAR *progname
		, const CWT_CHAR *copyright
		, const CWT_CHAR *usageSummary
		, const CwtOption *optset
		, FILE *out)
{
	CwtStdioNS *stdioNS = cwtStdioNS();
	const CwtOption *popt = optset;

	if (!progname)
		return;

	if (copyright)
		stdioNS->fprintf(out, _T("%s\n\n"), copyright);

	if (usageSummary) {
		stdioNS->fprintf(out, _Tr("Usage: %s %s\n\n"), progname, usageSummary);
	} else {
		stdioNS->fprintf(out, _Tr("Usage: %s OPTIONS [ARGS]\n\n"), progname);
	}

	if (!optset) {
		return;
	}

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

		if (popt->optType != Cwt_Opt_BOOL) {
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
