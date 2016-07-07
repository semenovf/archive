/*
 * string.hpp
 *
 *  Created on: Oct 25, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF_STRING_HPP__
#define __PFS_UTF_STRING_HPP__

#include <string>
#include <ostream>
#include <pfs/shared_ptr.hpp>
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
    typedef typename utf_traits_type::iterator         iterator;
    typedef typename utf_traits_type::const_iterator   const_iterator;
    typedef std::reverse_iterator<iterator>            reverse_iterator;
    typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
    
    typedef typename pfs::shared_ptr<string<CodeUnit, UtfTag> > shared_type;

private:
    rep_type  _d;

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
	explicit string (const std::string & s);

	explicit string (const_pointer s);
    
    explicit string (const_pointer s, size_type n);

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
	string (InputIterator first, InputIterator last)
	{
		while(first != last)
			push_back(*first++);
	}

	virtual ~string ()
	{}

	string & operator = (const std::string & s)
	{
		_d = reinterpret_cast<const rep_type &>(s);
		return *this;
	}

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
    const CodeUnit * c_str () const
    {
    	return _d.c_str();
    }

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
    size_type length () const
    {
    	return std::distance(cbegin(), cend());
    }

    /**
     * @brief Reserve Attempt to preallocate enough memory
     *        for specified number of code units.
     *
     * @param sz Number of code units required.
     */
    void reserve (size_type sz)
    {
    	_d.reserve(sz);
    }

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

    string & append (const std::string & s)
    {
    	_d.append(reinterpret_cast<const rep_type &>(s));
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

    /**
     *  @brief Append a string to this string.
     *
     *  @param s The string to append.
     *  @return  Reference to this string.
     */
    string & prepend (const string & s)
    {
    	_d.insert(0, s._d);
    	return *this;
    }

    string & prepend (size_type n, value_type c)
    {
    	return prepend(string(n, c));
    }

    string & prepend (size_type n, char c)
    {
    	return prepend(string(n, c));
    }

    template <typename InputIterator>
    string & prepend (InputIterator first, InputIterator last)
    {
    	return prepend(string(first, last));
    }


    /**
     *  @brief  Insert multiple characters.
     *
     *  @param p  Iterator referencing location in string to insert at.
     *  @param n  Number of characters to insert
     *  @param c  The character to insert.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     *
     *  @details Inserts @a n copies of character @a c starting at the
     *  position referenced by iterator @a p.  If adding characters causes
     *  the length to exceed max_size(), length_error is thrown.
     *  The value of the string doesn't change if an error is thrown.
    */
    void insert (iterator p, size_type n, value_type c)
    {
    	string s(n, c);
    	_d.insert(std::distance(begin().base(), p.base()), s._d);
    }


    // FIXME
#if __FIXME__
    /**
     *  @brief  Insert a range of characters.
     *  @param __p  Iterator referencing location in string to insert at.
     *  @param __beg  Start of range.
     *  @param __end  End of range.
     *  @throw  std::length_error  If new length exceeds @c max_size().
     *
     *  Inserts characters in range [__beg,__end).  If adding
     *  characters causes the length to exceed max_size(),
     *  length_error is thrown.  The value of the string doesn't
     *  change if an error is thrown.
    */
    template <class InputIterator>
    void insert (iterator p, InputIterator beg, InputIterator end);

#if __cplusplus >= 201103L
    /**
     *  @brief  Insert an initializer_list of characters.
     *  @param __p  Iterator referencing location in string to insert at.
     *  @param __l  The initializer_list of characters to insert.
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    void insert (iterator p, initializer_list<value_type> l);
#endif // C++11
#endif


    /**
     *  @brief Insert value of a string.
     *
     *  @details Inserts value of @a str starting at @a pos1.
     *           If adding characters causes the length to exceed max_size(),
     *           length_error is thrown. The value of the string doesn't
     *           change if an error is thrown.
     *
     *  @param pos1  Iterator referencing location in string to insert at.
     *  @param str   The string to insert.
     *
     *  @return Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    string & insert (size_type pos, const string & str);

    /**
     *  @brief Insert a substring.
     *
     *  @details Starting at @a pos1, insert @a n character of @a str
     *           beginning with @a pos2. If adding characters causes the
     *           length to exceed max_size(), length_error is thrown.
     *           If @a pos1 is beyond the end of this string or @a pos2 is
     *           beyond the end of @a str, out_of_range is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param pos1  Iterator referencing location in string to insert at.
     *  @param str  The string to insert.
     *  @param pos2  Start of characters in str to insert.
     *  @param n  Number of characters to insert.
     *
     *  @return Reference to this string.
     *  @throw std::length_error  If new length exceeds @c max_size().
     *  @throw std::out_of_range  If @a pos1 > size() or @a pos2 > @a str.size().
     */
    string & insert (size_type pos1, const string & str, size_type pos2, size_type n)
    {
    	return this->insert(pos1, str.substr(pos2, n));
    }


    /**
     *  @brief Insert a C substring.
     *
     *  @details Inserts the first @a n characters of @a s starting at @a pos.
     *           If adding characters causes the length to exceed
     *           max_size(), length_error is thrown.  If @a pos is beyond end(),
     *           out_of_range is thrown. The value of the string doesn't change
     *           if an error is thrown.
     *
     *  @param pos  Iterator referencing location in string to insert at.
     *  @param s  The C string to insert.
     *  @param n  The number of characters to insert.
     *  @return   Reference to this string.
     *
     *  @throw  std::length_error If new length exceeds @c max_size().
     *  @throw  std::out_of_range If @a pos is beyond the end of this string.
     */
    string & insert (size_type pos, const_pointer s, size_type n)
    {
    	return this->insert(pos, string(s, n));
    }


    /**
     *  @brief Insert a C string.
     *
     *  @details Inserts the first @a n characters of @a s starting at @a pos.
     *           If adding characters causes the length to exceed max_size(),
     *           length_error is thrown. If @a pos is beyond end(),
     *           out_of_range is thrown. The value of the string doesn't
     *           change if an error is thrown.
     *
     *  @param pos Iterator referencing location in string to insert at.
     *  @param s   The C string to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error If new length exceeds @c max_size().
     *  @throw  std::out_of_range If @a pos is beyond the end of this string.
     *
     */
    string & insert (size_type pos, const_pointer s)
    {
    	return this->insert(pos, string(s));
    }


    /**
     *  @brief  Insert multiple characters.
     *  @details Inserts @a n copies of character @a c starting at index @a pos.
     *           If adding characters causes the length to exceed max_size(),
     *           length_error is thrown. If @a pos > length(), out_of_range is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param pos  Index in string to insert at.
     *  @param n  Number of characters to insert
     *  @param c  The character to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error If new length exceeds @c max_size().
     *  @throw  std::out_of_range If @a pos is beyond the end of this string.
     */
    string & insert (size_type pos, size_type n, value_type c)
    {
    	return this->insert(pos, string(n, c));
    }


    /**
     *  @brief Insert one character.
     *
     *  @details Inserts character @a c at position referenced by @a p.
     *           If adding character causes the length to exceed max_size(),
     *           length_error is thrown. If @a p is beyond end of string,
     *           out_of_range is thrown. The value of the string doesn't
     *           change if an error is thrown.
     *
     *  @param p  Iterator referencing position in string to insert at.
     *  @param c  The character to insert.
     *
     *  @return Iterator referencing newly inserted char.
     *
     *  @throw std::length_error If new length exceeds @c max_size().
     */
    iterator insert (iterator p, value_type c);

    /**
     *  @brief  Remove characters.
     *
     *  @details Removes @a n characters from this string starting at @a pos.
     *           The length of the string is reduced by @a n.
     *           If there are < @a n characters to remove, the remainder of
     *           the string is truncated. If @a p is beyond end of string,
     *           out_of_range is thrown.  The value of the string doesn't
     *           change if an error is thrown.
     *
     *  @param pos Index of first character to remove (default 0).
     *  @param n   Number of characters to remove (default remainder).
     *
     *  @return Reference to this string.
     *
     *  @throw  std::out_of_range If @a pos is beyond the end of this string.
     */
    string & erase (size_type pos = 0, size_type n = max_value<size_type>());

    /**
     *  @brief  Remove one character.
     *
     *  @details Removes the character at @a pos from this string. The value
     *           of the string doesn't change if an error is thrown.
     *
     *  @param pos Iterator referencing the character to remove.
     *
     *  @return Iterator referencing same location after removal.
     */
    iterator erase (iterator position);

    /**
     *  @brief  Remove a range of characters.
     *
     *  @details Removes the characters in the range [first,last) from this string.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param first  Iterator referencing the first character to remove.
     *  @param last  Iterator referencing the end of the range.
     *
     *  @return  Iterator referencing location of first after removal.
     */
    iterator erase (iterator first, iterator last);

    /**
     *  @brief  Replace characters with value from another string.
     *
     *  @details Removes the characters in the range [@a pos, @a pos + @a n) from
     *           this string.  In place, the value of @a str is inserted.
     *           If @a pos is beyond end of string, out_of_range is thrown.
     *           If the length of the result exceeds max_size(), length_error
     *           is thrown. The value of the string doesn't change if an
     *           error is thrown.
     *
     *  @param pos  Index of first character to replace.
     *  @param n    Number of characters to be replaced.
     *  @param str  String to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::out_of_range  If @a pos is beyond the end of this string.
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    string & replace (size_type pos, size_type n, const string & str);

    /**
     *  @brief  Replace characters with value from another string.
     *  @param __pos1  Index of first character to replace.
     *  @param __n1  Number of characters to be replaced.
     *  @param __str  String to insert.
     *  @param __pos2  Index of first character of str to use.
     *  @param __n2  Number of characters from str to use.
     *  @return  Reference to this string.
     *  @throw  std::out_of_range  If @a __pos1 > size() or @a __pos2 >
     *  __str.size().
     *  @throw  std::length_error  If new length exceeds @c max_size().
     *
     *  Removes the characters in the range [__pos1,__pos1 + n) from this
     *  string.  In place, the value of @a __str is inserted.  If @a __pos is
     *  beyond end of string, out_of_range is thrown.  If the length of the
     *  result exceeds max_size(), length_error is thrown.  The value of the
     *  string doesn't change if an error is thrown.
    */
    // FIXME Implement
#if __FIXME__
    string & replace (size_type pos1, size_type n1, const string & str, size_type pos2, size_type n2);
#endif
    /**
     *  @brief  Replace characters with value of a C substring.
     *  @param __pos  Index of first character to replace.
     *  @param __n1  Number of characters to be replaced.
     *  @param __s  C string to insert.
     *  @param __n2  Number of characters from @a s to use.
     *  @return  Reference to this string.
     *  @throw  std::out_of_range  If @a pos1 > size().
     *  @throw  std::length_error  If new length exceeds @c max_size().
     *
     *  Removes the characters in the range [__pos,__pos + __n1)
     *  from this string.  In place, the first @a __n2 characters of
     *  @a __s are inserted, or all of @a __s if @a __n2 is too large.  If
     *  @a __pos is beyond end of string, out_of_range is thrown.  If
     *  the length of result exceeds max_size(), length_error is
     *  thrown.  The value of the string doesn't change if an error
     *  is thrown.
    */
    // FIXME Implement
#if __FIXME__
    string & replace (size_type pos, size_type n1, const_pointer s, size_type n2);
#endif

    /**
     *  @brief  Replace characters with value of a C string.
     *  @param __pos  Index of first character to replace.
     *  @param __n1  Number of characters to be replaced.
     *  @param __s  C string to insert.
     *  @return  Reference to this string.
     *  @throw  std::out_of_range  If @a pos > size().
     *  @throw  std::length_error  If new length exceeds @c max_size().
     *
     *  Removes the characters in the range [__pos,__pos + __n1)
     *  from this string.  In place, the characters of @a __s are
     *  inserted.  If @a __pos is beyond end of string, out_of_range
     *  is thrown.  If the length of result exceeds max_size(),
     *  length_error is thrown.  The value of the string doesn't
     *  change if an error is thrown.
    */
    // FIXME Implement
#if __FIXME__
    string & replace (size_type pos, size_type n1, const_pointer s);
#endif

    /**
     *  @brief  Replace characters with multiple characters.
     *  @param __pos  Index of first character to replace.
     *  @param __n1  Number of characters to be replaced.
     *  @param __n2  Number of characters to insert.
     *  @param __c  Character to insert.
     *  @return  Reference to this string.
     *  @throw  std::out_of_range  If @a __pos > size().
     *  @throw  std::length_error  If new length exceeds @c max_size().
     *
     *  Removes the characters in the range [pos,pos + n1) from this
     *  string.  In place, @a __n2 copies of @a __c are inserted.
     *  If @a __pos is beyond end of string, out_of_range is thrown.
     *  If the length of result exceeds max_size(), length_error is
     *  thrown.  The value of the string doesn't change if an error
     *  is thrown.
    */
    // FIXME Implement
#if __FIXME__
    string & replace (size_type pos, size_type n1, size_type n2, value_type c);
#endif

    /**
     *  @brief  Replace range of characters with string.
     *
     *  @details Removes the characters in the range [@a i1, @a i2).
     *           In place, the value of @a str is inserted. If the length
     *           of result exceeds @c max_size(), length_error is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param i1  Iterator referencing start of range to replace.
     *  @param i2  Iterator referencing end of range to replace.
     *  @param str  String value to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    string & replace (iterator i1, iterator i2, const string & str)
    {
    	_d.replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, str._d);
    	return *this;
    }

    /**
     *  @brief  Replace range of characters with C substring.
     *
     *  @details Removes the characters in the range [@a i1, @a i2). In place,
     *           the first @a n characters of @a s are inserted. If the
     *           length of result exceeds @c max_size(), length_error is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param i1  Iterator referencing start of range to replace.
     *  @param i2  Iterator referencing end of range to replace.
     *  @param s  C string value to insert.
     *  @param n  Number of characters from s to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    string & replace (iterator i1, iterator i2, const_pointer s, size_type n)
    {
    	return this->replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, string(s, n));
    }

    /**
     *  @brief  Replace range of characters with C string.
     *
     *  @details Removes the characters in the range [@a i1, @a i2).
     *           In place, the characters of @a s are inserted. If the length of
     *           result exceeds @c max_size(), length_error is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param i1  Iterator referencing start of range to replace.
     *  @param i2  Iterator referencing end of range to replace.
     *  @param s  C string value to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    string & replace (iterator i1, iterator i2, const_pointer s)
    {
    	return this->replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, string(s));
    }

    /**
     *  @brief  Replace range of characters with multiple characters.
     *
     *  @details Removes the characters in the range [@a i1, @ai2).
     *           In place, @a n copies of @a c are inserted. If the length of
     *           result exceeds @c max_size(), length_error is thrown. The
     *           value of the string doesn't change if an error is thrown.
     *
     *  @param i1  Iterator referencing start of range to replace.
     *  @param i2  Iterator referencing end of range to replace.
     *  @param n  Number of characters to insert.
     *  @param c  Character to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    string & replace (iterator i1, iterator i2, size_type n, value_type c)
    {
    	return this->replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, string(n, c));
    }

    /**
     *  @brief  Replace range of characters with range.
     *
     *  @details Removes the characters in the range [@a i1, @a i2).
     *           In place, characters in the range [@a k1, @a k2) are inserted.
     *           If the length of result exceeds max_size(), length_error is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param i1  Iterator referencing start of range to replace.
     *  @param i2  Iterator referencing end of range to replace.
     *  @param k1  Iterator referencing start of range to insert.
     *  @param k2  Iterator referencing end of range to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    template <class InputIterator>
    string & replace (iterator i1, iterator i2, InputIterator k1, InputIterator k2)
    {
    	return this->replace(i1, i2, string(k1, k2));
    }

    // Specializations for the common case of pointer and iterator:
    // useful to avoid the overhead of temporary buffering in _M_replace.
    //
    string & replace (iterator i1, iterator i2, pointer k1, pointer k2)
    {
    	_d.replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, k1, k2);
    	return *this;
    }

    string & replace (iterator i1, iterator i2, const_pointer k1, const_pointer k2)
    {
    	_d.replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, k1, k2);
    	return *this;
    }

    string & replace (iterator i1, iterator i2, iterator k1, iterator k2)
    {
    	_d.replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, utf_traits_type::to_rep_iterator(k1)
    		, utf_traits_type::to_rep_iterator(k2));
    	return *this;
    }

    string & replace (iterator i1, iterator i2, const_iterator k1, const_iterator k2)
    {
    	_d.replace(utf_traits_type::to_rep_iterator(i1)
    		, utf_traits_type::to_rep_iterator(i2)
    		, utf_traits_type::to_rep_iterator(k1)
    		, utf_traits_type::to_rep_iterator(k2));
    	return *this;
    }

#if __cplusplus >= 201103L
    /**
     *  @brief  Replace range of characters with initializer_list.
     *
     *  @details Removes the characters in the range [i1, i2).  In place,
     *           characters in the range [k1, k2) are inserted.  If the
     *           length of result exceeds max_size(), length_error is thrown.
     *           The value of the string doesn't change if an error is thrown.
     *
     *  @param i1  Iterator referencing start of range to replace.
     *  @param i2  Iterator referencing end of range to replace.
     *  @param l  The initializer_list of characters to insert.
     *
     *  @return  Reference to this string.
     *
     *  @throw  std::length_error  If new length exceeds @c max_size().
     */
    // FIXME Implement
#if __FIXME__
    string & replace (iterator i1, iterator i2, initializer_list<value_type> l);
#endif
#endif // C++11


	string & push_back (value_type c)
	{
		utf_traits_type::encode(c, std::back_inserter(_d));
		return *this;
	}

	string & push_back (char c)
	{
		return push_back(value_type(c));
	}

    value_type at (size_type index) const
    {
    	const_iterator it(begin());
    	std::advance(it, index);
    	PFS_ASSERT(it < end());
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

    size_type capacity() const
    {
    	return base_class::cast()->capacity();
    }

    size_type max_size() const
    {
    	return base_class::cast()->max_size();
    }

    void pop_back ()
    {
    	if (!isEmpty())
    		erase(length() - 1);
    }

private:
    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::utf_string’"
    template <typename InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last, utf_string_type_trait<InputIt>);
    iterator insert (const_iterator pos, const_iterator first, const_iterator last, utf_string_type_trait<const_iterator>);

    // This private methods need to fix "error: explicit specialization in non-namespace scope ‘class pfs::utf_string’"
    template <typename InputIt>
    utf_string & replace (const_iterator first, const_iterator last, InputIt first2, InputIt last2, utf_string_type_trait<InputIt>);
    utf_string & replace (const_iterator first, const_iterator last, const_iterator first2, const_iterator last2, utf_string_type_trait<const_iterator>);

#endif // __COMMENT__

	bool contains (const string & s) const
	{
		return _d.find(s._d) != rep_type::npos;
	}

	bool contains (const char * s) const
	{
		return _d.find(string(s)._d) != rep_type::npos;
	}

	bool contains (value_type c) const
	{
		return _d.find(string(1, c)._d) != rep_type::npos;
	}

	bool contains (char c) const
	{
		return _d.find(string(1, c)._d) != rep_type::npos;
	}

	bool starts_with (const string & needle) const;

	bool ends_with (const string & needle) const;

	string substr (const_iterator begin, const_iterator end) const;

	/**
	 * @brief Returns a substring [@a pos, @a pos + @a count).
	 *
	 * @details If the requested substring extends past the end of the string,
	 *          or if @a count == pfs::max_value<size_type>(),
	 *          the returned substring is [@a pos, length()).
	 *
	 * @param pos Position of the first character to include.
	 * @param count Length of the substring.
	 * @return String containing the substring [@a pos, @a pos + @a count)
	 *         or an empty string if @a pos == length().
	 */
	string substr (size_type pos, size_type count) const;

	string substr (const_iterator begin) const
	{
		return substr(begin, end());
	}

	string substr (size_t pos) const
	{
		return substr(pos, max_value<size_type>());
	}

	string mid (size_t pos, size_t count) const
	{
		return substr(pos, count);
	}

	string left (size_t count) const
	{
		return substr(0, count);
	}

	string right (size_t count) const
	{
		// TODO Optimize it (avoid length() using)
		return substr(length() - count, count);
	}
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
intmax_t utf_string<CodeUnitT>::toSignedIntegral (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return strtointegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, intmax_t(pfs::min_value<intmax_t>())
		, uintmax_t(pfs::max_value<intmax_t>()));
}

template <typename CodeUnitT>
uintmax_t utf_string<CodeUnitT>::toUnsignedIntegral (bool * ok, int base) const
{
	if (this->isEmpty()) {
		if (ok) *ok = false;
		return 0;
	}
	return strtouintegral_helper<utf_string<CodeUnitT>::char_type, utf_string<CodeUnitT>::const_iterator >
		(cbegin(), cend(), ok, base
		, uintmax_t(pfs::max_value<uintmax_t>()));
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
        , intmax_t(pfs::min_value<signed char>())
        , uintmax_t(pfs::max_value<signed char>())));
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
        , uintmax_t(pfs::max_value<unsigned char>())));
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
		, intmax_t(pfs::min_value<short>())
		, uintmax_t(pfs::max_value<short>()));
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
		, uintmax_t(pfs::max_value<unsigned short>()));
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
		, intmax_t(pfs::min_value<int>())
		, uintmax_t(pfs::max_value<int>()));
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
		, uintmax_t(pfs::max_value<unsigned int>()));
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
		, intmax_t(pfs::min_value<long>())
		, uintmax_t(pfs::max_value<long>()));
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
		, uintmax_t(pfs::max_value<unsigned long>()));
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
		, intmax_t(pfs::min_value<long long>())
		, uintmax_t(pfs::max_value<long long>()));
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
		, uintmax_t(pfs::max_value<unsigned long long>()));
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

#endif // __COMMENT__

template <typename CodeUnit, typename UtfTag>
string<CodeUnit, UtfTag> & string<CodeUnit, UtfTag>::insert (size_type pos, const string & str)
{
	iterator it = begin();
	std::advance(it, pos);
	_d.insert(std::distance(begin().base(), it.base()), str._d);
	return *this;
}

template <typename CodeUnit, typename UtfTag>
typename string<CodeUnit, UtfTag>::iterator string<CodeUnit, UtfTag>::insert (iterator p, value_type c)
{
	// TODO Need to optimize
	//
	size_type pos = std::distance(begin(), p);
	this->insert(p, 1, c);
	iterator it = this->begin();
	std::advance(it, pos);
	return it;
}

template <typename CodeUnit, typename UtfTag>
string<CodeUnit, UtfTag> & string<CodeUnit, UtfTag>::erase (size_type pos, size_type n)
{
	size_type length = this->length();

	if (pos > length)
		pos = length;

	if (pos + n > length)
		n = length - pos;

	if (pos == 0 && n >= length) {
		_d.erase();
	} else {
		iterator first = this->begin();
		iterator last = this->begin();

		std::advance(first, pos);
		std::advance(last, pos + n);

		this->erase(first, last);
	}

	return *this;
}

template <typename CodeUnit, typename UtfTag>
inline typename string<CodeUnit, UtfTag>::iterator string<CodeUnit, UtfTag>::erase (iterator position)
{
	iterator last = position;
	std::advance(last, 1);
	return this->erase(position, last);
}

template <typename CodeUnit, typename UtfTag>
typename string<CodeUnit, UtfTag>::iterator string<CodeUnit, UtfTag>::erase (iterator first, iterator last)
{
	pointer p0 = this->begin().base();
	pointer p1 = first.base();
	pointer p2 = last.base();
	size_type pos = std::distance(p0, p1);
	size_type n   = std::distance(p1, p2);

	_d.erase(pos, n);

	return string::iterator(this->begin().base() + pos);
}

template <typename CodeUnit, typename UtfTag>
string<CodeUnit, UtfTag> & string<CodeUnit, UtfTag>::replace (size_type pos, size_type n, const string & str)
{
	iterator first(this->begin());
	std::advance(first, pos);
	iterator last(first);
	std::advance(last, n);
	return this->replace(first, last, str);
}

template <typename CodeUnit, typename UtfTag>
string<CodeUnit, UtfTag> string<CodeUnit, UtfTag>::substr (const_iterator begin, const_iterator end) const
{
	if (begin >= this->end())
		return string();

	if (end >= this->end())
		end == this->end();

	return string(begin, end);
}


template <typename CodeUnit, typename UtfTag>
string<CodeUnit, UtfTag> string<CodeUnit, UtfTag>::substr (size_type pos, size_type count) const
{
	const_iterator begin = this->begin();
	std::advance(begin, pos);

	const_iterator end = begin;
	if (count == max_value<size_type>())
		end = this->end();
	else
		std::advance(end, count);

	return substr(begin, end);
}

template <typename CodeUnit, typename UtfTag>
inline string<CodeUnit, UtfTag> operator + (string<CodeUnit, UtfTag> const & lhs
    , string<CodeUnit, UtfTag> const & rhs)
{
	string<CodeUnit, UtfTag> r(lhs);
    return r.append(rhs);
}

template <typename CodeUnit, typename UtfTag>
inline string<CodeUnit, UtfTag> operator + (string<CodeUnit, UtfTag> const & lhs
    , char const * rhs)
{
	string<CodeUnit, UtfTag> r(lhs);
    return r.append(rhs);
}

template <typename CodeUnit, typename UtfTag>
inline string<CodeUnit, UtfTag> operator + (char const * lhs
    , string<CodeUnit, UtfTag> const & rhs)
{
	string<CodeUnit, UtfTag> r(lhs);
    return r.append(rhs);
}

template <typename CodeUnit, typename UtfTag>
inline string<CodeUnit, UtfTag> operator + (string<CodeUnit, UtfTag> const & lhs
    , char const rhs)
{
	string<CodeUnit, UtfTag> r(lhs);
    return r.append(1, rhs);
}

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
