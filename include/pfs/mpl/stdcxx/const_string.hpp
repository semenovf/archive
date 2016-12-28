/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   const_string.hpp
 * Author: wladt
 *
 * Created on December 27, 2016
 */

#ifndef __PFS_MPL_IMPL_STDCXX_CONST_STRING_HPP__
#define __PFS_MPL_IMPL_STDCXX_CONST_STRING_HPP__

#include <string>
#include <pfs/mpl/const_string.hpp>

namespace pfs {
namespace mpl {

template <>
struct const_string_traits<std::string>
{
    typedef std::string const &                 const_self_reference;
    typedef std::string::size_type              size_type;
    typedef std::string::value_type             value_type;
    typedef std::string::const_pointer          const_pointer;
    typedef std::string::const_iterator         const_iterator;
    typedef std::string::const_reverse_iterator const_reverse_iterator;

    struct data_type 
    {
        std::string s;
    };
};

template <>
inline const_string<std::string>::
const_string ()
{}

template <>
inline const_string<std::string>::
const_string (const_self_reference s)
{
    _d.s = std::string(s);
}

template <>
inline const_string<std::string>::
const_string (const_iterator begin, const_iterator end)
{
    _d.s = std::string(begin, end);
}

template <>
inline typename const_string<std::string>::const_iterator
const_string<std::string>::begin () const
{
    return _d.s.begin();
}

template <>
inline typename const_string<std::string>::const_iterator
const_string<std::string>::end () const
{
    return _d.s.end();
}

template <>
inline typename const_string<std::string>::const_reverse_iterator
const_string<std::string>::rbegin () const
{
    return _d.s.rbegin();
}

template <>
inline typename const_string<std::string>::const_reverse_iterator
const_string<std::string>::rend () const
{
    return _d.s.rend();
}

template <>
inline typename const_string<std::string>::const_pointer 
const_string<std::string>::data () const
{
    return _d.s.data();
}

template <>
inline typename const_string<std::string>::size_type 
const_string<std::string>::size () const
{
    return _d.s.size();
}

template <>
inline typename const_string<std::string>::size_type
const_string<std::string>::length () const
{
    return this->size();
}

template <>
inline int 
const_string<std::string>::compare (const_string const & lhs) const
{
    return _d.s.compare(lhs._d.s);
}

}} // pfs::mpl

#endif /* __PFS_MPL_IMPL_STDC_CONST_STRING_HPP__ */
