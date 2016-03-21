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
#include <pfs/utf/string.hpp>
#include <pfs/utf8/decode.hpp>
#include <pfs/utf8/encode.hpp>
#include <pfs/utf8/advance.hpp>

namespace pfs { namespace utf8 {

struct tag {};

typedef pfs::utf::string<char/*uint8_t*/, tag> string;

template <typename CodeUnitIterator>
struct iterator
{
	typedef typename string::utf_traits_type::iterator_impl<CodeUnitIterator> type;
};

}} // pfs::utf8

namespace pfs { namespace utf {

typedef pfs::utf8::string string_type;
typedef pfs::utf8::string::size_type size_type;
typedef pfs::utf8::string::value_type value_type;
typedef pfs::utf8::string::pointer pointer_type;
typedef pfs::utf8::string::const_pointer const_pointer_type;
typedef pfs::utf8::string::difference_type difference_type;

template <>
template <typename CodeUnitIterator>
inline value_type string_type::utf_traits_type::decode (CodeUnitIterator & p)
{
	return value_type(static_cast<uintmax_t>(pfs::utf8::decode(p)));
}

template <>
inline void string_type::utf_traits_type::encode (value_type uc
		, std::back_insert_iterator<string_type::utf_traits_type::rep_type> begin)
{
	pfs::utf8::encode(lexical_cast<uint32_t>(uc), begin);
}

template <>
inline pointer_type
string_type::utf_traits_type::encode (value_type uc, pointer begin)
{
	return pfs::utf8::encode(lexical_cast<uint32_t>(uc), begin);
}

template <>
template <typename CodeUnitIterator>
inline void string_type::utf_traits_type::advance_forward (CodeUnitIterator & p, difference_type n)
{
	pfs::utf8::advance_forward(p, n);
}

template <>
template <typename CodeUnitIterator>
inline void string_type::utf_traits_type::advance_backward (CodeUnitIterator & p, difference_type n)
{
	pfs::utf8::advance_backward(p, n);
}

template <>
inline string_type::string (const std::string & s)
	: _d(s)
{}

template <>
inline string_type::string (const_pointer s)
	: _d(s)
{}

template <>
inline bool string_type::starts_with (const string & needle) const
{
	return _d.size() < needle._d.size()
			? false
			: _d.compare(0, needle._d.size(), needle._d) == 0;
}

template <>
inline bool string_type::ends_with (const string & needle) const
{
	return _d.size() < needle._d.size()
			? false
			: _d.compare(_d.size() - needle._d.size(), needle._d.size(), needle._d) == 0;
}

inline std::ostream & operator << (std::ostream & os, const string_type & s)
{
    os << reinterpret_cast<const char *>(s.c_str());
    return os;
}

inline std::ostringstream & operator << (std::ostringstream & oss, const string_type & s)
{
    oss << reinterpret_cast<const char *>(s.c_str());
    return oss;
}


inline bool operator == (const string_type & lhs, const std::string & rhs)
{
    return rhs == reinterpret_cast<const std::string::const_pointer>(lhs.c_str());
}

inline bool operator == (const std::string & lhs, const string_type & rhs)
{
	return operator == (rhs, lhs);
}

}} // pfs::utf


#endif /* __PFS_UTF8_STRING_HPP__ */
