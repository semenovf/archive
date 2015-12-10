/*
 * string.hpp
 *
 *  Created on: Dec 10, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_STRING_HPP__
#define __PFS_UTF8_STRING_HPP__

#include <ostream>
#include <sstream>
#include <pfs/utf8/traits.hpp>
#include <pfs/utf/string.hpp>

namespace pfs { namespace utf8 {

typedef pfs::utf::string<uint8_t, tag> string;

}} // pfs::utf8

inline bool operator == (pfs::utf8::string & lhs, const std::string & rhs)
{
    return rhs == reinterpret_cast<const std::string::const_pointer>(lhs.c_str());
}

inline bool operator == (const std::string & lhs, pfs::utf8::string & rhs)
{
	return operator == (rhs, lhs);
}

inline std::ostream & operator << (std::ostream & os, const pfs::utf8::string & s)
{
    os << reinterpret_cast<const std::string::const_pointer>(s.c_str());
    return os;
}

inline std::ostringstream & operator << (std::ostringstream & oss, const pfs::utf8::string & s)
{
    oss << reinterpret_cast<const std::string::const_pointer>(s.c_str());
    return oss;
}

#endif /* __PFS_UTF8_STRING_HPP__ */
