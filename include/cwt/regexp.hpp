/*
 * regexp.hpp
 *
 *  Created on: Oct 21, 2013
 *      Author: wladt
 */

#ifndef __CWT_REGEXP_HPP__
#define __CWT_REGEXP_HPP__

#include <pfs/string.hpp>
#include <pfs/vector.hpp>
#include <pfs/pimpl.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace cwt {

class DLL_API regexp
{
	PFS_PIMPL_DECL(regexp, impl);

public:
	enum LineBreak {
		  LineBreak_CR         = 0x0001
		, LineBreak_LF         = 0x0002
		, LineBreak_CRLF       = 0x0004
		, LineBreak_AnyCRLF    = LineBreak_CR | LineBreak_LF | LineBreak_CRLF
		, LineBreak_AnyUnicode = 0x0008
	};

public:
	regexp ();
	regexp (const pfs::string & pattern);
	regexp (const char * latin1Pattern);

	void setLineBreak (int lineBreak);
	void setPattern (const pfs::string & pattern);
	void setPattern (const char * latin1Pattern)
	{
		setPattern(pfs::string::fromLatin1(latin1Pattern));
	}
	const pfs::string & errorString () const;
	int errorOffset () const;
	bool isError () const;

	bool match (const pfs::string & s);
	bool match (const char * s);
	bool matchNext ();
	size_t capturedCount () const;
	pfs::string captured (size_t index = 0) const;
	pfs::vector<pfs::string> groups () const;
};

} // cwt

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __CWT_REGEXP_HPP__ */
