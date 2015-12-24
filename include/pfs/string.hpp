/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#if PFS_STRING_UTF16

#include <pfs/utf16/string.hpp>
#define _u8(s) pfs::utf16::string(s) // FIXME need convert

#elif PFS_STRING_UTF32

#include <pfs/utf32/string.hpp>
#define _u8(s) pfs::utf32::string(s) // FIXME need convert

#else

#include <pfs/utf8/string.hpp>
#define _u8(s) pfs::utf8::string(s)

#endif

#include <pfs/type_traits.hpp>

namespace pfs {

#if PFS_STRING_UTF16

typedef utf16::string string;

#elif PFS_STRING_UTF32

typedef utf32::string string;

#else

typedef utf8::string string;

#endif

DLL_API intmax_t strtointmax (string::const_iterator begin
		, string::const_iterator end
		, int radix
		, intmax_t min_value
		, uintmax_t max_value
		, string::const_iterator * endref);

DLL_API uintmax_t strtouintmax (string::const_iterator begin
		, string::const_iterator end
		, int radix
		, uintmax_t max_value
		, string::const_iterator * endref = 0);

DLL_API char * intmax_to_string (intmax_t n
		, int base
		, int uppercase
		, char * buf
		, int bufsz);

DLL_API char * uintmax_to_string (uintmax_t n
		, int base
		, int uppercase
		, char * buf
		, int bufsz);

DLL_API char * real_to_string (real_t n
		, char f
		, int prec
		, char * buf
		, int * bufsz);


inline string to_string (bool value)
{
	return value ? string("true") : string("false");
}

#define BITS_SIZE(T) (sizeof(T) * 8)

template <typename Integer>
string __signed_int_to_string (Integer value, int base, bool uppercase)
{
	char buf[BITS_SIZE(Integer) + 1];
	char * s = intmax_to_string(static_cast<intmax_t>(value)
			, base
			, static_cast<int>(uppercase)
			, buf
			, sizeof(buf));
	return string(s);
}

template <typename UnsignedInteger>
string __unsigned_int_to_string (UnsignedInteger value, int base, bool uppercase)
{
	char buf[BITS_SIZE(UnsignedInteger) + 1];
	char * s = uintmax_to_string(static_cast<uintmax_t>(value)
			, base
			, static_cast<int>(uppercase), buf
			, sizeof(buf));
	return string(s);
}

inline string to_string (signed char value, int base, bool uppercase)
{
	return __signed_int_to_string<signed char>(value, base, uppercase);
}

inline string to_string (signed char value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (signed char value)
{
	return to_string(value, 10, false);
}

inline string to_string (unsigned char value, int base, bool uppercase)
{
	return __unsigned_int_to_string<unsigned char>(value, base, uppercase);
}

inline string to_string (unsigned char value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (unsigned char value)
{
	return to_string(value, 10, false);
}

inline string to_string (short value, int base, bool uppercase)
{
	return __signed_int_to_string<short>(value, base, uppercase);
}

inline string to_string (short value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (short value)
{
	return to_string(value, 10, false);
}

inline string to_string (unsigned short value, int base, bool uppercase)
{
	return __unsigned_int_to_string<unsigned short>(value, base, uppercase);
}

inline string to_string (unsigned short value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (unsigned short value)
{
	return to_string(value, 10, false);
}

inline string to_string (int value, int base, bool uppercase)
{
	return __signed_int_to_string<int>(value, base, uppercase);
}

inline string to_string (int value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (int value)
{
	return to_string(value, 10, false);
}

inline string to_string (unsigned int value, int base, bool uppercase)
{
	return __unsigned_int_to_string<unsigned int>(value, base, uppercase);
}

inline string to_string (unsigned int value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (unsigned int value)
{
	return to_string(value, 10, false);
}

inline string to_string (long value, int base, bool uppercase)
{
	return __signed_int_to_string<long>(value, base, uppercase);
}

inline string to_string (long value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (long value)
{
	return to_string(value, 10, false);
}

inline string to_string (unsigned long value, int base, bool uppercase)
{
	return __unsigned_int_to_string<unsigned long>(value, base, uppercase);
}

inline string to_string (unsigned long value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (unsigned long value)
{
	return to_string(value, 10, false);
}

#ifdef PFS_HAVE_LONGLONG

inline string to_string (long long value, int base, bool uppercase)
{
	return __signed_int_to_string<long long>(value, base, uppercase);
}

inline string to_string (long long value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (long long value)
{
	return to_string(value, 10, false);
}

inline string to_string (unsigned long long value, int base, bool uppercase)
{
	return __unsigned_int_to_string<unsigned long long>(value, base, uppercase);
}

inline string to_string (unsigned long long value, int base)
{
	return to_string(value, base, false);
}

inline string to_string (unsigned long long value)
{
	return to_string(value, 10, false);
}

#endif

// 1.18973e+4932 with 'f' flag has length 4940
//
inline string __real_to_string (real_t value, char f, int prec)
{
	static const size_t LEXICAL_CAST_BUFSZ = 128;

	int sz = LEXICAL_CAST_BUFSZ;
	char buf[LEXICAL_CAST_BUFSZ];
	char * pbuf = buf;

	char * s = real_to_string(value, f, prec, pbuf, & sz);

	if (!s) {
		pbuf = new char [sz + 1];
		s = real_to_string(value, f, prec, pbuf, & sz);
		delete [] pbuf;
	}

	return string(s);
}

inline string to_string (float value, char f, int prec)
{
	return __real_to_string(static_cast<real_t>(value), f, prec);
}

inline string to_string (float value, char f)
{
	return __real_to_string(static_cast<real_t>(value), f, 6);
}

inline string to_string (float value)
{
	return __real_to_string(static_cast<real_t>(value), 'f', 6);
}

inline string to_string (double value, char f, int prec)
{
	return __real_to_string(static_cast<real_t>(value), f, prec);
}

inline string to_string (double value, char f)
{
	return __real_to_string(static_cast<real_t>(value), f, 6);
}

inline string to_string (double value)
{
	return __real_to_string(static_cast<real_t>(value), 'f', 6);
}

#ifdef PFS_HAVE_LONG_DOUBLE

inline string to_string (long double value, char f, int prec)
{
	return __real_to_string(static_cast<real_t>(value), f, prec);
}

inline string to_string (long double value, char f)
{
	return __real_to_string(static_cast<real_t>(value), f, 6);
}

inline string to_string (long double value)
{
	return __real_to_string(static_cast<real_t>(value), 'f', 6);
}

#endif

template <typename Integer>
inline string lexical_cast (pfs::enable_if<!pfs::is_unsigned<Integer>::value, Integer>::type value)
{
	return to_string(value);
}

template <typename UnsignedInteger>
inline string lexical_cast (pfs::enable_if<pfs::is_unsigned<UnsignedInteger>::value, UnsignedInteger>::type value)
{
	return to_string(value);
}

template <typename Float>
inline string lexical_cast (pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type value)
{
	return to_string(value);
}

inline bool lexical_cast (const string & s)
{
	return (s == string("true") || s == string("yes"))
			? true : false;
}

template <typename Integer>
typename pfs::enable_if<!pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (string::const_iterator begin, string::const_iterator end, int radix, bool & ok)
{
	ok = true;

	string::const_iterator endptr(begin);
	intmax_t r = strtointmax(begin, end
			, radix
			, intmax_t(pfs::min_type<Integer>())
			, uintmax_t(pfs::max_type<Integer>())
			, & endptr);

    if ((errno == ERANGE && (r == pfs::max_type<intmax_t>() || r == pfs::min_type<intmax_t>()))
            || (errno != 0 && r == 0)
            || endptr != end) {

    	ok = false;
    }

    return static_cast<Integer>(r);
}

template <typename Integer>
inline typename pfs::enable_if<!pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (const string & s, int radix, bool & ok)
{
	return lexical_cast<string::const_iterator, Integer>(s.begin(), s.end(), radix, ok);
}

template <typename String, typename Integer>
inline typename pfs::enable_if<!pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (const string & s, bool & ok)
{
    return lexical_cast<string::const_iterator, Integer>(s.begin(), s.end(), 10, ok);
}

template <typename UnsignedInteger>
typename pfs::enable_if<pfs::is_unsigned<UnsignedInteger>::value, UnsignedInteger>::type
lexical_cast (string::const_iterator begin, string::const_iterator end, int radix, bool & ok)
{
	ok = true;
	string::const_iterator endptr(begin);
	uintmax_t r = strtouintmax(begin, end
			, radix
			, uintmax_t(pfs::max_type<UnsignedInteger>())
			, & endptr);

    if ((errno == ERANGE && (r == pfs::max_type<uintmax_t>()))
    		|| (errno != 0 && r == 0)
    		|| endptr != end) {
    	ok = false;
    }

    return static_cast<UnsignedInteger>(r);
}

template <typename UnsignedInteger>
typename pfs::enable_if<pfs::is_unsigned<UnsignedInteger>::value, UnsignedInteger>::type
lexical_cast (const string & s, int radix, bool & ok)
{
	return lexical_cast<string::const_iterator, UnsignedInteger>(s.begin(), s.end(), radix, ok);
}

template <typename UnsignedInteger>
inline typename pfs::enable_if<pfs::is_unsigned<UnsignedInteger>::value, UnsignedInteger>::type
lexical_cast (const string & s, bool & ok)
{
	return lexical_cast<string::const_iterator, UnsignedInteger>(s.begin(), s.end(), 10, ok);
}

} // pfs

#include <pfs/bits/to_string.hpp>


#endif /* __PFS_STRING_HPP__ */
