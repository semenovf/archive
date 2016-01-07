/*
 * byte_string.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_BYTE_STRING_HPP__
#define __PFS_BYTE_STRING_HPP__

#include <string>
#include <pfs.hpp>
#include <pfs/endian.hpp>
#include <pfs/ostream.hpp>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API byte_string
{
	typedef std::basic_string<uint8_t> rep_type;

public:
	typedef uint8_t                                   value_type;
	typedef typename rep_type::traits_type	          traits_type;
	typedef typename rep_type::size_type	          size_type;
	typedef typename rep_type::difference_type        difference_type;
	typedef typename rep_type::reference 	          reference;
	typedef typename rep_type::const_reference        const_reference;
	typedef typename rep_type::pointer 	              pointer;
	typedef typename rep_type::const_pointer          const_pointer;
	typedef typename rep_type::iterator               iterator;
	typedef typename rep_type::const_iterator         const_iterator;
	typedef typename rep_type::reverse_iterator       reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;

	static const size_type npos = rep_type::npos;
private:
    rep_type  _d;

    explicit byte_string (const rep_type & d) : _d(d) {}
    byte_string & assign (const rep_type & d)
    {
    	_d.assign(d);
    	return *this;
    }

public:
    byte_string () : _d() {}

    byte_string (const byte_string & other)
    	: _d(other._d)
    {}

    byte_string (const byte_string & other,
                  size_type pos,
                  size_type count = npos)
    	: _d(other._d, pos, count)
    {}

    explicit byte_string (const_pointer s)
    	: _d(s)
    {}

    byte_string (const_pointer s, size_type n)
    	: _d(s, n)
    {}

    explicit byte_string (const char * s)
    	: _d(reinterpret_cast<const uint8_t *>(s))
    {}

    byte_string (const char * s, size_type n)
    	: _d(reinterpret_cast<const uint8_t *>(s), n)
    {}

	byte_string (size_type n, uint8_t c)
		: _d(n, c)
	{}

	byte_string (size_type n, char c)
		: _d(n, static_cast<uint8_t>(c))
	{}

    template <typename InputIterator>
    byte_string (InputIterator first, InputIterator last)
    	: _d(first, last)
    {}

    ~byte_string () {}

    byte_string & operator = (const byte_string & s)
    {
    	_d = s._d;
    	return *this;
    }

    byte_string & operator = (const_pointer s)
    {
    	_d = s;
    	return *this;
    }

    byte_string & operator = (value_type ch)
    {
    	_d = ch;
    	return *this;
    }

    byte_string & assign (size_type count, value_type ch)
    {
    	_d.assign(count, ch);
    	return *this;
    }

    byte_string & assign (const byte_string & s
    		, size_type pos
    		, size_type count)
    {
    	_d.assign(s._d, pos, count);
    	return *this;
    }

    byte_string & assign (const byte_string & s)
    {
    	return assign(s, 0, npos);
    }

    byte_string & assign (const byte_string & s, size_type pos)
    {
    	return assign(s, pos, npos);
    }

    byte_string & assign (const_pointer s, size_type count)
    {
    	_d.assign(s, count);
    	return *this;
    }

    byte_string & assign (const_pointer s)
    {
    	_d.assign(s);
    	return *this;
    }

    template <typename InputIterator>
    byte_string & assign (InputIterator first, InputIterator last)
    {
    	_d.assign<InputIterator>(first, last);
    	return *this;
    }

    reference at (size_type pos)
    {
    	return _d.at(pos);
    }

    const_reference at (size_type pos) const
    {
    	return _d.at(pos);
    }

    reference operator[] (size_type pos)
    {
    	return _d.operator [](pos);
    }

    const_reference operator[] (size_type pos) const
    {
    	return _d.operator [](pos);
    }

    reference front ()
    {
    	PFS_ASSERT(!empty());
#if __cplusplus < 201103L
    	return at(0);
#else
    	return _d.front();
#endif
    }

    const_reference front () const
    {
    	PFS_ASSERT(!empty());
#if __cplusplus < 201103L
    	return at(0);
#else
    	return _d.front();
#endif
    }

    reference back ()
    {
    	PFS_ASSERT(!empty());
#if __cplusplus < 201103L
    	return at(size() - 1);
#else
    	return _d.back();
#endif
    }

    const_reference back () const
    {
    	PFS_ASSERT(!empty());
#if __cplusplus < 201103L
    	return at(size() - 1);
#else
    	return _d.back();
#endif
    }

    const_pointer data () const
	{
		return _d.data();
	}

	const char * c_str () const
	{
		return reinterpret_cast<const char *>(_d.c_str());
	}


	iterator begin ()
	{
		return _d.begin();
	}

	const_iterator begin () const
	{
		return _d.begin();
	}

	const_iterator cbegin () const
	{
		return begin();
	}

	iterator end ()
	{
		return _d.end();
	}

	const_iterator end () const
	{
		return _d.end();
	}

	const_iterator cend () const
	{
    	return end();
	}

	reverse_iterator rbegin ()
	{
		return _d.rbegin();
	}

	const_reverse_iterator rbegin () const
	{
		return _d.rbegin();
	}

	const_reverse_iterator crbegin () const
	{
#if __cplusplus < 201103L
    	return _d.rbegin();
#else
    	return _d.crbegin();
#endif
	}

	reverse_iterator rend ()
	{
		return _d.rend();
	}

	const_reverse_iterator rend () const
	{
		return _d.rend();
	}

	const_reverse_iterator crend () const
	{
#if __cplusplus < 201103L
    	return _d.rend();
#else
    	return _d.crend();
#endif
	}

	/**
	 * @brief Checks if byte string is empty.
	 *
	 * @return @c true if byte string is empty (size() == 0) or @c false otherwise.
	 */
    bool empty () const
    {
    	return _d.empty();
    }

	/**
	 * @brief Return size of string in code units.
	 *
	 * @return String in code units.
	 */
	size_type size () const
	{
		return _d.size();
	}

	/**
	 * @brief Return size of byte string.
	 *
	 * @return Size of byte string.
	 * @see size()
	 */
    size_type length () const
    {
    	return _d.length();
    }

    size_type max_size () const
    {
    	return _d.max_size();
    }

    void reserve (size_type new_cap = 0)
    {
    	_d.reserve(new_cap);
    }

    size_type capacity () const
    {
    	return _d.capacity();
    }

    void shrink_to_fit ()
    {
#if __cplusplus < 201103L
    	if (capacity() > size()) {
    	    reserve(0);
    	}
#else
    	_d.shrink_to_fit();
#endif
    }

	void clear ()
	{
		_d.clear();
	}

	/**
	 * @brief Inserts @c count copies of character @c ch at the position @c index.
	 * @param index Position at which the content will be inserted.
	 * @param count Number of characters to insert.
	 * @param ch Character to insert.
	 * @return @code *this @endcode
	 */
	byte_string & insert (size_type index, size_type count, value_type ch)
	{
		PFS_ASSERT_RANGE(index <= size());
		PFS_ASSERT(max_size() - count >= size());
		_d.insert(index, count, ch);
		return *this;
	}

	/**
	 * @brief Inserts @c count copies of character @c ch at the position @c index.
	 * @param index Position at which the content will be inserted.
	 * @param count Number of characters to insert.
	 * @param ch Character to insert.
	 * @return @code *this @endcode
	 */
	byte_string & insert (size_type index, size_type count, char ch)
	{
		return insert(index, count, static_cast<value_type>(ch));
	}

	/**
	 * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
	 *        The length of the string is determined by the first null character.
	 * @param index Position at which the content will be inserted.
	 * @param s Pointer to the character string to insert.
	 * @return @code *this @endcode
	 */
	byte_string & insert (size_type index, const_pointer s)
	{
		PFS_ASSERT_RANGE(index <= size());
		PFS_ASSERT(max_size() - traits_type::length(s) >= size());
		_d.insert(index, s);
		return *this;
	}

	/**
	 * @brief Inserts null-terminated character string pointed to by @c s at the position @c index.
	 *        The length of the string is determined by the first null character.
	 * @param index Position at which the content will be inserted.
	 * @param s Pointer to the character string to insert.
	 * @return @code *this @endcode
	 */
	byte_string & insert (size_type index, const char * s)
	{
		return insert(index, reinterpret_cast<const_pointer>(s));
	}

	byte_string & insert (size_type index, const_pointer s, size_type count)
	{
		_d.insert(index, s, count);
		return *this;
	}

	byte_string & insert (size_type index, const byte_string & s)
	{
		_d.insert(index, s._d);
		return *this;
	}

	byte_string & insert (size_type index, const byte_string & s
			,size_type index_str, size_type count = npos)
	{
		_d.insert(index, s._d, index_str, count);
		return *this;
	}

// FIXME
//	iterator insert (const_iterator pos, value_type ch)
//	{
//#if __cplusplus < 201103L
//		size_type index = pos - cbegin();
//		insert(index, ch);
//		return iterator(begin() + index);
//#else
//		return _d.insert(pos, ch);
//#endif
//	}

// FIXME
//	iterator insert (const_iterator pos, size_type count, value_type ch)
//	{
//#if __cplusplus < 201103L
//		iterator it(begin());
//		std::advance(it, std::distance<const_iterator>(begin(), pos));
//		_d.insert(it, count, ch);
//		it = begin();
//		std::advance(it, std::distance<const_iterator>(begin(), pos));
//		return it;
//#else
//		return _d.insert(pos, count, ch);
//#endif
//	}

	template <typename InputIterator>
	iterator insert (const_iterator pos, InputIterator first, InputIterator last)
	{
		return _d.insert<InputIterator>(pos, first, last);
	}

	byte_string & erase( size_type index = 0, size_type count = npos)
	{
		PFS_ASSERT_RANGE(index <= size());
		_d.erase(index, count);
		return *this;
	}

	iterator erase (const_iterator position)
	{
#if __cplusplus < 201103L
		size_type index = position - cbegin();
		if (index < size()) {
			erase(index, 1);
			return begin() + index;
		}
		return end();
#else
		return _d.erase(position);
#endif
	}

	iterator erase (const_iterator first, const_iterator last)
	{
#if __cplusplus < 201103L
		size_type pos1 = first - cbegin();
		size_type pos2 = last - cbegin();
		if (pos1 < pos2) {
			erase(pos1, pos2 - pos1);
			return begin() + pos1;
		}
		return end();
#else
		return _d.erase(first, last);
#endif
	}

	void push_back (value_type ch)
	{
		_d.push_back(ch);
	}

	void pop_back ()
	{
#if __cplusplus < 201103L
		if (!empty())
			erase(size()-1, 1);
#else
		_d.pop_back();
#endif
	}

	byte_string & append (size_type count, value_type ch)
	{
		_d.append(count, ch);
		return *this;
	}

	byte_string & append (size_type count, char ch)
	{
		return append(count, static_cast<value_type>(ch));
	}

	byte_string & append (const byte_string & s)
	{
		_d.append(s._d);
		return *this;
	}

	byte_string & append (const byte_string & s
				, size_type pos
				, size_type count = npos)
	{
		_d.append(s._d, pos, count);
		return *this;
	}

	byte_string & append (const_pointer s, size_type count)
	{
		_d.append(s, count);
		return *this;
	}

	byte_string & append (const char * s, size_type count)
	{
		return append(reinterpret_cast<const_pointer>(s), count);
	}

	byte_string & append (const_pointer s)
	{
		_d.append(s);
		return *this;
	}

	byte_string & append (const char * s)
	{
		return append(reinterpret_cast<const_pointer>(s));
	}

	template <typename InputIterator>
	byte_string & append (InputIterator first, InputIterator last)
	{
		_d.append<InputIterator>(first, last);
		return *this;
	}

	byte_string & operator += (const byte_string & s)
	{
		_d.operator += (s._d);
		return *this;
	}

	byte_string & operator += (value_type ch)
	{
		_d.operator += (ch);
		return *this;
	}

	byte_string & operator += (char ch)
	{
		return operator += (static_cast<value_type>(ch));
	}

	byte_string & operator += (const_pointer s)
	{
		_d.operator += (s);
		return *this;
	}

	byte_string & operator += (const char * s)
	{
		return operator += (reinterpret_cast<const_pointer>(s));
	}

	int compare (const byte_string & s) const
	{
		return _d.compare(s._d);
	}

	int compare (size_type pos1, size_type count1, const byte_string & s) const
	{
		return _d.compare(pos1, count1, s._d);
	}

	int compare (size_type pos1, size_type count1
			, const byte_string & s
			, size_type pos2, size_type count2 = npos ) const
	{
		return _d.compare(pos1, count1, s._d, pos2, count2);
	}

	int compare (const_pointer s) const
	{
		return _d.compare(s);
	}

	int compare (const char * s) const
	{
		return compare(reinterpret_cast<const_pointer>(s));
	}

	int compare (size_type pos1, size_type count1, const_pointer s) const
	{
		return _d.compare(pos1, count1, s);
	}

	int compare (size_type pos1, size_type count1, const char * s) const
	{
		return compare(pos1, count1, reinterpret_cast<const_pointer>(s));
	}

	int compare (size_type pos1, size_type count1, const_pointer s, size_type count2) const
	{
		return _d.compare(pos1, count1, s, count2);
	}

	int compare (size_type pos1, size_type count1, const char * s, size_type count2) const
	{
		return compare(pos1, count1, reinterpret_cast<const_pointer>(s), count2);
	}

	byte_string & replace (size_type pos, size_type count, const byte_string & s)
	{
		_d.replace(pos, count, s._d);
		return *this;
	}

//	byte_string & replace (const_iterator first, const_iterator last, const byte_string & s)
//	{
//		_d.replace(first, last, s._d);
//		return *this;
//	}

	byte_string & replace (size_type pos, size_type count
			, const byte_string & s
			, size_type pos2, size_type count2 = npos)
	{
		_d.replace(pos, count, s._d, pos2, count2);
		return *this;
	}


	template <typename InputIterator>
	byte_string & replace (const_iterator first, const_iterator last
			, InputIterator first2, InputIterator last2)
	{
		_d.replace<InputIterator>(first, last, first2, last2);
		return *this;
	}


	byte_string & replace (size_type pos, size_type count, const_pointer s, size_type count2)
	{
		_d.replace(pos, count, s, count2);
		return *this;
	}

//	byte_string & replace (const_iterator first, const_iterator last, const_pointer s, size_type count2)
//	{
//		_d.replace(first, last, s, count2);
//		return *this;
//	}

	byte_string & replace (size_type pos, size_type count, const_pointer s)
	{
		_d.replace(pos, count, s);
		return *this;
	}

// FIXME
//	byte_string & replace (const_iterator first, const_iterator last, const_pointer s)
//	{
//		_d.replace(first, last, s);
//		return *this;
//	}

	byte_string & replace (size_type pos, size_type count, size_type count2, value_type ch)
	{
		_d.replace(pos, count, count2, ch);
		return *this;
	}

// FIXME
//	byte_string & replace (const_iterator first, const_iterator last, size_type count2, value_type ch)
//	{
//		_d.replace(first, last, count2, ch);
//		return *this;
//	}

	byte_string substr (size_type pos = 0, size_type count = npos) const
	{
		PFS_ASSERT_RANGE(pos <= size());
		return byte_string(_d.substr(pos, count));
	}

	byte_string & substr (byte_string & result, size_type pos = 0, size_type count = npos) const
	{
		PFS_ASSERT_RANGE(pos <= size());
		return result.assign(_d.substr(pos, count));
	}

	size_type copy (pointer dest, size_type count, size_type pos = 0) const
	{
		return _d.copy(dest, count, pos);
	}

	size_type copy (char * dest, size_type count, size_type pos = 0) const
	{
		return copy(reinterpret_cast<pointer>(dest), count, pos);
	}

	void resize (size_type count)
	{
		_d.resize(count);
	}

	void resize (size_type count, value_type ch)
	{
		_d.resize(count, ch);
	}

	void swap (byte_string & other)
	{
		_d.swap(other._d);
	}

#if __COMMENT__
	template <typename T>
	size_t readNumber (T & v, size_t pos = 0, endian::type_enum order = endian::nativeOrder()) const;

	intmax_t toIntegral (bool * ok = 0, int base = 10) const;
	uintmax_t toUIntegral (bool * ok = 0, int base = 10) const;

	short          toShort  (bool * ok = 0, int base = 10) const;
	unsigned short toUShort (bool * ok = 0, int base = 10) const;
	int	           toInt    (bool * ok = 0, int base = 10) const;
	unsigned int   toUInt   (bool * ok = 0, int base = 10) const;
	long           toLong   (bool * ok = 0, int base = 10) const;
	unsigned long  toULong  (bool * ok = 0, int base = 10) const;

#ifdef PFS_HAVE_LONGLONG
	long long toLongLong (bool * ok = 0, int base = 10) const;
	unsigned long long toULongLong (bool * ok = 0, int base = 10) const;
#endif

	real_t toReal (bool * ok = 0, char decimalPoint = '.') const;
	float toFloat (bool * ok = 0, char decimalPoint = '.') const;
	double toDouble (bool * ok = 0, char decimalPoint = '.') const;

#ifdef PFS_HAVE_LONG_DOUBLE
	long double toLongDouble (bool * ok = 0, char decimalPoint = '.') const { return toReal(ok, decimalPoint); }
#endif

private:

	template <typename ExType>
	struct extra_trait { typedef ExType type; };

#ifdef __COMMENT__
    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, mbcs_string_type_trait<const_iterator>);

#endif

    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::byte_string’"
    template <typename InputIt>
    byte_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2, extra_trait<InputIt>);
    byte_string & replace (const_iterator first, const_iterator last, const_iterator first2, const_iterator last2, extra_trait<const_iterator>);

public:
	template <typename T>
	static byte_string toBytes (const T & v, endian::type_enum order = endian::nativeOrder());
#endif //__COMMENT__
};


#if __FIXME__

//
// Specialization for utf8_string
//
template <>
inline byte_string byte_string::toBytes<utf8_string> (const utf8_string & v, endian::type_enum /*order*/)
{
	return byte_string(v.constData(), v.size());
}


#endif


// TODO For integers only supported by endian class
//
template <typename T>
byte_string::size_type unpack (byte_string::const_iterator pos, endian::type_enum order, T & v)
{
	union u
	{
		const T v;
		const byte_string::value_type b[sizeof(T)];
	};

	const u * d = reinterpret_cast<const u *>(pos.base());
	v = (order == endian::LittleEndian) ? endian::toLittleEndian(d->v) : endian::toBigEndian(d->v);
	return sizeof(T);
}

template <typename T>
inline byte_string::size_type unpack (byte_string::const_iterator pos, T & v)
{
	return unpack(pos, endian::native_order(), v);
}

//
// For integers only supported by endian class
//
template <typename T>
byte_string & pack (const T & v, endian::type_enum order, byte_string & result) // TODO back_inserter
{
	T a = ((order == endian::LittleEndian) ? endian::toLittleEndian<T>(v) : endian::toBigEndian<T>(v));
	union { T v; byte_string::value_type b[sizeof(T)]; } d;
	d.v = a;
	return result.assign(d.b, sizeof(T));
}

template <typename T>
inline byte_string & pack (const T & v, byte_string & result) // TODO back_inserter
{
	return pack<T>(v, endian::native_order(), result);
}

//
// Specialization for bool
//
template <>
inline byte_string & pack<bool> (const bool & v, endian::type_enum order, byte_string & result) // TODO back_inserter
{
	return pack<char>(v ? '\x01' : '\x00', order, result);
}

//
// Specialization for float
// TODO as mentioned at http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#serialization
//
template <>
inline byte_string & pack<float> (const float & v, endian::type_enum order, byte_string & result)// TODO back_inserter
{
	PFS_UNUSED2(v, order);
	PFS_ASSERT_TODO();
	return result;
}

//
// Specialization for double
// TODO as for float
//
template <>
inline byte_string & pack<double> (const double & v, endian::type_enum order, byte_string & result) // TODO back_inserter
{
	PFS_UNUSED2(v, order);
	PFS_ASSERT_TODO();
	return result;
}

#ifdef PFS_HAVE_LONG_DOUBLE
//
// Specialization for long double
// TODO as for float
//
template <>
inline byte_string & pack<long double> (const long double & v, endian::type_enum order, byte_string & result)// TODO back_inserter
{
	PFS_UNUSED2(v, order);
	PFS_ASSERT_TODO();
	return result;
}
#endif // PFS_HAVE_LONG_DOUBLE

// Specialization for byte_string
//
template <>
inline byte_string & pack<byte_string> (const byte_string & v, endian::type_enum order, byte_string & result) // TODO back_inserter
{
	return result.assign(v);
}


byte_string & base64_encode (const byte_string & src, byte_string & result);
byte_string & base64_decode (const byte_string & src, byte_string & result);

inline std::ostream & operator << (std::ostream & os, const byte_string & o)
{
	os << o.c_str();
	return os;
}

inline bool operator == (const byte_string & lhs, const byte_string & rhs)
{
	return lhs.compare(rhs) == 0;
}

inline bool operator != (const byte_string & lhs, const byte_string & rhs)
{
	return lhs.compare(rhs) != 0;
}

inline bool operator <  (const byte_string & lhs, const byte_string & rhs)
{
	return lhs.compare(rhs) < 0;
}

inline bool operator <= (const byte_string & lhs, const byte_string & rhs)
{
	return lhs.compare(rhs) <= 0;
}

inline bool operator >  (const byte_string & lhs, const byte_string & rhs)
{
	return lhs.compare(rhs) > 0;
}

inline bool operator >= (const byte_string & lhs, const byte_string & rhs)
{
	return lhs.compare(rhs) >= 0;
}

inline bool operator == (const byte_string & lhs, const char * rhs)
{
	return lhs.compare(rhs) == 0;
}

inline bool operator != (const byte_string & lhs, const char * rhs)
{
	return lhs.compare(rhs) != 0;
}

inline bool operator <  (const byte_string & lhs, const char * rhs)
{
	return lhs.compare(rhs) < 0;
}

inline bool operator <= (const byte_string & lhs, const char * rhs)
{
	return lhs.compare(rhs) <= 0;
}

inline bool operator >  (const byte_string & lhs, const char * rhs)
{
	return lhs.compare(rhs) > 0;
}

inline bool operator >= (const byte_string & lhs, const char * rhs)
{
	return lhs.compare(rhs) >= 0;
}

inline bool operator == (const char * lhs, const byte_string & rhs)
{
	return rhs.compare(lhs) == 0;
}

inline bool operator != (const char * lhs, const byte_string & rhs)
{
	return rhs.compare(lhs) != 0;
}

inline bool operator <  (const char * lhs, const byte_string & rhs)
{
	return rhs.compare(lhs) > 0;
}

inline bool operator <= (const char * lhs, const byte_string & rhs)
{
	return rhs.compare(lhs) >= 0;
}

inline bool operator >  (const char * lhs, const byte_string & rhs)
{
	return rhs.compare(lhs) < 0;
}

inline bool operator >= (const char * lhs, const byte_string & rhs)
{
	return rhs.compare(lhs) <= 0;
}

} // pfs

//#include <pfs/bits/byte_string_impl_inc.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BYTE_STRING_HPP__ */
