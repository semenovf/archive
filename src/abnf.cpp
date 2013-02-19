/*
 * abnf.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: wladt
 */


#include <cwt/abnf.hpp>
#include <cwt/fsm.hpp>
#include "abnf-rfc5234.hpp"

CWT_NS_BEGIN

static bool on_begin_rule(void *userContext)
{
	CWT_UNUSED(userContext);
	return true;
}

static bool on_end_rule(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("\n");
	return true;
}

static bool on_begin_def(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("=");
	return true;
}

static bool on_rulename(const String &rulename, void *userContext)
{
	CWT_UNUSED(userContext);
	printf("RN{%s}", rulename.toUtf8().data());
	return true;
}


static bool on_comment(const String &comment, void *userContext)
{
	CWT_UNUSED2(userContext, comment);
	//printf("CM{%s}", String().sprintf("%ls", comment.data()).toUtf8().data());
	return true;
}

static bool on_begin_option(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("[");
	return true;
}

static bool on_end_option(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("]");
	return true;
}

static bool on_begin_group(void *userContext)
{
	CWT_UNUSED(userContext);
	printf("(");
	return true;
}

static bool on_end_group(void *userContext)
{
	CWT_UNUSED(userContext);
	printf(")");
	return true;
}

static bool on_numeric_value(const String &val, void *userContext)
{
	CWT_UNUSED(userContext);
	printf("NV{%s}", val.toUtf8().data());
	return true;
}

static bool on_prose_value(const String &val, void *userContext)
{
	CWT_UNUSED(userContext);
	printf("PV{%s}", val.toUtf8().data());
	return true;
}


bool Abnf::parse(const String &abnf)
{
	CWT_UNUSED(abnf);

	AbnfParseContext ctx = {
		  NULL
		, NULL
		, Callback1<void*>(on_begin_rule)
		, Callback1<void*>(on_end_rule)
		, Callback1<void*>(on_begin_def)
		, Callback2<const String&,void*>(on_rulename)
		, Callback2<const String&,void*>(on_comment)
		, Callback1<void*>(on_begin_option)
		, Callback1<void*>(on_end_option)
		, Callback1<void*>(on_begin_group)
		, Callback1<void*>(on_end_group)
		, Callback2<const String&,void*>(on_numeric_value)
		, Callback2<const String&,void*>(on_prose_value)
	};

	Fsm<Char> fsm(rulelist_fsm, &ctx);

	clearErrors();
	if (fsm.exec(0, abnf.data(), abnf.length()) == abnf.length()) {
		if (isGood()) {
			return true;
		}
	}
	return false;
}

CWT_NS_END
