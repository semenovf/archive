/*
 * json_simplepath.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: wladt
 */

#include "json_simplepath_fsm.hpp"

CWT_NS_BEGIN

bool JsonSimplePath::changeRoot(const String &jpath)
{
	m_jsonRootValue = &find(jpath);
	return m_jsonRootValue != &JsonValue::sharedNull ? true : false;
}

const JsonValue& JsonSimplePath::findValue(const String &jpath) const
{
	JsonSimplePathContext ctx(*this, *m_jsonRootValue);
	Fsm<Char> fsm(jpath_fsm, &ctx);
	ssize_t result = fsm.exec(0, jpath.data(), jpath.length());

	if (result > 0 && size_t(result) == jpath.length()) {
		return *ctx.m_jsonValuePtr;
	}

	return JsonValue::sharedNull;
}

bool JsonSimplePath::contains(const String &jpath) const
{
	const JsonValue *p = & find(jpath);
	if (p == &JsonValue::sharedNull)
		return false;
	return true;
}

CWT_NS_END

