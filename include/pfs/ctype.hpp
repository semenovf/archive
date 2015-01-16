/*
 * ctype.hpp
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_CTYPE_HPP__
#define __PFS_CTYPE_HPP__

#include <pfs/ucchar.hpp>
#include <cctype>

namespace pfs {

template <typename CharT>
bool is_space (CharT v);

template <typename CharT>
CharT to_upper (CharT v);

template <>
inline bool is_space<char> (char v)
{
	return std::isspace(v);
}

template <>
inline bool is_space<ucchar> (ucchar v)
{
	return v.isSpace();
}

template <>
inline char to_upper<char> (char v)
{
	return (char)std::toupper(v);
}

template <>
inline ucchar to_upper<ucchar> (ucchar v)
{
	return v.toUpper();
}


template <typename CharT>
bool is_latin1 (CharT v)
{
	return uint32_t(v) <= 127;
}

}

#endif /* __PFS_CTYPE_HPP__ */
