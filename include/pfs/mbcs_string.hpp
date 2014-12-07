/*
 * mbcs_string.hpp
 *
 *      Author: wladt
 */

#ifndef __PFS_MBCS_STRING_HPP__
#define __PFS_MBCS_STRING_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/iterator.hpp>
#include <pfs/endian.hpp>
#include <pfs/ucchar.hpp>
#include <pfs/bits/mbcs_string_impl.hpp>
#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

template <typename T>
struct mbcs_string_insert_trait { typedef T type; };

//TODO implement `class utf16string : protected vector<int16_t,allocator<int16_t, 1> >'

template <typename _CodeUnitT>
class mbcs_string : public nullable<mbcs_string_impl<_CodeUnitT> >
{
//	friend class utf8string_impl;
//	friend class utf8string_ptr;

protected:
	typedef nullable<mbcs_string_impl<_CodeUnitT> > base_class;
	typedef mbcs_string                             self_class;
	typedef mbcs_string_impl<_CodeUnitT>            impl_class;

public:
	typedef ucchar                           value_type; // Unicode character
	typedef mbcs_string_ptr<_CodeUnitT, self_class>       pointer;
	typedef mbcs_string_ptr<_CodeUnitT, const self_class> const_pointer;
	typedef typename impl_class::size_type            size_type;
	typedef typename impl_class::difference_type      difference_type;

	typedef pfs::reference<self_class>                    reference;
	typedef pfs::reference<const self_class>              const_reference;
	typedef pfs::random_access_iterator<self_class>       iterator;
	typedef pfs::random_access_iterator<const self_class> const_iterator;
	typedef std::reverse_iterator<iterator>		          reverse_iterator;
	typedef std::reverse_iterator<const_iterator>         const_reverse_iterator;

	typedef value_type char_type;

public: // static
	static const char TerminatorChar  = '\0';
	static const char EndOfLineChar   = '\n';
	static const char ReplacementChar = '?';

public:
	mbcs_string () : base_class() {}
	explicit mbcs_string (const char * latin1);
	mbcs_string (const char * latin1, size_t n);
	mbcs_string (size_t count, char latin1);
	mbcs_string (size_t count, ucchar ch);
	mbcs_string (const_iterator first, const_iterator last);

	virtual ~mbcs_string () {}

	bool isEmpty () const { return base_class::isNull() || base_class::cast()->isEmpty(); }
	bool empty () const { return isEmpty(); }
	void clear () { base_class::detach(); base_class::cast()->clear(); }

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

	typename impl_class::const_pointer constData () const { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
	typename impl_class::const_pointer data () const      { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
	typename impl_class::const_pointer c_str () const     { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }

    value_type valueAt (size_type index) const { return at(index); }
    reference at (size_type index) const { pointer p(*const_cast<self_class *>(this), 0); p += index; return p.ref(); }
    reference operator [] (size_type index) const { return at(index); }

    // Size in bytes
    size_type size () const
    {
    	return base_class::isNull() ? 0 : base_class::cast()->size();
    }

    // Length in unicode chars
    size_type length () const
    {
    	return base_class::isNull() ? 0 : base_class::cast()->length();
    }

    size_type capacity() const
    {
    	return base_class::cast()->capacity();
    }

    size_type max_size() const
    {
    	return base_class::cast()->max_size();
    }

    int compare (size_type pos1, size_type count1, const char * s, size_type count2) const;
    int compare (size_type pos1, size_type count1, const mbcs_string & s) const
    {
    	return compare(pos1, count1, s.constData(), s.size());
    }

    int compare (const mbcs_string & s) const
    {
    	return compare(s.constData());
    }

    int compare (size_type pos1, size_type count1, const mbcs_string & s, size_type pos2, size_type count2) const;
    int compare (const char * s) const;
    int compare (size_type pos, size_type count, const char * s) const;

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
    	return insert(pos, first, last, mbcs_string_insert_trait<InputIt>());
    }

    mbcs_string & replace (size_type pos, size_type count, const mbcs_string & str ); //	(1)
    mbcs_string & replace (const_iterator first, const_iterator last, const basic_string & str); // 	(1)    	(2)
    mbcs_string & replace (size_type pos, size_type count, const basic_string & str, size_type pos2, size_type count2 ); // (until C++14)
    mbcs_string & replace (size_type pos, size_type count, const basic_string & str, size_type pos2, size_type count2 = npos); // (since C++14)

    template< class InputIt >
    mbcs_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2); // (3)
    basic_string& replace (size_type pos, size_type count, const CharT* cstr, size_type count2);
    	(4)
    basic_string& replace( const_iterator first, const_iterator last,
                           const CharT* cstr, size_type count2 );
    	(4)
    basic_string& replace( size_type pos, size_type count,
                           const CharT* cstr );
    	(5)
    basic_string& replace( const_iterator first, const_iterator last,
                           const CharT* cstr );
    	(5)
    basic_string& replace( size_type pos, size_type count,
                           size_type count2, CharT ch );
    	(6)
    basic_string& replace( const_iterator first, const_iterator last,
                           size_type count2, CharT ch );
    	(6)
    basic_string& replace( const_iterator first, const_iterator last,
                           std::initializer_list<CharT> ilist );
    	(7) 	(since C++11)


	mbcs_string substr (size_type index, size_type count) const;

	mbcs_string & operator += (const mbcs_string & other) { return append(other); }
	mbcs_string & operator += (const char * latin1) { return append(mbcs_string(latin1)); }
	mbcs_string & operator += (const ucchar & ch) { return append(mbcs_string(1, ch)); }
	mbcs_string & operator += (char latin1) { return append(mbcs_string(1, latin1)); }

//	template <typename _IntT>
//	mbcs_string & setNumber (_IntT n, int base = 10, bool uppercase = false);
//
//	template <typename _FloatT>
//	mbcs_string & setNumber (_FloatT n, char f = 'g', int prec = 6);

//	utf8string & setNumber (long_t n, int base = 10, bool uppercase = false);
//	utf8string & setNumber (ulong_t n, int base = 10, bool uppercase = false);
//	utf8string & setNumber (int_t n, int base = 10, bool uppercase = false)    { return setNumber(long_t(n), base, uppercase); }
//	utf8string & setNumber (uint_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
//	utf8string & setNumber (short_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
//	utf8string & setNumber (ushort_t n, int base = 10, bool uppercase = false) { return setNumber(ulong_t(n), base, uppercase); }
//	utf8string & setNumber (sbyte_t n, int base = 10, bool uppercase = false)  { return setNumber(long_t(n), base, uppercase); }
//	utf8string & setNumber (byte_t n, int base = 10, bool uppercase = false)   { return setNumber(ulong_t(n), base, uppercase); }
//	utf8string & setNumber (real_t n, char f = 'g', int prec = 6);

private:
    // Need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template<class InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_insert_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, mbcs_string_insert_trait<const_iterator>);

public:
	struct ConvertState
	{
		ConvertState() : nremain(0), invalidChars(0), replacementChar(ucchar::ReplacementChar) {}
		size_type nremain;
		size_type invalidChars;
		ucchar    replacementChar;
	};

	static DLL_API mbcs_string fromLatin1 (const char * latin1, size_t n, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromLatin1 (const char * latin1, ConvertState * state = nullptr);
//	static mbcs_string fromLatin1 (const pfs::bytearray & latin1, ConvertState * state = nullptr)
//		{ return fromLatin1(latin1.data(), latin1.size(), state); }

	static DLL_API mbcs_string fromUtf8 (const char * utf8, size_t size, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromUtf8 (const char * utf8, ConvertState * state = nullptr);
//	static mbcs_string fromUtf8 (const bytearray & utf8, ConvertState * state = nullptr)
//		{ return fromUtf8(utf8.data(), utf8.size(), state);	}
};

template <typename _CodeUnitT>
template<class InputIt>
typename mbcs_string<_CodeUnitT>::iterator mbcs_string<_CodeUnitT>::insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_insert_trait<InputIt>)
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
inline std::ostream & operator << (std::ostream & os, const mbcs_string<_CodeUnitT> & o)
{
// FIXME is c_str() that function to use in this case?
	os << o.c_str();
	return os;
}

} // pfs

#include <pfs/bits/mbcs_string_impl_inc.hpp>
#include <pfs/bits/mbcs_string_inc.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_MBCS_STRING_HPP__ */
