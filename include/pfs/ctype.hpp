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

template <typename CharT>
bool is_latin1 (CharT v);

template <typename CharT>
bool is_digit (CharT v);

template <typename CharT>
int to_digit (CharT v);

template <typename CharT>
bool eq_latin1 (CharT v, char latin1);

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

template <>
inline bool is_latin1<char> (char v)
{
	return v >= 0;// && v <= 127;
}

template <>
inline bool is_latin1<ucchar> (ucchar v)
{
	return v.value() <= 127;
}

template <>
inline bool eq_latin1<char> (char v, char latin1)
{
	PFS_ASSERT(latin1 >= 0/* && latin1 <= 127*/);
	return v == latin1;
}

template <>
inline bool eq_latin1<ucchar> (ucchar v, char latin1)
{
	PFS_ASSERT(latin1 >= 0 /*&& latin1 <= 127*/);
	return v.value() == uint32_t(latin1);
}

template <>
inline bool is_digit<char> (char v)
{
	return isdigit(v);
}

template <>
inline bool is_digit<ucchar> (ucchar v)
{
	return v.isDigit();
}

template <>
int to_digit<char> (char v)
{
	PFS_ASSERT(is_digit(v));
	return v - '0';
}

template <>
int to_digit<ucchar> (ucchar v)
{
	PFS_ASSERT(is_digit(v));
	return v.value() - uint32_t('0');
}


}

#endif /* __PFS_CTYPE_HPP__ */
