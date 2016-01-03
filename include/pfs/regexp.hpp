/*
 * regexp.hpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_REGEXP_HPP__
#define __PFS_REGEXP_HPP__

#include <pfs/string.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/pimpl.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API regexp
{
	// TODO avoid pimpl
	class impl;
	pfs::pimpl _d;

public:
	enum LineBreak {
		  LineBreak_CR         = 0x0001
		, LineBreak_LF         = 0x0002
		, LineBreak_CRLF       = 0x0004
		, LineBreak_AnyCRLF    = LineBreak_CR | LineBreak_LF | LineBreak_CRLF
		, LineBreak_AnyUnicode = 0x0008
	};

private:
	regexp (const regexp &);
	regexp & operator = (const regexp &);

public:
	regexp ();
	regexp (const string & pattern);
	regexp (const char * latin1Pattern);

	void setLineBreak (int lineBreak);
	void setPattern (const string & pattern);
	void setPattern (const char * latin1Pattern)
	{
		setPattern(string(latin1Pattern));
	}
	const string & errorString () const;
	int errorOffset () const;
	bool isError () const;

	bool match (const string & s);
	bool match (const char * s);
	bool matchNext ();
	size_t capturedCount () const;
	string captured (size_t index = 0) const;
	stringlist groups () const;
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_REGEXP_HPP__ */
