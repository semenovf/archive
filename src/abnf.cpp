/*
 * abnf.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */


#include <cwt/abnf.hpp>
#include <cwt/fsm.hpp>
#include "abnf-rfc5234.hpp"

static ssize_t abnf_parse      (const CWT_CHAR *abnf_string);
static ssize_t abnf_parse_file (const CWT_CHAR *filepath);

static CwtAbnfNS __cwt_abnf_ns = {
	  abnf_parse
	, abnf_parse_file
};

DLL_API_EXPORT CwtAbnfNS* cwt_abnf_ns(void)
{
	CWT_UNUSED(rulelist_fsm);
	return &__cwt_abnf_ns;
}


static ssize_t abnf_parse (const CWT_CHAR *abnf_string)
{
	CWT_UNUSED(abnf_string);
	return (ssize_t)-1;
}

static ssize_t abnf_parse_file (const CWT_CHAR *filepath)
{
	CWT_UNUSED(filepath);
	return (ssize_t)-1;
}

