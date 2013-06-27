/*
 * json_prettyprint.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: wladt
 */

#include "../include/cwt/json.hpp"

CWT_NS_BEGIN

JsonPrettyPrint::JsonPrettyPrint(JsonPrettyPrint::Style style)
{

	switch(style) {
	case StyleKR:          // Kernighan and Ritchie, baseIndent = 4, bracePosition = BraceSameLine, commaPosition = CommaSameLine
		m_format.baseIndent = 4;
		m_format.bracePosition = BraceSameLine;
		m_format.commaPosition = CommaSameLine;
		m_format.keysOrder     = NoOrder;
		break;

	case StyleBSD:         // baseIndent = 4, bracePosition = BraceNextLine, commaPosition = CommaSameLine
		m_format.baseIndent = 4;
		m_format.bracePosition = BraceNextLine;
		m_format.commaPosition = CommaSameLine;
		m_format.keysOrder     = NoOrder;
		break;

	case StyleGNU:         // baseIndent = 2, bracePosition = BraceNextLine, commaPosition = CommaSameLine
		m_format.baseIndent = 2;
		m_format.bracePosition = BraceNextLine;
		m_format.commaPosition = CommaSameLine;
		m_format.keysOrder     = NoOrder;
		break;

	case StyleWhitesmiths: // baseIndent = 4, bracePosition = BraceNextLineIndent, commaPosition = CommaSameLine
		m_format.baseIndent = 4;
		m_format.bracePosition = BraceNextLineIndent;
		m_format.commaPosition = CommaSameLine;
		m_format.keysOrder     = NoOrder;
		break;

	case StyleFavorite:
		m_format.baseIndent = 4;
		m_format.bracePosition = BraceSameLine;
		m_format.commaPosition = CommaNextLineIndent;
		m_format.keysOrder     = OrderAscending;
		break;

	case StyleCompact:
	default:
		m_format.baseIndent  = -1;
		break;
	}
}

JsonPrettyPrint::JsonPrettyPrint(const JsonPrettyPrint::Format & format)
{
	m_format = format;
}

String JsonPrettyPrint::toString(const Json & json) const
{
	String r;

	if (m_format.baseIndent < 0) // Compact
		return json.toString();

	//int indent = 0;

	if (json.isObject()) {
		r += Char('{');
		r += Char('\n');
		r += Char('}');
	} else if (json.isArray()) {
		r += Char('[');
		r += Char('\n');
		r += Char(']');
	} else {
		;
	}

	return r;
}

CWT_NS_END
