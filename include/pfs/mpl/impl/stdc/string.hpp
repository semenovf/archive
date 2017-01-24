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

#include <cstring>
#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {

template <typename T>
struct string_traits<T *>
{
    typedef T const * const_native_reference;
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
    static size_type find (const_pointer lhs, const_pointer rhs);
    static size_type rfind (const_pointer lhs, size_type lpos, const_pointer rhs, size_type rpos);
};

template <typename T>
struct string_traits<T const *> : public string_traits<T *>
{};

template <typename T>
typename string_traits<T *>::size_type
string_traits<T *>::rfind (const_pointer lhs, size_type lpos, const_pointer rhs, size_type rpos)
{
    if (lpos == size_type(-1))
        lpos = size(lhs);

    if (rpos == size_type(-1))
        rpos = size(rhs);
    
    const_iterator begin1(lhs);
    const_iterator end1(lhs);
    pfs::advance(end1, lpos);

    const_iterator begin2(rhs);
    const_iterator end2(rhs);
    pfs::advance(end2, rpos);
    
    const_iterator i = pfs::mpl::rfind(begin1, end1, begin2, end2);
    return i == end1 ? size_type(-1) : pfs::distance(end1, i);
}


template <>
inline string_traits<char *>::size_type
string_traits<char *>::size (const_pointer s)
{
    return std::strlen(s);
}

template <>
inline int
string_traits<char *>::compare_n (const_pointer lhs, const_pointer rhs, size_type n)
{
    return std::strncmp(lhs, rhs, n);
}

template <>
inline string_traits<char *>::size_type
string_traits<char *>::find (const_pointer lhs, const_pointer rhs)
{
    const_pointer p = std::strstr(lhs, rhs);
    return p == 0 ? size_type(-1) : size_type(p - lhs);
}

#ifdef _WCHAR_H

template <>
inline string_traits<wchar_t *>::size_type
string_traits<wchar_t *>::size (const_pointer s)
{
    return std::wcslen(s);
}

template <>
inline int
string_traits<wchar_t *>::compare_n (const_pointer lhs, const_pointer rhs, size_type n)
{
    return std::wcsncmp(lhs, rhs, n);
}

template <>
inline string_traits<wchar_t *>::size_type
string_traits<wchar_t *>::find (const_pointer lhs, const_pointer rhs)
{
    const_pointer p = std::wcsstr(lhs, rhs);
    return p == 0 ? size_type(-1) : size_type(p - lhs);
}

#endif

template <typename T>
class basic_string<T *> : public details::basic_string<T *>
{
    typedef details::basic_string<T *> base_type;
    typedef typename base_type::data_type data_type;
    
public:    
    typedef typename base_type::traits_type            traits_type;
    typedef typename base_type::const_native_reference const_native_reference;
    typedef typename base_type::size_type              size_type;
    typedef typename base_type::value_type             value_type;
    typedef typename base_type::const_pointer          const_pointer;
    typedef typename base_type::const_iterator         const_iterator;
    typedef typename base_type::const_reverse_iterator const_reverse_iterator;

protected:
    virtual size_type xsize () const
    {
        return this->_d.end - this->_d.begin;
    }

    virtual const_iterator xbegin () const
    {
        return this->_d.begin;
    }

    virtual const_iterator xend () const
    {
        return this->_d.end;
    }

    virtual const_reverse_iterator xrbegin () const
    {
        return const_reverse_iterator(this->_d.end);
    }

    virtual const_reverse_iterator xrend () const
    {
        return const_reverse_iterator(this->_d.begin);
    }
    
    virtual value_type xat (size_type pos) const
    {
        return this->_d.begin[pos];
    }

    virtual int xcompare (size_type pos1, size_type count1
        , base_type const & rhs, size_type pos2, size_type count2) const;

    virtual size_type xfind (const_native_reference rhs, size_type pos) const
    {
        return traits_type::find(this->_d.begin + pos, rhs);
    }
    
    virtual size_type xfind (value_type c, size_type pos) const
    {
        value_type rhs[] = { c, 0 };
        return traits_type::find(this->_d.begin + pos, rhs);
    }
    
    virtual size_type xrfind (const_native_reference rhs, size_type pos) const
    {
        return traits_type::rfind(this->_d.begin, pos, rhs, traits_type::size(rhs));
    }

    virtual size_type xrfind (value_type c, size_type pos) const
    {
        value_type rhs[] = { c, 0 };
        return traits_type::rfind(this->_d.begin, pos, rhs, traits_type::size(rhs));
    }

protected:
    basic_string ()
    {
        this->_d.begin = 0;
        this->_d.end = 0;
    }
    
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

    explicit basic_string (const_native_reference rhs)
    {
        this->_d.begin = rhs;
        this->_d.end = rhs + traits_type::size(rhs);
    }

    basic_string & operator = (basic_string const & rhs)
    {
        this->_d.begin = rhs._d.begin;
        this->_d.end = rhs._d.end;
        return *this;
    }
    
    basic_string & operator = (const_native_reference rhs)
    {
        this->_d.begin = rhs;
        this->_d.end = rhs + traits_type::size(rhs);
        return *this;
    }
    
public:
    virtual const_native_reference native () const
    {
        return this->_d.begin;
    }
};

template <typename T>
int basic_string<T *>::xcompare (size_type pos1, size_type count1
    , base_type const & rhs, size_type pos2, size_type count2) const
{
    size_type n1 = this->xsize();
    size_type n2 = rhs.xsize();
    
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
    int result = traits_type::compare_n(this->_d.begin + pos1, rhs._d.begin + pos2, count);
    
    if (result == 0 && count1 != count2) {
        return count1 < count2 ? -1 : 1;
    }
        
    return result;
};

}} // pfs::mpl

namespace std {

template <>
ostream & operator << <char const *> (ostream & out, pfs::mpl::string<char const *> const & s)
{
    char const * p = s.native();
    pfs::mpl::string<char const *>::size_type n = s.size();
    
    while (n--)
        out << *p++;
    
    return out;
}

template <>
ostream & operator<< <char *> (ostream & out, pfs::mpl::string<char *> const & s)
{
    char const * p = s.native();
    pfs::mpl::string<char *>::size_type n = s.size();
    
    while (n--)
        out << *p++;
    
    return out;
}

} // std

#endif /* __PFS_MPL_STDC_STRING_HPP__ */


