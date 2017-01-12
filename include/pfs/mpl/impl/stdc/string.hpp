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

template <typename T>
class basic_string<T *> : public details::basic_string<T *>
{
    typedef details::basic_string<T *> base_type;
    typedef typename base_type::data_type data_type;
    
public:    
    typedef typename base_type::traits                 traits;
    typedef typename base_type::const_impl_reference   const_impl_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

//    using base_type::compare; // To use base class compare() overloaded methods.
protected:
    virtual int xcompare (size_type pos1, size_type count1
        , base_type const & rhs, size_type pos2, size_type count2) const;

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

    explicit basic_string (const_impl_reference rhs)
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

    /**
     * 
     * @param index
     * @return 
     * @trows out_of_range if pos >= size()
     */
    virtual value_type at (size_type pos) const
    {
        if (pos >= size())
            throw out_of_range("string::at");
        return this->_d.begin[pos];
    }
};

template <typename T>
int basic_string<T *>::xcompare (size_type pos1, size_type count1
    , base_type const & rhs, size_type pos2, size_type count2) const
{
    size_type n1 = this->size();
    size_type n2 = rhs.size();
    
    if (pos1 >= n1) {
        pos1 = n1;
        count1 = 0;
    }
    
    if (pos2 >= n2) {
        pos2 = n2;
        count2 = 0;
    }
    
    if (n1 < pos1 + count1)
        count1 = n1 - pos1;
    
    if (n2 < pos2 + count2)
        count2 = n2 - pos2;
    
    size_type count = count1 < count2 ? count1 : count2;
    int result = traits::compare_n(this->_d.begin + pos1, rhs._d.begin + pos2, count);
    
    if (result == 0 && count1 != count2) {
        return count1 < count2 ? -1 : 1;
    }
        
    return result;
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

    explicit string (const_impl_reference rhs)
        : base_type(rhs)
    {}

//    Inherited from base class:
//    string (string const & rhs)
//        : base_type(rhs)
//    {}

//    Inherited from base class:
//    string & operator = (string const & rhs)
    
    string & operator = (const_impl_reference rhs)
    {
        base_type::operator = (rhs);
        return *this;
    }
};

}} // pfs::mpl

#endif /* __PFS_MPL_STDC_STRING_HPP__ */


