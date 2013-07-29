/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   May 15, 2013 4:37:44 PM
 *
 * @brief
 */


#include "bytearray_p.hpp"
#include "../include/cwt/hash.hpp"
//#include <cstdlib>
//#include <cerrno>
//#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdarg>

CWT_NS_BEGIN

CWT_PIMPL_IMPL_COPYABLE(ByteArray);

DLL_API uint_t hash_func(const ByteArray & key, uint_t seed)
{
	return hash_bytes(reinterpret_cast<const byte_t *>(key.constData()), key.size(), seed);
}


ByteArray::ByteArray() : pimpl(new ByteArray::Impl())
{
}

ByteArray::ByteArray(const char * s) : pimpl(new ByteArray::Impl(s))
{
}

ByteArray::ByteArray(const char * s, size_t size) : pimpl(new ByteArray::Impl(s, size))
{
}

ByteArray::ByteArray(size_t size, char ch) : pimpl(new ByteArray::Impl(size, ch))
{
}

const char * ByteArray::data() const
{
	return pimpl->data();
}

const char * ByteArray::c_str() const
{
	return pimpl->c_str();
}

size_t ByteArray::size() const
{
	return pimpl->size();
}

char & ByteArray::charAt(size_t pos)
{
	CWT_ASSERT(pos < length());
	return pimpl->at(pos);
}

const char & ByteArray::charAt (size_t pos) const
{
	CWT_ASSERT(pos < length());
	return pimpl->at(pos);
}


void ByteArray::clear()
{
	detach();
	pimpl->clear();
}

bool ByteArray::isEmpty() const
{
	return pimpl->empty();
}

ByteArray & ByteArray::insert (const char * s, size_t n, const const_iterator & pos)
{
	detach();
	const_iterator it(pos);
	if (it > end())
		it = end();
	if (it < begin())
		it = begin();

	size_t i = it.distance(begin());
	pimpl->insert(i, s, n);
	return *this;
}

ByteArray::iterator ByteArray::begin()
{
	detach();
	return iterator(pimpl->data());
}

ByteArray::iterator ByteArray::end()
{
	detach();
	return iterator(pimpl->data() + pimpl->size());
}

ByteArray::const_iterator ByteArray::begin() const
{
	return iterator(pimpl->data());
}

ByteArray::const_iterator ByteArray::end() const
{
	return iterator(pimpl->data() + pimpl->size());
}


int ByteArray::compare (const ByteArray & s) const
{
	return pimpl->compare(s.constData());
}

int ByteArray::compare (size_t pos, size_t len, const ByteArray & s) const
{
	return pimpl->compare(pos, len, s.constData());
}

int ByteArray::compare (size_t pos, size_t len, const ByteArray & s, size_t subpos, size_t sublen) const
{
	return pimpl->compare(pos, len, s.constData(), subpos, sublen);
}

int ByteArray::compare (const char * s) const
{
	return pimpl->compare(s);
}

int ByteArray::compare (size_t pos, size_t len, const char * s) const
{
	return pimpl->compare(pos, len, s);
}

int ByteArray::compare (size_t pos, size_t len, const char * s, size_t n) const
{
	return pimpl->compare(pos, len, s, n);
}

bool ByteArray::endsWith(const char * s, size_t n) const
{
	size_t pimpl_size = pimpl->size();
	if (pimpl_size < n)
		return false;
	return pimpl->compare(pimpl_size - n, n, s, n) == 0;
}

bool ByteArray::startsWith(const char * s, size_t n) const
{
	size_t pimpl_size = pimpl->size();

	if (pimpl_size < n)
		return false;
	return pimpl->compare(0, n, s, n) == 0;
}


ByteArray::const_iterator ByteArray::find(const char * s, size_t pos, size_t n) const
{
	if (pos >= length())
		return end();

	size_t find_pos = pimpl->find(s, pos, n);

	if (find_pos == ByteArray::Impl::npos)
		return end();

	return ByteArray::const_iterator(data() + find_pos);
}

ByteArray::const_iterator ByteArray::find(const ByteArray & s, const_iterator from) const
{
	return find(s.constData(), from.distance(begin()), s.size());
}

void ByteArray::reserve (size_t n)
{
	detach();
	pimpl->reserve(n);
}

void ByteArray::resize(size_t size)
{
	detach();
	pimpl->resize(size);
}

ByteArray ByteArray::substr(size_t pos, size_t n) const
{
	ByteArray ba;
	*ba.pimpl = pimpl->substr(pos, n);
	return ba;
}

ByteArray ByteArray::substr(const const_iterator & from) const
{
	ByteArray ba;
	*ba.pimpl = pimpl->substr(from.distance(begin()), length());
	return ba;
}

ByteArray ByteArray::substr(const const_iterator & from, size_t n) const
{
	ByteArray ba;
	*ba.pimpl = pimpl->substr(from.distance(begin()), n);
	return ba;
}

// TODO need to implement without using the standard stream library
ByteArray & ByteArray::setNumber (long_t n, int base)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this->pimpl = os.str();
	return *this;
}

// TODO need to implement without using the standard stream library
ByteArray & ByteArray::setNumber (ulong_t n, int base)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base) << n;
	*this->pimpl = os.str();
	return *this;
}

ByteArray & ByteArray::setNumber (double n, char f, int prec)
{
	detach();
	char fmt[32];
	char num[64];
	if (prec)
		CWT_ASSERT(::sprintf(fmt, "%%.%d%c", prec, f) > 0);
	else
		CWT_ASSERT(::sprintf(fmt, "%%%c", f) > 0);

	CWT_ASSERT(::sprintf(num, fmt, n) > 0);
	// TODO need to implement without using the standard stream library
	this->pimpl->assign(num);// TODO need to implement without using the standard stream library
	return *this;
}

ByteArray & ByteArray::sprintf(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	detach();
	ByteArray::vsprintf(cformat, args);
	va_end(args);
	return *this;
}


// FIXME Need implementation based on FSM
ByteArray & ByteArray::vsprintf(const char *cformat, va_list ap)
{
	char buf[4096];
	int n = ::vsprintf(buf, cformat, ap);
	CWT_ASSERT(n >= 0);
	*this = ByteArray(buf, size_t(n));
	return *this;
}


bool operator != (const ByteArray &s1, const ByteArray &s2)
{
	return *s1.pimpl != *s2.pimpl;
}

bool operator < (const ByteArray &s1, const ByteArray &s2)
{
	return *s1.pimpl < *s2.pimpl;
}

bool operator <= (const ByteArray &s1, const ByteArray &s2)
{
	return *s1.pimpl <= *s2.pimpl;
}

bool operator == (const ByteArray &s1, const ByteArray &s2)
{
	return *s1.pimpl == *s2.pimpl;
}

bool operator > (const ByteArray &s1, const ByteArray &s2)
{
	return *s1.pimpl > *s2.pimpl;
}

bool operator >= (const ByteArray &s1, const ByteArray &s2)
{
	return *s1.pimpl >= *s2.pimpl;
}

ByteArray ByteArray::fromBase64 (const ByteArray & base64)
{
    unsigned int buf = 0;
    int nbits = 0;
    ByteArray tmp((base64.size() * 3) / 4, 0);

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

ByteArray ByteArray::toBase64 () const
{
    const char alphabet[] = "ABCDEFGH" "IJKLMNOP" "QRSTUVWX" "YZabcdef"
		            "ghijklmn" "opqrstuv" "wxyz0123" "456789+/";
    const char padchar = '=';
    int padlen = 0;
    size_t sz = size();

    ByteArray tmp((sz * 4) / 3 + 3, 0);

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


CWT_NS_END
