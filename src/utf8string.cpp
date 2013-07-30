/*
 * utf8string.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#include "utf8string_p.hpp"
#include "../include/cwt/string.h"
//#include <stdlib.h>
//#include <cerrno>
#include <sstream>
#include <iomanip>
#include <cstdarg>

CWT_NS_BEGIN

CWT_PIMPL_IMPL_COPYABLE(Utf8String);

Utf8String::Utf8String() : pimpl(new Utf8String::Impl())
{
}

Utf8String::Utf8String(const char *latin1) : pimpl(new Utf8String::Impl())
{
	pimpl->m_length = strlen(latin1);
	for (size_t i = 0; i < pimpl->m_length; ++i) {
		if(byte_t(latin1[i]) > 127)
			pimpl->append(1, ReplacementChar);
		else
			pimpl->append(1, latin1[i]);
	}
}

Utf8String::Utf8String(const char *latin1, size_t length) : pimpl(new Utf8String::Impl())
{
	pimpl->m_length = length;
	for (size_t i = 0; i < pimpl->m_length; ++i) {
		if(byte_t(latin1[i]) > 127)
			pimpl->append(1, ReplacementChar);
		else
			pimpl->append(1, latin1[i]);
	}
}

Utf8String::Utf8String(size_t count, char latin1) : pimpl(new Utf8String::Impl())
{
	if (byte_t(latin1) > 127)
		latin1 = ReplacementChar;
	pimpl->append(count, latin1);
	pimpl->m_length = count;
}

Utf8String::Utf8String(const const_iterator & begin, const const_iterator & end)
	: pimpl(new Utf8String::Impl())
{
	if (begin < end) {
		pimpl->append(begin.m_entry.cursor, begin.distance(end));
		calculateLength();
	}
}

Utf8String::Utf8String(size_t count, UChar c)
{
	char utf8[6];

	if (!Unicode::isValid(c))
		c = UChar(Utf8String::ReplacementChar);

	ssize_t sz = encodeUcs4(utf8, 6, c);
	CWT_ASSERT(sz > 0);

	pimpl->reserve(pimpl->size() + count * 4);

	for (size_t i = 0; i < count; ++i) {
		pimpl->append(utf8, size_t(sz));
	}

	pimpl->m_length = count;
}

bool Utf8String::isEmpty() const
{
	return pimpl->m_length == 0;
}

/**
 *
 * @brief Returns string size in unicode code points (UChar).
 *
 * @return size of string in unicode code points (bytes).
 */
size_t Utf8String::length() const
{
	return pimpl->m_length;
}


size_t Utf8String::length(const const_iterator & begin, const const_iterator & end) const
{
	size_t r = 0;
	const_iterator it = begin;

	while (it++ < end)
		++r;

	return r;
}

size_t Utf8String::calculateLength()
{
	pimpl->m_length = length(begin(), end());
	return pimpl->m_length;
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

void Utf8String::reserve (size_t n)
{
	detach();
	pimpl->reserve(n * 6);
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
		r.calculateLength();
	}

	return r;
}

Utf8String & Utf8String::insert(const Utf8String & s, const Utf8String::const_iterator & pos)
{
	detach();
	const_iterator it(pos);
	if (it > end())
		it = end();
	if (it < begin())
		it = begin();

	size_t i = it.distance(begin());
	pimpl->insert(i, s.data(), s.size());
	pimpl->m_length += s.length();
	return *this;
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
	CWT_ASSERT(pos < length());
	return (begin() + pos).value();
}

void Utf8String::clear()
{
	detach();
	pimpl->clear();
	pimpl->m_length = 0;
}

const char*	Utf8String::data() const
{
	return pimpl->data();
}

const char*	Utf8String::c_str() const
{
	return pimpl->c_str();
}



double Utf8String::toDouble(bool *ok) const
{
	return cwt_str_to_double(c_str(), ok);
}

float Utf8String::toFloat(bool *pok) const
{
	return cwt_str_to_float(c_str(), pok);
}

long_t Utf8String::toLong(bool *pok, int base) const
{
	return cwt_str_to_long(c_str(), pok, base);
}

ulong_t	Utf8String::toULong(bool *pok, int base) const
{
	return cwt_str_to_ulong(c_str(), pok, base);
}

int_t Utf8String::toInt(bool *pok, int base) const
{
	return cwt_str_to_int(c_str(), pok, base);
}

uint_t Utf8String::toUInt(bool *pok, int base) const
{
	return cwt_str_to_uint(c_str(), pok, base);
}

short_t Utf8String::toShort(bool *pok, int base) const
{
	return cwt_str_to_short(c_str(), pok, base);
}

ushort_t Utf8String::toUShort(bool *pok, int base) const
{
	return cwt_str_to_ushort(c_str(), pok, base);
}

sbyte_t Utf8String::toSByte(bool *pok, int base) const
{
	return cwt_str_to_sbyte(c_str(), pok, base);
}

byte_t Utf8String::toByte(bool *pok, int base) const
{
	return cwt_str_to_byte(c_str(), pok, base);
}


// FIXME need full support of Unicode standard
Utf8String Utf8String::toLower () const
{
	Utf8String r;
	const_iterator it = begin();
	const_iterator itEnd = end();

	while (it < itEnd) {
		UChar uc = *it;
		if (uc < 128) {
			if (char(uc) >= 'A' && char(uc) <= 'Z')
				uc += 0x20;
			r.append(1, char(uc));
		} else {
			r.append(1, uc);
		}
		++it;
	}
	return r;
}

// FIXME need full support of Unicode standard
Utf8String Utf8String::toUpper () const
{
	Utf8String r;
	const_iterator it = begin();
	const_iterator itEnd = end();

	while (it < itEnd) {
		UChar uc = *it;
		if (uc < 128) {
			if (char(uc) >= 'a' && char(uc) <= 'z')
				uc -= 0x20;
			r.append(1, char(uc));
		} else {
			r.append(1, uc);
		}
		++it;
	}
	return r;
}


// TODO need to implement without using the standard stream library
Utf8String & Utf8String::setNumber (long_t n, int base, bool uppercase)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base);
	if (uppercase) {
		os << std::uppercase;
	}
	os << n;
	*this->pimpl = os.str();
	calculateLength();
	return *this;
}

Utf8String & Utf8String::setNumber (ulong_t n, int base, bool uppercase)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base);
	if (uppercase) {
		os << std::uppercase;
	}
	os << n;
	*this->pimpl = os.str();
	calculateLength();
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
	char fmt[32];
	char num[64];
	if (prec)
		CWT_ASSERT(::sprintf(fmt, "%%.%d%c", prec, f) > 0);
	else
		CWT_ASSERT(::sprintf(fmt, "%%%c", f) > 0);

	CWT_ASSERT(::sprintf(num, fmt, n) > 0);

	this->pimpl->assign(num);
	calculateLength();
	return *this;
}

Utf8String & Utf8String::sprintf(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	detach();
	Utf8String::vsprintf(cformat, args);
	va_end(args);
	return *this;
}


// FIXME Need implementation based on FSM
Utf8String & Utf8String::vsprintf(const char *cformat, va_list ap)
{
	char buf[4096];
	int n = ::vsprintf(buf, cformat, ap);
	CWT_ASSERT(n >= 0);
	*this = Utf8String(buf, size_t(n));
	return *this;
}


Utf8String operator + (const Utf8String & s1, const Utf8String & s2)
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
