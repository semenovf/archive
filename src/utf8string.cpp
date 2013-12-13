/*
 * utf8string.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#include "utf8string_p.hpp"
#include <pfs/utility.hpp>
#include <cstring>
#include <sstream>
#include <iomanip>

namespace pfs {

utf8string::utf8string ()
	: _pimpl()
{
}

utf8string::utf8string (const char * latin1)
	: _pimpl(new utf8string::impl())
{
	if (latin1) {
		utf8string d(fromLatin1(latin1, strlen(latin1)));
		this->swap(d);
	}
}

utf8string::utf8string (const char * latin1, size_t size)
	: _pimpl(new utf8string::impl())
{
	if (latin1 && size > 0) {
		utf8string d(fromLatin1(latin1, size));
		this->swap(d);
	}
}

utf8string::utf8string (size_t count, char latin1)
	: _pimpl(new utf8string::impl())
{
	if (byte_t(latin1) > 127)
		latin1 = '?';

	utf8string::impl s(count, latin1);
	utf8string d(fromLatin1(s.data(), count));
	this->swap(d);
}

utf8string::utf8string (const_iterator begin, const_iterator end)
	: _pimpl(new utf8string::impl())
{
	if (begin < end) {
		_pimpl->append(begin.base()->ptr(), end.distance(begin));
		updateLength();
	}
}


utf8string::utf8string (size_t count, ucchar c)
	: _pimpl(new utf8string::impl())
{
	char utf8[7];

	if (!ucchar::isValid(c))
		c = ucchar(ucchar::ReplacementChar);

	ssize_t sz = c.encodeUtf8(utf8);
	_pimpl->reserve(_pimpl->size() + count * 4);

	for (size_t i = 0; i < count; ++i) {
		_pimpl->append(utf8, size_t(sz));
	}

	updateLength();
}

/*
utf8string::utf8string (bool no_ut8_check, const char * utf8)
	: _pimpl(new utf8string::impl())
{
	if (no_ut8_check) {
		_pimpl->append(utf8, strlen(utf8));
		updateLength();
	} else {
		utf8string d(fromUtf8(utf8, strlen(utf8)));
		this->swap(d);
	}
}

utf8string::utf8string (bool no_ut8_check, const char * utf8, size_t size)
	: _pimpl(new utf8string::impl())
{
	if (no_ut8_check) {
		_pimpl->append(utf8, size);
		updateLength();
	} else {
		utf8string d(fromUtf8(utf8, size));
		this->swap(d);
	}
}
*/

void utf8string::updateLength ()
{
	utf8string::const_iterator it(cbegin());
	utf8string::const_iterator itEnd = cend();
	_pimpl->_length = 0;

	while (it ++ < itEnd)
		++_pimpl->_length;
}

void utf8string::swap (utf8string & other)
{
	pfs::swap(_pimpl, other._pimpl);
}


const char * utf8string::data () const
{
	return _pimpl->data();
}

const char * utf8string::c_str () const
{
	return _pimpl->c_str();
}

size_t utf8string::sizeInBytes () const
{
	return _pimpl->size();
}

size_t utf8string::size () const
{
	return _pimpl->_length;
}

bool utf8string::isNull () const
{
	return _pimpl.get() == nullptr;
}

void utf8string::clear ()
{
	detach();
	_pimpl->clear();
	_pimpl->_length = 0;
}

utf8string::iterator utf8string::end ()
{
	return iterator(ucchar_ptr::end(*this));
}


utf8string::const_iterator utf8string::end () const
{
	return const_iterator(ucchar_const_ptr::end(*this));
}

/**
 * @fn size_t utf8string::length() const
 *
 * @brief Returns string size in unicode code points.
 *
 * @return size of string in unicode code points.
 */

/**
 * @fn size_t utf8string::size() const
 *
 * @brief Returns string size in unicode code points.
 *
 * @return size of string in unicode code points.
 */

/**
 * @brief Inserts string after @c pos position.
 *
 * @param s
 * @param pos
 * @return
 */
utf8string & utf8string::insert (size_t offset, const utf8string & s)
{
	detach();

	const_iterator it(cbegin() + offset);

	if (it > cend())
		it = cend();
	if (it < cbegin())
		it = cbegin();

	size_t pos = it.distance(cbegin());
	_pimpl->insert(pos, s._pimpl->data(), s._pimpl->size());
	_pimpl->_length += s._pimpl->_length;
	return *this;
}

// TODO need to implement without using the standard stream library ?
utf8string & utf8string::setNumber (long_t n, int base, bool uppercase)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base);
	if (uppercase) {
		os << std::uppercase;
	}
	os << n;
	_pimpl->assign(os.str());
	updateLength();
	return *this;
}

utf8string & utf8string::setNumber (ulong_t n, int base, bool uppercase)
{
	detach();
	std::ostringstream os;
	os << std::setbase(base);
	if (uppercase) {
		os << std::uppercase;
	}
	os << n;
	_pimpl->assign(os.str());
	updateLength();
	return *this;
}

/**
 *
 * @param n
 * @param f    The format, can be 'f', 'F', 'e', 'E', 'g' or 'G'
 * @param prec
 * @return
 */
utf8string & utf8string::setNumber (double n, char f, int prec)
{
	detach();
	char fmt[32];
	char num[64];
	if (prec)
		PFS_ASSERT(::sprintf(fmt, "%%.%d%c", prec, f) > 0);
	else
		PFS_ASSERT(::sprintf(fmt, "%%%c", f) > 0);

	PFS_ASSERT(::sprintf(num, fmt, n) > 0);

	_pimpl->assign(num);
	updateLength();
	return *this;
}


double utf8string::toDouble(bool * ok) const
{
	return bytearray(c_str()).toDouble(ok);
}

long_t utf8string::toLong(bool * ok, int base) const
{
	return bytearray(c_str()).toLong(ok, base);
}

ulong_t	utf8string::toULong(bool * ok, int base) const
{
	return bytearray(c_str()).toULong(ok, base);
}

int_t utf8string::toInt(bool * ok, int base) const
{
	return bytearray(c_str()).toInt(ok, base);
}

uint_t utf8string::toUInt(bool * ok, int base) const
{
	return bytearray(c_str()).toUInt(ok, base);
}

short_t utf8string::toShort(bool * ok, int base) const
{
	return bytearray(c_str()).toShort(ok, base);
}

ushort_t utf8string::toUShort(bool * ok, int base) const
{
	return bytearray(c_str()).toUShort(ok, base);
}

sbyte_t utf8string::toSByte(bool * ok, int base) const
{
	return bytearray(c_str()).toSByte(ok, base);
}

byte_t utf8string::toByte(bool * ok, int base) const
{
	return bytearray(c_str()).toByte(ok, base);
}

static const uint32_t halfBase = uint32_t(0x0010000);
static const uint32_t halfMask = uint32_t(0x3FF);
static const int halfShift  = 10; /* used for shifting by 10 bits */

vector<uint16_t> utf8string::toUtf16 () const
{
	vector<uint16_t> r;

	const_iterator it = cbegin();
	const_iterator itEnd = cend();

	while (it != itEnd) {
		ucchar ch = *it;

		if (ch <= ucchar::MaxBMP) { /* Target is a character <= 0xFFFF */
			/* UTF-16 surrogate values are illegal in UTF-32 */
			if (ch >= ucchar::HiSurrogateStart && ch <= ucchar::LowSurrogateEnd) {
				ch = ucchar::ReplacementChar;
			}
			r.append(uint16_t(ch));
		} else if (ch > ucchar::MaxCodePoint) {
			ch = ucchar::ReplacementChar;
			r.append(uint16_t(ch));
		} else {
			ch -= halfBase;
			r.append(uint16_t((ch >> halfShift) + ucchar::HiSurrogateStart));
			r.append(uint16_t((ch & halfMask) + ucchar::LowSurrogateStart));
		}

		++it;
	}
	r.append(uint16_t(0));
	return r;
}


vector<ucchar> utf8string::toUnicode() const
{
	vector<ucchar> r;

	const_iterator it = cbegin();
	const_iterator itEnd = cend();

	while (it != itEnd) {
		r.append(*it++);
	}
	return r;
}

utf8string utf8string::substr(const const_iterator & from, size_t n) const
{
	utf8string r;
	utf8string::const_iterator itEnd(from + n);

	if (itEnd > end())
		itEnd = end();

	if (from >= begin() && from < itEnd) {
		size_t pos = from.distance(begin());
		size_t sz  = itEnd.distance(from);
		r._pimpl->assign(_pimpl->substr(pos, sz));
		r.updateLength();
	}

	return r;
}


utf8string utf8string::substr(const const_iterator & begin, const const_iterator & end) const
{
	const_iterator b(begin);
	const_iterator e(end);

	if (b < cbegin())
		b = cbegin();
	if (e > cend())
		e = cend();

	size_t pos = b.distance(cbegin());
	size_t sz  = b.distance(e);

	utf8string r;
	r._pimpl->assign(_pimpl->substr(pos, sz));
	r.updateLength();
	return r;
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
int utf8string::compare (const_iterator from, size_t len, const char * str, size_t subpos, size_t sublen) const
{
	PFS_ASSERT(from <= end());
	const_iterator to(from + len);

	if (to > end())
		to = end();

	size_t pos = from.distance(begin());
	size_t n = to.distance(from);
	return _pimpl->compare(pos, n, str, subpos, sublen);
}

int utf8string::compare (const utf8string & s) const
{
	return compare(begin(), length(), s.data(), 0, s._pimpl->size());
}

int utf8string::compare (size_t pos, size_t len, const utf8string & utf8) const
{
	return compare(begin() + pos, len, utf8.data(), 0, utf8._pimpl->size());
}

/**
 *
 * @param pos    From position in unicode characters.
 * @param len    Length in unicode characters.
 * @param utf8   String to compare.
 * @param subpos @c utf8's start position in unicode characters.
 * @param sublen Length in unicode characters of substring of @c utf8.
 * @return       A signed integral indicating the relation between the strings:
 * 			  0  They compare equal.
 *          < 0  Either the value of the first character that does not match
 * 				 is lower in the compared string, or all compared characters
 * 				 match but the compared string is shorter.
 * 		    > 0  Either the value of the first character that does not match
 * 		    	 is greater in the compared string, or all compared characters
 * 		    	 match but the compared string is longer.
 */
int utf8string::compare (size_t pos, size_t len, const utf8string & utf8, size_t subpos, size_t sublen) const
{
	PFS_ASSERT(subpos <= utf8._pimpl->_length);
	const_iterator from(utf8.begin() + subpos);
	const_iterator to(from + sublen);

	if (to > utf8.end())
		to = utf8.end();

	return compare (begin() + pos, len
			, utf8.data()
			, from.distance(utf8.begin())
			, to.distance(from));
}

int utf8string::compare (const char * str) const
{
	return compare(begin(), length(), str, 0, strlen(str));
}

int utf8string::compare (size_t pos, size_t len, const char * str) const
{
	return compare(begin() + pos, len, str, 0, strlen(str));
}

int utf8string::compare (size_t pos, size_t len, const char * str, size_t n) const
{
	return compare(begin() + pos, len, str, 0, n);
}

bool utf8string::contains (const char * s) const
{
	return find(s, 0, strlen(s)) != end();
}

/**
 * @brief Searches the string for the first occurrence of the sequence
 *
 * @param s   Another string with the subject to search for.
 * @param pos Position of the first character in the string to be considered in the search.
 * @param n   Length of sequence of characters to match.
 * @return
 */
utf8string::const_iterator utf8string::find (const char * s, size_t pos, size_t n) const
{
	if (pos >= length())
		return end();

	size_t off = (begin() + pos).distance(begin());
	off = _pimpl->find(s, off, n);

	if (off == _pimpl->npos)
		return end();


	return utf8string::const_iterator(ucchar_const_ptr::middle(*this, off));
}

utf8string::const_iterator utf8string::find (const utf8string & s, utf8string::const_iterator pos) const
{
	PFS_ASSERT(pos >= begin());

	size_t off = pos.distance(begin());
	off = _pimpl->find(s.constData(), off, s._pimpl->size());

	if (off == _pimpl->npos)
		return end();

	return utf8string::const_iterator(ucchar_const_ptr::middle(*this, off));
}

utf8string::const_iterator utf8string::find (const char * s, size_t from) const
{
	return find(s, from, strlen(s));
}


bool utf8string::endsWith (const utf8string & s) const
{
	return endsWith(s.data(), s._pimpl->size());
}

bool utf8string::endsWith (const char * s) const
{
	return endsWith(s, strlen(s));
}

bool utf8string::endsWith (const char * s, size_t n) const
{
	size_t size = _pimpl->size();
	return size < n ? false : _pimpl->compare(size - n, n, s, n) == 0;
}

bool utf8string::startsWith (const char * s) const
{
	return startsWith(s, strlen(s));
}

bool utf8string::startsWith (const char * s, size_t n) const
{
	size_t size = _pimpl->size();
	return size < n ? false : _pimpl->compare(0, n, s, n) == 0;
}

bool utf8string::startsWith (const utf8string & s, const_iterator from) const
{
	PFS_ASSERT(from >= begin() && from < end());
	size_t off = from.distance(cbegin());
	return _pimpl->compare(off, s._pimpl->size(), s.constData(), s._pimpl->size()) == 0;
}

utf8string utf8string::ltrim () const
{
	const_iterator it = cbegin();
	const_iterator itEnd = cend();

	while (it != itEnd && it->value().isSpace())
		++it;

	if (it != itEnd) {
		return substr(it);
	}

	return *this;
}

utf8string utf8string::rtrim () const
{
	const_reverse_iterator itr = crbegin();
	const_reverse_iterator itrEnd = crend();

	if (itr != itrEnd && *itr == ucchar::Null) // skip null-terminator
		++itr;

	while (itr != itrEnd && (*itr).value().isSpace())
		++itr;

	if (itr != itrEnd) {
		return substr(cbegin(), itr.base());
	}

	return *this;
}

utf8string utf8string::trim () const
{
	const_iterator it = cbegin();
	const_iterator itEnd = cend();

	while (it != itEnd && it->value().isSpace())
		++it;

	const_reverse_iterator itr = crbegin();
	const_reverse_iterator itrEnd = crend();

	if (itr != itrEnd && *itr == ucchar::Null) // skip null-terminator
		++itr;

	while (itr != itrEnd && (*itr).value().isSpace())
		++itr;

	if (itr != itrEnd) {
		return substr(it, itr.base());
	}
	return *this;
}

utf8string & utf8string::replace (size_t pos, size_t len, const utf8string & str, size_t subpos, size_t sublen)
{
	detach();

	const_iterator i1 = cbegin() + pos;
	const_iterator i2 = i1 + len;

	PFS_ASSERT(i1 >= cbegin() && i1 <= cend());
	PFS_ASSERT(i2 >= cbegin() && i2 <= cend());
	size_t off  = i1.distance(cbegin());
	size_t size = i2.distance(i1);

	const_iterator str_i1 = str.cbegin() + subpos;
	const_iterator str_i2 = str_i1 + sublen;
	if (str_i2 > str.cend())
		str_i2 = str.cend();
	PFS_ASSERT(str_i1 >= str.cbegin() && str_i1 <= str.cend());
	PFS_ASSERT(str_i2 >= str.cbegin() && str_i2 <= str.cend());
	size_t str_off  = str_i1.distance(str.cbegin());
	size_t str_size = str_i2.distance(str_i1);

	_pimpl->replace(off, size, *str._pimpl, str_off, str_size);
	this->updateLength();
	return *this;
}

/**
 * @brief Replaces every occurrence of the string @c before with the string
 *        @c after and returns a reference to this string.
 * @param before Replaceable string.
 * @param after Replacement string.
 * @return
 */
utf8string & utf8string::replace (const utf8string & before, const utf8string & after)
{
	detach();
	utf8string r;
	const_iterator it1 = cbegin();
	const_iterator it2 = find(before, it1);
	size_t beforeLength = before.length();

	// nothing to replace
	while (it2 != cend()) {
		r.append(utf8string(it1, it2));
		r.append(after);

		it1 = it2;
		it1 += beforeLength;
		it2 = find(before, it1);
	}


	if (it1 != cend()) {
		r.append(utf8string(it1, cend()));
	}

	if (!r.isEmpty()) {
		this->swap(r);
	}

	return *this;
}

void utf8string::reserve (size_t n)
{
	detach();
	_pimpl->reserve(n * 6);
}

utf8string & utf8string::remove (size_t pos, size_t n)
{
	detach();

	if (pos < length()) {
		n = pos + n > length() ? length() - pos : n;
		const_iterator it(cbegin() + pos);
		size_t off = it.distance(cbegin());
		size_t sz  = it.distance((it + n));
		_pimpl->erase(off, sz);
		updateLength();
	}

	return *this;
}

vector<utf8string> utf8string::split(bool isOneSeparatorChar, const utf8string & separator, bool keepEmpty, ucchar quoteChar) const
{
	vector<utf8string> r;
	utf8string::const_iterator it = cbegin();
	utf8string::const_iterator itBegin = it;
	utf8string::const_iterator itEnd = cend();
	size_t separatorLength = separator.length();
	bool quote = false;

	while (it != itEnd) {
		if (quoteChar != ucchar::Null && *it == quoteChar) {
			if (quote) {
				quote = false;
			} else {
				quote = true;
			}
			++it;
		} else if (!quote
				&& ((isOneSeparatorChar && separator.contains(*it))
					|| (!isOneSeparatorChar && this->startsWith(separator, it)))) {
			utf8string s(itBegin, it);
			if (!s.isEmpty()) {
				r.append(s);
			} else {
				if (keepEmpty)
					r.append(s);
			}
			it += isOneSeparatorChar ? 1 : separatorLength;
			itBegin = it;
		} else {
			++it;
		}
	}

	if (itBegin != it || itBegin == itEnd) {
		utf8string s(itBegin, it);
		if (!s.isEmpty()) {
			r.append(s);
		} else {
			if (keepEmpty)
				r.append(s);
		}
	}

	if (r.isEmpty())
		r.append(*this);


	PFS_VERIFY_X(!quote, _Tr("Unbalanced quote"));
	if (quote)
		r.clear();

	return r;
}

utf8string utf8string::toLower () const
{
	utf8string r;
	const_iterator it = begin();
	const_iterator itEnd = end();

	while (it < itEnd) {
		r.append(utf8string(1, it->value().toLower()));
		++it;
	}
	return r;
}

utf8string utf8string::toUpper () const
{
	utf8string r;
	const_iterator it = begin();
	const_iterator itEnd = end();

	while (it < itEnd) {
		r.append(utf8string(1, it->value().toUpper()));
		++it;
	}
	return r;
}


} // pfs
