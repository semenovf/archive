/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   May 15, 2013 4:37:44 PM
 *
 * @brief
 */

//#include "../include/cwt/hash.hpp"
#include "../include/pfs/bytearray.hpp"
#include <cstring>
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <iomanip>
//#include <cmath>

namespace pfs {

//const bytearray bytearray::EndOfLine(1, '\n');

/*
DLL_API uint_t hash_func(const bytearray & key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t *>(key.constData()), key.size(), seed);
}
*/
bytearray & bytearray::insert (const char * s, const const_iterator & pos)
{
	return insert(s, strlen(s), pos);
}

bytearray & bytearray::insert (const char * s, size_t n, const const_iterator & pos)
{
	const_iterator it(pos);

	if (it > end())
		it = end();
	if (it < begin())
		it = begin();

	size_t i = begin() - it;
	base_class::insert(i, s, n);
	return *this;
}


bool bytearray::endsWith(const char * s, size_t n) const
{
	size_t len = length();
	if (len < n)
		return false;
	return compare(len - n, n, s, n) == 0;
}

bool bytearray::endsWith (const char * s) const
{
	return endsWith(s, strlen(s));
}

bool bytearray::startsWith(const char * s, size_t n) const
{
	size_t len = length();

	if (len < n)
		return false;
	return compare(0, n, s, n) == 0;
}

bool bytearray::startsWith (const char * s) const
{
	return startsWith(s, strlen(s));
}

bytearray::const_iterator bytearray::find(const char * s, size_t pos, size_t n) const
{
	if (pos >= length())
		return end();

	size_t find_pos = base_class::find(s, pos, n);

	if (find_pos == base_class::npos)
		return end();

	return bytearray::const_iterator(data() + find_pos);
}

bytearray::const_iterator bytearray::find(const char * s, size_t pos) const
{
	return find(s, pos, strlen(s));
}

bytearray & bytearray::remove(const const_iterator & from, size_t n)
{
	bytearray::const_iterator itEnd = from + n;

	if (itEnd > end())
		itEnd = end();

	if (from >= begin() && from < itEnd) {
		size_t pos = from - begin();
		size_t sz  = itEnd - from;
		base_class::erase(pos, sz);
	}

	return *this;
}


// TODO need to implement without using the standard stream library
bytearray & bytearray::setNumber (long_t n, int base)
{
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this = os.str();
	return *this;
}

// TODO need to implement without using the standard stream library
bytearray & bytearray::setNumber (ulong_t n, int base)
{
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this = os.str();
	return *this;
}

bytearray & bytearray::setNumber (double n, char f, int prec)
{
	char fmt[32];
	char num[64];
	if (prec)
		PFS_ASSERT(::sprintf(fmt, "%%.%d%c", prec, f) > 0);
	else
		PFS_ASSERT(::sprintf(fmt, "%%%c", f) > 0);

	PFS_ASSERT(::sprintf(num, fmt, n) > 0);
	// TODO need to implement without using the standard stream library
	base_class::assign(num);// TODO need to implement without using the standard stream library
	return *this;
}


static long_t __str_to_long_helper(const char *s, bool * pok, int base, long_t min_val, long_t max_val)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
#ifdef CWT_HAVE_INT64
	long_t r = strtoll(s, & endptr, base);
#else
	long_t r = strtol(s, & endptr, base);
#endif

	if ((errno == ERANGE && (r == PFS_LONG_MAX || r == PFS_LONG_MIN))
			|| (errno != 0 && r == (long_t)0)
			|| endptr == s
			|| *endptr != '\0'
			|| r < min_val || r > max_val) {
		r = (long_t)0;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}


static ulong_t	__str_to_ulong_helper(const char *s, bool * pok, int base, ulong_t max_val)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
#ifdef HAVE_INT64
	ulong_t r = strtoull(s, & endptr, base);
#else
	ulong_t r = strtoul(s, & endptr, base);
#endif

	if ((errno == ERANGE && r == PFS_ULONG_MAX)
			|| (errno != 0 && r == (ulong_t)0)
			|| endptr == s
			|| *endptr != '\0'
			|| r > max_val ) {
		r = (ulong_t)0;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}


static double __str_to_double (const char * s, bool * pok)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
	double r = strtod(s, & endptr);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = (double)0.0f;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}

static float __str_to_float (const char * s, bool *pok)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
	float r = strtof(s, & endptr);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = (float)0.0f;
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}

double bytearray::toDouble (bool * ok) const
{
	return __str_to_double(c_str(), ok);
}

float	 bytearray::toFloat (bool * ok) const
{
	return __str_to_float(c_str(), ok);
}

long_t bytearray::toLong (bool * ok, int base) const
{
	return __str_to_long_helper(c_str(), ok, base, PFS_LONG_MIN, PFS_LONG_MAX);
}

ulong_t bytearray::toULong (bool * ok, int base) const
{
	return __str_to_ulong_helper(c_str(), ok, base, PFS_ULONG_MAX);
}

int_t bytearray::toInt (bool * ok, int base) const
{
	return int_t(__str_to_long_helper(c_str(), ok, base, long_t(PFS_INT_MIN), long_t(PFS_INT_MAX)));
}

uint_t bytearray::toUInt (bool * ok, int base) const
{
	return uint_t(__str_to_ulong_helper(c_str(), ok, base, ulong_t(PFS_UINT_MAX)));
}

short_t	 bytearray::toShort  (bool * ok, int base) const
{
	return short_t(__str_to_long_helper(c_str(), ok, base, long_t(PFS_SHORT_MIN), long_t(PFS_SHORT_MAX)));
}

ushort_t bytearray::toUShort (bool * ok, int base) const
{
	return ushort_t(__str_to_ulong_helper(c_str(), ok, base, ulong_t(PFS_USHORT_MAX)));
}

sbyte_t bytearray::toSByte (bool * ok, int base) const
{
	return sbyte_t(__str_to_long_helper(c_str(), ok, base, long_t(PFS_SBYTE_MIN), long_t(PFS_SBYTE_MAX)));
}

byte_t bytearray::toByte (bool * ok, int base) const
{
	return byte_t(__str_to_ulong_helper(c_str(), ok, base, ulong_t(PFS_BYTE_MAX)));
}


bytearray bytearray::fromBase64 (const bytearray & base64)
{
    unsigned int buf = 0;
    int nbits = 0;
    bytearray tmp((base64.size() * 3) / 4, 0);

    int offset = 0;
    for (size_t i = 0; i < base64.size(); ++i) {
		int ch = base64.charAt(i);
		int d;

		if (ch >= 'A' && ch <= 'Z')
			d = ch - 'A';
		else if (ch >= 'a' && ch <= 'z')
			d = ch - 'a' + 26;
		else if (ch >= '0' && ch <= '9')
			d = ch - '0' + 52;
		else if (ch == '+')
			d = 62;
		else if (ch == '/')
			d = 63;
		else
			d = -1;

		if (d != -1) {
			buf = (buf << 6) | d;
			nbits += 6;
			if (nbits >= 8) {
				nbits -= 8;
				tmp[offset++] = buf >> nbits;
				buf &= (1 << nbits) - 1;
			}
		}
    }

    tmp.resize(offset);
    return tmp;
}

bytearray bytearray::toBase64 () const
{
    const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
		            "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    const char padchar = '=';
    int padlen = 0;
    bytearray tmp;
    size_t sz = size();

    tmp.reserve((sz * 4) / 3 + 3);

    size_t i = 0;
    const char * d = constData();

    while (i < sz) {
		int chunk = 0;
			chunk |= int(uchar_t(d[i++])) << 16;
		if (i == sz) {
			padlen = 2;
		} else {
			chunk |= int(uchar_t(d[i++])) << 8;

			if (i == sz)
				padlen = 1;
			else
				chunk |= int(uchar_t(d[i++]));
		}

		int j = (chunk & 0x00fc0000) >> 18;
		int k = (chunk & 0x0003f000) >> 12;
		int l = (chunk & 0x00000fc0) >> 6;
		int m = (chunk & 0x0000003f);

		tmp.append(1, alphabet[j]);
		tmp.append(1, alphabet[k]);
		if (padlen > 1)
			tmp.append(1, padchar);
		else
			tmp.append(1, alphabet[l]);
		if (padlen > 0)
			tmp.append(1, padchar);
		else
			tmp.append(1, alphabet[m]);
    }

    return tmp;
}

} // pfs
