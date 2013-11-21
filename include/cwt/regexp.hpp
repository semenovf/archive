/*
 * regexp.hpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_REGEXP_HPP__
#define __CWT_REGEXP_HPP__

#include <cwt/string.hpp>
#include <cwt/stringlist.hpp>
#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class DLL_API RegExp
{
	CWT_PIMPL_DECL_COPYABLE(RegExp);

public:
	enum LineBreak {
		  LineBreak_CR         = 0x0001
		, LineBreak_LF         = 0x0002
		, LineBreak_CRLF       = 0x0004
		, LineBreak_AnyCRLF    = LineBreak_CR | LineBreak_LF | LineBreak_CRLF
		, LineBreak_AnyUnicode = 0x0008
	};

public:
	RegExp ();
	RegExp (const String & pattern);

	void setLineBreak (int lineBreak);
	void setPattern (const String & pattern);
	const String & errorString () const;
	int errorOffset () const;
	bool isError () const;

	bool match (const String & s);
	bool matchNext ();
	size_t capturedCount () const;
	String captured (size_t index = 0) const;
	StringList groups () const;
};

CWT_NS_END

#endif /* __CWT_REGEXP_HPP__ */
