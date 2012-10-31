/*
 * abnf.c
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */


#include "cwt/abnf.h"
#include <cwt/fsm.h>
#include <cwt/fsm_common.h>
#include "abnf-rfc5234.h"

static ssize_t __abnf_parse (const CWT_CHAR *abnf_string);
static ssize_t __abnf_parseFile (const CWT_CHAR *filepath);


static CwtAbnfNS __cwtAbnfNS = {
	  __abnf_parse
	, __abnf_parseFile
};

DLL_API_EXPORT CwtAbnfNS* cwtAbnfNS(void)
{
	CWT_UNUSED(fsm_unused_commons_fn);
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
