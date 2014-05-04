/*
 * bytearray.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_BYTEARRAY_HPP__
#define __PFS_BYTEARRAY_HPP__

#include "pimpl.hpp"
#include "endian.hpp"
#include "vector.hpp"
#include <string>
#include <cstring>
#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class utf8string;
class bytearray;

struct bytearray_terminator
{
	bytearray * p;
	bytearray_terminator (bytearray * ptr) : p(ptr) {}
	~bytearray_terminator ();
};


template <class T>
struct bytearray_allocator : public std::allocator<T>
{
	typedef std::allocator<T>      base_class;
	typedef T value_type;

	bytearray_allocator () : base_class() {}
	bytearray_allocator (const std::allocator<T> & a) : base_class(a) {}

	template <class U>
	bytearray_allocator (const bytearray_allocator<U> &) : base_class() /*noexcept*/ {}

	template <class U>
	struct rebind
	{
	   typedef bytearray_allocator<U> other;
	};

	value_type * allocate (std::size_t n, const void * = 0)
	{
		return base_class::allocate(n+1);
	}
	void deallocate (value_type * p, std::size_t /*n*/)
	{
		base_class::deallocate(p, 0);
	}
};

#ifdef __COMMENT__
// see http://en.cppreference.com/w/cpp/concept/Allocator
struct bytearray_allocator
{
	typedef char value_type;

	//template<typename _Tp1>
	struct rebind
	{ typedef bytearray_allocator other; };

	bytearray_allocator(/*ctor args*/) {}
	bytearray_allocator (const bytearray_allocator &) {}
	char * allocate (std::size_t n) { return static_cast<char *>(::operator new((n+1) * sizeof(char))) ; }
	void deallocate (char * p, std::size_t) { ::operator delete(p); }
};

// Returns true only if the storage allocated by the allocator a1
// can be deallocated through a2. Establishes reflexive, symmetric,
// and transitive relationship. Does not throw exceptions.
bool operator == (const bytearray_allocator & /*a1*/, const bytearray_allocator & /*a2*/) { return true; }
// same as !(a1==a2)
bool operator != (const bytearray_allocator & a1, const bytearray_allocator & a2) { return !(a1==a2); }
#endif


typedef item_ref<char, bytearray_allocator<char> > byteref;

class DLL_API bytearray : public vector<char, bytearray_allocator<char> >
{
	friend struct bytearray_terminator;

public:
	typedef vector<char, bytearray_allocator<char> > base_class;
	typedef base_class::value_type char_type;
	typedef base_class::value_type item_type;
	typedef base_class::iterator iterator;
	typedef base_class::const_iterator const_iterator;

	static const char_type TerminatorChar;
	static const char_type EndOfLineChar;

public:
	bytearray () : base_class() {}
//	bytearray (const impl & other) : _d(new impl(other)) {}
	bytearray (const char * s);
	bytearray (const char * s, size_t size);
	bytearray (size_t n, const char & c);
	bytearray (const std::string & s);
//	bytearray (const_iterator begin, const_iterator end) : _d(new impl(begin, end)) {}

	virtual ~bytearray () {}

	virtual void resize (size_t n, char v = TerminatorChar)
	{
		bytearray_terminator bt(this);
		base_class::resize(n, v);
	}

	virtual void clear ()
	{
		bytearray_terminator bt(this);
		base_class::clear();
	}

	virtual void insert (iterator position, const char * ptr, size_t n)
	{
		bytearray_terminator bt(this);
		base_class::insert(position, ptr, n);
	}

	virtual void insert (iterator position, size_t n, const char & val)
	{
		bytearray_terminator bt(this);
		base_class::insert(position, n, val);
	}

    virtual iterator erase (iterator first, iterator last)
    {
		bytearray_terminator bt(this);
		return base_class::erase(first, last);
    }

    const char * c_str () const { return constData(); }

//    bool contains (const char * s) const { return find(begin(), s) != end(); }

    size_t length () const { return size(); }

	static size_t length (const iterator & begin, const iterator & end)
	{
		return begin <= end ? end - begin : begin - end;
	}

	static size_t length (const const_iterator & begin, const const_iterator & end)
	{
		return begin <= end ? end - begin : begin - end;
	}

	void append (const char * s)
	{
		bytearray_terminator bt(this);
		base_class::append(s, strlen(s));
	}

	void append (const char * s, size_t n)
	{
		bytearray_terminator bt(this);
		base_class::append(s, n);
	}

	void append (size_t n, const char & c)
	{
		bytearray_terminator bt(this);
		base_class::append(n, c);
	}

	void append (const bytearray & other)
	{
		bytearray_terminator bt(this);
		base_class::append(other);
	}

	void prepend (const char * s)
	{
		bytearray_terminator bt(this);
		base_class::prepend(s, strlen(s));
	}

	void prepend (const char * s, size_t n)
	{
		bytearray_terminator bt(this);
		base_class::prepend(s, n);
	}

	void prepend (size_t n, const char & c)
	{
		bytearray_terminator bt(this);
		base_class::prepend(n, c);
	}

	void prepend (const bytearray & other)
	{
		bytearray_terminator bt(this);
		base_class::prepend(other);
	}

	void insert (iterator pos, const char * s)
	{
		bytearray_terminator bt(this);
		base_class::insert(pos, s, strlen(s));
	}

//	void insert (iterator pos, const char * s, size_t n)
//	{
//		bytearray_terminator bt(this);
//		base_class::insert(pos, s, n);
//	}

	void insert (iterator pos, const bytearray & other)
	{
		bytearray_terminator bt(this);
		base_class::insert(pos, other);
	}

	//void append (size_t n, char ch) { base_class::append(n, ch); }

/*
	bytearray & append  (const bytearray & s) { return insert(s, end()); }
	bytearray & append  (const char * s, size_t n) { return insert(s, n, end()); }
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
*/


	bool endsWith (const bytearray & s) const { return endsWith(s.data(), s.size()); }
	bool endsWith (const char * s) const;
	bool endsWith (const char * s, size_t n) const;

	bool startsWith (const bytearray & s) const { return startsWith(s.data(), s.size()); }
	bool startsWith (const char * s) const;
	bool startsWith (const char * s, size_t n) const;

//	const_iterator find (const_iterator pos, const bytearray & s) const;
//	const_iterator find (size_t pos, const char * s, size_t n) const;
//	const_iterator find (size_t pos, size_t len, const char * s, size_t n) const;
//	const_iterator find (size_t pos, char ch) const;

//	bytearray & remove (size_t pos)                  { return remove(pos, length()); }
//	bytearray & remove (size_t pos, size_t n)        { return remove(begin() + pos, n); }
//	bytearray & remove (const const_iterator & from) { return remove(from, length()); }
//	bytearray & remove (const const_iterator & from, size_t n);

//	void reserve (size_t n = 0);
//	void resize  (size_t size);
//	void swap (bytearray & other) { pfs_swap(_d, other._d); }

	bytearray substr (const const_iterator & from, size_t n) const;
	bytearray substr (size_t pos) const { return substr(pos, length()); }
	bytearray substr (size_t pos, size_t n) const;
	bytearray substr (const const_iterator & from) const { return substr(from, size()); }
	bytearray mid    (size_t pos, size_t n) const { return substr(pos, n); }
	bytearray left   (size_t n) const { return substr(0, n); }
	bytearray right  (size_t n) const { return substr(length() - n, n); }

	double	 toDouble (bool * ok = 0, char decimalPoint = '.') const;
	long_t   toLong   (bool * ok = 0, int base = 10) const;
	ulong_t	 toULong  (bool * ok = 0, int base = 10) const;
	int_t	 toInt    (bool * ok = 0, int base = 10) const;
	uint_t	 toUInt   (bool * ok = 0, int base = 10) const;
	short_t	 toShort  (bool * ok = 0, int base = 10) const;
	ushort_t toUShort (bool * ok = 0, int base = 10) const;
	sbyte_t  toSByte  (bool * ok = 0, int base = 10) const;
	byte_t	 toByte   (bool * ok = 0, int base = 10) const;

	void setNumber (long_t n, int base = 10);
	void setNumber (ulong_t n, int base = 10);
	void setNumber (int_t n, int base = 10)    { setNumber(long_t(n), base); }
	void setNumber (uint_t n, int base = 10)   { setNumber(ulong_t(n), base); }
	void setNumber (short_t n, int base = 10)  { setNumber(long_t(n), base); }
	void setNumber (ushort_t n, int base = 10) { setNumber(ulong_t(n), base); }
	void setNumber (sbyte_t n, int base = 10)  { setNumber(long_t(n), base); }
	void setNumber (byte_t n, int base = 10)   { setNumber(ulong_t(n), base); }
	void setNumber (double n, char f = 'g', int prec = 6);

	bytearray & operator += (const bytearray & other) { append(other); return *this; }

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

	static bytearray number (double n, char fmt = 'g', int prec = 6) { bytearray r; r.setNumber(n, fmt, prec); return r; }
	static bytearray number (float n, char fmt = 'g', int prec = 6)  { bytearray r; r.setNumber(n, fmt, prec); return r; }
	static bytearray number (int_t n, int base = 10)                 { bytearray r; r.setNumber(n, base); return r; }
	static bytearray number (uint_t n, int base = 10)                { bytearray r; r.setNumber(n, base); return r; }
	static bytearray number (long_t n, int base = 10)                { bytearray r; r.setNumber(n, base); return r; }
	static bytearray number (ulong_t n, int base = 10)               { bytearray r; r.setNumber(n, base); return r; }

	template <typename T>
	size_t readNumber (T & v, size_t pos = 0, endian::type_enum order = endian::nativeOrder()) const;

	template <typename T>
	static bytearray toBytes (const T & v, endian::type_enum order = endian::nativeOrder());
	static bytearray fromBase64 (const bytearray & base64);
	bytearray        toBase64 () const;
};

inline bytearray_terminator::~bytearray_terminator ()
{
	size_t sz = p->size();
	char * d = p->data();

	d[sz] = bytearray::TerminatorChar;
//	p->data()[sz] = bytearray::TerminatorChar;
}

inline bytearray operator + (const bytearray & s1, const bytearray & s2)
{
	bytearray s(s1);
	s.append(s2);
	return s;
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

inline bool	operator != (const bytearray & s1, const char * s2) { return s1.compare(s2, strlen(s2)) != 0; }
inline bool	operator <  (const bytearray & s1, const char * s2) { return s1.compare(s2, strlen(s2)) <  0; }
inline bool	operator <= (const bytearray & s1, const char * s2) { return s1.compare(s2, strlen(s2)) <= 0; }
inline bool	operator == (const bytearray & s1, const char * s2) { return s1.compare(s2, strlen(s2)) == 0; }
inline bool	operator >  (const bytearray & s1, const char * s2) { return s1.compare(s2, strlen(s2)) >  0; }
inline bool	operator >= (const bytearray & s1, const char * s2) { return s1.compare(s2, strlen(s2)) >= 0; }

inline std::ostream & operator << (std::ostream & os, const bytearray & o)
{
	os << o.c_str();
	return os;
}

template <typename T>
size_t bytearray::readNumber (T & v, size_t pos, endian::type_enum order) const
{
	if (size() - pos < sizeof(T))
		return 0;
	union u { const T v; const char b[sizeof(T)]; };
	const char * s = constData() + pos;
	const u * d = reinterpret_cast<const u *>(s/*constData() + pos*/);
	v = order == endian::LittleEndian ? endian::toLittleEndian(d->v) : endian::toBigEndian(d->v);
	return sizeof(T);
}

template <typename T>
inline bytearray bytearray::toBytes (const T & v, endian::type_enum order)
{
	T a = order == endian::LittleEndian ? endian::toLittleEndian(v) : endian::toBigEndian(v);
	union { T v; char b[sizeof(T)]; } d;
	d.v = a;
	return bytearray(d.b, sizeof(T));
}

template <>
inline bytearray bytearray::toBytes<bool> (const bool & v, endian::type_enum order)
{
	return toBytes<char>(v ? '\x01' : '\x00', order);
}

template <>
bytearray bytearray::toBytes<pfs::utf8string> (const pfs::utf8string & v, endian::type_enum /*order*/);

template <>
inline bytearray bytearray::toBytes<pfs::bytearray> (const pfs::bytearray & v, endian::type_enum /*order*/)
{
	return bytearray(v);
}


//DLL_API uint_t hash_func(const bytearray & key, uint_t seed = 0);

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BYTEARRAY_HPP__ */
