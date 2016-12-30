/**
 * @file   string.hpp
 * @author wladt
 * @date   Jan 24, 2013 2:31:19 PM
 *
 * @brief
 */

#ifndef __PFS_STRING_HPP__
#define __PFS_STRING_HPP__

#include <cmath>
#include <cerrno>

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
#include <pfs/mpl/algo/find.hpp>
#include <pfs/traits/binary_stream.hpp>

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

DLL_API real_t string_to_real (string::const_iterator begin
		, string::const_iterator end
		, string::value_type decimalPoint
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

inline string to_string (string::value_type value)
{
	return string(1, value);
}

namespace details {
namespace integral {

template <typename Integer>
string to_string (typename enable_if<is_signed<Integer>::value, Integer>::type value
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
string to_string (typename pfs::enable_if<pfs::is_unsigned<UnsignedInteger>::value, UnsignedInteger>::type value
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

}} // details::integral

namespace details {
namespace fp {

// 1.18973e+4932 with 'f' flag has length 4940
//
template <typename Float>
string to_string (typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type value
		, char f
		, int prec)
{
	static const size_t LEXICAL_CAST_BUFSZ = 128;

	string r;

	int sz = LEXICAL_CAST_BUFSZ;
	char buf[LEXICAL_CAST_BUFSZ];
	char * pbuf = buf;

	char * s = real_to_string(value, f, prec, pbuf, & sz);

	if (!s) {
		++sz;
		pbuf = new char [sz];
		s = real_to_string(value, f, prec, pbuf, & sz);
		PFS_ASSERT(s);
		r = string(s);
		delete [] pbuf;
	} else {
		r = string(s);
	}

	return r;
}

}} // details::fp

inline string to_string (signed char value, int base, bool uppercase)
{
	return details::integral::to_string<signed char>(value, base, uppercase);
}

inline string to_string (signed char value, int base)
{
	return details::integral::to_string<signed char>(value, base, false);
}

inline string to_string (signed char value)
{
	return details::integral::to_string<signed char>(value, 10, false);
}

inline string to_string (unsigned char value, int base, bool uppercase)
{
	return details::integral::to_string<unsigned char>(value, base, uppercase);
}

inline string to_string (unsigned char value, int base)
{
	return details::integral::to_string<unsigned char>(value, base, false);
}

inline string to_string (unsigned char value)
{
	return details::integral::to_string<unsigned char>(value, 10, false);
}

inline string to_string (short value, int base, bool uppercase)
{
	return details::integral::to_string<short>(value, base, uppercase);
}

inline string to_string (short value, int base)
{
	return details::integral::to_string<short>(value, base, false);
}

inline string to_string (short value)
{
	return details::integral::to_string<short>(value, 10, false);
}

inline string to_string (unsigned short value, int base, bool uppercase)
{
	return details::integral::to_string<unsigned short>(value, base, uppercase);
}

inline string to_string (unsigned short value, int base)
{
	return details::integral::to_string<unsigned short>(value, base, false);
}

inline string to_string (unsigned short value)
{
	return details::integral::to_string<unsigned short>(value, 10, false);
}

inline string to_string (int value, int base, bool uppercase)
{
	return details::integral::to_string<int>(value, base, uppercase);
}

inline string to_string (int value, int base)
{
	return details::integral::to_string<int>(value, base, false);
}

inline string to_string (int value)
{
	return details::integral::to_string<int>(value, 10, false);
}

inline string to_string (unsigned int value, int base, bool uppercase)
{
	return details::integral::to_string<unsigned int>(value, base, uppercase);
}

inline string to_string (unsigned int value, int base)
{
	return details::integral::to_string<unsigned int>(value, base, false);
}

inline string to_string (unsigned int value)
{
	return details::integral::to_string<unsigned int>(value, 10, false);
}

inline string to_string (long value, int base, bool uppercase)
{
	return details::integral::to_string<long>(value, base, uppercase);
}

inline string to_string (long value, int base)
{
	return details::integral::to_string<long>(value, base, false);
}

inline string to_string (long value)
{
	return details::integral::to_string<long>(value, 10, false);
}

inline string to_string (unsigned long value, int base, bool uppercase)
{
	return details::integral::to_string<unsigned long>(value, base, uppercase);
}

inline string to_string (unsigned long value, int base)
{
	return details::integral::to_string<unsigned long>(value, base, false);
}

inline string to_string (unsigned long value)
{
	return details::integral::to_string<unsigned long>(value, 10, false);
}

#ifdef PFS_HAVE_LONGLONG

inline string to_string (long long value, int base, bool uppercase)
{
	return details::integral::to_string<long long>(value, base, uppercase);
}

inline string to_string (long long value, int base)
{
	return details::integral::to_string<long long>(value, base, false);
}

inline string to_string (long long value)
{
	return details::integral::to_string<long long>(value, 10, false);
}

inline string to_string (unsigned long long value, int base, bool uppercase)
{
	return details::integral::to_string<unsigned long long>(value, base, uppercase);
}

inline string to_string (unsigned long long value, int base)
{
	return details::integral::to_string<unsigned long long>(value, base, false);
}

inline string to_string (unsigned long long value)
{
	return details::integral::to_string<unsigned long long>(value, 10, false);
}

#endif

inline string to_string (float value, char f, int prec)
{
	return details::fp::to_string<float>(value, f, prec);
}

inline string to_string (float value, char f)
{
	return details::fp::to_string<float>(value, f, 6);
}

inline string to_string (float value)
{
	return details::fp::to_string<float>(value, 'f', 6);
}

inline string to_string (double value, char f, int prec)
{
	return details::fp::to_string<double>(value, f, prec);
}

inline string to_string (double value, char f)
{
	return details::fp::to_string<double>(value, f, 6);
}

inline string to_string (double value)
{
	return details::fp::to_string<double>(value, 'f', 6);
}

#ifdef PFS_HAVE_LONG_DOUBLE

inline string to_string (long double value, char f, int prec)
{
	return details::fp::to_string<long double>(value, f, prec);
}

inline string to_string (long double value, char f)
{
	return details::fp::to_string<long double>(value, f, 6);
}

inline string to_string (long double value)
{
	return details::fp::to_string<long double>(value, 'f', 6);
}

#endif


template <typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value && pfs::is_signed<Integer>::value, Integer>::type
lexical_cast (string::const_iterator begin
		, string::const_iterator end
		, int radix
		, bool * ok)
{
	if (ok)
		*ok = true;

	string::const_iterator endptr(begin);
	intmax_t r = strtointmax(begin, end
			, radix
			, intmax_t(pfs::min_value<Integer>())
			, uintmax_t(pfs::max_value<Integer>())
			, & endptr);

    if ((errno == ERANGE && (r == pfs::max_value<intmax_t>() || r == pfs::min_value<intmax_t>()))
            || (errno != 0 && r == 0)
            || endptr != end) {

    	if (ok)
    		*ok = false;
    	return static_cast<Integer>(0);
    }

    return static_cast<Integer>(r);
}

template <typename Integer>
inline
typename pfs::enable_if<pfs::is_integral<Integer>::value && pfs::is_signed<Integer>::value, Integer>::type
lexical_cast (const string & s, int radix, bool * ok = 0)
{
	return lexical_cast<Integer>(s.begin(), s.end(), radix, ok);
}

template <typename Integer>
inline
typename pfs::enable_if<pfs::is_integral<Integer>::value 
                     && pfs::is_signed<Integer>::value, Integer>::type
lexical_cast (const string & s, bool * ok = 0)
{
    return lexical_cast<Integer>(s.begin(), s.end(), 10, ok);
}

template <typename Integer>
typename pfs::enable_if<pfs::is_integral<Integer>::value 
                     && pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (string::const_iterator begin
		, string::const_iterator end
		, int radix
		, bool * ok)
{
	if (ok)
		*ok = true;

	string::const_iterator endptr(begin);
	uintmax_t r = strtouintmax(begin
			, end
			, radix
			, uintmax_t(pfs::max_value<Integer>())
			, & endptr);

    if ((errno == ERANGE && (r == pfs::max_value<uintmax_t>()))
    		|| (errno != 0 && r == 0)
    		|| endptr != end) {

    	if (ok)
    		*ok = false;
    	return static_cast<Integer>(0);
    }

    return static_cast<Integer>(r);
}

template <typename Integer>
inline
typename pfs::enable_if<pfs::is_integral<Integer>::value && pfs::is_unsigned<Integer>::value, Integer>::type
lexical_cast (const string & s, int radix, bool * ok = 0)
{
	return lexical_cast<Integer>(s.begin(), s.end(), radix, ok);
}

template <typename Integer>
inline
typename pfs::enable_if<pfs::is_integral<Integer>::value 
                     && pfs::is_unsigned<Integer>::value
                     && ! pfs::is_bool<Integer>::value
            , Integer>::type
lexical_cast (const string & s, bool * ok = 0)
{
	return lexical_cast<Integer>(s.begin(), s.end(), 10, ok);
}

template <typename Float>
typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type
lexical_cast (string::const_iterator begin
		, string::const_iterator end
		, string::value_type decimalPoint
		, bool * ok)
{
	if (ok)
		*ok = true;

	string::const_iterator endptr(begin);

	real_t r = string_to_real(begin
			, end
			, decimalPoint
			, & endptr);

	if (errno || endptr != end) {
    	if (ok)
    		*ok = false;
    	return static_cast<Float>(0.0f);
	}

#if PFS_HAVE_LONG_DOUBLE
	if (not (fabsl(r) <= fabsl(max_value<Float>()))) {
    	if (ok)
    		*ok = false;
    	return static_cast<Float>(0.0f);
	}
#else
	if (not (fabs(r) <= fabs(max_value<Float>()))) {
		if (ok)
			*ok = false;
		return static_cast<Float>(0.0f);
	}
#endif

    return static_cast<Float>(r);
}

template <typename Float>
inline
typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type
lexical_cast (const string & s, string::value_type decimalPoint, bool * ok = 0)
{
	return lexical_cast<Float>(s.cbegin(), s.cend(), decimalPoint, ok);
}

template <typename Float>
inline
typename pfs::enable_if<pfs::is_floating_point<Float>::value, Float>::type
lexical_cast (const string & s, bool * ok = 0)
{
	return lexical_cast<Float>(s.cbegin(), s.cend(), string::value_type('.'), ok);
}

template <typename Boolean>
inline
typename pfs::enable_if<pfs::is_bool<Boolean>::value, Boolean>::type
lexical_cast (const string & s, bool * ok = 0)
{
	return (s == string("true") || s == string("yes"))
		? true
		: false;
}

#if defined(PFS_STRING_UTF16) || defined(PFS_STRING_UTF32)
//
//template <>
//byte_string & pack<string> (byte_string & appender
//        , string const & v
//        , endian const & order)
//{
//    pack(appender, v.size(), order);
//    
//    string::const_iterator it = v.cbegin();
//    string::const_iterator it_end = v.cend();
//    
//    for (; it != it_end; ++it) {
//        pack<string::value_type>(appender, *it, order);
//    }
//    
//	return appender;
//}

// TODO pack/unpack specializations

#else

template <typename Device>
inline ssize_t write_binary (Device & dev, endian order, string const & v)
{
    return details::sequence::write_binary<Device>(dev, order, v.c_str(), v.size());
}

template <typename Device>
ssize_t read_binary (Device & dev, endian order, string & v)
{
    char * buffer = 0;
    size_t size = 0;
    
    ssize_t result = details::sequence::read_binary<Device>(dev, order, & buffer, & size);
    
    if (buffer) {
        v.append(buffer, size);
        delete buffer;
    }
    
    return result;
}

#endif

} // pfs

#endif /* __PFS_STRING_HPP__ */
