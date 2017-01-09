/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string.hpp
 * Author: wladt
 *
 * Created on January 9, 2017, 10:57 AM
 */

#ifndef __PFS_MPL_STDC_STRING_HPP__
#define __PFS_MPL_STDC_STRING_HPP__

#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {

template <typename T>
struct string_traits<T *>
{
    typedef T const * const_impl_reference;
    typedef size_t    size_type;
    typedef T         value_type;
    typedef T const * const_pointer;
    typedef T const * const_iterator;
    typedef std::reverse_iterator<T const *> const_reverse_iterator;

    struct data_type 
    {
        const_iterator begin;
        const_iterator end;
    };
    
    static size_type size (const_pointer);
    static int compare_n (const_pointer lhs, const_pointer rhs, size_type n);
};

template <typename T>
struct string_traits<T const *> : public string_traits<T *>
{};

template <>
inline string_traits<char *>::size_type
string_traits<char *>::size (string_traits<char *>::const_pointer s)
{
    return std::strlen(s);
}

template <>
inline int
string_traits<char *>::compare_n (string_traits<char *>::const_pointer lhs
    , string_traits<char *>::const_pointer rhs
    , string_traits<char *>::size_type n)
{
    return std::strncmp(lhs, rhs, n);
}

#ifdef _WCHAR_H

template <>
inline string_traits<wchar_t *>::size_type
string_traits<wchar_t *>::size (string_traits<wchar_t *>::const_pointer s)
{
    return std::wcslen(s);
}

template <>
inline int
string_traits<wchar_t *>::compare_n (string_traits<wchar_t *>::const_pointer lhs
    , string_traits<wchar_t *>::const_pointer rhs
    , string_traits<wchar_t *>::size_type n)
{
    return std::wcsncmp(lhs, rhs, n);
}

#endif

//template <typename T>
//class basic_cstring;

template <typename T>
class basic_string<T *> : public details::basic_string<T *>
{
    typedef details::basic_string<T *> basic_type;
    typedef typename basic_type::data_type data_type;
    
public:    
    typedef typename basic_type::traits                 traits;
    typedef typename basic_type::const_impl_reference   const_impl_reference;
    typedef typename basic_type::size_type              size_type;
    typedef typename basic_type::value_type             value_type;
    typedef typename basic_type::const_pointer          const_pointer;
    typedef typename basic_type::const_iterator         const_iterator;
    typedef typename basic_type::const_reverse_iterator const_reverse_iterator;

protected:
    basic_string (const_iterator begin, const_iterator end)
    {
        this->_d.begin = begin;
        this->_d.end = end;
    }

    basic_string (basic_string const & rhs)
    {
        this->_d.begin = rhs._d.begin;
        this->_d.end = rhs._d.end;
    }

    basic_string (const_impl_reference rhs)
    {
        this->_d.begin = rhs;
        this->_d.end = rhs + traits::size(rhs);
    }

    basic_string & operator = (basic_string const & rhs)
    {
        this->_d.begin = rhs._d.begin;
        this->_d.end = rhs._d.end;
        return *this;
    }
    
    basic_string & operator = (const_impl_reference rhs)
    {
        this->_d.begin = rhs;
        this->_d.end = rhs + traits::size(rhs);
        return *this;
    }
    
public:
    virtual size_type size () const
    {
        return this->_d.end - this->_d.begin;
    }

    virtual const_iterator begin () const
    {
        return this->_d.begin;
    }

    virtual const_iterator end () const
    {
        return this->_d.end;
    }

    virtual const_reverse_iterator rbegin () const
    {
        return const_reverse_iterator(this->_d.end);
    }

    virtual const_reverse_iterator rend () const
    {
        return const_reverse_iterator(this->_d.begin);
    }

    // TODO Implement
    virtual int compare (size_type pos1, size_type count1
        , basic_type const & rhs, size_type pos2, size_type count2) const
    {
        return 0;
    }
};

template <typename T>
class string<T *> : public basic_string<T *>
{
    typedef basic_string<T *> base_type;
    
public:    
    typedef typename base_type::const_impl_reference   const_impl_reference;
    typedef typename base_type::const_iterator         const_iterator;

public:
    string ()
        : base_type(0, 0)
    {}

    string (const_iterator begin, const_iterator end)
        : base_type(begin, end)
    {}

    string (const_impl_reference rhs)
        : base_type(rhs)
    {}
    
    string (string const & rhs)
        : base_type(rhs)
    {}

    string & operator = (string const & rhs)
    {
        base_type::operator = (rhs);
        return *this;
    }
    
    string & operator = (const_impl_reference rhs)
    {
        base_type::operator = (rhs);
        return *this;
    }
};


//	bool ends_with (string const & rhs) const
//    {
//        return pfs::mpl::ends_with(this->begin(), this->end()
//                , rhs.begin(), rhs.end());
//    }

//template <typename T>
//int string<T *>::compare (string const & rhs, size_type n) const
//{
//    size_type n1  = this->_d.end - this->_d.begin;
//    size_type n2 = rhs._d.end - rhs._d.begin;
//
//    // Minimum value between string sizes
//    size_type n0 = (n1 < n2 || n1 == n2) ? n1 : n2;
//
//    // Minimum value between string sizes and requested size
//    if (n0 > n)
//        n0 = n;
//
//    int r = traits::compare_n(this->_d.begin, rhs._d.begin, n0);
//
//    if (r != 0)
//
//    if (n1 < n)
//        n = n1;
//
//    return traits::compare_n(this->_d.begin, rhs._d.begin, n);    
//}

}} // pfs::mpl

#endif /* __PFS_MPL_STDC_STRING_HPP__ */


