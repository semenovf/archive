/*
 * utf8string.hpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_UTF8STRING_HPP__
#define __PFS_UTF8STRING_HPP__

#include <pfs.hpp>
#include <pfs/bits/utf8string_iterator.hpp>
#include <pfs/vector.hpp>
#include <pfs/bytearray.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/utility.hpp>
#include <ostream>

#define _u8(s) pfs::utf8string::fromUtf8(s)

namespace pfs {

class ucchar_ref;
class ucchar_ptr;

class DLL_API utf8string
{
	friend class ucchar_ref;
	friend class ucchar_ptr;

private:
// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

	class impl;
	shared_ptr<impl> _pimpl;

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

	void detach ();

public:
	utf8string (const utf8string & other) : _pimpl(other._pimpl) {}
	utf8string & operator = (const utf8string & other)
	{
		_pimpl = other._pimpl;
		return *this;
	}

public:
    typedef utf8string_iterator<utf8string_ptr> const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
	void updateLength();
	int compare (const_iterator begin, size_t len, const char * s, size_t subpos, size_t sublen) const;
	vector<utf8string> split (bool isOneSeparatorChar, const utf8string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;

public:
	utf8string ();
	explicit utf8string (const char * latin1);
	explicit utf8string (const char * latin1, size_t size);
	explicit utf8string (size_t count, char latin1);
	explicit utf8string (const_iterator begin, const_iterator end);
	explicit utf8string (size_t count, ucchar c);

#define NO_UTF8_CHECK true
#define UTF8_CHECK    false

	explicit utf8string (bool no_ut8_check, const char * utf8);
	explicit utf8string (bool no_ut8_check, const char * utf8, size_t size);

	const char * data  () const;
	const char * constData () const { return data(); }
	const char * c_str () const;
	const char * utf8  () const { return c_str(); }

	size_t length  () const { return size(); }
	size_t size    () const;
	bool   isEmpty () const { return size() == 0; }
	void   clear   ();

	ucchar charAt (size_t pos) const { utf8string_ptr p(this->data()); return p[pos]; }
	ucchar operator [] (size_t pos) const { return charAt(pos); }

    const_iterator begin () const { return const_iterator(*this); }
    const_iterator end   () const;
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }
    const_reverse_iterator rbegin  () const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend    () const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crbegin () const { return rbegin(); }
    const_reverse_iterator crend   () const { return rend(); }

	utf8string & insert  (size_t pos, const utf8string & s);
	utf8string & insert  (size_t pos, size_t n, ucchar ch) { return insert(pos, utf8string(n, ch)); }
	utf8string & append  (const utf8string & s) { return insert(length(), s); }
	utf8string & append  (const char * latin1, size_t size) { return append(utf8string(latin1, size)); }
	utf8string & append  (const char * latin1)  { return append(utf8string(latin1)); }
	utf8string & append  (size_t n, ucchar ch)  { return append(utf8string(n, ch)); }
	utf8string & prepend (const utf8string & s) { return insert(0, s); }
	utf8string & prepend (size_t n, ucchar ch)  { return insert(0, utf8string(n, ch)); }

	void swap (utf8string & other);

	utf8string & setNumber (long_t n, int base = 10, bool uppercase = false);
	utf8string & setNumber (ulong_t n, int base = 10, bool uppercase = false);
	utf8string & setNumber (int_t n, int base = 10, bool uppercase = false)    { return setNumber(long_t(n), base, uppercase); }
	utf8string & setNumber (uint_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
	utf8string & setNumber (short_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
	utf8string & setNumber (ushort_t n, int base = 10, bool uppercase = false) { return setNumber(ulong_t(n), base, uppercase); }
	utf8string & setNumber (sbyte_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
	utf8string & setNumber (byte_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
	//utf8string & setNumber (float n, char f = 'g', int prec = 6)               { return setNumber(double(n), f, prec); }
	utf8string & setNumber (double n, char f = 'g', int prec = 6);

	utf8string substr (const const_iterator & begin, size_t n) const;
	utf8string substr (const const_iterator & begin, const const_iterator & end) const;
	utf8string substr (const const_iterator & begin) const { return substr(begin, length()); }
	utf8string substr (size_t pos) const                   { return substr(pos, length()); }
	utf8string substr (size_t pos, size_t n) const         { return substr(begin() + pos, n); }
	utf8string mid    (size_t pos, size_t n) const         { return substr(pos, n); }
	utf8string left   (size_t n) const                     { return substr(0, n); }
	utf8string right  (size_t n) const                     { return substr(length() - n, n); }

	double	 toDouble (bool * ok = 0) const;
	//float	 toFloat  (bool * ok = 0) const;
	long_t   toLong   (bool * ok = 0, int base = 10) const;
	ulong_t	 toULong  (bool * ok = 0, int base = 10) const;
	int_t	 toInt    (bool * ok = 0, int base = 10) const;
	uint_t	 toUInt   (bool * ok = 0, int base = 10) const;
	short_t	 toShort  (bool * ok = 0, int base = 10) const;
	ushort_t toUShort (bool * ok = 0, int base = 10) const;
	sbyte_t  toSByte  (bool * ok = 0, int base = 10) const;
	byte_t	 toByte   (bool * ok = 0, int base = 10) const;
	vector<uint16_t> toUtf16 () const;
	vector<ucchar>   toUnicode() const;

	utf8string toLower () const;
	utf8string toUpper () const;

    int compare (const utf8string & s) const;
    int compare (size_t pos, size_t len, const utf8string & s) const;
    int compare (size_t pos, size_t len, const utf8string & s, size_t subpos, size_t sublen) const;
    int compare (const char * s) const;
    int compare (size_t pos, size_t len, const char * s) const;
    int compare (size_t pos, size_t len, const char * s, size_t n) const;

	bool contains (const utf8string & s) const     { return find(s, begin()) != end(); }
	bool contains (ucchar ch) const                { return find(utf8string(1, ch), begin()) != end(); }
	bool contains (const char * s) const;
	bool contains (const char * s, size_t n) const { return find(s, 0, n) != end(); }

	const_iterator find (const utf8string & s, const_iterator from) const;
	const_iterator find (const utf8string & s) const { return find(s, begin()); }
	const_iterator find (const char * s, size_t from, size_t n) const;
	const_iterator find (const char * s, size_t from) const;

	bool endsWith (const utf8string & s) const;
	bool endsWith (const char * s) const;
	bool endsWith (const char * s, size_t n) const;

	bool startsWith (const utf8string & s, const_iterator from) const;
	bool startsWith (const utf8string & s) const { return startsWith(s, cbegin()); }
	bool startsWith (const char * s) const;
	bool startsWith (const char * s, size_t n) const;

	utf8string ltrim () const;
	utf8string rtrim () const;
	utf8string trim  () const;

	utf8string & remove (size_t pos) { return remove(pos, length()); }
	utf8string & remove (size_t pos, size_t n);

	utf8string & replace (size_t pos, size_t len, const utf8string & str, size_t subpos, size_t sublen);
	utf8string & replace (size_t pos, size_t len, const utf8string & str, size_t subpos);
	utf8string & replace (size_t pos, size_t len, const utf8string & str);
	utf8string & replace (size_t pos, size_t len, size_t n, ucchar c);

	utf8string & replace (const utf8string & before, const utf8string & after);

	void reserve (size_t n);

	vector<utf8string> split (const utf8string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;
	vector<utf8string> split (const ucchar & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;
	vector<utf8string> splitOneOf (const utf8string & separators, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;

	utf8string & operator += (const utf8string & other) { return append(other); }
	utf8string & operator += (const char * latin1) { return append(utf8string(latin1)); }
	utf8string & operator += (ucchar ch) { return append(1, ch); }
	utf8string & operator += (char latin1) { return append(1, ucchar(latin1)); }

	utf8string & operator << (const utf8string & other) { return append(other); }
	utf8string & operator << (const char * latin1) { return append(utf8string(latin1)); }
	utf8string & operator << (ucchar ch) { return append(1, ch); }
	utf8string & operator << (char latin1) { return append(1, ucchar(latin1)); }

public:
	struct ConvertState
	{
		ConvertState() : nremain(0), invalidChars(0), replacementChar(ucchar::ReplacementChar) {}
		size_t nremain;
		size_t invalidChars;
		ucchar replacementChar;
	};

	static utf8string fromLatin1 (const char * latin1, size_t size, ConvertState * state = nullptr);
	static utf8string fromLatin1 (const char * latin1, ConvertState * state = nullptr);
	static utf8string fromLatin1 (const pfs::bytearray & latin1, ConvertState * state = nullptr)
		{ return fromLatin1(latin1.data(), latin1.size(), state); }

	static utf8string fromUtf8 (const char * utf8, size_t size, ConvertState * state = nullptr);
	static utf8string fromUtf8 (const char * utf8, ConvertState * state = nullptr);
	static utf8string fromUtf8 (const bytearray & utf8, ConvertState * state = nullptr)
		{ return fromUtf8(utf8.data(), utf8.size(), state);	}

	utf8string fromUtf16 (const uint16_t * utf16, size_t size, ConvertState * state = nullptr);

	static utf8string number (double n, char fmt = 'g', int prec = 6);
	//static utf8string number (float n, char fmt = 'g', int prec = 6);
	static utf8string number (int_t n, int base = 10, bool uppercase = false);
	static utf8string number (uint_t n, int base = 10, bool uppercase = false);
	static utf8string number (long_t n, int base = 10, bool uppercase = false);
	static utf8string number (ulong_t n, int base = 10, bool uppercase = false);

	friend utf8string operator + (const utf8string & s1, const utf8string & s2);
	friend utf8string operator + (const utf8string & s, const char * latin1) { return s + utf8string(latin1); }
	friend utf8string operator + (const utf8string & s, ucchar ch) { return s + utf8string(1, ch); }
	friend utf8string operator + (const utf8string & s, char latin1) { return s + utf8string(1, ucchar(latin1)); }

	friend bool	operator != (const utf8string & s1, const utf8string & s2);
	friend bool	operator <  (const utf8string & s1, const utf8string & s2);
	friend bool	operator <= (const utf8string & s1, const utf8string & s2);
	friend bool	operator == (const utf8string & s1, const utf8string & s2);
	friend bool	operator >  (const utf8string & s1, const utf8string & s2);
	friend bool	operator >= (const utf8string & s1, const utf8string & s2);
};

template <typename _P>
inline utf8string_iterator<_P>::utf8string_iterator(const utf8string & s)
	: _p(s.data())
{
}

inline vector<utf8string>
utf8string::split(const utf8string & separator, bool keepEmpty, ucchar quoteChar) const
{
	return split(false, separator, keepEmpty, quoteChar);
}

inline vector<utf8string>
utf8string::split(const ucchar & separator, bool keepEmpty, ucchar quoteChar) const
{
	return split(false, utf8string(1, separator), keepEmpty, quoteChar);
}

inline vector<utf8string>
utf8string::splitOneOf(const utf8string & separators, bool keepEmpty, ucchar quoteChar) const
{
	return split(true, utf8string(separators), keepEmpty, quoteChar);
}

inline utf8string utf8string::number (double n, char fmt, int prec)
{
	return utf8string().setNumber(n, fmt, prec);
}

/*
inline utf8string utf8string::number (float n, char fmt, int prec)
{
	return utf8string().setNumber(n, fmt, prec);
}
*/

inline utf8string utf8string::number (int_t n, int base, bool uppercase)
{
	return utf8string().setNumber(n, base, uppercase);
}

inline utf8string utf8string::number (uint_t n, int base, bool uppercase)
{
	return utf8string().setNumber(n, base, uppercase);
}

inline utf8string utf8string::number (long_t n, int base, bool uppercase)
{
	return utf8string().setNumber(n, base, uppercase);
}

inline utf8string utf8string::number (ulong_t n, int base, bool uppercase)
{
	return utf8string().setNumber(n, base, uppercase);
}

/*
inline utf8string & utf8string::replace (const_iterator i1, const_iterator i2, const utf8string & str, size_t sublen)
{
	return this->replace(i1, i2, str, sublen, str.length());
}

inline utf8string & utf8string::replace (const_iterator i1, const_iterator i2, const utf8string & str)
{
	return this->replace(i1, i2, str, str.length());
}
*/

/*
inline utf8string & utf8string::replace (size_t pos, size_t len, const utf8string & str, size_t subpos, size_t sublen)
{
	detach();
	const_iterator i1 = cbegin() + pos;
	const_iterator i2 = i1 + len;
	return this->replace(i1, i2, str, subpos, sublen);
}
*/

inline utf8string & utf8string::replace (size_t pos, size_t len, const utf8string & str, size_t subpos)
{
	return this->replace(pos, len, str, subpos, str.length());
}

inline utf8string & utf8string::replace (size_t pos, size_t len, const utf8string & str)
{
	return this->replace(pos, len, str, 0, str.length());
}

/*
inline utf8string & utf8string::replace (const_iterator i1, const_iterator i2, size_t n, ucchar c)
{
	return this->replace(i1, i2, utf8string(n, c));
}
*/

inline utf8string & utf8string::replace (size_t pos, size_t len, size_t n, ucchar c)
{
	return this->replace(pos, len, utf8string(n, c));
}

inline std::ostream & operator << (std::ostream & os, const utf8string & o)
{
	os << o.c_str();
	return os;
}

inline utf8string operator + (const utf8string & s1, const utf8string & s2)
{
	utf8string s(s1);
	s.append(s2);
	return s;
}

inline bool operator != (const utf8string & s1, const utf8string & s2)
{
	return s1.compare(s2) != 0;
}

inline bool operator < (const utf8string & s1, const utf8string & s2)
{
	return s1.compare(s2) < 0;
}

inline bool operator <= (const utf8string & s1, const utf8string & s2)
{
	return s1.compare(s2) <= 0;
}

inline bool operator == (const utf8string & s1, const utf8string & s2)
{
	return s1.compare(s2) == 0;
}

inline bool operator > (const utf8string & s1, const utf8string & s2)
{
	return s1.compare(s2) > 0;
}

inline bool operator >= (const utf8string & s1, const utf8string & s2)
{
	return s1.compare(s2) >= 0;
}

} // pfs

#endif /* __PFS_UTF8STRING_HPP__ */
