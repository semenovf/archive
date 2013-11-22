/*
 * bytearray.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_BYTEARRAY_HPP__
#define __PFS_BYTEARRAY_HPP__

#include <pfs.hpp>
#include <string>
#include <ostream>

namespace pfs {

class DLL_API bytearray : private std::string
{
	typedef std::string base_class;
	bytearray (const base_class & other) : base_class(other) {}

public:
	typedef base_class::iterator iterator;
	typedef base_class::const_iterator const_iterator;

public:
	bytearray () : base_class() {}

	bytearray(const char * s) : base_class(s) {}
	bytearray(const char * s, size_t size) : base_class(s, size) {}
	bytearray(size_t size, char ch) : base_class(size, ch) {}

	const char * c_str() const { return base_class::c_str(); }
	const char * data() const  { return base_class::data(); }
	const char * constData() const { return data(); }
	char &       charAt(size_t pos) { PFS_ASSERT(pos < length()); return base_class::at(pos); }
	const char & charAt (size_t pos) const { PFS_ASSERT(pos < length()); return base_class::at(pos); }
	void         clear() { base_class::clear(); }
	bool	     isEmpty() const { return base_class::empty(); }
	size_t       size () const { return base_class::size(); }
	size_t       length () const { return size(); }

	bytearray & append  (const bytearray & s) { return insert(s, end()); }
	bytearray & append  (const char * s) { return insert(s, end()); }
	bytearray & append  (const char * s, size_t n) { return insert(s, n, end()); }
	bytearray & append  (size_t size, char ch) { append(bytearray(size, ch)); return *this; }
	bytearray & prepend (const bytearray & s) { return insert(s, begin()); }
	bytearray & prepend (const char * s) { return insert(s, begin()); }
	bytearray & prepend (const char * s, size_t n)  { return insert(s, n, begin()); }
	bytearray & prepend (size_t size, char ch) { prepend(bytearray(size, ch)); return *this; }
	bytearray & insert  (const bytearray & s, size_t pos) { return insert(s, begin() + pos); }
	bytearray & insert  (const bytearray & s, const const_iterator & pos) { return insert(s.constData(), s.size(), pos); }
	bytearray & insert  (const char * s, const const_iterator & pos);
	bytearray & insert  (const char * s, size_t n, const const_iterator & pos);
	bytearray & insert  (size_t size, char ch, size_t pos) { return insert(bytearray(size, ch), pos); }
	bytearray & insert  (size_t size, char ch, const const_iterator & pos) { return insert(bytearray(size, ch), pos); }

    iterator       begin () { return base_class::begin(); }
    iterator       end   () { return base_class::end(); }
    const_iterator begin () const { return base_class::begin(); }
    const_iterator end   () const { return base_class::end(); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }

    int compare (const bytearray & s) const;
    int compare (size_t pos, size_t len, const bytearray & s) const;
    int compare (size_t pos, size_t len, const bytearray & s, size_t subpos, size_t sublen) const;
    int compare (const char * s) const;
    int compare (size_t pos, size_t len, const char * s) const;
    int compare (size_t pos, size_t len, const char * s, size_t n) const;

	bool contains (const bytearray & s) const { return find(s, begin()) != end(); }
	bool contains (const char * s) const { return find(s, begin()) != end(); }
	bool contains (const char * s, size_t n) const { return find(s, 0, n) != end(); }

	bool endsWith (const bytearray & s) const { return endsWith(s.data(), s.size()); }
	bool endsWith (const char * s) const;
	bool endsWith (const char * s, size_t n) const;

	bool startsWith (const bytearray & s) const { return startsWith(s.data(), s.size()); }
	bool startsWith (const char * s) const;
	bool startsWith (const char * s, size_t n) const;

	const_iterator find(const bytearray & s, const_iterator pos) const;
	const_iterator find(const char * s, size_t pos, size_t n) const;
	const_iterator find(const char * s, size_t pos) const;

	bytearray & remove(size_t pos)                  { return remove(pos, length()); }
	bytearray & remove(size_t pos, size_t n)        { return remove(begin() + pos, n); }
	bytearray & remove(const const_iterator & from) { return remove(from, length()); }
	bytearray & remove(const const_iterator & from, size_t n);

	void reserve (size_t n = 0);
	void resize(size_t size);

	bytearray substr(size_t pos) const            { return substr(pos, length()); }
	bytearray substr(size_t pos, size_t n) const;
	bytearray substr(const const_iterator & from) const;
	bytearray substr(const const_iterator & from, size_t n) const;
	bytearray mid(size_t pos, size_t n) const     { return substr(pos, n); }
	bytearray left(size_t n) const                { return substr(0, n); }
	bytearray right(size_t n) const               { return substr(length() - n, n); }

	double	 toDouble (bool *ok = 0) const;
	float	 toFloat  (bool *ok = 0) const;
	long_t   toLong   (bool *ok = 0, int base = 10) const;
	ulong_t	 toULong  (bool *ok = 0, int base = 10) const;
	int_t	 toInt    (bool *ok = 0, int base = 10) const;
	uint_t	 toUInt   (bool *ok = 0, int base = 10) const;
	short_t	 toShort  (bool *ok = 0, int base = 10) const;
	ushort_t toUShort (bool *ok = 0, int base = 10) const;
	sbyte_t  toSByte  (bool *ok = 0, int base = 10) const;
	byte_t	 toByte   (bool *ok = 0, int base = 10) const;

	bytearray & setNumber (long_t n, int base = 10);
	bytearray & setNumber (ulong_t n, int base = 10);
	bytearray & setNumber (int_t n, int base = 10)    { return setNumber(long_t(n), base); }
	bytearray & setNumber (uint_t n, int base = 10)   { return setNumber(ulong_t(n), base); }
	bytearray & setNumber (short_t n, int base = 10)  { return setNumber(long_t(n), base); }
	bytearray & setNumber (ushort_t n, int base = 10) { return setNumber(ulong_t(n), base); }
	bytearray & setNumber (sbyte_t n, int base = 10)  { return setNumber(long_t(n), base); }
	bytearray & setNumber (byte_t n, int base = 10)   { return setNumber(ulong_t(n), base); }
	bytearray & setNumber (float n, char f = 'g', int prec = 6) { return setNumber(double(n), f, prec); }
	bytearray & setNumber (double n, char f = 'g', int prec = 6);

	bytearray & operator += (const bytearray & other) { return append(other); }
	char & operator [] (size_t pos) { return charAt(pos); }
	const char & operator [] (size_t pos) const { return charAt(pos); }

	friend bytearray operator + (const bytearray & s1, const bytearray & s2);

	friend bool	operator != (const bytearray & s1, const bytearray & s2);
	friend bool	operator <  (const bytearray & s1, const bytearray & s2);
	friend bool	operator <= (const bytearray & s1, const bytearray & s2);
	friend bool	operator == (const bytearray & s1, const bytearray & s2);
	friend bool	operator >  (const bytearray & s1, const bytearray & s2);
	friend bool	operator >= (const bytearray & s1, const bytearray & s2);

	friend bool	operator != (const bytearray & s1, const char * s2);
	friend bool	operator <  (const bytearray & s1, const char * s2);
	friend bool	operator <= (const bytearray & s1, const char * s2);
	friend bool	operator == (const bytearray & s1, const char * s2);
	friend bool	operator >  (const bytearray & s1, const char * s2);
	friend bool	operator >= (const bytearray & s1, const char * s2);

	static bytearray number (double n, char fmt = 'g', int prec = 6) { return bytearray().setNumber(n, fmt, prec); }
	static bytearray number (float n, char fmt = 'g', int prec = 6)  { return bytearray().setNumber(n, fmt, prec); }
	static bytearray number (int_t n, int base = 10)                 { return bytearray().setNumber(n, base); }
	static bytearray number (uint_t n, int base = 10)                { return bytearray().setNumber(n, base); }
	static bytearray number (long_t n, int base = 10)                { return bytearray().setNumber(n, base); }
	static bytearray number (ulong_t n, int base = 10)               { return bytearray().setNumber(n, base); }

	static bytearray fromBase64 (const bytearray & base64);
	bytearray toBase64 () const;
};

inline bytearray operator + (const bytearray & s1, const bytearray & s2)
{
	bytearray s(s1);
	s.append(s2);
	return s;
}

inline int bytearray::compare (const bytearray & s) const
{
	return base_class::compare(s.constData());
}

inline int bytearray::compare (size_t pos, size_t len, const bytearray & s) const
{
	return base_class::compare(pos, len, s.constData());
}

inline int bytearray::compare (size_t pos, size_t len, const bytearray & s, size_t subpos, size_t sublen) const
{
	return base_class::compare(pos, len, s.constData(), subpos, sublen);
}

inline int bytearray::compare (const char * s) const
{
	return base_class::compare(s);
}

inline int bytearray::compare (size_t pos, size_t len, const char * s) const
{
	return base_class::compare(pos, len, s);
}

inline int bytearray::compare (size_t pos, size_t len, const char * s, size_t n) const
{
	return base_class::compare(pos, len, s, n);
}

inline bytearray::const_iterator bytearray::find(const bytearray & s, const_iterator from) const
{
	return find(s.constData(), from - begin(), s.size());
}

inline void bytearray::reserve (size_t n)
{
	base_class::reserve(n);
}

inline void bytearray::resize (size_t size)
{
	base_class::resize(size);
}

inline bytearray bytearray::substr(size_t pos, size_t n) const
{
	bytearray ba(base_class::substr(pos, n));
	return ba;
}

bytearray bytearray::substr(const const_iterator & from) const
{
	bytearray ba(base_class::substr(from - begin(), length()));
	return ba;
}

bytearray bytearray::substr(const const_iterator & from, size_t n) const
{
	bytearray ba(base_class::substr(from - begin(), n));
	return ba;
}

inline bool operator != (const bytearray & s1, const bytearray & s2)
{
	return s1.compare(s2) != 0;
}

inline bool operator < (const bytearray &s1, const bytearray &s2)
{
	return s1.compare(s2) < 0;
}

inline bool operator <= (const bytearray &s1, const bytearray &s2)
{
	return s1.compare(s2) <= 0;
}

inline bool operator == (const bytearray &s1, const bytearray &s2)
{
	return s1.compare(s2) == 0;
}

inline bool operator > (const bytearray & s1, const bytearray & s2)
{
	return s1.compare(s2) > 0;
}

inline bool operator >= (const bytearray &s1, const bytearray &s2)
{
	return s1.compare(s2) >= 0;
}

inline bool	operator != (const bytearray & s1, const char * s2) { return s1.compare(s2) != 0; }
inline bool	operator <  (const bytearray & s1, const char * s2) { return s1.compare(s2) <  0; }
inline bool	operator <= (const bytearray & s1, const char * s2) { return s1.compare(s2) <= 0; }
inline bool	operator == (const bytearray & s1, const char * s2) { return s1.compare(s2) == 0; }
inline bool	operator >  (const bytearray & s1, const char * s2) { return s1.compare(s2) >  0; }
inline bool	operator >= (const bytearray & s1, const char * s2) { return s1.compare(s2) >= 0; }

inline std::ostream & operator << (std::ostream & os, const bytearray & o) { os << o.c_str(); return os; }

//DLL_API uint_t hash_func(const bytearray & key, uint_t seed = 0);

} // pfs

#endif /* __PFS_BYTEARRAY_HPP__ */
