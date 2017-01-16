/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:49 AM
 * 
 * @brief Immutable string interface.
 */

#ifndef __PFS_MPL_STRING_HPP__
#define __PFS_MPL_STRING_HPP__

#include <pfs/exception.hpp>
#include <pfs/iterator.hpp>
#include <pfs/mpl/algo/find.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {
namespace mpl {

template <typename T>
struct string_traits
{
    typedef T const &                          const_impl_reference;
    typedef typename T::size_type              size_type;
    typedef typename T::value_type             value_type;
    typedef typename T::const_pointer          const_pointer;
    typedef typename T::const_iterator         const_iterator;
    typedef typename T::const_reverse_iterator const_reverse_iterator;
    
    typedef T data_type;
};

template <typename T>
class basic_string;

namespace details {
    
template <typename T>
class basic_string 
{
public:
    typedef string_traits<T>                             traits_type;
    typedef typename traits_type::const_impl_reference   const_impl_reference;
    typedef typename traits_type::size_type              size_type;
    typedef typename traits_type::value_type             value_type;
    typedef typename traits_type::const_pointer          const_pointer;
    typedef typename traits_type::const_iterator         const_iterator;
    typedef typename traits_type::const_reverse_iterator const_reverse_iterator;
    typedef typename traits_type::data_type              data_type;

public:
    data_type _d;

public:
    virtual const_impl_reference xbase () const = 0;
    virtual size_type xsize () const = 0;
    virtual const_iterator xbegin () const = 0;
    virtual const_iterator xend () const = 0;
    virtual const_reverse_iterator xrbegin () const = 0;
    virtual const_reverse_iterator xrend () const = 0;
    virtual value_type xat (size_type pos) const = 0;

    virtual int xcompare (size_type pos1, size_type count1
        , basic_string const & rhs, size_type pos2, size_type count2) const = 0;
    virtual size_type xfind (const_impl_reference rhs, size_type pos) const = 0;
    virtual size_type xfind (value_type c, size_type pos) const = 0;
    virtual size_type xrfind (value_type c, size_type pos) const = 0;
    virtual size_type xrfind (const_impl_reference rhs, size_type pos) const = 0;
    
public:
    basic_string ()
    {}

    basic_string (data_type const & d)
        : _d(d)
    {}
    
//	string<T> substr (size_type pos = 0, size_type count = size_type(-1)) const
//    {
//        return this->xsubstr(pos, count);
//    }
    
//    string<T> substr (const_iterator begin, const_iterator end) const
//    {
//        
//    }

//
//	string substr (const_iterator begin) const
//	{
//		return substr(begin, end());
//	}
//
//	string substr (size_t pos) const
//	{
//		return substr(pos, max_value<size_type>());
//	}
};

} // details

template <typename T>
class string : public basic_string<T>
{
    typedef basic_string<T> basic_type;
    
public:    
    typedef typename basic_type::traits_type            traits_type;
    typedef typename basic_type::const_impl_reference   const_impl_reference;
    typedef typename basic_type::size_type              size_type;
    typedef typename basic_type::value_type             value_type;
    typedef typename basic_type::const_pointer          const_pointer;
    typedef typename basic_type::const_iterator         const_iterator;
    typedef typename basic_type::const_reverse_iterator const_reverse_iterator;
    
public:
    string ()
        : basic_type()
    {}
    
    explicit string (const_impl_reference s)
        : basic_type(s)
    {}

    /**
     * @details @a str Interpreted as UTF-8 encoded string. 
     * @param str
     * @note 
     */
    explicit string (char const * str, size_type n)
        : basic_type(str)
    {}
    
#ifdef _WCHAR_H
    /**
     * @details If @c wchar_t is 4 bytes, the string is interpreted as UCS-4, 
     *          if @c wchar_t is 2 bytes it is interpreted as UCS-2.
     * @param s
     */
    explicit string (wchar_t const * str, size_type n)
        : basic_type(str)
    {}
#endif
    
    string (const_iterator begin, const_iterator end)
        : basic_type(begin, end)
    {}

    string (string const & rhs)
        : basic_type(rhs)
    {}
    
    /**
     * @function string & string::operator = (string const & rhs)
     * @param rhs
     * @return 
     */

    /**
     * @param rhs
     * @return 
     */
    string & operator = (const_impl_reference rhs)
    {
        basic_type::operator = (rhs);
        return *this;
    }

    const_impl_reference base () const
    {
        return this->xbase();
    }

    size_type size () const
    {
        return this->xsize();
    }
    
    size_type length () const
    {
        return this->size();
    }
    
  	/**
	 * @brief Checks if string is empty.
	 * @return @c true if string is empty (size() == 0) or @c false otherwise.
	 */
    bool empty () const
    {
    	return this->size() == 0;
    }
    
    const_iterator begin () const
    {
        return this->xbegin();
    }

    const_iterator end () const
    {
        return this->xend();
    }

    const_iterator cbegin () const
    {
        return this->begin();
    }

    const_iterator cend () const
    {
        return this->end();
    }

    const_reverse_iterator rbegin () const
    {
        return this->xrbegin();
    }

    const_reverse_iterator rend () const
    {
        return this->xrend();
    }

    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }

    const_reverse_iterator crend () const
    {
        return this->rend();
    }

    /**
     * @brief
     * @param pos Position of the character to return.
     * @trows out_of_range if pos >= size()
     */
    value_type at (size_type pos) const
    {
        if (pos >= this->size())
            throw out_of_range("string::at");
        return this->xat(pos);
    }
    
    value_type operator [] (size_type pos) const
    {
    	return this->at(pos);
    }
    
    int compare (size_type pos1, size_type count1
        , string const & rhs, size_type pos2, size_type count2) const
    {
        return this->xcompare(pos1, count1, rhs, pos2, count2) ;
    }
    
    int compare (size_type pos1, size_type count1, string const & rhs) const
    {
        return this->xcompare(pos1, count1, rhs, 0, rhs.size());
    }
    
    int compare (string const & rhs) const
    {
        return this->xcompare(0, this->size(), rhs, 0, rhs.size());
    }
    
    bool starts_with (string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return this->compare(0, rhs.size(), rhs) == 0;
    }
    
    /**
     * 
     * @param rhs
     * @return 
     */
  	bool ends_with (string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return this->compare(this->size() - rhs.size(), rhs.size(), rhs) == 0;
    }
    
    bool contains (string const & rhs, size_type pos = 0) const
	{
		return this->xfind(rhs.base(), pos) != size_type(-1);
	}

    bool contains (const_impl_reference rhs, size_type pos = 0)
    {
        return this->xfind(rhs, pos) != size_type(-1);
    }
    
	bool contains (value_type c, size_type pos = 0) const
	{
        return this->xfind(c, pos) != size_type(-1);
	}
   
    /**
     * @brief Find position of a string.
     * 
     * @details Starting from @a pos, searches forward for value 
     *          of @a str within this string. 
     *          If found, returns the index where it begins.
     *          If not found, returns size_type(-1).
     * 
     * @param rhs  String to locate.
     * @param pos  Index of character to search from (default 0).
     * @return Index of start of first occurrence.
     *
     */
    size_type find (string const & rhs, size_type pos = 0) const
    {
        return this->xfind(rhs.base(), pos);
    }
    
    size_type find (const_impl_reference rhs, size_type pos = 0) const
    {
        return this->xfind(rhs.base(), pos);
    }
    
    size_type find (value_type c, size_type pos = 0) const
    {
        return this->xfind(c, pos);
    }
    
    size_type rfind (string const & rhs, size_type pos = size_type(-1)) const
    {
        return this->xrfind(rhs.base(), pos);
    }
    
    size_type rfind (const_impl_reference rhs, size_type pos = size_type(-1)) const
    {
        return this->xfind(rhs.base(), pos);
    }
    
    size_type rfind (value_type c, size_type pos = size_type(-1)) const
    {
        return this->xrfind(c, pos);
    }
    
    /**
     * @function string substr (size_type pos, size_type count) const;
     * @brief  Get a substring.
     * @details Construct and return a new string using the @a n
     *          characters starting at @a pos.  If the string is too
     *          short, use the remainder of the characters.  If @a pos is
     *          beyond the end of the string, out_of_range is thrown. 
     * @param pos  Index of first character (default 0).
     * @param n  Number of characters in substring (default remainder).
     * @return The new string.
     * @throw out_of_range  If pos > size().
     */
    string substr (size_type pos, size_type count = size_type(-1)) const;
    
	string left (size_t count) const
	{
		return substr(0, count);
	}

	string right (size_t count) const
	{
		return substr(this->size() - count, count);
	}
};

template <typename T>
string<T> string<T>::substr (size_type pos, size_type count) const
{
    if (pos >= this->size())
        throw out_of_range("string::substr");
    size_type n = this->size() - pos;

    if (n > count)
        n = count;

    const_iterator b(this->begin());
    const_iterator e(this->begin());
    pfs::advance(b, pos);
    pfs::advance(e, pos + n);

    return string(b, e);
}

template <typename T>
inline bool operator == (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) == 0;
}

template <typename T>
inline bool operator != (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) != 0;
}

template <typename T>
inline bool operator < (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) < 0;
}

template <typename T>
inline bool operator <= (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) <= 0;
}

template <typename T>
inline bool operator > (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) > 0;
}

template <typename T>
inline bool operator >= (string<T> const & lhs
    , string<T> const & rhs)
{
    return lhs.compare(rhs) >= 0;
}

}} // pfs::mpl

#endif /* __PFS_MPL_STRING_HPP__ */

