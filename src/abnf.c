/*
 * abnf.c
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */


#include "cwt/abnf.h"
#include <cwt/fsm.h>
#include "abnf-rfc5234.h"

static ssize_t __abnf_parse      (const CWT_CHAR *abnf_string);
static ssize_t __abnf_parseFile  (const CWT_CHAR *filepath);
static void    __abnf_setNewLine (CwtNewLine nl);

static CWT_CHAR *__abnf_nl_win  = _T("\r\n");
static CWT_CHAR *__abnf_nl_mac9 = _T("\n\r");
static CWT_CHAR *__abnf_nl_unix = _T("\n ");


static CwtAbnfNS __cwtAbnfNS = {
	  __abnf_parse
	, __abnf_parseFile
	, __abnf_setNewLine
};

DLL_API_EXPORT CwtAbnfNS* cwtAbnfNS(void)
{
	CWT_UNUSED(rulelist_fsm);
	return &__cwtAbnfNS;
}


static ssize_t __abnf_parse (const CWT_CHAR *abnf_string)
{
	CWT_UNUSED(abnf_string);
	return (ssize_t)-1;
}

static ssize_t __abnf_parseFile (const CWT_CHAR *filepath)
{
	CWT_UNUSED(filepath);
	return (ssize_t)-1;
}

static void __abnf_setNewLine (CwtNewLine nl)
{
	switch( nl ) {
	case Cwt_NL_Win:
		NL_FSM[0].condition.str.chars = __abnf_nl_win;
		NL_FSM[0].condition.str.len = 2;
		break;
	case Cwt_NL_MacOS9:
		NL_FSM[0].condition.str.chars = __abnf_nl_mac9;
		NL_FSM[0].condition.str.len = 2;
		break;
	default:
		NL_FSM[0].condition.str.chars = __abnf_nl_unix;
		NL_FSM[0].condition.str.len = 1;
		break;
	}
}
