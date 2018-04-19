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
	enum line_break_enum {
		  line_break_cr         = 0x0001
		, line_break_lf         = 0x0002
		, line_break_crlf       = 0x0004
		, line_break_any_crlf    = line_break_cr | line_break_lf | line_break_crlf
		, line_break_any_unicode = 0x0008
	};

private:
	regexp (regexp const &);
	regexp & operator = (regexp const &);

public:
	regexp ();
	explicit regexp (string const & pattern);
	explicit regexp (char const * pattern);

    regexp & operator = (string const & pattern);
    regexp & operator = (char const * pattern);
    
	void set_line_break (int lineBreak);
	void set_pattern (string const & pattern);
	void set_pattern (char const * pattern)
	{
		set_pattern(string(pattern));
	}
	const string & error_string () const;
	int error_offset () const;
	bool is_error () const;

	bool match (string const & s);
	bool match (char const * s);
	bool match_next ();
	size_t captured_count () const;
	string captured (size_t index = 0) const;
	stringlist groups () const;
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_REGEXP_HPP__ */
