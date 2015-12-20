/*
 * string.hpp
 *
 *  Created on: Oct 22, 2015
 *      Author: wladt
 */

#ifndef __PFS_CAST_STRING_HPP__
#define __PFS_CAST_STRING_HPP__

#include <pfs.hpp>
#include <pfs/type_traits.hpp>
#include <pfs/cast/strtointegral.hpp>

//EXTERN_C_BEGIN
//
//DLL_API char * pfs_integral_to_string  (intmax_t n, int base, int uppercase, char * buf, int bufsz);
//DLL_API char * pfs_uintegral_to_string (uintmax_t n, int base, int uppercase, char * buf, int bufsz);
//DLL_API char * pfs_real_to_string  (real_t n, char f, int prec, char * buf, int * bufsz);
//
//EXTERN_C_END

namespace pfs {

/**
 * @brief Cast UTF-8-encoded c-string into pfs::mbcs_string.
 *
 * @param s Source UTF-8-encoded c-string.
 * @param result Reference to string to store result.
 * @return Reference to @c result.
 */
//template <typename String>
//DLL_API String & lexical_cast (const char * utf8, String & result);


//template <typename InputIterator>
//inline bool strtointmax_helper (InputIterator begin
//		, InputIterator end
//		, int radix
//		, intmax_t min_value
//		, uintmax_t max_value
//		, intmax_t & result)
//{
//	InputIterator endptr(begin);
//	result = strtointmax<InputIterator>(begin, end, radix, min_value, max_value, & endptr);
//    if ((errno == ERANGE && (result == pfs::max_type<intmax_t>() || result == pfs::min_type<intmax_t>()))
//            || (errno != 0 && result == 0))
//    	return false;
//	return true;
//}

//template <typename InputIterator>
//inline bool strtouintmax_helper (InputIterator begin
//		, InputIterator end
//		, int radix
//		, uintmax_t max_value
//		, uintmax_t & result)
//{
//	InputIterator endptr(begin);
//	result = strtouintmax<InputIterator>(begin, end, radix, max_value, & endptr);
//    if ((errno == ERANGE && (result == pfs::max_type<uintmax_t>()))
//    		|| (errno != 0 && result == 0))
//    	return false;
//	return true;
//}

inline bool lexical_cast (const string & s)
{
	return (s == string("true") || s == string("yes"))
			? true : false;
}

template <typename InputIterator, typename Integer>
typename pfs::enable_if<!pfs::is_unsigned<Integer>::value, bool>::type
lexical_cast (InputIterator begin, InputIterator end, int radix, Integer & result)
{
	InputIterator endptr(begin);
	intmax_t r = strtointmax<InputIterator>(begin, end
			, radix
			, intmax_t(pfs::min_type<Integer>())
			, uintmax_t(pfs::max_type<Integer>())
			, & endptr);

    if ((errno == ERANGE && (result == pfs::max_type<intmax_t>() || result == pfs::min_type<intmax_t>()))
            || (errno != 0 && result == 0)
            || endptr != end)
    	return false;

    result = static_cast<Integer>(r);
	return true;
}

template <typename String, typename Integer>
inline typename pfs::enable_if<!pfs::is_unsigned<Integer>::value, bool>::type
lexical_cast (const String & s, int radix, Integer & result)
{
	typedef typename String::const_iterator iterator_type;
	return lexical_cast<iterator_type, Integer>(s.begin(), s.end(), radix, result);
}

template <typename String, typename Integer>
inline typename pfs::enable_if<!pfs::is_unsigned<Integer>::value, bool>::type
lexical_cast (const String & s, Integer & result)
{
	typedef typename String::const_iterator iterator_type;
    return lexical_cast<iterator_type, Integer>(s.begin(), s.end(), 10, result);
}

template <typename InputIterator, typename Integer>
typename pfs::enable_if<pfs::is_unsigned<Integer>::value, bool>::type
lexical_cast (InputIterator begin, InputIterator end, int radix, Integer & result)
{
	InputIterator endptr(begin);
	uintmax_t r = strtouintmax<InputIterator>(begin, end
			, radix
			, uintmax_t(pfs::max_type<Integer>())
			, & endptr);

    if ((errno == ERANGE && (result == pfs::max_type<uintmax_t>()))
    		|| (errno != 0 && result == 0)
    		|| endptr != end)
    	return false;

    result = static_cast<Integer>(r);
	return true;
}

template <typename String, typename Integer>
typename pfs::enable_if<pfs::is_unsigned<Integer>::value, bool>::type
lexical_cast (const String & s, int radix, Integer & result)
{
	typedef typename String::const_iterator iterator_type;
	return lexical_cast<iterator_type, Integer>(s.begin(), s.end(), radix, result);
}

template <typename String, typename Integer>
inline typename pfs::enable_if<pfs::is_unsigned<Integer>::value, bool>::type
lexical_cast (const String & s, Integer & result)
{
	typedef typename String::const_iterator iterator_type;
	return lexical_cast<iterator_type, Integer>(s.begin(), s.end(), 10, result);
}

} // pfs

#endif /* __PFS_CAST_STRING_HPP__ */
