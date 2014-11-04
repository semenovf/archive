/*
 * bytearray.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_BYTEARRAY_HPP__
#define __PFS_BYTEARRAY_HPP__

#include "vector.hpp"
#include "bits/allocator.hpp"
#include "endian.hpp"
//#include "bits/cast_traits.hpp"
#include <string>
#include <cstring>
//#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

//class utf8string;
class bytearray;

struct bytearray_terminator
{
	bytearray * p;
	bytearray_terminator (bytearray * ptr) : p(ptr) {}
	~bytearray_terminator ();
};


template <class T>
struct bytearray_allocator : public pfs::allocator<T>
{
	typedef pfs::allocator<T> base_class;
	typedef typename pfs::allocator<T>::size_type size_type;
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

	value_type * allocate (size_type n, const void * = 0)
	{
		return base_class::allocate(n+1);
	}
	void deallocate (value_type * p, size_type /*n*/)
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


class DLL_API bytearray : protected vector<char, bytearray_allocator<char> >
{
	friend struct bytearray_terminator;

protected:
	typedef vector<char, bytearray_allocator<char> > base_class;
	typedef bytearray self_class;

public: // typedefs
	typedef base_class::value_type             value_type;
	typedef base_class::pointer                pointer;
	typedef base_class::const_pointer          const_pointer;
	typedef base_class::size_type              size_type;
	typedef base_class::difference_type        difference_type;

	typedef base_class::reference              reference;
	typedef base_class::const_reference        const_reference;
	typedef base_class::iterator               iterator;
	typedef base_class::const_iterator         const_iterator;
	typedef base_class::reverse_iterator	   reverse_iterator;
	typedef base_class::const_reverse_iterator const_reverse_iterator;

	typedef base_class::value_type char_type;

public: // static
	static const char_type TerminatorChar/* = '\0'*/;
	static const char_type EndOfLineChar/* = '\n'*/;

public:
	bytearray () : base_class() {}

	bytearray (const char * s)
		: base_class(s, s + (s != nullptr ? strlen(s) : 0))
	{
		bytearray_terminator bt(this);
	}

	bytearray (const char * s, size_type size)
		: base_class(s, s + (s != nullptr ? size : 0))
	{
		bytearray_terminator bt(this);
	}

	bytearray (const std::string & s)
		: base_class(s.begin(), s.end())
	{
		bytearray_terminator bt(this);
	}

	bytearray (size_type n, char ch)
		: base_class(n, ch)
	{
		bytearray_terminator bt(this);
	}

	bytearray (const_iterator first, const_iterator last)
		: base_class(first, last)
	{
		bytearray_terminator bt(this);
	}

	virtual ~bytearray () {}

	bool isNull ()  const  { return base_class::isNull(); }
	bool isEmpty () const  { return base_class::isEmpty(); }
	bool empty ()   const  { return base_class::isEmpty(); }

    iterator begin () { return base_class::begin(); }
    iterator end   () { return base_class::end(); }
    const_iterator begin () const { return cbegin(); }
    const_iterator end   () const { return cend(); }
    const_iterator cbegin() const { return base_class::cbegin(); }
    const_iterator cend  () const { return base_class::cend(); }
    reverse_iterator rbegin  ()   { return base_class::rbegin(); }
    reverse_iterator rend    ()   { return base_class::rend(); }
    const_reverse_iterator rbegin  () const { return crbegin(); }
    const_reverse_iterator rend    () const { return crend(); }
    const_reverse_iterator crbegin () const { return base_class::crbegin(); }
    const_reverse_iterator crend   () const { return base_class::crend(); }

	reference at (size_type index) const { return base_class::at(index); }
	reference operator [] (size_type i) const { return at(i); }

	iterator find (char value) const { return base_class::find(value); }

	reference front () const { return at(0); }
	reference first () const { return at(0); }
	reference back  () const { return at(size()-1); }
	reference last  () const { PFS_ASSERT(size() > 0); return at(size()-1); }

	const char * constData () const { return base_class::constData(); }
	const char * data () const      { return base_class::data(); }
	char * data ()                  { return base_class::data(); }
    const char * c_str () const { return constData(); }

	size_type size () const { return base_class::size(); }
	size_type length () const { return base_class::size(); }
	size_type capacity() const { return base_class::capacity() - 1; }


	void clear ()
	{
		bytearray_terminator bt(this);
		base_class::clear();
	}

    void reserve (size_type n)
    {
		bytearray_terminator bt(this);
    	base_class::reserve(n);
    }

    void resize (size_type n)
    {
    	resize(n, TerminatorChar);
    }

    void resize (size_type n, const value_type & value)
    {
    	bytearray_terminator bt(this);
    	base_class::resize(n, value);
    }

    void shrink_to_fit ()
    {
    	bytearray_terminator bt(this);
    	base_class::shrink_to_fit();
    }

	iterator erase (const_iterator pos)
	{
		bytearray_terminator bt(this);
		return base_class::erase(pos);
	}

	iterator erase (const_iterator first, const_iterator last)
	{
		bytearray_terminator bt(this);
		return base_class::erase(first, last);
	}

	iterator remove (const_iterator pos) { return erase(pos); }
	iterator remove (const_iterator first, const_iterator last) { return erase(first, last); }
	iterator remove (size_type index) { return erase(cbegin() + index); }
	iterator remove (size_type index, size_t n) { return erase(cbegin() + index, cbegin() + index + n); }
	void pop_back () { if(size() > 0) erase(cend() - 1); }
	void pop_front () { if(size() > 0) erase(cbegin()); }

	iterator insert (const_iterator pos, size_type count, char value)
	{
		bytearray_terminator bt(this);
		return base_class::insert(pos, count, value);
	}

	iterator insert (const_iterator pos, char value)
	{
		return insert(pos, 1, value);
	}

	iterator insert (const_iterator pos, const_iterator first, const_iterator last)
	{
		bytearray_terminator bt(this);
		return base_class::insert(pos, first, last);
	}

	void push_back (char value) { insert(cend(), value); }
	void append    (char value) { insert(cend(), value); }
	void append    (size_type n, char value)  { insert(cend(), n, value); }
	void append    (const bytearray & other) { insert(cend(), other.cbegin(), other.cend()); }
	void append    (const char * s, size_type n)
	{
		if (s && n > 0) {
			bytearray_terminator bt(this);
			base_class::insert(cend(), s, s + n);
		}
	}

	void append (const char * s)
	{
		append(s, strlen(s));
	}

	void push_front (char value) { insert(cbegin(), value); }
	void prepend    (char value) { insert(cbegin(), value); }
	void prepend    (size_type n, char value)  { insert(cbegin(), n, value); }
	void prepend    (const bytearray & other) { insert(cbegin(), other.cbegin(), other.cend()); }
	void prepend    (const char * s, size_type n)
	{
		if (s && n > 0) {
			bytearray_terminator bt(this);
			base_class::insert(cend(), s, s + n);
		}
	}

	void prepend (const char * s)
	{
		prepend(s, strlen(s));
	}

	bytearray   operator +  (const bytearray & o) const { bytearray r(*this); r.append(o); return r; }
	bytearray & operator += (const bytearray & o) { append(o); return *this; }
	bytearray & operator += (const char value)    { append(value); return *this; }
	bytearray & operator << (const char value)    { append(value); return *this; }
	bytearray & operator << (const bytearray & o) { append(o); return *this; }

	bool contains   (const bytearray & ba) const     { return base_class::contains(ba); }
	bool contains   (char value) const               { return base_class::contains(value); }
	bool startsWith (char value) const               { return base_class::startsWith(value); }
	bool startsWith (const bytearray & ba) const     { return base_class::startsWith(ba); }
	bool startsWith (const char * s) const           { return startsWith(bytearray(s)); }
	bool startsWith (const char * s, size_t n) const { return startsWith(bytearray(s, n)); }
	bool endsWith   (char value) const               { return base_class::endsWith(value); }
	bool endsWith   (const bytearray & ba) const     { return base_class::endsWith(ba); }
	bool endsWith   (const char * s) const           { return endsWith(bytearray(s)); }
	bool endsWith   (const char * s, size_t n) const { return endsWith(bytearray(s, n)); }

	void swap (bytearray & other) { base_class::swap(other); }

	bytearray substr (const_iterator from, const_iterator last) const
	{
		return bytearray(from, last);
	}

	bytearray substr (size_type pos) const
	{
		PFS_ASSERT(pos < size());
		return substr(cbegin() + pos, cbegin() + pos + size());
	}

	bytearray substr (size_type pos, size_type n) const
	{
		PFS_ASSERT(pos < size());
		if (pos + n > size())
			n = size() - pos;
		return substr(cbegin() + pos, cbegin() + pos + n);
	}

	bytearray mid (size_type pos, size_type n) const
	{
		return substr(pos, n);
	}

	bytearray left   (size_type n) const
	{
		return size() <= n
				? bytearray(*this)
				: bytearray(cbegin(), cbegin() + n);
	}
	bytearray right  (size_type n) const
	{
		return (size() <= n)
				? bytearray(*this)
				: bytearray(cbegin() + n, cend());
	}

	real_t   toReal   (bool * ok = 0, char decimalPoint = '.') const;
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
	void setNumber (float n, char f = 'g', int prec = 6) { return setNumber(real_t(n), f, prec); }
#ifdef PFS_HAVE_LONG_DOUBLE
	void setNumber (double n, char f = 'g', int prec = 6) { return setNumber(real_t(n), f, prec); }
#endif
	void setNumber (real_t n, char f = 'g', int prec = 6);

	static bytearray number (real_t n, char fmt = 'g', int prec = 6) { bytearray r; r.setNumber(n, fmt, prec); return r; }
#ifdef PFS_HAVE_LONG_DOUBLE
	static bytearray number (double n, char fmt = 'g', int prec = 6) { bytearray r; r.setNumber(n, fmt, prec); return r; }
#endif
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
	char * d = p->data();
	if (d) {
		d[p->size()] = bytearray::TerminatorChar;
	}
}

inline bool	operator != (const bytearray & s1, const bytearray & s2) { return strcmp(s1.c_str(), s2.c_str()) != 0; }
inline bool	operator <  (const bytearray & s1, const bytearray & s2) { return strcmp(s1.c_str(), s2.c_str()) <  0; }
inline bool	operator <= (const bytearray & s1, const bytearray & s2) { return strcmp(s1.c_str(), s2.c_str()) <= 0; }
inline bool	operator == (const bytearray & s1, const bytearray & s2) { return strcmp(s1.c_str(), s2.c_str()) == 0; }
inline bool	operator >  (const bytearray & s1, const bytearray & s2) { return strcmp(s1.c_str(), s2.c_str()) >  0; }
inline bool	operator >= (const bytearray & s1, const bytearray & s2) { return strcmp(s1.c_str(), s2.c_str()) >= 0; }

inline bool	operator != (const bytearray & s1, const char * s2) { return strcmp(s1.c_str(), s2) != 0; }
inline bool	operator <  (const bytearray & s1, const char * s2) { return strcmp(s1.c_str(), s2) <  0; }
inline bool	operator <= (const bytearray & s1, const char * s2) { return strcmp(s1.c_str(), s2) <= 0; }
inline bool	operator == (const bytearray & s1, const char * s2) { return strcmp(s1.c_str(), s2) == 0; }
inline bool	operator >  (const bytearray & s1, const char * s2) { return strcmp(s1.c_str(), s2) >  0; }
inline bool	operator >= (const bytearray & s1, const char * s2) { return strcmp(s1.c_str(), s2) >= 0; }

inline bool	operator != (const char * s2, const bytearray & s1) { return strcmp(s2, s1.c_str()) != 0; }
inline bool	operator <  (const char * s2, const bytearray & s1) { return strcmp(s2, s1.c_str()) <  0; }
inline bool	operator <= (const char * s2, const bytearray & s1) { return strcmp(s2, s1.c_str()) <= 0; }
inline bool	operator == (const char * s2, const bytearray & s1) { return strcmp(s2, s1.c_str()) == 0; }
inline bool	operator >  (const char * s2, const bytearray & s1) { return strcmp(s2, s1.c_str()) >  0; }
inline bool	operator >= (const char * s2, const bytearray & s1) { return strcmp(s2, s1.c_str()) >= 0; }

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
	T a = (order == endian::LittleEndian ? endian::toLittleEndian(v) : endian::toBigEndian(v));
	union { T v; char b[sizeof(T)]; } d;
	d.v = a;
	return bytearray(d.b, sizeof(T));
}

// Specialization for bool
//
template <>
inline bytearray bytearray::toBytes<bool> (const bool & v, endian::type_enum order)
{
	return toBytes<char>(v ? '\x01' : '\x00', order);
}

//template <>
//bytearray bytearray::toBytes<pfs::utf8string> (const pfs::utf8string & v, endian::type_enum /*order*/);
//
//template <>
//inline bytearray bytearray::toBytes<pfs::bytearray> (const pfs::bytearray & v, endian::type_enum /*order*/)
//{
//	return bytearray(v);
//}

inline std::ostream & operator << (std::ostream & os, const pfs::bytearray & o)
{
	os << o.c_str();
	return os;
}

////DLL_API uint_t hash_func(const bytearray & key, uint_t seed = 0);

} // pfs

namespace pfs {
//
//template <> inline bool cast_trait<bool, pfs::bytearray> (const pfs::bytearray & v)
//{
//	return v.isEmpty() ? false
//		: (v[0] == '\x00' ? false : true);
//}
//
//template <> inline int cast_trait<int, pfs::bytearray> (const pfs::bytearray & v) { return int(v.toInt()); }
//template <> inline unsigned int cast_trait<unsigned int, pfs::bytearray> (const pfs::bytearray & v) { return int(v.toUInt()); }
//template <> inline long cast_trait<long, pfs::bytearray> (const pfs::bytearray & v) { return long(v.toLong()); }
//#ifdef HAVE_LONGLONG
//template <> inline long long cast_trait<long long, pfs::bytearray> (const pfs::bytearray & v) { return (long long)v.toLong(); }
//#endif
//
//template <> inline float cast_trait<float, pfs::bytearray> (const pfs::bytearray & v) { return float(v.toDouble()); }
//template <> inline double cast_trait<double, pfs::bytearray> (const pfs::bytearray & v) { return v.toDouble(); }
//
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, bool> (const bool & v)                     { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, char> (const char & v)                     { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, unsigned char> (const unsigned char & v)   { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, short> (const short & v)                   { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, unsigned short> (const unsigned short & v) { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, int> (const int & v)                       { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, unsigned int> (const unsigned int & v)     { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, long> (const long & v)                     { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, unsigned long> (const unsigned long & v)   { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, float> (const float & v)                   { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, double> (const double & v)                 { return pfs::bytearray::toBytes(v); }
//#ifdef HAVE_LONGLONG
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, long long> (const long long & v)           { return pfs::bytearray::toBytes(v); }
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, unsigned long long> (const unsigned long long & v) { return pfs::bytearray::toBytes(v); }
//#endif
//template <> inline pfs::bytearray cast_trait<pfs::bytearray, pfs::bytearray> (const pfs::bytearray & v) { return pfs::bytearray(v); }

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BYTEARRAY_HPP__ */
