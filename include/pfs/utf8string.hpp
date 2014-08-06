/*
 * utf8string.hpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_UTF8STRING_HPP__
#define __PFS_UTF8STRING_HPP__

#include "bits/utf8string_iterator.hpp"
#include "bits/stringlist_basic.hpp"
#include "bits/cast_traits.hpp"
#include "vector.hpp"
#include "bytearray.hpp"
#include "shared_ptr.hpp"
#include "utility.hpp"
#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

// FIXME need to check behavior of member functions under null string

namespace pfs {

class DLL_API utf8string
{
	template <typename _Str>
	friend class ucchar_ref_basic;

	template <typename _Str, typename _Ref>
	friend class ucchar_ptr_basic;

	friend class ucchar_ref;
	friend class ucchar_const_ref;
	friend class ucchar_ptr;
	friend class ucchar_const_ptr;

private:

	class impl;
	shared_ptr<impl> _pimpl;

	void detach ();

public:
	typedef const char * const_codeunit_pointer;
	typedef char * codeunit_pointer;
	typedef ucchar char_type;
	typedef ucchar item_type;
    typedef utf8string_iterator<ucchar_ptr> iterator;
    typedef utf8string_iterator<ucchar_const_ptr> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static const char_type EndOfLineChar; // TODO Need to refuse

private:
	void updateLength ();
	int compare (const_iterator begin, size_t len, const std::string & str/*const char * s*/, size_t subpos, size_t sublen) const;
	stringlist_basic<utf8string> split (bool isOneSeparatorChar, const utf8string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;

public:
	utf8string ();
	explicit utf8string (const char * latin1);
	explicit utf8string (const char * latin1, size_t size);
	explicit utf8string (size_t count, char latin1);
	explicit utf8string (const_iterator begin, const_iterator end);
	explicit utf8string (size_t count, ucchar c);

	utf8string (const utf8string & other) : _pimpl(other._pimpl) {}
	utf8string & operator = (const utf8string & other)
	{
		_pimpl = other._pimpl;
		return *this;
	}

/* In any case need to update length of the resulting string, so below
   constructors are meaningless:

	explicit utf8string (bool no_ut8_check, const char * utf8);
	explicit utf8string (bool no_ut8_check, const char * utf8, size_t size);
*/

	const char * data  () const;
	const char * constData () const { return data(); }
	const char * c_str () const;
	const char * utf8  () const { return c_str(); }

	size_t sizeInBytes () const;
	size_t sizeInCodeUnits () const { return sizeInBytes(); }
	size_t size    () const;
	size_t length  () const { return size(); }
	static size_t length (const iterator & begin, const iterator & end)
	{
		return utf8string(begin, end).length();
	}
	static size_t length (const const_iterator & begin, const const_iterator & end)
	{
		return utf8string(begin, end).length();
	}

	bool   isNull  () const;
	bool   isEmpty () const { return size() == 0; }
	void   clear   ();

	ucchar charAt (size_t pos) const { return cbegin()[pos].value(); }
	ucchar operator [] (size_t pos) const { return charAt(pos); }

    iterator begin () { return iterator(*this); }
    iterator end   ();
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

	double	 toDouble (bool * ok = 0, char decimalPoint = '.') const;
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

	stringlist_basic<utf8string> split (const utf8string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;
	stringlist_basic<utf8string> split (const ucchar & separator, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;
	stringlist_basic<utf8string> splitOneOf (const utf8string & separators, bool keepEmpty = true, ucchar quoteChar = ucchar::Null) const;

	utf8string & operator += (const utf8string & other) { return append(other); }
	utf8string & operator += (const char * latin1) { return append(utf8string(latin1)); }
	utf8string & operator += (const ucchar & ch) { return append(utf8string(1, ch)); }
//	utf8string & operator += (char ch) { return append(utf8string(1, ch)); }

	utf8string & operator << (const utf8string & other) { return append(other); }
	utf8string & operator << (const char * latin1) { return append(utf8string(latin1)); }
	utf8string & operator << (const ucchar & ch) { return append(utf8string(1, ch)); }
	//utf8string & operator << (char ch) { return append(utf8string(1, ch)); }

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

	static utf8string fromUtf16 (const uint16_t * utf16, size_t size, ConvertState * state = nullptr);

	static utf8string number (double n, char fmt = 'g', int prec = 6);
	//static utf8string number (float n, char fmt = 'g', int prec = 6);
	static utf8string number (int_t n, int base = 10, bool uppercase = false);
	static utf8string number (uint_t n, int base = 10, bool uppercase = false);
	static utf8string number (long_t n, int base = 10, bool uppercase = false);
	static utf8string number (ulong_t n, int base = 10, bool uppercase = false);
#ifndef PFS_OS_64BITS
	static utf8string number (long n, int base = 10, bool uppercase = false) { return number(long_t(n), base, uppercase); }
	static utf8string number (unsigned long n, int base = 10, bool uppercase = false) { return number(ulong_t(n), base, uppercase); }
#endif

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

	friend bool	operator != (const char * s1, const utf8string & s2);
	friend bool	operator <  (const char * s1, const utf8string & s2);
	friend bool	operator <= (const char * s1, const utf8string & s2);
	friend bool	operator == (const char * s1, const utf8string & s2);
	friend bool	operator >  (const char * s1, const utf8string & s2);
	friend bool	operator >= (const char * s1, const utf8string & s2);

	friend bool	operator != (const utf8string & s1, const char * s2);
	friend bool	operator <  (const utf8string & s1, const char * s2);
	friend bool	operator <= (const utf8string & s1, const char * s2);
	friend bool	operator == (const utf8string & s1, const char * s2);
	friend bool	operator >  (const utf8string & s1, const char * s2);
	friend bool	operator >= (const utf8string & s1, const char * s2);
};

/*
template <typename _P>
inline utf8string_iterator<_P>::utf8string_iterator(utf8string & s)
	: _p(s)
{
}
*/

inline stringlist_basic<utf8string>
utf8string::split(const utf8string & separator, bool keepEmpty, ucchar quoteChar) const
{
	return split(false, separator, keepEmpty, quoteChar);
}

inline stringlist_basic<utf8string>
utf8string::split(const ucchar & separator, bool keepEmpty, ucchar quoteChar) const
{
	return split(false, utf8string(1, separator), keepEmpty, quoteChar);
}

inline stringlist_basic<utf8string>
utf8string::splitOneOf(const utf8string & separators, bool keepEmpty, ucchar quoteChar) const
{
	return split(true, utf8string(separators), keepEmpty, quoteChar);
}

inline utf8string utf8string::number (double n, char fmt, int prec)
{
	return utf8string().setNumber(n, fmt, prec);
}

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

template <typename _Int>
inline utf8string stringify (_Int n, int base, bool uppercase = false)
{
	return utf8string().setNumber(n, base, uppercase);
}

template <typename _Int>
inline utf8string stringify (_Int n)
{
	return utf8string().setNumber(n, 10, false);
}

template <>
inline utf8string stringify (const utf8string & s)
{
	return utf8string(s);
}

template <>
inline utf8string stringify (const bytearray & s)
{
	return utf8string(s.c_str());
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

inline bool operator != (const char * s1, const utf8string & s2)
{
	return s2.compare(s1) != 0;
}

inline bool operator < (const char * s1, const utf8string & s2)
{
	return s2.compare(s1) > 0;
}

inline bool operator <= (const char * s1, const utf8string & s2)
{
	return s2.compare(s1) >= 0;
}

inline bool operator == (const char * s1, const utf8string & s2)
{
	return s2.compare(s1) == 0;
}

inline bool operator > (const char * s1, const utf8string & s2)
{
	return s2.compare(s1) < 0;
}

inline bool operator >= (const char * s1, const utf8string & s2)
{
	return s2.compare(s1) <= 0;
}

inline bool operator != (const utf8string & s1, const char * s2)
{
	return s1.compare(s2) != 0;
}

inline bool operator < (const utf8string & s1, const char * s2)
{
	return s1.compare(s2) < 0;
}

inline bool operator <= (const utf8string & s1, const char * s2)
{
	return s1.compare(s2) <= 0;
}

inline bool operator == (const utf8string & s1, const char * s2)
{
	return s1.compare(s2) == 0;
}

inline bool operator > (const utf8string & s1, const char * s2)
{
	return s1.compare(s2) > 0;
}

inline bool operator >= (const utf8string & s1, const char * s2)
{
	return s1.compare(s2) >= 0;
}

} // pfs


namespace pfs {

template <> inline bool cast_trait<bool, pfs::utf8string> (const pfs::utf8string & v)
{
	return v.isEmpty() ? false
		: (v == "false" ? false : true);
}

template <> inline int cast_trait<int, pfs::utf8string> (const pfs::utf8string & v) { return int(v.toInt()); }
template <> inline long cast_trait<long, pfs::utf8string> (const pfs::utf8string & v) { return long(v.toLong()); }

#ifdef HAVE_LONGLONG
template <> inline long long cast_trait<long long, pfs::utf8string> (const pfs::utf8string & v) { return (long long)v.toLong(); }
#endif

template <> inline float cast_trait<float, pfs::utf8string> (const pfs::utf8string & v) { return float(v.toDouble()); }
template <> inline double cast_trait<double, pfs::utf8string> (const pfs::utf8string & v) { return v.toDouble(); }

template <> inline pfs::utf8string cast_trait<pfs::utf8string, bool> (const bool & v) { return v ? pfs::utf8string("true") : pfs::utf8string("false"); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, char> (const char & v) { return pfs::utf8string(1, v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, unsigned char> (const unsigned char & v) { return pfs::utf8string(1, (const char)v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, short> (const short & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, unsigned short> (const unsigned short & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, int> (const int & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, unsigned int> (const unsigned int & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, long> (const long & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, unsigned long> (const unsigned long & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, float> (const float & v) { return pfs::utf8string::number(v); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, double> (const double & v) { return pfs::utf8string::number(v); }
#ifdef HAVE_LONGLONG
template <> inline pfs::utf8string cast_trait<pfs::utf8string, long long> (const long long & v) { return pfs::utf8string::number(long_t(v)); }
template <> inline pfs::utf8string cast_trait<pfs::utf8string, unsigned long long> (const unsigned long long & v) { return pfs::utf8string::number(ulong_t(v)); }
#endif
template <> inline pfs::utf8string cast_trait<pfs::utf8string, pfs::utf8string> (const pfs::utf8string & v) { return pfs::utf8string(v); }

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_UTF8STRING_HPP__ */
