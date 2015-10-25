/*
 * byte_string.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_BYTE_STRING_HPP__
#define __PFS_BYTE_STRING_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/iterator.hpp>
#include <pfs/endian.hpp>
#include <pfs/bits/byte_string_impl.hpp>
#include <pfs/string.hpp>
#include <cstring>
#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class DLL_API byte_string : public nullable<byte_string_impl>
{
protected:
	typedef byte_string_impl     impl_class;
	typedef nullable<impl_class> base_class;
	typedef byte_string          self_class;

public:
	typedef byte_t              value_type;
	typedef size_t              size_type;
	typedef ptrdiff_t           difference_type;

	typedef byte_string_ptr<self_class>       pointer;
	typedef byte_string_ptr<const self_class> const_pointer;

	typedef pfs::reference<self_class>                    reference;
	typedef pfs::reference<const self_class>              const_reference;
	typedef pfs::random_access_iterator<self_class>       iterator;
	typedef pfs::random_access_iterator<const self_class> const_iterator;
	typedef std::reverse_iterator<iterator>		          reverse_iterator;
	typedef std::reverse_iterator<const_iterator>         const_reverse_iterator;

	typedef value_type char_type;

	typedef const byte_t * const_data_pointer;

public: // static
	static const char TerminatorChar  = '\0';
	static const char EndOfLineChar   = '\n';
	static const char ReplacementChar = '?';

public:
	byte_string () : base_class() {}
	explicit byte_string (const char * str);
	byte_string (const_data_pointer bytes, size_type n);
	byte_string (const char * str, size_type n);
	byte_string (size_t count, byte_t ch);
	byte_string (size_t count, char ch);

	virtual ~byte_string () {}

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }

    size_type size () const;
    size_type length () const; // Equivalent to size()
    size_type capacity() const;
    size_type max_size() const;

	void clear ()
	{
		if (!isEmpty()) {
			base_class::detach();
			base_class::cast()->clear();
		}
	}
    void resize  (size_type n) { base_class::detach(); base_class::cast()->resize(n, 0); }
    void resize  (size_type n, const value_type & value) { base_class::detach(); base_class::cast()->resize(n, value); }
	void reserve (size_type n) { base_class::detach(); base_class::cast()->reserve(n); }

	byte_string & erase (size_type index = 0) { return erase(index, 1); }
	byte_string & erase (size_type index, size_type count);
	iterator      erase (const_iterator pos) { return erase(pos, pos + 1); }
	iterator      erase (const_iterator first, const_iterator last);
	byte_string & remove (size_type index = 0) { return erase(index, 1); }
	byte_string & remove (size_type index, size_type count) { return erase(index, count); }
	iterator      remove (const_iterator pos) { return erase(pos, pos + 1); }
	iterator      remove (const_iterator first, const_iterator last) { return erase(first, last); }

	iterator begin ()                       { return iterator(this, pointer(this, 0)); }
	iterator end   ()                       { return iterator(this, pointer(this, size())); }
    const_iterator begin () const           { return cbegin(); }
    const_iterator end   () const           { return cend(); }
	const_iterator cbegin() const           { return const_iterator(this, const_pointer(this, 0)); }
	const_iterator cend  () const           { return const_iterator(this, const_pointer(this, size())); }
    reverse_iterator rbegin  ()             { return reverse_iterator(end()); }
    reverse_iterator rend    ()             { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin  () const { return crbegin(); }
    const_reverse_iterator rend    () const { return crend(); }
    const_reverse_iterator crbegin () const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend   () const { return const_reverse_iterator(cbegin()); }

    const_data_pointer constData () const;
    const_data_pointer data () const;
    const char   * c_str () const;

    value_type valueAt (size_type index) const { return at(index); }
    reference at (size_type index) const
    {
    	pointer p(const_cast<self_class *>(this), 0);
    	p += index;
    	return p.ref();
    }
    reference operator [] (size_type index) const { return at(index); }

    iterator find (const_iterator pos, const byte_string & str) const;

    iterator find (const byte_string & str) const
    {
    	return find(cbegin(), str);
    }

    iterator find (const_iterator pos, const char * s, size_type count) const;
    iterator find (const_iterator pos, const char * s) const;
    iterator find (const char * s) const
    {
    	return find(cbegin(), s);
    }

    iterator find (const_iterator pos, const_data_pointer bytes, size_type count) const;

    iterator find (const_iterator pos, char ch) const
    {
    	return find(pos, & ch, 1);
    }

    iterator find (char ch) const
    {
    	return find(cbegin(), & ch, 1);
    }

    iterator find (const_iterator pos, byte_t byte) const
    {
    	return find(pos, & byte, 1);
    }

    iterator find (byte_t byte) const
    {
    	return find(cbegin(), & byte, 1);
    }

	bool contains   (const byte_string & str) const { return find(str) != cend(); }
	bool contains   (const char * s) const          { return find(s)   != cend(); }
	bool contains   (byte_t v) const                { return find(v)   != cend(); }
	bool contains   (char v) const                  { return find(v)   != cend(); }

	bool startsWith (const byte_string & str) const { return find(str) == cbegin(); }
	bool startsWith (const char * s) const          { return find(s)   == cbegin(); }
	bool startsWith (byte_t v) const                { return find(v)   == cbegin(); }
	bool startsWith (char v) const                  { return find(v)   == cbegin(); }

	bool endsWith   (const byte_string & str) const { return find(str) == cend() - str.length(); }
	bool endsWith   (const char * s) const          { return find(s)   == cend() - std::strlen(s); }
	bool endsWith   (byte_t v) const                { return find(v)   == cend() - 1; }
	bool endsWith   (char v) const                  { return find(v)   == cend() - 1; }

    int compare (size_type pos1, size_type count1, const_data_pointer bytes, size_type count2) const;
    int compare (size_type pos1, size_type count1, const char * s, size_type count2) const;
    int compare (size_type pos1, size_type count1, const byte_string & s, size_type pos2, size_type count2) const;
    int compare (const_data_pointer bytes, size_type count2) const;
    int compare (size_type pos1, size_type count1, const char * s) const;

    int compare (size_type pos1, size_type count1, const byte_string & s) const
    {
    	return compare(pos1, count1, s.constData(), s.size());
    }

    int compare (const byte_string & s) const
    {
    	return compare(s.constData(), s.length());
    }

    int compare (const char *s, size_type count2) const
    {
    	return compare(0, this->length(), s, count2);
    }

    int compare (const char * s) const
    {
    	return compare(0, this->length(), s);
    }

    void push_back (char ch)
    {
    	append(size_type(1), ch);
    }

    void push_back (byte_t byte)
    {
    	append(size_type(1), byte);
    }

    void pop_back ()
    {
    	if (!isEmpty())
    		erase(length() - 1);
    }

    byte_string & append (const byte_string & str, size_type index_str, size_type count)
    {
    	return insert(this->length(), str, index_str, count);
    }

    byte_string & append (size_type count, char ch)
    {
    	return insert(this->length(), count, ch);
    }

    byte_string & append (size_type count, byte_t byte)
    {
    	return insert(this->length(), count, byte);
    }

    byte_string & append (const char * s)
    {
    	return insert(this->length(), s);
    }

    byte_string & append (const char * s, size_type n)
    {
    	return insert(this->length(), s, n);
    }

    byte_string & append (const_data_pointer bytes, size_type n)
    {
    	return insert(this->length(), bytes, n);
    }

    byte_string & append (const byte_string & str)
    {
    	return insert(this->length(), str);
    }

    void push_front (char ch)
    {
    	prepend(size_type(1), ch);
    }

    void push_front (byte_t byte)
    {
    	prepend(size_type(1), byte);
    }

    void pop_front ()
    {
    	if (!isEmpty())
    		erase(0);
    }

    byte_string & prepend (const byte_string & str, size_type index_str, size_type count)
    {
    	return insert(0, str, index_str, count);
    }

    byte_string & prepend (size_type count, char ch)
    {
    	return insert(0, count, ch);
    }

    byte_string & prepend (size_type count, byte_t byte)
    {
    	return insert(0, count, byte);
    }

    byte_string & prepend (const char * s)
    {
    	return insert(0, s);
    }

    byte_string & prepend (const char * s, size_type n)
    {
    	return insert(0, s, n);
    }

    byte_string & prepend (const_data_pointer bytes, size_type n)
    {
    	return insert(0, bytes, n);
    }

    byte_string & prepend (const byte_string & str)
    {
    	return insert(0, str);
    }

    byte_string & insert (size_type index, const byte_string & str, size_type index_str, size_type count);

    byte_string & insert (size_type index, size_type count, char ch)
    {
    	return insert(index, byte_string(count, ch));
    }

    byte_string & insert (size_type index, size_type count, byte_t byte)
    {
    	return insert(index, byte_string(count, byte));
    }

    byte_string & insert (size_type index, const char * s)
    {
    	return insert(index, byte_string(s));
    }

    byte_string & insert (size_type index, const char * s, size_type n)
    {
    	return insert(index, byte_string(s, n));
    }

    byte_string & insert (size_type index, const_data_pointer bytes, size_type n)
    {
    	return insert(index, byte_string(bytes, n));
    }

    byte_string & insert (size_type index, const byte_string & str)
    {
    	return insert(index, str, 0, str.length());
    }

//    iterator insert (const_iterator pos, char latin1)
//    {
//    	mbcs_string s(1, latin1);
//    	return insert(pos, s.cbegin(), s.cend());
//    }
//
//    iterator insert (const_iterator pos, ucchar ch)
//    {
//    	mbcs_string s(1, ch);
//    	return insert(pos, s.cbegin(), s.cend());
//    }
//
//    iterator insert (const_iterator pos, size_type count, char latin1)
//    {
//    	mbcs_string s(count, latin1);
//    	return insert(pos, s.cbegin(), s.cend());
//    }
//
//    iterator insert (const_iterator pos, size_type count, ucchar ch)
//    {
//    	mbcs_string s(count, ch);
//    	return insert(pos, s.cbegin(), s.cend());
//    }
//
//    template<class InputIt>
//    iterator insert (const_iterator pos, InputIt first, InputIt last)
//    {
//    	return insert(pos, first, last, mbcs_string_type_trait<InputIt>());
//    }

    byte_string & replace (size_type pos1, size_type count1, const byte_string & str, size_type pos2, size_type count2);
    byte_string & replace (size_type pos, size_type count, const byte_string & str)
    {
    	return replace(pos, count, str, 0, str.length());
    }

    byte_string & replace (size_type pos1, size_type count1, const_data_pointer bytes, size_type count2);
    byte_string & replace (size_type pos1, size_type count1, const char * s, size_type count2)
    {
    	return replace(pos1, count1, reinterpret_cast<const_data_pointer>(s), count2);
    }

    byte_string & replace (size_type pos1, size_type count1, const char * s);

    byte_string & replace (size_type pos, size_type count, size_type count2, byte_t byte)
    {
    	return replace(pos, count, byte_string(count2, byte));
    }

    byte_string & replace (size_type pos, size_type count, size_type count2, char ch)
    {
    	return replace(pos, count, byte_string(count2, ch));
    }

    byte_string & replace (const_iterator first, const_iterator last, const byte_string & str)
    {
    	return replace(first, last, str.cbegin(), str.cend(), extra_trait<const_iterator>());
    }

    template< class InputIt >
    byte_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2)
    {
    	return replace(first, last, first2, last2, extra_trait<InputIt>());
    }

	byte_string substr (size_type index, size_type count) const;

	template <typename T>
	size_t readNumber (T & v, size_t pos = 0, endian::type_enum order = endian::nativeOrder()) const;

	integral_t toIntegral (bool * ok = 0, int base = 10) const;
	uintegral_t toUIntegral (bool * ok = 0, int base = 10) const;

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

	byte_string        toBase64 () const;
	static byte_string fromBase64 (const byte_string & base64);

	byte_string & operator += (const byte_string & other) { return append(other); }
	byte_string & operator += (const char * s) { return append(s); }
	byte_string & operator += (char ch) { return append(1, ch); }
	byte_string & operator += (byte_t byte) { return append(1, byte); }
	byte_string & operator << (const byte_string & other) { return append(other); }
	byte_string & operator << (const char * s) { return append(s); }
	byte_string & operator << (char ch) { return append(1, ch); }
	byte_string & operator << (byte_t byte) { return append(1, byte); }

	void detach_and_assign (pointer & p, const value_type & value); // pfs::reference class requirement

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


	// XXX DEPRECATED toString() functions, use lexical_cast() instead.
	static byte_string toString (int value, int base = 10, bool uppercase = false);
	static byte_string toString (long value, int base = 10, bool uppercase = false);
	static byte_string toString (unsigned int value, int base = 10, bool uppercase = false);
	static byte_string toString (unsigned long value, int base = 10, bool uppercase = false);
#ifdef PFS_HAVE_LONGLONG
	static byte_string toString (long long value, int base = 10, bool uppercase = false);
	static byte_string toString (unsigned long long value, int base = 10, bool uppercase = false);
#endif
	static byte_string toString (float value, char f = 'f', int prec = 6);
	static byte_string toString (double value, char f = 'f', int prec = 6);
#ifdef PFS_HAVE_LONG_DOUBLE
	static byte_string toString (long double value, char f = 'f', int prec = 6);
#endif

	static byte_string fromLatin1 (const char * latin1, size_t n) { return byte_string(latin1, n); }
	static byte_string fromLatin1 (const char * latin1) { return byte_string(latin1); }  // used in safeformat e.g.
};


#ifdef __COMMENT__
template <typename _CodeUnitT>
template <typename InputIt>
typename mbcs_string<_CodeUnitT>::iterator mbcs_string<_CodeUnitT>::insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>)
{
	PFS_ASSERT(pos.holder() == this);

	size_type index = pos.base().index();

	if (first >= last)
		return iterator(this, pointer(*this, index));

	for (InputIt it = first; it < last; ++it) {
		this->insert(pos++, *it);
	}

	return iterator(this, pointer(*this, index + 1));
}
#endif // __COMMENT__

template <typename InputIt>
byte_string & byte_string::replace (
		  const_iterator first
		, const_iterator last
		, InputIt first2
		, InputIt last2
		, extra_trait<InputIt>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(last.holder() == this);

	byte_string s;

	for (InputIt it = first2; it < last2; ++it) {
		s.append(*it);
	}

	return replace(first, last, s);
}

//
// For integers only supported by endian class
//
template <typename T>
inline byte_string byte_string::toBytes (const T & v, endian::type_enum order)
{
	T a = ((order == endian::LittleEndian) ? endian::toLittleEndian<T>(v) : endian::toBigEndian<T>(v));
	union { T v; char b[sizeof(T)]; } d;
	d.v = a;
	return byte_string(d.b, sizeof(T));
}

//
// Specialization for bool
//
template <>
inline byte_string byte_string::toBytes<bool> (const bool & v, endian::type_enum order)
{
	return toBytes<char>(v ? '\x01' : '\x00', order);
}

//
// Specialization for float
// TODO as mentioned at http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#serialization
//
template <>
inline byte_string byte_string::toBytes<float> (const float & v, endian::type_enum order)
{
	PFS_UNUSED2(v, order);
	PFS_ASSERT_TODO();
	return byte_string();
}

//
// Specialization for double
// TODO as for float
//
template <>
inline byte_string byte_string::toBytes<double> (const double & v, endian::type_enum order)
{
	PFS_UNUSED2(v, order);
	PFS_ASSERT_TODO();
	return byte_string();
}

#ifdef PFS_HAVE_LONG_DOUBLE
//
// Specialization for long double
// TODO as for float
//
template <>
inline byte_string byte_string::toBytes<long double> (const long double & v, endian::type_enum order)
{
	PFS_UNUSED2(v, order);
	PFS_ASSERT_TODO();
	return byte_string();
}
#endif // PFS_HAVE_LONG_DOUBLE

//
// Specialization for byte_string
//
template <>
inline byte_string byte_string::toBytes<byte_string> (const byte_string & v, endian::type_enum /*order*/)
{
	return byte_string(v);
}

//
// Specialization for utf8_string
//
template <>
inline byte_string byte_string::toBytes<utf8_string> (const utf8_string & v, endian::type_enum /*order*/)
{
	return byte_string(v.constData(), v.size());
}

//
// For integers only supported by endian class
//
template <typename T>
byte_string::size_type byte_string::readNumber (T & v, size_t pos, endian::type_enum order) const
{
	if (size() - pos < sizeof(T))
		return 0;
	union u { const T v; const char b[sizeof(T)]; };
	const char * s = c_str() + pos;
	const u * d = reinterpret_cast<const u *>(s/*constData() + pos*/);
	v = (order == endian::LittleEndian) ? endian::toLittleEndian(d->v) : endian::toBigEndian(d->v);
	return sizeof(T);
}

//XXX DEPRECATED
inline byte_string byte_string::toString (int value, int base, bool uppercase)
{
	char buf[65];
	return byte_string(pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

//XXX DEPRECATED
inline byte_string byte_string::toString (long value, int base, bool uppercase)
{
	char buf[65];
	return byte_string(pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

//XXX DEPRECATED
inline byte_string byte_string::toString (unsigned int value, int base, bool uppercase)
{
	char buf[65];
	return byte_string(pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

//XXX DEPRECATED
inline byte_string byte_string::toString (unsigned long value, int base, bool uppercase)
{
	char buf[65];
	return byte_string(pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

#ifdef PFS_HAVE_LONGLONG
//XXX DEPRECATED
inline byte_string byte_string::toString (long long value, int base, bool uppercase)
{
	char buf[65];
	return byte_string(pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

//XXX DEPRECATED
inline byte_string byte_string::toString (unsigned long long value, int base, bool uppercase)
{
	char buf[65];
	return byte_string(pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}
#endif

//XXX DEPRECATED
inline byte_string byte_string::toString (float value, char f, int prec)
{
	int sz = 65;
	char buf[65];
	return byte_string(pfs_real_to_string(real_t(value), f, prec, buf, & sz));
}

//XXX DEPRECATED
inline byte_string byte_string::toString (double value, char f, int prec)
{
	int sz = 129;
	char buf[129];
	return byte_string(pfs_real_to_string(real_t(value), f, prec, buf, & sz));
}

#ifdef PFS_HAVE_LONG_DOUBLE
//XXX DEPRECATED
inline byte_string byte_string::toString (long double value, char f, int prec)
{
	int sz = 129;
	char buf[129];
	return byte_string(pfs_real_to_string(real_t(value), f, prec, buf, & sz));
}
#endif

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

inline bool operator == (const byte_string::reference & o, char ch)
{
	return o == static_cast<byte_string::value_type>(ch);
}

inline bool operator == (const byte_string::const_reference & o, char ch)
{
	return o == static_cast<byte_string::value_type>(ch);
}

inline bool operator != (const byte_string::reference & o, char ch)
{
	return o != static_cast<byte_string::value_type>(ch);
}

inline bool operator != (const byte_string::const_reference & o, char ch)
{
	return o != static_cast<byte_string::value_type>(ch);
}

inline bool operator > (const byte_string::reference & o, char ch)
{
	return o > static_cast<byte_string::value_type>(ch);
}

inline bool operator > (const byte_string::const_reference & o, char ch)
{
	return o > static_cast<byte_string::value_type>(ch);
}

inline bool operator >= (const byte_string::reference & o, char ch)
{
	return o >= static_cast<byte_string::value_type>(ch);
}

inline bool operator >= (const byte_string::const_reference & o, char ch)
{
	return o >= static_cast<byte_string::value_type>(ch);
}

inline bool operator < (const byte_string::reference & o, char ch)
{
	return o < static_cast<byte_string::value_type>(ch);
}

inline bool operator < (const byte_string::const_reference & o, char ch)
{
	return o < static_cast<byte_string::value_type>(ch);
}

inline bool operator <= (const byte_string::reference & o, char ch)
{
	return o <= static_cast<byte_string::value_type>(ch);
}

inline bool operator <= (const byte_string::const_reference & o, char ch)
{
	return o <= static_cast<byte_string::value_type>(ch);
}

inline std::ostream & operator << (std::ostream & os, const byte_string & o)
{
	os << o.c_str();
	return os;
}

} // pfs

#include <pfs/bits/byte_string_impl_inc.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BYTE_STRING_HPP__ */
