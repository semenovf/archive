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

#include <pfs/mpl/algo/find.hpp>
#include <pfs/type_traits.hpp>

#ifndef PFS_INC_STRING_H
#   if defined(_STRING_H)
#       define PFS_INC_STRING_H 1
#   endif
#endif

#ifndef PFS_INC_WCHAR_H
#   if defined(_WCHAR_H)
#       define PFS_INC_WCHAR_H 1
#   endif
#endif

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
class basic_string
{
public:
    typedef string_traits<T>                        traits;
    typedef typename traits::const_impl_reference   const_impl_reference;
    typedef typename traits::size_type              size_type;
    typedef typename traits::value_type             value_type;
    typedef typename traits::const_pointer          const_pointer;
    typedef typename traits::const_iterator         const_iterator;
    typedef typename traits::const_reverse_iterator const_reverse_iterator;
    
protected:    
    typedef typename traits::data_type data_type;
    data_type _d;

public:
    basic_string ()
    {}
    
    basic_string (basic_string const & rhs)
        : _d(rhs._d)
    {}
        
    basic_string & operator = (basic_string const & rhs)
    {
        if (this != & rhs)
            this->_d = rhs._d;
        return *this;
    }

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
    
    virtual int compare (basic_string const & rhs, size_type n) const = 0;
    
    int compare (basic_string const & rhs) const
    {
        return this->compare(rhs, rhs.size());
    }
    
    bool starts_with (basic_string const & rhs) const
    {
        if (rhs.size() > this->size())
            return false;
        
        return compare(rhs, rhs.size());
    }
};

template <typename T>
class string : public basic_string<T>
{
    typedef basic_string<T> basic_type;
    
public:    
    typedef typename basic_type::traits                 traits;
    typedef typename basic_type::const_impl_reference   const_impl_reference;
    typedef typename basic_type::size_type              size_type;
    typedef typename basic_type::value_type             value_type;
    typedef typename basic_type::const_pointer          const_pointer;
    typedef typename basic_type::const_iterator         const_iterator;
    typedef typename basic_type::const_reverse_iterator const_reverse_iterator;
    
public:
    string ();
    
    string (const_impl_reference s);
    
    string (const_iterator begin, const_iterator end);

    string & operator = (const_impl_reference rhs)
    {
        this->_d = rhs;
        return *this;
    }

//    bool starts_with (string const & rhs) const
//    {
//        return pfs::mpl::starts_with(this->begin(), this->end()
//                , rhs.begin(), rhs.end());
//    }
//
//	bool ends_with (string const & rhs) const
//    {
//        return pfs::mpl::ends_with(this->begin(), this->end()
//                , rhs.begin(), rhs.end());
//    }

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

#if PFS_INC_STRING_H || PFS_INC_WCHAR_H
#   include <pfs/mpl/stdc/string.hpp>
#endif

#if PFS_INC_STRING
#   include <string>
#endif

#endif /* __PFS_MPL_STRING_HPP__ */

