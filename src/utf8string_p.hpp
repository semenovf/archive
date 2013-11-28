/**
 * @file utf8string_p.hpp
 * @author wladt
 * @date Nov 27, 2013
 */

#ifndef __PFS_UTF8STRING_P_HPP__
#define __PFS_UTF8STRING_P_HPP__

#include "../include/pfs/utf8string.hpp"
#include <string>

namespace pfs {

class utf8string::impl : public std::string
{
public:
	size_t _length; // length in unicode code points

	/*
	typedef std::string::iterator iterator;
	typedef std::string::const_iterator const_iterator;
	static const size_t npos = std::string::npos;
*/

	impl () : std::string(), _length(0) {}
	impl (const impl & other) : std::string(other), _length(other._length) { }
	impl (size_t n, char ch) : std::string(n, ch), _length(0) { }
	impl (const std::string & other) : std::string(other), _length(0) { }
};

inline void utf8string::detach ()
{
	if (!_pimpl.unique()) {
		shared_ptr<utf8string::impl> d(new utf8string::impl(*_pimpl));
		_pimpl.swap(d);
	}
}

} // pfs

#endif /* __PFS_UTF8STRING_P_HPP__ */
