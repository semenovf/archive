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

protected:
    virtual int xcompare (size_type pos1, size_type count1
        , basic_string const & rhs, size_type pos2, size_type count2) const = 0;

    // Default implementation
//    virtual inline const_iterator xfind (const_iterator begin1
//        , const_iterator end1
//		, const_iterator begin2
//		, const_iterator end2) const
//    {
//        return pfs::mpl::find(begin1, end1, begin2, end2);
//    }

    virtual size_type xfind (const_impl_reference rhs, size_type pos) const = 0;
    
    
//    virtual inline const_iterator xfind (const_iterator begin
//        , const_iterator end
//		, const_impl_reference rhs) const
//    {
//        return pfs::mpl::find(begin, end, rhs.begin(), rhs.end());
//    }

    virtual size_type xfind (value_type c, size_type pos) const = 0;
    
    virtual size_type xrfind (value_type c, size_type pos) const = 0;
    virtual size_type xrfind (const_impl_reference rhs, size_type pos) const = 0;
    
//    virtual inline const_iterator xfind (const_iterator begin
//        , const_iterator end
//		, value_type c) const
//    {
//        const_iterator i(begin);
//        for (; i != end && *i != c; ++i)
//            ;
//        return i;
//    }
    
public:
    basic_string ()
    {}

    basic_string (data_type const & d)
        : _d(d)
    {}
    
    virtual const_impl_reference base () const = 0;
    
    virtual size_type size () const = 0;
    
    size_type length () const
    {
        return this->size();
    }
    
    bool empty () const
    {
    	return this->size() == 0;
    }
    
    virtual const_iterator begin () const = 0;

    virtual const_iterator end () const = 0;

    const_iterator cbegin () const
    {
        return this->begin();
    }

    const_iterator cend () const
    {
        return this->end();
    }

    virtual const_reverse_iterator rbegin () const = 0;

    virtual const_reverse_iterator rend () const = 0;

    const_reverse_iterator crbegin () const
    {
        return this->rbegin();
    }

    const_reverse_iterator crend () const
    {
        return this->rend();
    }
    
    virtual value_type at (size_type pos) const = 0;

    value_type operator [] (size_type pos) const
    {
    	return this->at(pos);
    }

    int compare (size_type pos1, size_type count1
        , basic_string const & rhs, size_type pos2, size_type count2) const
    {
        return this->xcompare(pos1, count1, rhs, pos2, count2) ;
    }
    
    int compare (size_type pos1, size_type count1, basic_string const & rhs) const
    {
        return this->xcompare(pos1, count1, rhs, 0, rhs.size());
    }
    
    int compare (basic_string const & rhs) const
    {
        return this->xcompare(0, this->size(), rhs, 0, rhs.size());
    }
    
    bool starts_with (basic_string const & rhs) const
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
  	bool ends_with (basic_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return this->compare(this->size() - rhs.size(), rhs.size(), rhs) == 0;
    }
    
    bool contains (basic_string const & rhs, size_type pos = 0) const
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
    
    size_type find (basic_string const & rhs, size_type pos = 0) const
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
    
    size_type rfind (basic_string const & rhs, size_type pos = size_type(-1)) const
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
    explicit string (char const * str, size_type n = size_type(-1))
        : basic_type(str)
    {}
    
#ifdef _WCHAR_H
    /**
     * @details If @c wchar_t is 4 bytes, the string is interpreted as UCS-4, 
     *          if @c wchar_t is 2 bytes it is interpreted as UCS-2.
     * @param s
     */
    explicit string (wchar_t const * str, size_type n = size_type(-1))
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

  	/**
     * @function bool string::empty () const
	 * @brief Checks if string is empty.
	 *
	 * @return @c true if string is empty (size() == 0) or @c false otherwise.
	 */
    
    /**
     * @function value_type at (size_type pos) const
     * @param pos Position of the character to return.
     * @trows out_of_range if pos >= size()
     */

    /**
     * @function value_type string::operator [] (size_type pos) const
     * @param index
     * @return 
     */

    
    /**
     * @brief Find position of a string.
     * 
     * @details Starting from @a pos, searches forward for value 
     *          of @a str within this string. 
     *          If found, returns the index where it begins.
     *          If not found, returns size_type(-1).
     * 
     * @param str  String to locate.
     * @param pos  Index of character to search from (default 0).
     * @return Index of start of first occurrence.
     *
     */

    
//	string substr (const_iterator begin, const_iterator end) const
//    {
////        TODO:        
////        PFS_ASSERT_INVALID_ARGUMENT(begin <= end);
////        PFS_ASSERT_INVALID_ARGUMENT(begin >= _begin);
////        PFS_ASSERT_INVALID_ARGUMENT(end < _end);
//
//        return string(begin, end);
//    }
//    
//    string substr (const_iterator begin) const
//    {
//        return string(begin, this->end());
//    }
//    
//    string substr (size_type pos, size_type count) const
//    {
//        return this->substr(begin + pos, begin + pos + count);
//    }
//    
//	string substr (size_t pos) const
//    {
//        return this->substr(begin + pos, this->end());
//    }
//
//	string left (size_t count) const
//	{
//		return substr(0, count);
//	}
//
//	string right (size_t count) const
//	{
//		return substr(length() - count, count);
//	}
};

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

