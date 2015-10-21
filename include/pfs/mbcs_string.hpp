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
#include <pfs/bits/strtointegral.hpp>
#include <pfs/bits/strtoreal.hpp>
#include <pfs/bits/strtoreal.hpp>
#include <pfs/vector.hpp>
#include <cstring>
#include <ostream>

// See http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class byte_string;

template <typename T>
struct mbcs_string_type_trait { typedef T type; };

struct ConvertState
{
	ConvertState() : nremain(0), invalidChars(0), replacementChar(ucchar::ReplacementChar) {}
	size_t nremain;
	size_t invalidChars;
	ucchar replacementChar;
};


template <typename CodeUnitT>
class mbcs_string : public nullable<mbcs_string_impl<CodeUnitT> >
{
protected:
	typedef nullable<mbcs_string_impl<CodeUnitT> > base_class;
	typedef mbcs_string                            self_class;
	typedef mbcs_string_impl<CodeUnitT>            impl_class;

public:
	typedef vector<self_class> stringlist;

public:
	// Helper variables for split method
	//
	static const bool KeepEmpty = true; // TODO DEPRECATED
	static const bool DontKeepEmpty = false; // TODO DEPRECATED

public:
	class iterator : public random_access_iterator<self_class>
	{
	public:
		typedef typename random_access_iterator<self_class>::pointer pointer;
		typedef typename random_access_iterator<self_class>::size_type size_type;
		typedef typename random_access_iterator<self_class>::difference_type difference_type;

		iterator () : random_access_iterator<self_class>() {}
		iterator (self_class * holder, pointer ptr) : random_access_iterator<self_class>(holder, ptr) {}
		iterator (const iterator & it) : random_access_iterator<self_class>(it.holder(), it.base())	{}

		friend iterator operator + (const iterator & i, size_type n)
		{
			return iterator(i.holder(), i.base() + n);
		}

		friend iterator operator + (size_type n, const iterator & i)
		{
			return iterator(i.holder(), i.base() + n);
		}

		friend iterator operator - (const iterator & i, size_type n)
		{
			return iterator(i.holder(), i.base() - n);
		}

		friend difference_type operator - (const iterator & i1, const iterator & i2)
		{
			return i1.base() - i2.base();
		}
	};

	class const_iterator : public random_access_iterator<const self_class>
	{
	public:
		typedef typename random_access_iterator<const self_class>::pointer pointer;
		typedef typename random_access_iterator<const self_class>::size_type size_type;
		typedef typename random_access_iterator<const self_class>::difference_type difference_type;

		const_iterator () : random_access_iterator<const self_class>() {}
		const_iterator (const self_class * holder, pointer ptr) : random_access_iterator<const self_class>(holder, ptr) {}
		const_iterator (const const_iterator & it) : random_access_iterator<const self_class>(it.holder(), it.base()) {}
		const_iterator (const iterator & it)
			: random_access_iterator<const self_class>(it.holder()
					, pointer(it.holder(), it.base().index())) {}

//		const_iterator & operator = (const iterator & it)
//		{
//			this->_holder = it.holder();
//			this->_ptr = pointer(it.holder(), it.base().index());
//			return *this;
//		}

		friend const_iterator operator + (const const_iterator & i, size_type n)
		{
			return const_iterator(i.holder(), i.base() + n);
		}

		friend const_iterator operator + (size_type n, const const_iterator & i)
		{
			return const_iterator(i.holder(), i.base() + n);
		}

		friend const_iterator operator - (const const_iterator & i, size_type n)
		{
			return const_iterator(i.holder(), i.base() - n);
		}

		friend difference_type operator - (const const_iterator & i1, const const_iterator & i2)
		{
			return i1.base() - i2.base();
		}
	};


	typedef ucchar value_type; // Unicode character
	typedef typename impl_class::size_type                size_type;
	typedef typename impl_class::difference_type          difference_type;

	typedef mbcs_string_ptr<CodeUnitT, self_class>        pointer;
	typedef mbcs_string_ptr<CodeUnitT, const self_class>  const_pointer;

	typedef pfs::reference<self_class>                    reference;
	typedef pfs::reference<const self_class>              const_reference;
//	typedef pfs::random_access_iterator<self_class>       iterator;
//	typedef pfs::random_access_iterator<const self_class> const_iterator;
	typedef std::reverse_iterator<iterator>		          reverse_iterator;
	typedef std::reverse_iterator<const_iterator>         const_reverse_iterator;

	typedef value_type char_type;

	typedef typename impl_class::const_pointer const_data_pointer;

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

	bool isEmpty () const { return base_class::isNull() || size() == 0; }
	bool empty () const { return isEmpty(); }
	void clear ()
	{
		if (!isEmpty()) {
			base_class::detach();
			base_class::cast()->clear();
		}
	}

	mbcs_string & erase (size_type index = 0) { return erase(index, 1); }
	mbcs_string & erase (size_type index, size_type count);
	iterator erase (const_iterator pos) { return erase(pos, pos + 1); }
	iterator erase (const_iterator first, const_iterator last);

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

    const_data_pointer constData () const { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
    const_data_pointer data () const      { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }

    // Applicable only for UTF8-encoded string
    const char * c_str () const;

    value_type valueAt (size_type index) const { return at(index); }
    value_type charAt (size_type index) const { return at(index); }
    reference at (size_type index) const { pointer p(const_cast<self_class *>(this), 0); p += index; return p.ref(); }
    reference operator [] (size_type index) const { return at(index); }

    int compare (size_type pos1, size_type count1, const mbcs_string & s, size_type pos2, size_type count2) const;

    int compare (size_type pos1, size_type count1, const char * latin1, size_type count2) const
    {
        return this->compare(pos1, count1, mbcs_string(latin1), 0, count2);
    }

    int compare (size_type pos, size_type count, const mbcs_string & s) const
    {
        return compare(pos, count, s, 0, s.length());
    }

    int compare (const mbcs_string & s) const
    {
        return compare(0, this->length(), s);
    }

    int compare (const char * latin1) const
    {
        return compare(mbcs_string(latin1));
    }

    int compare (size_type pos, size_type count, const char * latin1) const
    {
        return compare(pos, count, mbcs_string(latin1));
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, const mbcs_string & str) const;

    // TODO DEPRECATED
    iterator find (const mbcs_string & str) const
    {
    	return find(cbegin(), str);
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, const char * latin1, size_type count) const
    {
    	return find(pos, mbcs_string(latin1, count));
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, const char * latin1) const
    {
    	return find(pos, mbcs_string(latin1, strlen(latin1)));
    }

    // TODO DEPRECATED
    iterator find (const char * latin1) const
    {
    	return find(cbegin(), latin1);
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, char latin1) const
    {
    	return find(pos, & latin1, 1);
    }

    // TODO DEPRECATED
    iterator find (char latin1) const
    {
    	return find(cbegin(), latin1);
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, ucchar ch) const
    {
    	return find(pos, mbcs_string(1, ch));
    }

    // TODO DEPRECATED
    iterator find (ucchar ch) const
    {
    	return find(cbegin(), ch);
    }

    // TODO Need to implement
#ifdef __COMMENT__
    iterator rfind (const_iterator pos, const mbcs_string & str) const;

    iterator rfind (const mbcs_string & str) const
    {
    	return rfind(cbegin(), str);
    }

    iterator rfind (const_iterator pos, const char * latin1, size_type count) const
    {
    	return rfind(pos, mbcs_string::fromLatin1(latin1, count));
    }

    iterator rfind (const_iterator pos, const char * latin1) const
    {
    	return rfind(pos, mbcs_string::fromLatin1(latin1, strlen(latin1)));
    }

    iterator rfind (const char * latin1) const
    {
    	return rfind(cbegin(), latin1);
    }

    iterator rfind (const_iterator pos, char latin1) const
    {
    	return rfind(pos, & latin1, 1);
    }

    iterator rfind (char latin1) const
    {
    	return rfind(cbegin(), latin1);
    }

    iterator rfind (const_iterator pos, ucchar ch) const
    {
    	return rfind(pos, mbcs_string(1, ch));
    }

    iterator rfind (ucchar ch) const
    {
    	return rfind(cbegin(), ch);
    }
#endif

	bool contains   (const mbcs_string & s) const { return find(s) != cend(); }
	bool contains   (const char * latin1) const   { return find(latin1) != cend(); }
	bool contains   (ucchar ch) const             { return find(ch) != cend(); }
	bool contains   (char latin1) const           { return find(latin1) != cend(); }

	// TODO Need tests
	// FIXME need optimization like startsWith (char v)
	bool startsWith (const mbcs_string & s)  const { return find(s) == cbegin(); }
	bool startsWith (const_iterator pos, const mbcs_string & s) const
	{
		return find(pos, s) == pos;
	}
	// FIXME need optimization like startsWith (char v)
	bool startsWith (const char * latin1) const    { return find(latin1) == cbegin(); }

	bool startsWith (ucchar ch) const              { return isEmpty() ? false : *(cbegin()) == ch; }
	bool startsWith (char latin1) const            { return isEmpty() ? false : *(cbegin()) == ucchar(latin1); }

	// TODO Need tests
	bool endsWith   (const mbcs_string & s) const  { return (length() >= s.length()) ? startsWith(cend() - s.length(), s) : false; }
	bool endsWith   (const char * latin1) const    { return endsWith(mbcs_string(latin1)); }
	bool endsWith   (ucchar ch) const              { return endsWith(mbcs_string(1, ch)); }
	bool endsWith   (char latin1) const            { return endsWith(mbcs_string(1, latin1)); }

    // Size in code units
	//
    size_type size () const
    {
    	return base_class::isNull() ? 0 : base_class::cast()->size();
    }

    // Length in unicode chars
    //
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

    void push_back (value_type ch)
    {
    	append(size_type(1), ch);
    }

    void pop_back ()
    {
    	if (!isEmpty())
    		erase(length() - 1);
    }

    mbcs_string & insert (size_type index, const mbcs_string & str, size_type index_str, size_type count);

    mbcs_string & insert (size_type index, size_type count, char latin1)
    {
      return insert(index, mbcs_string(count, latin1));
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

//    template<class InputIt>
//    mbcs_string & append (InputIt first, InputIt last);

    mbcs_string & prepend (const mbcs_string & str, size_type index_str, size_type count)
    {
    	return insert(0, str, index_str, count);
    }

    mbcs_string & prepend (size_type count, char latin1)
    {
    	return insert(0, count, latin1);
    }

    mbcs_string & prepend (size_type count, ucchar uc)
    {
    	return insert(0, count, uc);
    }

    mbcs_string & prepend (const char * latin1)
    {
    	return insert(0, latin1);
    }

    mbcs_string & prepend (const char * latin1, size_type n)
    {
    	return insert(0, latin1, n);
    }

    mbcs_string & prepend (const mbcs_string & str)
    {
    	return insert(0, str);
    }

//    template<class InputIt>
//    mbcs_string & prepend (InputIt first, InputIt last)


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

    mbcs_string & replace (size_type pos, size_type count, size_type count2, ucchar ch)
    {
    	return replace (pos, count, mbcs_string(count2, ch));
    }

    mbcs_string & replace (const_iterator first, const_iterator last, size_type count2, ucchar ch)
    {
    	return replace(first, last, mbcs_string(count2, ch));
    }

    mbcs_string & replace ( const mbcs_string & before, const mbcs_string & after);

	mbcs_string substr (size_type index, size_type count) const;
	mbcs_string substr (const_iterator begin, size_t count) const;
	mbcs_string substr (const_iterator begin, const_iterator end) const;

	mbcs_string substr (const_iterator begin) const
	{
		return substr(begin, length());
	}

	mbcs_string substr (size_t index) const
	{
		return substr(index, length());
	}

	mbcs_string mid (size_t index, size_t count) const
	{
		return substr(index, count);
	}

	mbcs_string left (size_t count) const
	{
		return substr(0, count);
	}

	mbcs_string right  (size_t count) const
	{
		return substr(length() - count, count);
	}

#if __DEPRECATED__
	stringlist split (const mbcs_string & separator
			, bool keepEmpty = KeepEmpty
			, ucchar quoteChar = ucchar(ucchar::Null)) const
	{
		return split(false, separator, keepEmpty, quoteChar);
	}

	stringlist split (const ucchar & separator
			, bool keepEmpty = KeepEmpty
			, ucchar quoteChar = ucchar(ucchar::Null)) const
	{
		return split(false, mbcs_string(1, separator), keepEmpty, quoteChar);
	}

	stringlist splitOneOf (const mbcs_string & separators
			, bool keepEmpty = KeepEmpty
			, ucchar quoteChar = ucchar(ucchar::Null)) const
	{
		return split(true, mbcs_string(separators), keepEmpty, quoteChar);
	}
#endif
	bool           toBoolean (bool * ok = 0) const;
	integral_t     toIntegral (bool * ok = 0, int base = 10) const { return toSignedIntegral(ok, base); }
	integral_t     toSignedIntegral (bool * ok = 0, int base = 10) const;
	uintegral_t    toUnsignedIntegral (bool * ok = 0, int base = 10) const;

	signed char    toSignedChar   (bool * ok = 0, int radix = 10) const;
	unsigned char  toUnsignedChar (bool * ok = 0, int radix = 10) const;
	short          toShort  (bool * ok = 0, int base = 10) const { return toSignedShort(ok, base); }
	short          toSignedShort  (bool * ok = 0, int base = 10) const;
	unsigned short toUnsignedShort (bool * ok = 0, int base = 10) const;
	int            toInt    (bool * ok = 0, int base = 10) const { return toSignedInt(ok, base); }
	int	           toSignedInt    (bool * ok = 0, int base = 10) const;
	unsigned int   toUnsignedInt   (bool * ok = 0, int base = 10) const;
	long           toLong   (bool * ok = 0, int base = 10) const { return toSignedLong(ok, base); }
	long           toSignedLong   (bool * ok = 0, int base = 10) const;
	unsigned long  toUnsignedLong  (bool * ok = 0, int base = 10) const;

#ifdef PFS_HAVE_LONGLONG
	long long toLongLong (bool * ok = 0, int base = 10) const { return toSignedLongLong(ok, base); }
	long long toSignedLongLong (bool * ok = 0, int base = 10) const;
	unsigned long long toUnsignedLongLong (bool * ok = 0, int base = 10) const;
#endif

	real_t toReal (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const;
	float  toFloat (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const;
	double toDouble (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const;

#ifdef PFS_HAVE_LONG_DOUBLE
	long double toLongDouble (bool * ok = 0, ucchar decimalPoint = ucchar('.')) const { return toReal(ok, decimalPoint); }
#endif

	DLL_API mbcs_string<uint8_t> toUtf8 () const;
	DLL_API mbcs_string<uint16_t> toUtf16 () const;

	mbcs_string & operator += (const mbcs_string & other) { return append(other); }
	mbcs_string & operator += (const char * latin1) { return append(mbcs_string(latin1)); }
	mbcs_string & operator += (const ucchar & ch) { return append(mbcs_string(1, ch)); }
	mbcs_string & operator += (char latin1) { return append(mbcs_string(1, latin1)); }

	mbcs_string & operator << (const mbcs_string & other) { return append(other); }
	mbcs_string & operator << (const char * latin1) { return append(mbcs_string(latin1)); }
	mbcs_string & operator << (const ucchar & ch) { return append(mbcs_string(1, ch)); }
	mbcs_string & operator << (char latin1) { return append(mbcs_string(1, latin1)); }

private:
    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, mbcs_string_type_trait<const_iterator>);

    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::mbcs_string’"
    template <typename InputIt>
    mbcs_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2, mbcs_string_type_trait<InputIt>);
    mbcs_string & replace (const_iterator first, const_iterator last, const_iterator first2, const_iterator last2, mbcs_string_type_trait<const_iterator>);

#if __DEPRECATED__
    stringlist split (bool isOneSeparatorChar, const mbcs_string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar(ucchar::Null)) const;
#endif

public:
    static DLL_API mbcs_string fromLatin1 (const uint8_t * latin1, size_t n, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromLatin1 (const char * latin1, size_t n, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromLatin1 (const char * latin1, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromLatin1 (const pfs::byte_string & latin1, ConvertState * state = nullptr);

	static DLL_API mbcs_string fromUtf8 (const uint8_t * utf8, size_t size, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromUtf8 (const char * utf8, size_t size, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromUtf8 (const char * utf8, ConvertState * state = nullptr);
	static DLL_API mbcs_string fromUtf8 (const byte_string & utf8, ConvertState * state = nullptr);

	static DLL_API mbcs_string fromUtf16 (const uint16_t * utf16, size_t size, ConvertState * state = nullptr);

	static mbcs_string toString (bool value);
	static mbcs_string toString (signed char value, int base = 10, bool uppercase = false);
	static mbcs_string toString (short value, int base = 10, bool uppercase = false);
	static mbcs_string toString (int value, int base = 10, bool uppercase = false);
	static mbcs_string toString (long value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned char value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned short value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned int value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned long value, int base = 10, bool uppercase = false);

#ifdef PFS_HAVE_LONGLONG
	static mbcs_string toString (long long value, int base = 10, bool uppercase = false);
	static mbcs_string toString (unsigned long long value, int base = 10, bool uppercase = false);
#endif

	static mbcs_string toString (float value, char f = 'f', int prec = 6);
	static mbcs_string toString (double value, char f = 'f', int prec = 6);

#ifdef PFS_HAVE_LONG_DOUBLE
	static mbcs_string toString (long double value, char f = 'f', int prec = 6);
#endif
};

// Forward declaration to avoid
// `specialization after instantiation error'
template <>
mbcs_string<uint8_t> mbcs_string<uint8_t>::toUtf8 () const;

// Forward declaration to avoid
// `specialization after instantiation error'
template <>
mbcs_string<uint8_t> mbcs_string<uint16_t>::toUtf8 () const;


/**
 * @brief Returns C-string representation of UTF8-encoded string
 *
 * @return C-string representation of UTF8-encoded string.
 *
 * @note Applicable only for UTF8-encoded string
 */
template <>
inline const char * mbcs_string<uint8_t>::c_str () const
{
    return base_class::isNull()
        ? ""
        : reinterpret_cast<const char *>(base_class::cast()->constData());
}


template <typename CodeUnitT>
template <typename InputIt>
typename mbcs_string<CodeUnitT>::iterator mbcs_string<CodeUnitT>::insert (const_iterator pos, InputIt first, InputIt last, mbcs_string_type_trait<InputIt>)
{
	PFS_ASSERT(pos.holder() == this);

	size_type index = pos.base().index();

	if (first >= last)
		return iterator(this, pointer(this, index));

	for (InputIt it = first; it < last; ++it) {
		this->insert(pos++, *it);
	}

	return iterator(this, pointer(this, index + 1));
}


template <typename CodeUnitT>
template <typename InputIt>
mbcs_string<CodeUnitT> & mbcs_string<CodeUnitT>::replace (
		  const_iterator first
		, const_iterator last
		, InputIt first2
		, InputIt last2
		, mbcs_string_type_trait<InputIt>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(last.holder() == this);

	mbcs_string<CodeUnitT> s;

	for (InputIt it = first2; it < last2; ++it) {
		s.append(*it);
	}

	return replace(first, last, s);
}

template <typename CodeUnitT>
bool mbcs_string<CodeUnitT>::toBoolean (bool * ok) const
{
	bool okk = false;
	bool r = false;

	if (*this == "true") {
		r = true;
		okk = true;
	} else if (*this == "false") {
		r = false;
		okk = true;
	}

	if (ok)
		*ok = okk;

	return r;
}

template <typename CodeUnitT>
integral_t mbcs_string<CodeUnitT>::toSignedIntegral (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<integral_t>())
		, uintegral_t(pfs::max_type<integral_t>()));
}

template <typename CodeUnitT>
uintegral_t mbcs_string<CodeUnitT>::toUnsignedIntegral (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<uintegral_t>()));
}


/**
 * @brief Converts string to signed char value.
 *
 * @param ok Pointer to store result of conversion.
 * @param radix Radix, must be 0 or from 2 to 36 inclusively.
 * @return On successful returns signed char value converted from string.
 *         On error returns @c 0.
 *         If @c ok is not @c null @c *ok will store @c true on successful or @c false if otherwise.
 */
template <typename CodeUnitT>
signed char mbcs_string<CodeUnitT>::toSignedChar (bool * ok, int radix) const
{
    if (this->isEmpty()) {
        if (ok) *ok = false;
        return 0;
    }

    return static_cast<signed char>(strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
        (cbegin(), cend(), ok, radix
        , integral_t(pfs::min_type<signed char>())
        , uintegral_t(pfs::max_type<signed char>())));
}

/**
 * @brief Converts string to unsigned char value.
 *
 * @param ok Pointer to store result of conversion.
 * @param radix Radix, must be 0 or from 2 to 36 inclusively.
 * @return On successful returns unsigned char value converted from string.
 *         On error returns @c 0.
 *         If @c ok is not @c null @c *ok will store @c true on successful or @c false if otherwise.
 */
template <typename CodeUnitT>
unsigned char mbcs_string<CodeUnitT>::toUnsignedChar (bool * ok, int radix) const
{
    if (this->isEmpty()) {
        if (ok) *ok = false;
        return 0;
    }
    return static_cast<unsigned char>(strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
        (cbegin(), cend(), ok, radix
        , uintegral_t(pfs::max_type<unsigned char>())));
}


template <typename CodeUnitT>
short mbcs_string<CodeUnitT>::toSignedShort (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}

	return (short)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<short>())
		, uintegral_t(pfs::max_type<short>()));
}

template <typename CodeUnitT>
unsigned short mbcs_string<CodeUnitT>::toUnsignedShort (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (unsigned short)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned short>()));
}

template <typename CodeUnitT>
int	mbcs_string<CodeUnitT>::toSignedInt (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (int)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<int>())
		, uintegral_t(pfs::max_type<int>()));
}

template <typename CodeUnitT>
unsigned int mbcs_string<CodeUnitT>::toUnsignedInt (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (unsigned int)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned int>()));
}

template <typename CodeUnitT>
long mbcs_string<CodeUnitT>::toSignedLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (long)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<long>())
		, uintegral_t(pfs::max_type<long>()));
}

template <typename CodeUnitT>
unsigned long mbcs_string<CodeUnitT>::toUnsignedLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}

	return (unsigned long)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned long>()));
}

#ifdef PFS_HAVE_LONGLONG
template <typename CodeUnitT>
long long mbcs_string<CodeUnitT>::toSignedLongLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (long long)strtointegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<long long>())
		, uintegral_t(pfs::max_type<long long>()));
}

template <typename CodeUnitT>
unsigned long long mbcs_string<CodeUnitT>::toUnsignedLongLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (unsigned long long)strtouintegral_helper<mbcs_string<CodeUnitT>::char_type, mbcs_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned long long>()));
}
#endif


/**
 * @brief Converts the string to a real value.
 *
 * @param ok If a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
 * @param decimalPoint The separator between integral and fractal parts.
 * @return The string converted to a real value. Returns 0.0 if the conversion fails.
 */
template <typename CodeUnitT>
real_t mbcs_string<CodeUnitT>::toReal (bool * ok, ucchar decimalPoint) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return real_t(.0);
	}
	const_iterator begin(cbegin());
	const_iterator end(cend());
	const_iterator endptr(end);
	bool ok1 = true;

	real_t r = pfs::strtoreal<ucchar, const_iterator>(begin, end, decimalPoint, & endptr);
	if (errno || endptr != end) {
		ok1 = false;
		r = real_t(.0);
	}

	if (ok) *ok = ok1;
	return r;
}

template <typename CodeUnitT>
float mbcs_string<CodeUnitT>::toFloat (bool * ok, ucchar decimalPoint) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return float(.0);
	}

	bool ok1;
	real_t r = toReal(& ok1, decimalPoint);
	if (!ok1 || r < PFS_FLOAT_MIN || r > PFS_FLOAT_MAX) {
		ok1 = false;
		r = float(0.0f);
	}
	if (ok) *ok = ok1;
	return float(r);
}

template <typename CodeUnitT>
double mbcs_string<CodeUnitT>::toDouble (bool * ok, ucchar decimalPoint) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return double(.0);
	}

#ifndef PFS_HAVE_LONG_DOUBLE
	return toReal(ok, decimalPoint);
#else
	bool ok1;
	real_t r = toReal(& ok1, decimalPoint);
	if (!ok1 || r < PFS_DOUBLE_MIN || r > PFS_DOUBLE_MAX) {
		ok1 = false;
		r = double(0.0f);
	}
	if (ok) *ok = ok1;
	return double(r);
#endif
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (bool value)
{
	return mbcs_string<CodeUnitT>::fromLatin1( value ? "true" : "false");
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (signed char value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (short value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (int value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned char value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned short value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned int value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

#ifdef PFS_HAVE_LONGLONG
template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (long long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (unsigned long long value, int base, bool uppercase)
{
	char buf[65];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}
#endif

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (float value, char f, int prec)
{
	char buf[5020];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, 5020));
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (double value, char f, int prec)
{
	char buf[5020];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, 5020));
}

#ifdef PFS_HAVE_LONG_DOUBLE
// 1.18973e+4932 with 'f' flag has length 4940
template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> mbcs_string<CodeUnitT>::toString (long double value, char f, int prec)
{
	char buf[5020];
	return mbcs_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, 5020));
}
#endif


template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, const mbcs_string<CodeUnitT> & rhs)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += rhs;
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, const char * latin1)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += latin1;
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, const ucchar & ch)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += ch;
}

template <typename CodeUnitT>
inline mbcs_string<CodeUnitT> operator + (const mbcs_string<CodeUnitT> & lhs, char latin1)
{
	mbcs_string<CodeUnitT> r(lhs);
	return r += latin1;
}

template <typename CodeUnitT>
inline bool operator == ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnitT>
inline bool operator != ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnitT>
inline bool operator < ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnitT>
inline bool operator <= ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnitT>
inline bool operator > ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnitT>
inline bool operator >= ( const mbcs_string<CodeUnitT> & lhs
		, const mbcs_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) >= 0;
}

template <typename CodeUnitT>
inline bool operator == (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnitT>
inline bool operator != (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnitT>
inline bool operator < (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnitT>
inline bool operator <= (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnitT>
inline bool operator > (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnitT>
inline bool operator >= (const mbcs_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) >= 0;
}

template <typename CodeUnitT>
inline std::ostream & operator << (std::ostream & os, const mbcs_string<CodeUnitT> & o);
//{
//// FIXME is c_str() that function to use in this case?
//	os << o.c_str();
//	return os;
//}

template <>
inline std::ostream & operator << <uint8_t> (std::ostream & os, const mbcs_string<uint8_t> & s)
{
    os << reinterpret_cast<const char *>(s.constData());
    return os;
}

//
/**
 * @brief Converts UTF16-encoded to UTF8-encoded and output to stream.
 *
 * @param os Output stream.
 * @param o UTF16-encoded string.
 * @return Output stream.
 */
template <>
inline std::ostream & operator << <uint16_t> (std::ostream & os, const mbcs_string<uint16_t> & s)
{
    mbcs_string<uint8_t> s1(s.toUtf8());
    os << s1.constData();
    return os;
}

} // pfs

#include <pfs/bits/mbcs_string_impl_inc.hpp>
#include <pfs/bits/mbcs_string_inc.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_MBCS_STRING_HPP__ */
