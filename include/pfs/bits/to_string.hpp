/*
 * to_string.hpp
 *
 *  Created on: Dec 20, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_TO_STRING_HPP__
#define __PFS_BITS_TO_STRING_HPP__

namespace pfs {

DLL_API char * intmax_to_string  (intmax_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * uintmax_to_string (uintmax_t n, int base, int uppercase, char * buf, int bufsz);
DLL_API char * real_to_string    (real_t n, char f, int prec, char * buf, int * bufsz);

inline string to_string (bool value)
{
	return value ? string("true") : string("false");
}

#define BITS_SIZE(T) (sizeof(T) * 8)

template <typename T>
string __signed_int_to_string (T value, int base, bool uppercase)
{
	char buf[BITS_SIZE(T) + 1];
	char * s = intmax_to_string(static_cast<intmax_t>(value)
			, base
			, static_cast<int>(uppercase)
			, buf
			, sizeof(buf));
	return string(s);
}

template <typename T>
string __unsigned_int_to_string (T value, int base, bool uppercase)
{
	char buf[BITS_SIZE(T) + 1];
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

} // pfs

#endif /* __PFS_BITS_TO_STRING_HPP__ */
