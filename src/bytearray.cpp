/**
 * @file   bytearray_exp.cpp
 * @author wladt
 * @date   May 15, 2013 4:37:44 PM
 *
 * @brief
 */

#include "../include/pfs/bytearray.hpp"
#include "../include/pfs/string.hpp"
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <iomanip>

namespace pfs {

const bytearray::char_type bytearray::TerminatorChar('\0');
const bytearray::char_type bytearray::EndOfLineChar('\n');

bytearray::bytearray (const char * s)
	: base_class(s, strlen(s))
{
	bytearray_terminator bt(this);
}

bytearray::bytearray (const char * s, size_t size)
	: base_class(s, size)
{
	bytearray_terminator bt(this);
}

bytearray::bytearray (const std::string & s)
	: base_class(s.c_str(), s.size())
{
	bytearray_terminator bt(this);
}

bytearray::bytearray (size_t size, const char & c)
	: base_class(size, c)
{
	bytearray_terminator bt(this);
}

static long_t __str_to_long_helper (const char * s, bool * pok, int base, long_t min_val, long_t max_val)
{
	PFS_ASSERT(s);
	bool ok = true;
	char * endptr = nullptr;

	errno = 0;
	long_t r = strtolong(s, & endptr, base);

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


static ulong_t	__str_to_ulong_helper (const char * s, bool * pok, int base, ulong_t max_val)
{
	PFS_ASSERT(s);
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
	ulong_t r = strtoulong(s, & endptr, base);

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

extern "C" double vim_strtod ( const char * string
    , char ** endPtr
    , char decimalPoint);

#define pfs_strtod vim_strtod

static double __str_to_double (const char * s, bool * pok, char decimalPoint)
{
	PFS_ASSERT(s);
	bool ok = true;
	char * endptr = nullptr;

	errno = 0;
	double r = pfs_strtod(s, & endptr, decimalPoint);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = double(0);
		ok = false;
	}

	if (pok)
		*pok = ok;

	return r;
}

double bytearray::toDouble (bool * ok, char decimalPoint) const
{
	return __str_to_double(isNull() ? "" : c_str(), ok, decimalPoint);
}

/*
float	 bytearray::toFloat (bool * ok) const
{
	return __str_to_float(c_str(), ok);
}
*/

long_t bytearray::toLong (bool * ok, int base) const
{
	return __str_to_long_helper(isNull() ? "" : c_str(), ok, base, PFS_LONG_MIN, PFS_LONG_MAX);
}

ulong_t bytearray::toULong (bool * ok, int base) const
{
	return __str_to_ulong_helper(isNull() ? "" : c_str(), ok, base, PFS_ULONG_MAX);
}

int_t bytearray::toInt (bool * ok, int base) const
{
	return int_t(__str_to_long_helper(isNull() ? "" : c_str(), ok, base, long_t(PFS_INT_MIN), long_t(PFS_INT_MAX)));
}

uint_t bytearray::toUInt (bool * ok, int base) const
{
	return uint_t(__str_to_ulong_helper(isNull() ? "" : c_str(), ok, base, ulong_t(PFS_UINT_MAX)));
}

short_t	 bytearray::toShort  (bool * ok, int base) const
{
	return short_t(__str_to_long_helper(isNull() ? "" : c_str(), ok, base, long_t(PFS_SHORT_MIN), long_t(PFS_SHORT_MAX)));
}

ushort_t bytearray::toUShort (bool * ok, int base) const
{
	return ushort_t(__str_to_ulong_helper(isNull() ? "" : c_str(), ok, base, ulong_t(PFS_USHORT_MAX)));
}

sbyte_t bytearray::toSByte (bool * ok, int base) const
{
	return sbyte_t(__str_to_long_helper(isNull() ? "" : c_str(), ok, base, long_t(PFS_SBYTE_MIN), long_t(PFS_SBYTE_MAX)));
}

byte_t bytearray::toByte (bool * ok, int base) const
{
	return byte_t(__str_to_ulong_helper(isNull() ? "" : c_str(), ok, base, ulong_t(PFS_BYTE_MAX)));
}


// TODO need to implement without using the standard stream library
void bytearray::setNumber (long_t n, int base)
{
	bytearray_terminator bt(this);
	_d.detach();
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this = os.str();
}

// TODO need to implement without using the standard stream library
void bytearray::setNumber (ulong_t n, int base)
{
	bytearray_terminator bt(this);
	_d.detach();
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this = os.str();
}

void bytearray::setNumber (double n, char f, int prec)
{
	bytearray_terminator bt(this);
	_d.detach();
	char fmt[32];
	char num[64];
	if (prec)
		PFS_ASSERT(::sprintf(fmt, "%%.%d%c", prec, f) > 0);
	else
		PFS_ASSERT(::sprintf(fmt, "%%%c", f) > 0);

	PFS_ASSERT(::sprintf(num, fmt, n) > 0);
	// TODO need to implement without using the standard stream library
	*this = bytearray(num);
}


bytearray bytearray::fromBase64 (const bytearray & base64)
{
    unsigned int buf = 0;
    int nbits = 0;
    bytearray tmp((base64.size() * 3) / 4, 0);

    int offset = 0;
    for (size_t i = 0; i < base64.size(); ++i) {
		int ch = base64.at(i);
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

template <>
bytearray bytearray::toBytes<pfs::string> (const pfs::string & v, endian::type_enum /*order*/)
{
	return bytearray(v.constData(), v.sizeInBytes());
}

/*
DLL_API uint_t hash_func(const bytearray & key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t *>(key.constData()), key.size(), seed);
}
*/



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

bytearray bytearray::substr (size_t pos, size_t n) const
{
	pos = size() > pos ? pos : size();
	n = size() - pos > n ? n : size() - pos;

	return n
			? bytearray(this->constData() + pos, n)
			: bytearray();
}

bytearray bytearray::substr (const const_iterator & from, size_t n) const
{
	return (from >= cbegin() && from < cend())
			? substr(from - cbegin(), n)
			: bytearray();
}


#ifdef __COMMENT__
bytearray::const_iterator bytearray::find (const char * s, size_t pos, size_t n) const
{
	if (pos >= length())
		return end();

	size_t find_pos = _d.cast<impl>()->find(s, pos, n);

	if (find_pos == _d.cast<impl>()->npos)
		return end();

	return bytearray::const_iterator(begin() + find_pos);
}

bytearray::const_iterator bytearray::find (const char * s, size_t pos) const
{
	return find(s, pos, strlen(s));
}

bytearray & bytearray::remove (const const_iterator & from, size_t n)
{
	bytearray::const_iterator itEnd = from + n;

	if (itEnd > end())
		itEnd = end();

	if (from >= begin() && from < itEnd) {
		size_t pos = from - begin();
		size_t sz  = itEnd - from;

		_d.detach();
		_d.cast<impl>()->erase(pos, sz);
	}

	return *this;
}


#endif

} // pfs
