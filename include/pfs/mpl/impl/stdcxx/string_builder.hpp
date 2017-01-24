/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_builder.hpp
 * Author: wladt
 *
 * Created on January 24, 2017, 2:49 PM
 */

#ifndef __PFS_MPL_IMPL_STDCXX_STRING_BUILDER_HPP__
#define __PFS_MPL_IMPL_STDCXX_STRING_BUILDER_HPP__

#include <string>
#include <pfs/mpl/impl/stdcxx/string.hpp>
#include <pfs/mpl/string_builder.hpp>

namespace pfs {
namespace mpl {

template <>
class basic_string_builder<char> : public details::basic_string_builder<char>
{
    typedef details::basic_string_builder<char> base_type;
    typedef std::string data_type;
    
public:    
    typedef pfs::mpl::string<std::string>       string_type;
    typedef string_type::size_type              size_type;
//    typedef string_type::const_native_reference const_native_reference;
//    typedef string_type::const_pointer          const_pointer;
    
protected:
    data_type _d;
};

template <>
inline string_builder<char>::string_builder ()
{}

template <>
template <>
inline string_builder<char> & string_builder<char>::push_back<std::string> (std::string const & s)
{
    _d.append(s);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (char const * s)
{
    _d.append(s);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (char const * s, size_type n)
{
    _d.append(s, n);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (value_type c)
{
    _d.append(1, c);
    return *this;
}

template <>
inline string_builder<char> & string_builder<char>::push_back (size_type n, value_type c)
{
    _d.append(n, c);
    return *this;
}

template <>
template <>
std::string string_builder<char>::str<std::string> () const
{
    return _d;
}

template <>
template <>
pfs::mpl::string<std::string> string_builder<char>::str<pfs::mpl::string<std::string> > () const
{
    return pfs::mpl::string<std::string>(_d);
}

}}

#endif /* __PFS_MPL_IMPL_STDCXX_STRING_BUILDER_HPP__ */

