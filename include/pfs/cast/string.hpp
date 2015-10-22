/*
 * string.hpp
 *
 *  Created on: Oct 22, 2015
 *      Author: wladt
 */

#ifndef __PFS_CAST_STRING_HPP__
#define __PFS_CAST_STRING_HPP__

#include <pfs.hpp>

EXTERN_C_BEGIN

DLL_API char * pfs_integral_to_string  (integral_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * pfs_uintegral_to_string (uintegral_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * pfs_real_to_string  (real_t n, char f, int prec, char * buf, int * bufsz);
DLL_API int    pfs_latin1_to_digit (char latin1);

EXTERN_C_END

namespace pfs {

#define BITS_SIZE(T) (sizeof(T) * 8)

template <typename String>
String & lexical_cast (bool value, String & result)
{
	result = value ? String("true") : String("false");
	return result;
}

template <typename String, typename T>
String & __lexical_cast_signed (T value, int base, bool uppercase, String & result)
{
	char buf[BITS_SIZE(T) + 1];
	char * s = pfs_integral_to_string(static_cast<integral_t>(value)
			, base
			, static_cast<int>(uppercase)
			, buf
			, sizeof(buf));
	result = String(s);
	return result;
}

template <typename String, typename T>
String & __lexical_cast_unsigned (T value, int base, bool uppercase, String & result)
{
	char buf[BITS_SIZE(unsigned char) + 1];
	char * s = pfs_uintegral_to_string(static_cast<uintegral_t>(value)
			, base
			, static_cast<int>(uppercase), buf
			, sizeof(buf));
	result = String(s);
	return result;
}

template <typename String>
inline String & lexical_cast (signed char value, int base, bool uppercase, String & result)
{
	return __lexical_cast_signed<String, signed char>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (signed char value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (signed char value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned char value, int base, bool uppercase, String & result)
{
	return __lexical_cast_unsigned<String, unsigned char>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (unsigned char value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned char value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (short value, int base, bool uppercase, String & result)
{
	return __lexical_cast_signed<String, short>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (short value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (short value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned short value, int base, bool uppercase, String & result)
{
	return __lexical_cast_unsigned<String, unsigned short>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (unsigned short value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned short value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (int value, int base, bool uppercase, String & result)
{
	return __lexical_cast_signed<String, int>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (int value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (int value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned int value, int base, bool uppercase, String & result)
{
	return __lexical_cast_unsigned<String, unsigned int>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (unsigned int value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned int value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (long value, int base, bool uppercase, String & result)
{
	return __lexical_cast_signed<String, long>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (long value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (long value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned long value, int base, bool uppercase, String & result)
{
	return __lexical_cast_unsigned<String, unsigned long>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (unsigned long value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned long value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

#ifdef PFS_HAVE_LONGLONG

template <typename String>
inline String & lexical_cast (long long value, int base, bool uppercase, String & result)
{
	return __lexical_cast_signed<String, long long>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (long long value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (long long value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned long long value, int base, bool uppercase, String & result)
{
	return __lexical_cast_unsigned<String, unsigned long long>(value, base, uppercase, result);
}

template <typename String>
inline String & lexical_cast (unsigned long long value, int base, String & result)
{
	return lexical_cast(value, base, false, result);
}

template <typename String>
inline String & lexical_cast (unsigned long long value, String & result)
{
	return lexical_cast(value, 10, false, result);
}

#endif

// 1.18973e+4932 with 'f' flag has length 4940
//
template <typename String>
String & __lexical_cast_real (real_t value, char f, int prec, String & result)
{
#define LEXICAL_CAST_BUFSZ 64

	int sz = LEXICAL_CAST_BUFSZ;
	char buf[LEXICAL_CAST_BUFSZ];

	char * s = pfs_real_to_string(value, f, prec, buf, & sz);
	if (s) {
		result = String(s);
	} else  {
		buf = new char [sz + 1];
		s = pfs_real_to_string(value, f, prec, buf, & sz);
		PFS_ASSERT(s);
		result = String(s);
		delete [] buf;
	}

	return result;
}

template <typename String>
String & lexical_cast (float value, char f, int prec, String & result)
{
	return lexical_cast(static_cast<real_t>(value), f, prec, result);
}

template <typename String>
String & lexical_cast (float value, char f, String & result)
{
	return lexical_cast(static_cast<real_t>(value), f, 6, result);
}

template <typename String>
String & lexical_cast (float value, String & result)
{
	return lexical_cast(static_cast<real_t>(value), 'f', 6, result);
}

template <typename String>
String & lexical_cast (double value, char f, int prec, String & result)
{
	return lexical_cast(static_cast<real_t>(value), f, prec, result);
}

template <typename String>
String & lexical_cast (double value, char f, String & result)
{
	return lexical_cast(static_cast<real_t>(value), f, 6, result);
}

template <typename String>
String & lexical_cast (double value, String & result)
{
	return lexical_cast(static_cast<real_t>(value), 'f', 6, result);
}

#ifdef PFS_HAVE_LONG_DOUBLE

template <typename String>
String & lexical_cast (long double value, char f, int prec, String & result)
{
	return lexical_cast(static_cast<real_t>(value), f, prec, result);
}

template <typename String>
String & lexical_cast (long double value, char f, String & result)
{
	return lexical_cast(static_cast<real_t>(value), f, 6, result);
}

template <typename String>
String & lexical_cast (long double value, String & result)
{
	return lexical_cast(static_cast<real_t>(value), 'f', 6, result);
}

#endif

} // pfs

#endif /* __PFS_CAST_STRING_HPP__ */
