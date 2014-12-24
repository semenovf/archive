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
//#include <pfs/bits/byte_string_impl.hpp>
//#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

//template <typename T>
//struct mbcs_string_type_trait { typedef T type; };

class byte_string_impl;

class DLL_API byte_string : public nullable<byte_string_impl>
{
//	friend class utf8string_impl;
//	friend class utf8string_ptr;

protected:
	typedef byte_string_impl     impl_class;
	typedef nullable<impl_class> base_class;
	typedef byte_string          self_class;

public:
	typedef byte_t              value_type;
	typedef byte_t *            pointer;
	typedef const byte_t *      const_pointer;
	typedef size_t              size_type;
//	typedef impl_class::difference_type difference_type;

	typedef pfs::reference<self_class>                    reference;
	typedef pfs::reference<const self_class>              const_reference;
	typedef pfs::random_access_iterator<self_class>       iterator;
	typedef pfs::random_access_iterator<const self_class> const_iterator;
	typedef std::reverse_iterator<iterator>		          reverse_iterator;
	typedef std::reverse_iterator<const_iterator>         const_reverse_iterator;

	typedef value_type char_type;

//	typedef impl_class::const_pointer const_data_pointer;

public: // static
	static const char TerminatorChar  = '\0';
	static const char EndOfLineChar   = '\n';
	static const char ReplacementChar = '?';

public:
	byte_string () : base_class() {}
	explicit byte_string (const char * str);
	byte_string (const_pointer bytes, size_type n);
	byte_string (const char * str, size_type n);
	byte_string (size_t count, byte_t ch);
	byte_string (size_t count, char ch);
//	mbcs_string (const_iterator first, const_iterator last);

	virtual ~byte_string () {}

	bool isEmpty () const;
	bool empty () const { return isEmpty(); }
	void clear ();

#ifdef __COMMENT__
	mbcs_string & erase (size_type index = 0) { return erase(index, 1); }
	mbcs_string & erase (size_type index, size_type count);
	iterator erase (const_iterator pos) { return erase(pos, pos + 1); }
	iterator erase (const_iterator first, const_iterator last);

    iterator begin ()                       { return iterator(this, pointer(*this, 0)); }
    iterator end   ()                       { return iterator(this, pointer(*this, size())); }
    const_iterator begin () const           { return cbegin(); }
    const_iterator end   () const           { return cend(); }
    const_iterator cbegin() const           { return const_iterator(this, const_pointer(*this, 0)); }
    const_iterator cend  () const           { return const_iterator(this, const_pointer(*this, size())); }
    reverse_iterator rbegin  ()             { return reverse_iterator(end()); }
    reverse_iterator rend    ()             { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin  () const { return crbegin(); }
    const_reverse_iterator rend    () const { return crend(); }
    const_reverse_iterator crbegin () const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend   () const { return const_reverse_iterator(cbegin()); }

#endif

    const_pointer constData () const;
    const_pointer data () const;
    const char   * c_str () const;

#ifdef __COMMENT__
    value_type valueAt (size_type index) const { return at(index); }
    reference at (size_type index) const { pointer p(*const_cast<self_class *>(this), 0); p += index; return p.ref(); }
    reference operator [] (size_type index) const { return at(index); }

    // Non-lexical find
    iterator find (const mbcs_string & str, const_iterator pos) const;

    iterator find (const mbcs_string & str) const
    {
    	return find(str, cbegin());
    }

    iterator find (const char * latin1, const_iterator pos, size_type count) const
    {
    	return find(mbcs_string::fromLatin1(latin1, count), pos);
    }

    iterator find (const char * latin1, const_iterator pos) const
    {
    	return find(mbcs_string::fromLatin1(latin1, strlen(latin1)), pos);
    }

    iterator find (char latin1, const_iterator pos) const
    {
    	return find(& latin1, pos, 1);
    }

    iterator find (char latin1) const
    {
    	return find(latin1, cbegin());
    }

    iterator find (ucchar ch, const_iterator pos) const
    {
    	return find(mbcs_string(1, ch), pos);
    }

    iterator find (ucchar ch) const
    {
    	return find(ch, cbegin());
    }
#endif
    size_type size () const;
    size_type length () const; // Equivalent to size()
    size_type capacity() const;
    size_type max_size() const;

    int compare (size_type pos1, size_type count1, const_pointer bytes, size_type count2) const;
    int compare (size_type pos1, size_type count1, const char * s, size_type count2) const;
    int compare (size_type pos1, size_type count1, const byte_string & s, size_type pos2, size_type count2) const;
    int compare (const_pointer bytes, size_type count2) const;
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

#ifdef __COMMENT__
    void push_back (value_type ch)
    {
    	append(size_type(1), ch);
    }

    void pop_back ()
    {
    	if (!isEmpty())
    		erase(length() - 1);
    }

    mbcs_string & append (const mbcs_string & str, size_type index_str, size_type count)
    {
    	return insert(this->length(), str, index_str, count);
    }

    mbcs_string & append (size_type count, char latin1)
    {
    	return insert(this->length(), count, latin1);
    }

    mbcs_string & append (size_type count, ucchar uc)
    {
    	return insert(this->length(), count, uc);
    }

    mbcs_string & append (const char * latin1)
    {
    	return insert(this->length(), latin1);
    }

    mbcs_string & append (const char * latin1, size_type n)
    {
    	return insert(this->length(), latin1, n);
    }

    mbcs_string & append (const mbcs_string & str)
    {
    	return insert(this->length(), str);
    }

    template<class InputIt>
    mbcs_string & append (InputIt first, InputIt last);

    mbcs_string & insert (size_type index, const mbcs_string & str, size_type index_str, size_type count);

    mbcs_string & insert (size_type index, size_type count, char latin1)
    {
    	return insert(index, self_class(count, latin1));
    }

    mbcs_string & insert (size_type index, size_type count, ucchar uc)
    {
    	return insert(index, mbcs_string(count, uc));
    }

    mbcs_string & insert (size_type index, const char * latin1)
    {
    	return insert(index, mbcs_string(latin1));
    }

    mbcs_string & insert (size_type index, const char * latin1, size_type n)
    {
    	return insert(index, mbcs_string(latin1, n));
    }

    mbcs_string & insert (size_type index, const mbcs_string & str)
    {
    	return insert(index, str, 0, str.length());
    }

    iterator insert (const_iterator pos, char latin1)
    {
    	mbcs_string s(1, latin1);
    	return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, ucchar ch)
    {
    	mbcs_string s(1, ch);
    	return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, size_type count, char latin1)
    {
    	mbcs_string s(count, latin1);
    	return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, size_type count, ucchar ch)
    {
    	mbcs_string s(count, ch);
    	return insert(pos, s.cbegin(), s.cend());
    }

    template<class InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
    	return insert(pos, first, last, mbcs_string_type_trait<InputIt>());
    }

    mbcs_string & replace (size_type pos1, size_type count1, const mbcs_string & str, size_type pos2, size_type count2);

    mbcs_string & replace (size_type pos, size_type count, const mbcs_string & str)
    {
    	return replace(pos, count, str, 0, str.length());
    }

    mbcs_string & replace (const_iterator first, const_iterator last, const mbcs_string & str)
    {
    	return replace(first, last, str.cbegin(), str.cend(), mbcs_string_type_trait<const_iterator>());
    }

    template< class InputIt >
    mbcs_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2)
    {
    	return replace(first, last, first2, last2, mbcs_string_type_trait<InputIt>());
    }

    mbcs_string & replace ( size_type pos, size_type count, size_type count2, ucchar ch )
    {
    	return replace (pos, count, mbcs_string(count2, ch));
    }

    mbcs_string & replace (const_iterator first, const_iterator last, size_type count2, ucchar ch)
    {
    	return replace(first, last, mbcs_string(count2, ch));
    }

	mbcs_string substr (size_type index, size_type count) const;

	mbcs_string & operator += (const mbcs_string & other) { return append(other); }
	mbcs_string & operator += (const char * latin1) { return append(mbcs_string(latin1)); }
	mbcs_string & operator += (const ucchar & ch) { return append(mbcs_string(1, ch)); }
	mbcs_string & operator += (char latin1) { return append(mbcs_string(1, latin1)); }

//	template <typename _IntT>
//	mbcs_string & setNumber (_IntT n, int base = 10, bool uppercase = false);

//	template <typename _RealT>
//	mbcs_string & setNumber (_RealT n, char f = 'g', int prec = 6);

//	utf8string & setNumber (long_t n, int base = 10, bool uppercase = false);
//	utf8string & setNumber (ulong_t n, int base = 10, bool uppercase = false);
//	utf8string & setNumber (int_t n, int base = 10, bool uppercase = false)    { return setNumber(long_t(n), base, uppercase); }
//	utf8string & setNumber (uint_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
//	utf8string & setNumber (short_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
//	utf8string & setNumber (ushort_t n, int base = 10, bool uppercase = false) { return setNumber(ulong_t(n), base, uppercase); }
//	utf8string & setNumber (sbyte_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
//	utf8string & setNumber (byte_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
//	utf8string & setNumber (real_t n, char f = 'g', int prec = 6);
#endif

private:
#ifdef __COMMENT__
    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, mbcs_string_type_trait<const_iterator>);

    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    mbcs_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2, mbcs_string_type_trait<InputIt>);
    mbcs_string & replace (const_iterator first, const_iterator last, const_iterator first2, const_iterator last2, mbcs_string_type_trait<const_iterator>);
#endif
public:
	template <typename T>
	static byte_string toBytes (const T & v, endian::type_enum order = endian::nativeOrder());
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


template <typename _CodeUnitT>
template <typename InputIt>
mbcs_string<_CodeUnitT> & mbcs_string<_CodeUnitT>::replace (
		  const_iterator first
		, const_iterator last
		, InputIt first2
		, InputIt last2
		, mbcs_string_type_trait<InputIt>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(last.holder() == this);

	mbcs_string<_CodeUnitT> s;

	for (InputIt it = first2; it < last2; ++it) {
		s.append(*it);
	}

	return replace(first, last, s);
}
#endif // __COMMENT__


template <typename T>
inline byte_string byte_string::toBytes (const T & v, endian::type_enum order)
{
	T a = (order == endian::LittleEndian ? endian::toLittleEndian(v) : endian::toBigEndian(v));
	union { T v; char b[sizeof(T)]; } d;
	d.v = a;
	return byte_string(d.b, sizeof(T));
}

// Specialization for bool
//
template <>
inline byte_string byte_string::toBytes<bool> (const bool & v, endian::type_enum order)
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

inline std::ostream & operator << (std::ostream & os, const byte_string & o)
{
	os << o.c_str();
	return os;
}

} // pfs

//#include <pfs/bits/mbcs_string_impl_inc.hpp>
//#include <pfs/bits/mbcs_string_inc.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_BYTE_STRING_HPP__ */
