/*
 * utf_string.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_STRING_HPP__
#define __PFS_UTF_STRING_HPP__

#include <string>
#include <ostream>
#include <pfs/utf/traits.hpp>

namespace pfs { namespace utf {

template <typename CodeUnit, typename UtfTag>
class string
{
	typedef std::basic_string<CodeUnit> rep_type;

public:
    typedef pfs::utf::traits<rep_type, UtfTag>         utf_traits_type;
    typedef typename utf_traits_type::value_type       value_type;
    typedef typename utf_traits_type::allocator_type   allocator_type;
    typedef typename utf_traits_type::size_type	       size_type;
    typedef typename utf_traits_type::difference_type  difference_type;
    typedef typename utf_traits_type::pointer	       pointer;
    typedef typename utf_traits_type::const_pointer    const_pointer;
    typedef typename utf_traits_type::template iterator<pointer> iterator;
    typedef typename utf_traits_type::template iterator<const_pointer> const_iterator;
    typedef std::reverse_iterator<iterator>            reverse_iterator;
    typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;

//    typedef value_type char_type;

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
	string ()
		: _d()
	{}

	string (const string & other)
		: _d(other._d)
	{}

	/**
	 * @brief Construct string from std::string
	 * @param s String constructed from.
	 *
	 * @note @a s expected as UTF-8 encoded.
	 */
	string (const std::string & s);

//	string (const std::wstring & s);

	string (size_t n, char c)
	{
		this->append(n, c);
	}

	string (size_t n, value_type c)
	{
		this->append(n, c);
	}

	template <class InputIterator>
	string (InputIterator first, InputIterator last);

	virtual ~string ()
	{}

    iterator begin ()
    {
    	return iterator(_d.begin().base());
    }

    iterator end ()
    {
    	return iterator(_d.end().base());
    }

    const_iterator begin () const
    {
    	return const_iterator(_d.begin().base());
    }

    const_iterator end () const
    {
    	return const_iterator(_d.end().base());
    }

    const_iterator cbegin () const
    {
    	return begin();
    }

    const_iterator cend () const
    {
    	return end();
    }

    reverse_iterator rbegin ()
    {
    	return reverse_iterator(end());
    }

    reverse_iterator rend ()
    {
    	return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin () const
    {
    	return const_reverse_iterator(end());
    }

    const_reverse_iterator rend () const
    {
    	return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin () const
    {
    	return rbegin();
    }

    const_reverse_iterator crend () const
    {
    	return rend();
    }

	const_pointer data () const
	{
		return _d.data();
	}

	/**
	 * @brief  Returns const pointer to null-terminated contents.
	 *
	 * @return Const pointer to null-terminated contents.
	 */
    const CodeUnit * c_str() const
    {
    	return _d.c_str();
    }

    /**
     * @brief Returns UTF-8 encoded std::string instance.
     *
     * @return UTF-8 encoded std::string instance.
     */
    std::string stdstring () const;

	/**
	 * @brief Returns size of string in code units.
	 *
	 * @return String in code units.
	 */
	size_type size () const
	{
		return _d.size();
	}

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

	int compare (const string & str) const
	{
		return _d.compare(str._d);
	}

//	int compare( size_type pos1, size_type count1,
//	             const basic_string& str ) const;
//	int compare( size_type pos1, size_type count1,
//	             const basic_string& str,
//	             size_type pos2, size_type count2 ) const;
//	int compare( const CharT* s ) const noexcept;
//	int compare( size_type pos1, size_type count1,
//	             const CharT* s ) const;
//	int compare( size_type pos1, size_type count1,
//	             const CharT* s, size_type count2 ) const;

    /**
     *  @brief Append a string to this string.
     *
     *  @param s The string to append.
     *  @return  Reference to this string.
     */
    string & append (const string & s)
    {
    	_d.append(s._d);
    	return *this;
    }

	/**
	 *  @brief  Append a substring.
	 *  @details This function appends @a n characters from @a s
	 *           starting at @a pos to this string.  If @a n is larger
	 *           than the number of available characters in @a s, the
	 *           remainder of @a s is appended.
	 *
	 *  @param s   The string to append.
	 *  @param pos Index of the first character of str to append.
	 *  @param n   The number of characters to append.
	 *  @return Reference to this string.
	 */
//    string & append (const string & s, size_type pos, size_type n)
//    {
//    	PFS_ASSERT_RANGE(pos < s.size());
//
//
//    	_M_append(__str._M_data()
//			 + __str._M_check(__pos, "basic_string::append"),
//			 __str._M_limit(__pos, __n));
//
//    	return *this;
//    }

    /**
     *  @brief Append a C substring.
     *
     *  @param s The C string to append.
     *  @param n The number of characters to append.
     *  @return Reference to this string.
     */
//    string & append (const CodeUnit * s, size_type n)
//    {
//    	if (s) {
//    		_d.append(s, n);
//    	}
//    	return *this;
//    }

    /**
     *  @brief Append a C string.
     *
     *  @param s The C string to append.
     *  @return  Reference to this string.
     */
//    string & append (const CodeUnit * s)
//    {
//    	if (s)
//    		_d.append(s);
//    	return *this;
//    }

    /**
     *  @brief  Append multiple characters.
     *  @details Appends @a n copies of @a c to this string.
     *
     *  @param n  The number of characters to append.
     *  @param c  The character to use.
     *  @return  Reference to this string.
     */
    string & append (size_type n, value_type c)
    {
    	while (n--)
    		push_back(c);
    	return *this;
    }

    /**
     *
     * @param n
     * @param c
     * @return
     */
    string & append (size_type n, char c)
    {
    	while (n--)
    		push_back(c);
    	return *this;
    }

    /**
     *  @brief Append a range of characters.
     *  @details Appends characters in the range [@a first, @a last) to this string.
     *
     *  @param first  Iterator referencing the first character to append.
     *  @param last  Iterator marking the end of the range.
     *  @return  Reference to this string.
     */
    template <typename InputIterator>
    string & append (InputIterator first, InputIterator last)
    {
    	while (first != last) {
    		append(1, *first++);
    	}

    	return *this;
    }

	string & push_back (value_type c)
	{
		utf_traits_type::encode(c, std::back_inserter(_d));
		return *this;
	}

	/**
	 *
	 * @param latin1 Latin1 character (0 <= latin1 < 127)
	 * @return
	 */
	string & push_back (char c)
	{
		return push_back(value_type(c));
	}

    value_type at (size_type index) const
    {
    	// FIXME Check for out of bounds
    	//
    	const_iterator it(begin());
    	std::advance(it, index);
    	return *it;
    }

    value_type operator [] (size_type index) const
    {
    	return at(index);
    }

#if __COMMENT__

	utf_string & erase (size_type index = 0) { return erase(index, 1); }
	utf_string & erase (size_type index, size_type count);
	iterator erase (const_iterator pos) { return erase(pos, pos + 1); }
	iterator erase (const_iterator first, const_iterator last);

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

#endif // __COMMENT__


	bool starts_with (const string & needle) const;

	bool ends_with (const string & needle) const;
};

#if __COMMENT__

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


template <typename CodeUnit, typename UtfTag>
inline bool operator == (const string<CodeUnit, UtfTag> & lhs
		, const string<CodeUnit, UtfTag> & rhs )
{
	return lhs.compare(rhs) == 0;
}

template <typename CodeUnit, typename UtfTag>
inline bool operator != (const string<CodeUnit, UtfTag> & lhs
		, const string<CodeUnit, UtfTag> & rhs )
{
	return lhs.compare(rhs) != 0;
}

template <typename CodeUnit, typename UtfTag>
inline bool operator < (const string<CodeUnit, UtfTag> & lhs
		, const string<CodeUnit, UtfTag> & rhs)
{
	return lhs.compare(rhs) < 0;
}

template <typename CodeUnit, typename UtfTag>
inline bool operator <= (const string<CodeUnit, UtfTag> & lhs
		, const string<CodeUnit, UtfTag> & rhs)
{
	return lhs.compare(rhs) <= 0;
}

template <typename CodeUnit, typename UtfTag>
inline bool operator > (const string<CodeUnit, UtfTag> & lhs
		, const string<CodeUnit, UtfTag> & rhs)
{
	return lhs.compare(rhs) > 0;
}

template <typename CodeUnit, typename UtfTag>
inline bool operator >= (const string<CodeUnit, UtfTag> & lhs
		, const string<CodeUnit, UtfTag> & rhs)
{
	return lhs.compare(rhs) >= 0;
}

}} // pfs::utf

#endif /* __PFS_UTF_STRING_HPP__ */
