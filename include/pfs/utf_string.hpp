/*
 * utf_string.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_STRING_HPP__
#define __PFS_UTF_STRING_HPP__

#include <string>

namespace pfs { namespace ef {

//template <typename T, typename U>
//void convert (T & dest, const U & src);
//
//template <typename T, typename U>
//inline void convert<std::string, std::string> (std::string & dest, const std::string & src)
//{
//	dest = src;
//}

template <typename CodeUnit>
class utf_string
{
	typedef std::basic_string<CodeUnit> rep_type;

public:
	typedef int32_t code_point_type;

	// Types
//    typedef rep_type::traits_type	           traits_type;
    typedef int32_t              		       value_type;
//    typedef rep_type::allocator_type	       allocator_type;
    typedef typename rep_type::size_type	   size_type;
//    typedef typename rep_type::difference_type difference_type;
//    typedef typename rep_type::reference	   reference;
//    typedef typename rep_type::const_reference const_reference;
//    typedef typename rep_type::pointer	       pointer;
    typedef typename rep_type::const_pointer   const_pointer;
//    typedef rep_type::iterator                 iterator;
//    typedef rep_type::const_iterator           const_iterator;
//    typedef rep_type::const_reverse_iterator   const_reverse_iterator;
//    typedef rep_type::reverse_iterator         reverse_iterator;

    typedef value_type char_type;

//public:
//    static const size_type npos = rep_type::npos;

private:
    rep_type  _d;
	//size_type _length; // length in Unicode chars (code points)

//public: // static
//	static const uint32_t TerminatorChar  = '\0';
//	static const uint32_t EndOfLineChar   = '\n';
//	static const uint32_t ReplacementChar = '?';
//	static const size_type npos = -1;

public:
	utf_string ()
		: _d()
	{}

	utf_string (const utf_string & other)
		: _d(other._d)
	{}

	utf_string (const std::string & s);

#ifdef PFS_CC_MSC
	utf_string (const std::wstring & s);
#endif

	utf_string (size_t n, char c);
	utf_string (size_t n, int32_t uc);

	template <class InputIterator>
	utf_string (InputIterator first, InputIterator last);

	virtual ~utf_string ()
	{}

	const_pointer data () const
	{
		return _d.data();
	}

	/**
	 * @brief Returns size of string in code units.
	 *
	 * @return String in code units.
	 */
	bool size () const
	{
		return _d.size();
	}

    // Length in unicode chars
    //
	/**
	 * @brief Returns length in code points.
	 *
	 * @return Length in code points.
	 */
    size_type length () const;

	/**
	 * @brief Checks if string is empty.
	 *
	 * @return @c true if string is empty (size() == 0) or @c false otherwise.
	 */
    bool empty () const
    {
    	return _d.empty();
    }

	void clear ()
	{
		_d.clear();
	}

//	utf_string & append (const utf_string & str)
//	{
//		_d.append(str._d);
//		_length += _d.length;
//		return *this;
//	}

//	utf_string & append (const utf_string & str, size_t subpos, size_t sublen);
//	utf_string & append (const char * s);
//	utf_string & append (const char * s, size_t n);
//	utf_string & append (size_t n, char c);
//	utf_string & append (size_t n, uint32_t c);
//	utf_string & append (size_t n, ucchar c)
//	{
//		while (n-- > 0) {
//			this->push_back(c);
//		}
//		return *this;
//	}
//
//	template <class InputIterator>
//	utf_string & append (InputIterator first, InputIterator last)
//	{
//		while (first != last) {
//			push_back(*first++);
//		}
//		return *this;
//	}
//
//	utf_string & push_back (char latin1)
//	{
//		//return this->append(1, latin1);
//		return latin1_cast (const char * latin1, size_t n, *this);
//	}
//
//	utf_string & push_back (uint32_t c)
//	{
//		//return this->append(1, c);
//		return *this;
//	}
//
//	utf_string & push_back (ucchar c)
//	{
//		//return this->append(1, c);
//		return *this;
//	}

#if __COMMENT__

	utf_string & erase (size_type index = 0) { return erase(index, 1); }
	utf_string & erase (size_type index, size_type count);
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


    value_type valueAt (size_type index) const { return at(index); }
    value_type charAt (size_type index) const { return at(index); }
    reference at (size_type index) const { pointer p(const_cast<self_class *>(this), 0); p += index; return p.ref(); }
    reference operator [] (size_type index) const { return at(index); }

    int compare (size_type pos1, size_type count1, const utf_string & s, size_type pos2, size_type count2) const;

    int compare (size_type pos1, size_type count1, const char * latin1, size_type count2) const
    {
        return this->compare(pos1, count1, utf_string(latin1), 0, count2);
    }

    int compare (size_type pos, size_type count, const utf_string & s) const
    {
        return compare(pos, count, s, 0, s.length());
    }

    int compare (const utf_string & s) const
    {
        return compare(0, this->length(), s);
    }

    int compare (const char * latin1) const
    {
        return compare(utf_string(latin1));
    }

    int compare (size_type pos, size_type count, const char * latin1) const
    {
        return compare(pos, count, utf_string(latin1));
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, const utf_string & str) const;

    // TODO DEPRECATED
    iterator find (const utf_string & str) const
    {
    	return find(cbegin(), str);
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, const char * latin1, size_type count) const
    {
    	return find(pos, utf_string(latin1, count));
    }

    // TODO DEPRECATED
    iterator find (const_iterator pos, const char * latin1) const
    {
    	return find(pos, utf_string(latin1, strlen(latin1)));
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
    	return find(pos, utf_string(1, ch));
    }

    // TODO DEPRECATED
    iterator find (ucchar ch) const
    {
    	return find(cbegin(), ch);
    }

    // TODO Need to implement
#ifdef __COMMENT__
    iterator rfind (const_iterator pos, const utf_string & str) const;

    iterator rfind (const utf_string & str) const
    {
    	return rfind(cbegin(), str);
    }

    iterator rfind (const_iterator pos, const char * latin1, size_type count) const
    {
    	return rfind(pos, utf_string::fromLatin1(latin1, count));
    }

    iterator rfind (const_iterator pos, const char * latin1) const
    {
    	return rfind(pos, utf_string::fromLatin1(latin1, strlen(latin1)));
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
    	return rfind(pos, utf_string(1, ch));
    }

    iterator rfind (ucchar ch) const
    {
    	return rfind(cbegin(), ch);
    }
#endif

	bool contains   (const utf_string & s) const { return find(s) != cend(); }
	bool contains   (const char * latin1) const   { return find(latin1) != cend(); }
	bool contains   (ucchar ch) const             { return find(ch) != cend(); }
	bool contains   (char latin1) const           { return find(latin1) != cend(); }

	// TODO Need tests
	// FIXME need optimization like startsWith (char v)
	bool startsWith (const utf_string & s)  const { return find(s) == cbegin(); }
	bool startsWith (const_iterator pos, const utf_string & s) const
	{
		return find(pos, s) == pos;
	}
	// FIXME need optimization like startsWith (char v)
	bool startsWith (const char * latin1) const    { return find(latin1) == cbegin(); }

	bool startsWith (ucchar ch) const              { return isEmpty() ? false : *(cbegin()) == ch; }
	bool startsWith (char latin1) const            { return isEmpty() ? false : *(cbegin()) == ucchar(latin1); }

	// TODO Need tests
	bool endsWith   (const utf_string & s) const  { return (length() >= s.length()) ? startsWith(cend() - s.length(), s) : false; }
	bool endsWith   (const char * latin1) const    { return endsWith(utf_string(latin1)); }
	bool endsWith   (ucchar ch) const              { return endsWith(utf_string(1, ch)); }
	bool endsWith   (char latin1) const            { return endsWith(utf_string(1, latin1)); }

    // Size in code units
	//
    size_type size () const
    {
    	return base_class::isNull() ? 0 : base_class::cast()->size();
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

    utf_string & insert (size_type index, const utf_string & str, size_type index_str, size_type count);

    utf_string & insert (size_type index, size_type count, char latin1)
    {
      return insert(index, utf_string(count, latin1));
    }

    utf_string & insert (size_type index, size_type count, ucchar uc)
    {
        return insert(index, utf_string(count, uc));
    }

    utf_string & insert (size_type index, const char * latin1)
    {
      return insert(index, utf_string(latin1));
    }

    utf_string & insert (size_type index, const char * latin1, size_type n)
    {
      return insert(index, utf_string(latin1, n));
    }

    utf_string & insert (size_type index, const utf_string & str)
    {
        return insert(index, str, 0, str.length());
    }

    iterator insert (const_iterator pos, char latin1)
    {
      utf_string s(1, latin1);
      return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, ucchar ch)
    {
        utf_string s(1, ch);
        return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, size_type count, char latin1)
    {
      utf_string s(count, latin1);
      return insert(pos, s.cbegin(), s.cend());
    }

    iterator insert (const_iterator pos, size_type count, ucchar ch)
    {
        utf_string s(count, ch);
        return insert(pos, s.cbegin(), s.cend());
    }

    template<class InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last)
    {
        return insert(pos, first, last, utf_string_type_trait<InputIt>());
    }


    utf_string & append (const utf_string & str, size_type index_str, size_type count)
    {
    	return insert(this->length(), str, index_str, count);
    }

    utf_string & append (size_type count, char latin1)
    {
    	return insert(this->length(), count, latin1);
    }

    utf_string & append (size_type count, ucchar uc)
    {
    	return insert(this->length(), count, uc);
    }

    utf_string & append (const char * latin1)
    {
    	return insert(this->length(), latin1);
    }

    utf_string & append (const char * latin1, size_type n)
    {
    	return insert(this->length(), latin1, n);
    }

    utf_string & append (const utf_string & str)
    {
    	return insert(this->length(), str);
    }

//    template<class InputIt>
//    utf_string & append (InputIt first, InputIt last);

    utf_string & prepend (const utf_string & str, size_type index_str, size_type count)
    {
    	return insert(0, str, index_str, count);
    }

    utf_string & prepend (size_type count, char latin1)
    {
    	return insert(0, count, latin1);
    }

    utf_string & prepend (size_type count, ucchar uc)
    {
    	return insert(0, count, uc);
    }

    utf_string & prepend (const char * latin1)
    {
    	return insert(0, latin1);
    }

    utf_string & prepend (const char * latin1, size_type n)
    {
    	return insert(0, latin1, n);
    }

    utf_string & prepend (const utf_string & str)
    {
    	return insert(0, str);
    }

//    template<class InputIt>
//    utf_string & prepend (InputIt first, InputIt last)


    utf_string & replace (size_type pos1, size_type count1, const utf_string & str, size_type pos2, size_type count2);

    utf_string & replace (size_type pos, size_type count, const utf_string & str)
    {
    	return replace(pos, count, str, 0, str.length());
    }

    utf_string & replace (const_iterator first, const_iterator last, const utf_string & str)
    {
    	return replace(first, last, str.cbegin(), str.cend(), utf_string_type_trait<const_iterator>());
    }

    template< class InputIt >
    utf_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2)
    {
    	return replace(first, last, first2, last2, utf_string_type_trait<InputIt>());
    }

    utf_string & replace (size_type pos, size_type count, size_type count2, ucchar ch)
    {
    	return replace (pos, count, utf_string(count2, ch));
    }

    utf_string & replace (const_iterator first, const_iterator last, size_type count2, ucchar ch)
    {
    	return replace(first, last, utf_string(count2, ch));
    }

    utf_string & replace ( const utf_string & before, const utf_string & after);

	utf_string substr (size_type index, size_type count) const;
	utf_string substr (const_iterator begin, size_t count) const;
	utf_string substr (const_iterator begin, const_iterator end) const;

	utf_string substr (const_iterator begin) const
	{
		return substr(begin, length());
	}

	utf_string substr (size_t index) const
	{
		return substr(index, length());
	}

	utf_string mid (size_t index, size_t count) const
	{
		return substr(index, count);
	}

	utf_string left (size_t count) const
	{
		return substr(0, count);
	}

	utf_string right  (size_t count) const
	{
		return substr(length() - count, count);
	}

	// TODO DEPRECATED
	stringlist split (const utf_string & separator
			, bool keepEmpty = KeepEmpty
			, ucchar quoteChar = ucchar(ucchar::Null)) const
	{
		return split(false, separator, keepEmpty, quoteChar);
	}

	// TODO DEPRECATED
	stringlist split (const ucchar & separator
			, bool keepEmpty = KeepEmpty
			, ucchar quoteChar = ucchar(ucchar::Null)) const
	{
		return split(false, utf_string(1, separator), keepEmpty, quoteChar);
	}

	// TODO DEPRECATED
	stringlist splitOneOf (const utf_string & separators
			, bool keepEmpty = KeepEmpty
			, ucchar quoteChar = ucchar(ucchar::Null)) const
	{
		return split(true, utf_string(separators), keepEmpty, quoteChar);
	}

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

	DLL_API utf_string<uint8_t> toUtf8 () const;
	DLL_API utf_string<uint16_t> toUtf16 () const;

	utf_string & operator += (const utf_string & other) { return append(other); }
	utf_string & operator += (const char * latin1) { return append(utf_string(latin1)); }
	utf_string & operator += (const ucchar & ch) { return append(utf_string(1, ch)); }
	utf_string & operator += (char latin1) { return append(utf_string(1, latin1)); }

	utf_string & operator << (const utf_string & other) { return append(other); }
	utf_string & operator << (const char * latin1) { return append(utf_string(latin1)); }
	utf_string & operator << (const ucchar & ch) { return append(utf_string(1, ch)); }
	utf_string & operator << (char latin1) { return append(utf_string(1, latin1)); }

private:
    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::utf_string’"
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, utf_string_type_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, utf_string_type_trait<const_iterator>);

    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::utf_string’"
    template <typename InputIt>
    utf_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2, utf_string_type_trait<InputIt>);
    utf_string & replace (const_iterator first, const_iterator last, const_iterator first2, const_iterator last2, utf_string_type_trait<const_iterator>);

    // TODO DEPRECATED
    stringlist split (bool isOneSeparatorChar, const utf_string & separator, bool keepEmpty = true, ucchar quoteChar = ucchar(ucchar::Null)) const;


public:

	static DLL_API utf_string fromUtf16 (const uint16_t * utf16, size_t size, ConvertState * state = nullptr);

	static utf_string toString (bool value);
	static utf_string toString (signed char value, int base = 10, bool uppercase = false);
	static utf_string toString (short value, int base = 10, bool uppercase = false);
	static utf_string toString (int value, int base = 10, bool uppercase = false);
	static utf_string toString (long value, int base = 10, bool uppercase = false);
	static utf_string toString (unsigned char value, int base = 10, bool uppercase = false);
	static utf_string toString (unsigned short value, int base = 10, bool uppercase = false);
	static utf_string toString (unsigned int value, int base = 10, bool uppercase = false);
	static utf_string toString (unsigned long value, int base = 10, bool uppercase = false);

#ifdef PFS_HAVE_LONGLONG
	static utf_string toString (long long value, int base = 10, bool uppercase = false);
	static utf_string toString (unsigned long long value, int base = 10, bool uppercase = false);
#endif

	static utf_string toString (float value, char f = 'f', int prec = 6);
	static utf_string toString (double value, char f = 'f', int prec = 6);

#ifdef PFS_HAVE_LONG_DOUBLE
	static utf_string toString (long double value, char f = 'f', int prec = 6);
#endif

#endif // __COMMENT__
};

#if __COMMENT__
template <typename CodeUnitT>
utf_string<CodeUnitT>::iterator::iterator (const const_iterator & it)
	: random_access_iterator<self_class>(const_cast<self_class *>(it.holder())
			, pointer(const_cast<self_class *>(it.holder()), it.base().index()))
{}


// Forward declaration to avoid
// `specialization after instantiation error'
template <>
utf_string<uint8_t> utf_string<uint8_t>::toUtf8 () const;

// Forward declaration to avoid
// `specialization after instantiation error'
template <>
utf_string<uint8_t> utf_string<uint16_t>::toUtf8 () const;


/**
 * @brief Returns C-string representation of UTF8-encoded string
 *
 * @return C-string representation of UTF8-encoded string.
 *
 * @note Applicable only for UTF8-encoded string
 */
template <>
inline const char * utf_string<uint8_t>::c_str () const
{
    return base_class::isNull()
        ? ""
        : reinterpret_cast<const char *>(base_class::cast()->constData());
}


template <typename CodeUnitT>
template <typename InputIt>
typename utf_string<CodeUnitT>::iterator utf_string<CodeUnitT>::insert (const_iterator pos, InputIt first, InputIt last, utf_string_type_trait<InputIt>)
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
utf_string<CodeUnitT> & utf_string<CodeUnitT>::replace (
		  const_iterator first
		, const_iterator last
		, InputIt first2
		, InputIt last2
		, utf_string_type_trait<InputIt>)
{
	PFS_ASSERT(first.holder() == this);
	PFS_ASSERT(last.holder() == this);

	utf_string<CodeUnitT> s;

	for (InputIt it = first2; it < last2; ++it) {
		s.append(*it);
	}

	return replace(first, last, s);
}

template <typename CodeUnitT>
bool utf_string<CodeUnitT>::toBoolean (bool * ok) const
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
integral_t utf_string<CodeUnitT>::toSignedIntegral (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<integral_t>())
		, uintegral_t(pfs::max_type<integral_t>()));
}

template <typename CodeUnitT>
uintegral_t utf_string<CodeUnitT>::toUnsignedIntegral (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
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
signed char utf_string<CodeUnitT>::toSignedChar (bool * ok, int radix) const
{
    if (this->isEmpty()) {
        if (ok) *ok = false;
        return 0;
    }

    return static_cast<signed char>(strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
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
unsigned char utf_string<CodeUnitT>::toUnsignedChar (bool * ok, int radix) const
{
    if (this->isEmpty()) {
        if (ok) *ok = false;
        return 0;
    }
    return static_cast<unsigned char>(strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
        (cbegin(), cend(), ok, radix
        , uintegral_t(pfs::max_type<unsigned char>())));
}


template <typename CodeUnitT>
short utf_string<CodeUnitT>::toSignedShort (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}

	return (short)strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<short>())
		, uintegral_t(pfs::max_type<short>()));
}

template <typename CodeUnitT>
unsigned short utf_string<CodeUnitT>::toUnsignedShort (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (unsigned short)strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned short>()));
}

template <typename CodeUnitT>
int	utf_string<CodeUnitT>::toSignedInt (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (int)strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<int>())
		, uintegral_t(pfs::max_type<int>()));
}

template <typename CodeUnitT>
unsigned int utf_string<CodeUnitT>::toUnsignedInt (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (unsigned int)strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned int>()));
}

template <typename CodeUnitT>
long utf_string<CodeUnitT>::toSignedLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (long)strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<long>())
		, uintegral_t(pfs::max_type<long>()));
}

template <typename CodeUnitT>
unsigned long utf_string<CodeUnitT>::toUnsignedLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}

	return (unsigned long)strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintegral_t(pfs::max_type<unsigned long>()));
}

#ifdef PFS_HAVE_LONGLONG
template <typename CodeUnitT>
long long utf_string<CodeUnitT>::toSignedLongLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (long long)strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, integral_t(pfs::min_type<long long>())
		, uintegral_t(pfs::max_type<long long>()));
}

template <typename CodeUnitT>
unsigned long long utf_string<CodeUnitT>::toUnsignedLongLong (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return (unsigned long long)strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
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
real_t utf_string<CodeUnitT>::toReal (bool * ok, ucchar decimalPoint) const
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
float utf_string<CodeUnitT>::toFloat (bool * ok, ucchar decimalPoint) const
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
double utf_string<CodeUnitT>::toDouble (bool * ok, ucchar decimalPoint) const
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

// XXX DEPRECATED (see pfs::lexical_cast())
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (bool value)
{
	return utf_string<CodeUnitT>::fromLatin1( value ? "true" : "false");
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (signed char value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (short value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (int value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (long value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (unsigned char value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (unsigned short value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (unsigned int value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (unsigned long value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}

#ifdef PFS_HAVE_LONGLONG
// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (long long value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_integral_to_string(integral_t(value), base, int(uppercase), buf, 65));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (unsigned long long value, int base, bool uppercase)
{
	char buf[65];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_uintegral_to_string(uintegral_t(value), base, int(uppercase), buf, 65));
}
#endif

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (float value, char f, int prec)
{
	int sz = 5020;
	char buf[5020];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, & sz));
}

// XXX DEPRECATED
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (double value, char f, int prec)
{
	int sz = 5020;
	char buf[5020];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, & sz));
}

#ifdef PFS_HAVE_LONG_DOUBLE
// 1.18973e+4932 with 'f' flag has length 4940
template <typename CodeUnitT>
inline utf_string<CodeUnitT> utf_string<CodeUnitT>::toString (long double value, char f, int prec)
{
	int sz = 5020;
	char buf[5020];
	return utf_string<CodeUnitT>::fromLatin1(
			pfs_real_to_string(real_t(value), f, prec, buf, & sz));
}
#endif


template <typename CodeUnitT>
inline utf_string<CodeUnitT> operator + (const utf_string<CodeUnitT> & lhs, const utf_string<CodeUnitT> & rhs)
{
	utf_string<CodeUnitT> r(lhs);
	return r += rhs;
}

template <typename CodeUnitT>
inline utf_string<CodeUnitT> operator + (const utf_string<CodeUnitT> & lhs, const char * latin1)
{
	utf_string<CodeUnitT> r(lhs);
	return r += latin1;
}

template <typename CodeUnitT>
inline utf_string<CodeUnitT> operator + (const utf_string<CodeUnitT> & lhs, const ucchar & ch)
{
	utf_string<CodeUnitT> r(lhs);
	return r += ch;
}

template <typename CodeUnitT>
inline utf_string<CodeUnitT> operator + (const utf_string<CodeUnitT> & lhs, char latin1)
{
	utf_string<CodeUnitT> r(lhs);
	return r += latin1;
}

template <typename CodeUnitT>
inline bool operator == ( const utf_string<CodeUnitT> & lhs
		, const utf_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnitT>
inline bool operator != ( const utf_string<CodeUnitT> & lhs
		, const utf_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnitT>
inline bool operator < ( const utf_string<CodeUnitT> & lhs
		, const utf_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnitT>
inline bool operator <= ( const utf_string<CodeUnitT> & lhs
		, const utf_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnitT>
inline bool operator > ( const utf_string<CodeUnitT> & lhs
		, const utf_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnitT>
inline bool operator >= ( const utf_string<CodeUnitT> & lhs
		, const utf_string<CodeUnitT> & rhs )
{
	return lhs.compare(rhs) >= 0;
}

template <typename CodeUnitT>
inline bool operator == (const utf_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnitT>
inline bool operator != (const utf_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnitT>
inline bool operator < (const utf_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnitT>
inline bool operator <= (const utf_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnitT>
inline bool operator > (const utf_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnitT>
inline bool operator >= (const utf_string<CodeUnitT> & lhs, const char * rhs)
{
	return lhs.compare(rhs) >= 0;
}

template <typename CodeUnitT>
inline std::ostream & operator << (std::ostream & os, const utf_string<CodeUnitT> & o);
//{
//// FIXME is c_str() that function to use in this case?
//	os << o.c_str();
//	return os;
//}

template <>
inline std::ostream & operator << <uint8_t> (std::ostream & os, const utf_string<uint8_t> & s)
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
inline std::ostream & operator << <uint16_t> (std::ostream & os, const utf_string<uint16_t> & s)
{
    utf_string<uint8_t> s1(s.toUtf8());
    os << s1.constData();
    return os;
}

#endif // __COMMENT__

}} // pfs::ef

#endif /* __PFS_EF_MBCS_STRING_HPP__ */
