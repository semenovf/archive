/*
 * utf8string.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#include "utf8string_p.hpp"
#include <stdlib.h>
#include <cerrno>
#include <sstream>
#include <iomanip>

CWT_NS_BEGIN

CWT_PIMPL_IMPL_COPYABLE(Utf8String);

Utf8String::Utf8String() : pimpl(new Utf8String::Impl())
{
}

Utf8String::Utf8String(const char *latin1) : pimpl(new Utf8String::Impl())
{
	append(latin1, strlen(latin1));
}

Utf8String::Utf8String(const char *latin1, size_t length) : pimpl(new Utf8String::Impl())
{
	append(latin1, length);
}

Utf8String::Utf8String(size_t count, char latin1) : pimpl(new Utf8String::Impl())
{
	for (size_t i = 0; i < count; ++i) {
		if (latin1 < 127)
			pimpl->append(1, latin1);
		else
			pimpl->append(1, ReplacementChar);
	}
}

bool Utf8String::isEmpty() const
{
	return pimpl->empty();
}

/**
 *
 * @brief Returns string size in unicode code points (UChar).
 *
 * @return size of string in unicode code points (bytes).
 */
size_t Utf8String::length() const
{
	size_t r = 0;
	const_iterator it = begin();
	const_iterator itEnd = end();

	while (it++ < itEnd)
		++r;

	return r;
}


/**
 * @brief Returns string size in code units (bytes).
 *
 * @return size of string in code units (bytes).
 */
size_t Utf8String::size() const
{
	return pimpl->size();
}

Utf8String& Utf8String::append(const Utf8String & s)
{
	detach();
	pimpl->append(*s.pimpl);
	return *this;
}

void Utf8String::reserve (size_t n)
{
	detach();
	pimpl->reserve(n);
}

void Utf8String::resize (size_t n)
{
	detach();
	pimpl->resize(n);
}

void Utf8String::resize (size_t n, char c)
{
	detach();
	pimpl->resize(n, c);
}

Utf8String Utf8String::substr(const const_iterator & from, size_t n) const
{
	Utf8String r;
	Utf8String::const_iterator itEnd = from + n;

	if (itEnd > end())
		itEnd = end();

	if (from >= begin() && from < itEnd) {
		size_t pos = from.distance(begin());
		size_t len = itEnd.distance(from);
		*r.pimpl = pimpl->substr(pos, len);
	}

	return r;
}


Utf8String&	Utf8String::append(const char *latin1, int length)
{
	detach();
	const char *end = latin1 + length;

	while (latin1 < end) {
		if (*latin1 < 127)
			pimpl->append(1, *latin1);
		else
			pimpl->append(1, ReplacementChar);
		++latin1;
	}

	return *this;
}

Utf8String& Utf8String::append(const char *latin1)
{
	return append(latin1, strlen(latin1));
}


/**
 *
 * @param from   From position in unicode characters.
 * @param len    Length in unicode characters.
 * @param s      String to compare.
 * @param subpos Start position of @c s in bytes.
 * @param sublen Length in bytes of substring of @c s.
 * @return       A signed integral indicating the relation between the strings:
 * 			  0  They compare equal.
 *          < 0  Either the value of the first character that does not match
 * 				 is lower in the compared string, or all compared characters
 * 				 match but the compared string is shorter.
 * 		    > 0  Either the value of the first character that does not match
 * 		    	 is greater in the compared string, or all compared characters
 * 		    	 match but the compared string is longer.
 *
 */
int Utf8String::compare (const const_iterator & from, size_t len, const char * str, size_t subpos, size_t sublen) const
{
	CWT_ASSERT(from <= end());

	size_t nchars = length();
	size_t pos    = from.distance(begin());

	if (len > nchars - pos)
		len = nchars - pos;

	size_t pimpl_pos = from.distance(begin());
	size_t pimpl_len = begin().distance(begin() + pos + len);

	return pimpl->compare(pimpl_pos, pimpl_len, str, subpos, sublen);
}

int Utf8String::compare (const Utf8String & s) const
{
	return compare(begin(), length(), s.pimpl->data(), 0, s.size());
}

int Utf8String::compare (size_t pos, size_t len, const Utf8String & utf8) const
{
	return compare(begin() + pos, len, utf8.pimpl->data(), 0, utf8.size());
}

int Utf8String::compare (size_t pos, size_t len, const Utf8String & utf8, size_t subpos, size_t sublen) const
{
	size_t subnchars = utf8.length();
	CWT_ASSERT(subpos <= utf8.length());
	if (sublen > subnchars - subpos)
		sublen = subnchars - subpos;

	size_t pimpl_subpos = utf8.begin().distance(utf8.begin() + subpos);
	size_t pimpl_sublen = utf8.begin().distance(utf8.begin() + subpos + sublen);

	return compare(begin() + pos, len, utf8.pimpl->data(), pimpl_subpos, pimpl_sublen);
}

int Utf8String::compare (const char * str) const
{
	return compare(begin(), length(), str, 0, strlen(str));
}

int Utf8String::compare (size_t pos, size_t len, const char * str) const
{
	return compare(begin() + pos, len, str, 0, strlen(str));
}

int Utf8String::compare (size_t pos, size_t len, const char * str, size_t n) const
{
	return compare(begin() + pos, len, str, 0, n);
}


bool Utf8String::endsWith(const Utf8String & s) const
{
	return endsWith(s.data(), s.size());
}

bool Utf8String::endsWith(const char * s) const
{
	return endsWith(s, strlen(s));
}

bool Utf8String::endsWith(const char * s, size_t n) const
{
	size_t pimpl_size = pimpl->size();

	if (pimpl_size < n)
		return false;
	return pimpl->compare(pimpl_size - n, n, s, n) == 0;
}

bool Utf8String::startsWith(const Utf8String & s) const
{
	return startsWith(s.data(), s.size());
}

bool Utf8String::startsWith(const char * s) const
{
	return startsWith(s, strlen(s));
}

bool Utf8String::startsWith(const char * s, size_t n) const
{
	size_t pimpl_size = pimpl->size();

	if (pimpl_size < n)
		return false;
	return pimpl->compare(0, n, s, n) == 0;
}

UChar Utf8String::charAt(size_t pos) const
{
	if (pos >= length())
		return Unicode::Null;
	return (begin() + pos).value();
}

void Utf8String::clear()
{
	detach();
	pimpl->clear();
}

const char*	Utf8String::data() const
{
	return pimpl->data();
}

const char*	Utf8String::c_str() const
{
	return pimpl->c_str();
}


static long_t __str_to_long_helper(const char *s, bool *pok, int base, long_t min_val, long_t max_val)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
#ifdef CWT_HAS_INT64
	long_t r = strtoll(s, & endptr, base);
#else
	long_t r = strtol(s, & endptr, base);
#endif

	if ((errno == ERANGE && (r == CWT_LONG_MAX || r == CWT_LONG_MIN))
			|| (errno != 0 && r == long_t(0))
			|| endptr == s
			|| *endptr != '\0'
			|| r < min_val || r > max_val) {
		r = long_t(0);
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}


ulong_t	__str_to_ulong_helper(const char *s, bool *pok, int base, ulong_t max_val)
{
	bool ok = true;
	char *endptr = nullptr;

	errno = 0;
#ifdef CWT_HAS_INT64
	ulong_t r = strtoull(s, & endptr, base);
#else
	ulong_t r = strtoul(s, & endptr, base);
#endif

	if ((errno == ERANGE && r == CWT_ULONG_MAX)
			|| (errno != 0 && r == ulong_t(0))
			|| endptr == s
			|| *endptr != '\0'
			|| r > max_val ) {
		r = ulong_t(0);
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}

double Utf8String::toDouble(bool *pok) const
{
	bool ok = true;
	char *endptr = nullptr;

	const char * s = c_str();
	errno = 0;
	double r = strtod(s, & endptr);

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

float Utf8String::toFloat(bool *pok) const
{
	bool ok = true;
	char *endptr = nullptr;

	const char * s = c_str();
	errno = 0;
	float r = strtof(s, & endptr);

	if (errno == ERANGE
			|| endptr == s
			|| *endptr != '\0' ) {
		r = float(0);
		ok = false;
	}

	if (pok)
		*pok = ok;
	return r;
}

long_t Utf8String::toLong(bool *pok, int base) const
{
	return __str_to_long_helper(c_str(), pok, base, CWT_LONG_MIN, CWT_LONG_MAX);
}

ulong_t	Utf8String::toULong(bool *pok, int base) const
{
	return __str_to_ulong_helper(c_str(), pok, base, CWT_ULONG_MAX);
}

int_t Utf8String::toInt(bool *pok, int base) const
{
	return int_t(__str_to_long_helper(c_str(), pok, base, long_t(CWT_INT_MIN), long_t(CWT_INT_MAX)));
}

uint_t Utf8String::toUInt(bool *pok, int base) const
{
	return uint_t(__str_to_ulong_helper(c_str(), pok, base, ulong_t(CWT_UINT_MAX)));
}

short_t Utf8String::toShort(bool *pok, int base) const
{
	return int_t(__str_to_long_helper(c_str(), pok, base, long_t(CWT_SHORT_MIN), long_t(CWT_SHORT_MAX)));
}

ushort_t Utf8String::toUShort(bool *pok, int base) const
{
	return ushort_t(__str_to_ulong_helper(c_str(), pok, base, ulong_t(CWT_USHORT_MAX)));
}

sbyte_t Utf8String::toSByte(bool *pok, int base) const
{
	return sbyte_t(__str_to_long_helper(c_str(), pok, base, long_t(CWT_SBYTE_MIN), long_t(CWT_SBYTE_MAX)));
}

byte_t Utf8String::toByte(bool *pok, int base) const
{
	return byte_t(__str_to_ulong_helper(c_str(), pok, base, ulong_t(CWT_BYTE_MAX)));
}

// TODO need to implement without using the standard stream library
Utf8String & Utf8String::setNumber (long_t n, int base)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this->pimpl = os.str();
	return *this;
}

Utf8String & Utf8String::setNumber (ulong_t n, int base)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this->pimpl = os.str();
	return *this;
}

/**
 *
 * @param n
 * @param f    The format, can be 'f', 'F', 'e', 'E', 'g' or 'G'
 * @param prec
 * @return
 */
Utf8String & Utf8String::setNumber (double n, char f, int prec)
{
	detach();
	char fmt[16];
	char num[64];
	if (prec)
		CWT_ASSERT(sprintf(fmt, "%%.%d%c", prec, f) > 0);
	else
		CWT_ASSERT(sprintf(fmt, "%%%c", f) > 0);

	CWT_ASSERT(sprintf(num, fmt, n) > 0);

	this->pimpl->assign(num);
	return *this;
}


Utf8String operator + (const Utf8String &s1, const Utf8String &s2)
{
	Utf8String s(s1);
	s.append(s2);
	return s;
}

bool operator != (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl != *s2.pimpl;
}

bool operator < (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl < *s2.pimpl;
}

bool operator <= (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl <= *s2.pimpl;
}

bool operator == (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl == *s2.pimpl;
}

bool operator > (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl > *s2.pimpl;
}

bool operator >= (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl >= *s2.pimpl;
}


CWT_NS_END
