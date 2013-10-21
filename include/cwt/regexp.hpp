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

	RegExpMatch match (const String & s);
/*
	String capturedText(size_t index = 0);
	StringList capturedText() const;
*/
};


class DLL_API RegExpMatch
{
	CWT_PIMPL_DECL_COPYABLE(RegExpMatch);

public:
	class iterator {
		class iterator_data;
		shared_ptr<iterator_data> m_d;
	public:
		iterator () : m_d() {}
		iterator (const iterator & other);

    	String value () const;
        String operator * () const { return value(); }
        bool operator  == (const iterator & o) const { return m_d == o.m_d; }
        bool operator  != (const iterator & o) const { return m_d != o.m_d; }
        iterator & operator ++ ();
        iterator   operator ++ (int) {
            iterator r(*this);
            this->operator ++();
            return r;
        }
	};

public:
	iterator begin () const;
	iterator end () const;

/*
	String capturedText(size_t index = 0);
	StringList capturedText() const;
*/
};

CWT_NS_END

#endif /* __CWT_REGEXP_HPP__ */
