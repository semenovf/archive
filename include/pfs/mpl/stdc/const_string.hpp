/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   const_string.hpp
 * Author: wladt
 *
 * Created on December 26, 2016, 4:35 PM
 */

#ifndef __PFS_MPL_IMPL_STDC_CONST_STRING_HPP__
#define __PFS_MPL_IMPL_STDC_CONST_STRING_HPP__

#include <cstring>
#include <pfs/mpl/const_string.hpp>

#if defined(PFS_USE_WCHAR) && PFS_USE_WCHAR == 1
#   include <cwchar>
#endif

namespace pfs {
namespace mpl {

template <>
struct const_string_traits<char const *>
{
    typedef char const * const_self_reference;
    typedef size_t       size_type;
    typedef char         value_type;
    typedef char const * const_pointer;
    typedef char const * const_iterator;
    typedef std::reverse_iterator<char const *> const_reverse_iterator;

    struct data_type 
    {
        const_iterator begin;
        const_iterator end;
    };
};

#if defined(PFS_USE_WCHAR) && PFS_USE_WCHAR == 1
template <>
struct const_string_traits<wchar_t const *>
{
    typedef wchar_t const * const_self_reference;
    typedef size_t       size_type;
    typedef wchar_t      value_type;
    typedef wchar_t const * const_pointer;
    typedef wchar_t const * const_iterator;
    typedef std::reverse_iterator<wchar_t const *> const_reverse_iterator;

    struct data_type 
    {
        const_iterator begin;
        const_iterator end;
    };
};
#endif

template <>
inline const_string<char const *>::
const_string ()
{
    _d.begin = 0;
    _d.end = 0;
}

template <>
inline const_string<char const *>::
const_string (const_self_reference s)
{
    _d.begin = s;
    _d.end = s + std::strlen(s);
}

template <>
inline const_string<char const *>::
const_string (const_iterator begin, const_iterator end)
{
    _d.begin = begin;
    _d.end = end;
}

template <>
inline const_string<char const *> & const_string<char const *>::
operator = (const_pointer s)
{
    _d.begin = s;
    _d.end = s + std::strlen(s);
}

template <>
inline typename const_string<char const *>::const_iterator
const_string<char const *>::begin () const
{
    return _d.begin;
}

template <>
inline typename const_string<char const *>::const_iterator
const_string<char const *>::end () const
{
    return _d.end;
}

template <>
inline typename const_string<char const *>::const_reverse_iterator
const_string<char const *>::rbegin () const
{
    return const_reverse_iterator(_d.end);
}

template <>
inline typename const_string<char const *>::const_reverse_iterator
const_string<char const *>::rend () const
{
    return const_reverse_iterator(_d.begin);
}

template <>
inline typename const_string<char const *>::const_pointer 
const_string<char const *>::data () const
{
    return _d.begin;
}

template <>
inline typename const_string<char const *>::size_type 
const_string<char const *>::size () const
{
    return _d.end - _d.begin;
}

template <>
inline typename const_string<char const *>::size_type
const_string<char const *>::length () const
{
    return this->size();
}

template <>
inline int 
const_string<char const *>::compare (const_string const & lhs) const
{
    size_type n  = _d.end - _d.begin;
    size_type n1 = lhs._d.end - lhs._d.begin;
    
    if (n1 < n)
        n = n1;
    
    return std::strncmp(_d.begin, lhs._d.begin, n);
}

#if defined(PFS_USE_WCHAR) && PFS_USE_WCHAR == 1

template <>
inline const_string<wchar_t const *>::
const_string ()
{
    _d.begin = 0;
    _d.end = 0;
}

template <>
inline const_string<wchar_t const *>::
const_string (const_self_reference s)
{
    _d.begin = s;
    _d.end = s + std::wcslen(s);
}

template <>
inline const_string<wchar_t const *>::
const_string (const_iterator begin, const_iterator end)
{
    _d.begin = begin;
    _d.end = end;
}

template <>
inline const_string<wchar_t const *> & const_string<wchar_t const *>::
operator = (const_pointer s)
{
    _d.begin = s;
    _d.end = s + std::wcslen(s);
}

template <>
inline typename const_string<wchar_t const *>::const_iterator
const_string<wchar_t const *>::begin () const
{
    return _d.begin;
}

template <>
inline typename const_string<wchar_t const *>::const_iterator
const_string<wchar_t const *>::end () const
{
    return _d.end;
}

template <>
inline typename const_string<wchar_t const *>::const_reverse_iterator
const_string<wchar_t const *>::rbegin () const
{
    return const_reverse_iterator(_d.end);
}

template <>
inline typename const_string<wchar_t const *>::const_reverse_iterator
const_string<wchar_t const *>::rend () const
{
    return const_reverse_iterator(_d.begin);
}

template <>
inline typename const_string<wchar_t const *>::const_pointer 
const_string<wchar_t const *>::data () const
{
    return _d.begin;
}

template <>
inline typename const_string<wchar_t const *>::size_type 
const_string<wchar_t const *>::size () const
{
    return _d.end - _d.begin;
}

template <>
inline typename const_string<wchar_t const *>::size_type
const_string<wchar_t const *>::length () const
{
    return this->size();
}

template <>
inline int 
const_string<wchar_t const *>::compare (const_string const & lhs) const
{
    size_type n  = _d.end - _d.begin;
    size_type n1 = lhs._d.end - lhs._d.begin;
    
    if (n1 < n)
        n = n1;
    
    return std::wcsncmp(_d.begin, lhs._d.begin, n);
}
#endif

}} // pfs::mpl

#endif /* __PFS_MPL_IMPL_STDC_CONST_STRING_HPP__ */
