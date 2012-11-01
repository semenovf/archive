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
/* FIXME remove CWT_UNUSED lines */
	CWT_UNUSED(fsm_unused_commons_fn);
	CWT_UNUSED(rulename_fsm);
	CWT_UNUSED(comment_fsm);
	CWT_UNUSED(c_nl_fsm);
	CWT_UNUSED(c_wsp_fsm);
	CWT_UNUSED(char_val_fsm);
	CWT_UNUSED(bin_val_fsm);
	CWT_UNUSED(dec_val_fsm);
	CWT_UNUSED(hex_val_fsm);
	CWT_UNUSED(num_val_fsm);
	CWT_UNUSED(prose_val_fsm);
	CWT_UNUSED(defined_as_fsm);

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
