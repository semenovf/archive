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
#include <pfs/algo/find.hpp>

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


#define BITS_SIZE(T) (sizeof(T) * 8)

inline string to_string (bool value)
{
	return value ? string("true") : string("false");
}

template <typename Integer>
string __to_string (typename enable_if<is_signed<Integer>::value, Integer>::type value
		, int base
		, bool uppercase)
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
string __to_string (typename pfs::enable_if<pfs::is_unsigned<UnsignedInteger>::value, UnsignedInteger>::type value
		, int base
		, bool uppercase)
{
	char buf[BITS_SIZE(UnsignedInteger) + 1];
	char * s = uintmax_to_string(static_cast<uintmax_t>(value)
			, base
			, static_cast<int>(uppercase)
			, buf
			, sizeof(buf));
	return string(s);
}

// 1.18973e+4932 with 'f' flag has length 4940
//
template <typename Float>
string __to_string (typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type value
		, char f
		, int prec)
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

inline string to_string (signed char value, int base, bool uppercase)
{
	return __to_string<signed char>(value, base, uppercase);
}

inline string to_string (signed char value, int base)
{
	return __to_string<signed char>(value, base, false);
}

inline string to_string (signed char value)
{
	return __to_string<signed char>(value, 10, false);
}

inline string to_string (unsigned char value, int base, bool uppercase)
{
	return __to_string<unsigned char>(value, base, uppercase);
}

inline string to_string (unsigned char value, int base)
{
	return __to_string<unsigned char>(value, base, false);
}

inline string to_string (unsigned char value)
{
	return __to_string<unsigned char>(value, 10, false);
}

inline string to_string (short value, int base, bool uppercase)
{
	return __to_string<short>(value, base, uppercase);
}

inline string to_string (short value, int base)
{
	return __to_string<short>(value, base, false);
}

inline string to_string (short value)
{
	return __to_string<short>(value, 10, false);
}

inline string to_string (unsigned short value, int base, bool uppercase)
{
	return __to_string<unsigned short>(value, base, uppercase);
}

inline string to_string (unsigned short value, int base)
{
	return __to_string<unsigned short>(value, base, false);
}

inline string to_string (unsigned short value)
{
	return __to_string<unsigned short>(value, 10, false);
}

inline string to_string (int value, int base, bool uppercase)
{
	return __to_string<int>(value, base, uppercase);
}

inline string to_string (int value, int base)
{
	return __to_string<int>(value, base, false);
}

inline string to_string (int value)
{
	return __to_string<int>(value, 10, false);
}

inline string to_string (unsigned int value, int base, bool uppercase)
{
	return __to_string<unsigned int>(value, base, uppercase);
}

inline string to_string (unsigned int value, int base)
{
	return __to_string<unsigned int>(value, base, false);
}

inline string to_string (unsigned int value)
{
	return __to_string<unsigned int>(value, 10, false);
}

inline string to_string (long value, int base, bool uppercase)
{
	return __to_string<long>(value, base, uppercase);
}

inline string to_string (long value, int base)
{
	return __to_string<long>(value, base, false);
}

inline string to_string (long value)
{
	return __to_string<long>(value, 10, false);
}

inline string to_string (unsigned long value, int base, bool uppercase)
{
	return __to_string<unsigned long>(value, base, uppercase);
}

inline string to_string (unsigned long value, int base)
{
	return __to_string<unsigned long>(value, base, false);
}

inline string to_string (unsigned long value)
{
	return __to_string<unsigned long>(value, 10, false);
}

#ifdef PFS_HAVE_LONGLONG

inline string to_string (long long value, int base, bool uppercase)
{
	return __to_string<long long>(value, base, uppercase);
}

inline string to_string (long long value, int base)
{
	return __to_string<long long>(value, base, false);
}

inline string to_string (long long value)
{
	return __to_string<long long>(value, 10, false);
}

inline string to_string (unsigned long long value, int base, bool uppercase)
{
	return __to_string<unsigned long long>(value, base, uppercase);
}

inline string to_string (unsigned long long value, int base)
{
	return __to_string<unsigned long long>(value, base, false);
}

inline string to_string (unsigned long long value)
{
	return __to_string<unsigned long long>(value, 10, false);
}

#endif

inline string to_string (float value, char f, int prec)
{
	return __to_string<float>(value, f, prec);
}

inline string to_string (float value, char f)
{
	return __to_string<float>(value, f, 6);
}

inline string to_string (float value)
{
	return __to_string<float>(value, 'f', 6);
}

inline string to_string (double value, char f, int prec)
{
	return __to_string<double>(value, f, prec);
}

inline string to_string (double value, char f)
{
	return __to_string<double>(value, f, 6);
}

inline string to_string (double value)
{
	return __to_string<double>(value, 'f', 6);
}

#ifdef PFS_HAVE_LONG_DOUBLE

inline string to_string (long double value, char f, int prec)
{
	return __to_string<long double>(value, f, prec);
}

inline string to_string (long double value, char f)
{
	return __to_string<long double>(value, f, 6);
}

inline string to_string (long double value)
{
	return __to_string<long double>(value, 'f', 6);
}

#endif

inline bool starts_with (const string & haystack, const string & needle)
{
	return starts_with(haystack.begin(), haystack.end(), needle.begin(), needle.end());
}


} // pfs

#endif /* __PFS_STRING_HPP__ */
