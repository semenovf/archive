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

JsonValue& JsonSimplePath::makeTree(const String &jpath)
{
	JsonSimplePathContext ctx(*this, *m_jsonRootValue, true);
	Fsm<Char> fsm(jpath_fsm, &ctx);
	ssize_t result = fsm.exec(0, jpath.data(), jpath.length());

	if (result > 0 && size_t(result) == jpath.length()) {
		return *ctx.m_jsonValuePtr;
	}
}

bool JsonSimplePath::contains(const String &jpath) const
{
	const JsonValue *jvalue = & find(jpath);
	if (jvalue == &JsonValue::sharedNull)
		return false;
	return true;
}

bool JsonSimplePath::setValue(const String& jpath, const UniType& value)
{
	JsonValue *jvalue = &makeTree(jpath);

	if (jvalue == &JsonValue::sharedNull)
		return false;

	jvalue->setValue(value);
}

CWT_NS_END

