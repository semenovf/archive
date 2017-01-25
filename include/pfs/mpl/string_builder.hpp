/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   string_builder.hpp
 * Author: wladt
 *
 * Created on January 24, 2017, 2:22 PM
 */

#ifndef __PFS_MPL_STRING_BUILDER_HPP__
#define __PFS_MPL_STRING_BUILDER_HPP__

#include <pfs/mpl/string.hpp>

namespace pfs {
namespace mpl {

//template <typename T>
//struct string_builder_traits
//{
//    typedef typename T::value_type             value_type;
////    typedef T const &                          const_native_reference;
////    typedef typename T::size_type              size_type;
////    typedef typename T::value_type             value_type;
////    typedef typename T::const_pointer          const_pointer;
////    typedef typename T::const_iterator         const_iterator;
////    typedef typename T::const_reverse_iterator const_reverse_iterator;
//    
//    typedef T data_type;
//};

template <typename T>
class basic_string_builder;

namespace details {
    
template <typename T>
class basic_string_builder 
{};

} // details

template <typename T>
class string_builder : public basic_string_builder<T>
{
    typedef basic_string_builder<T> base_type;
            
public:
    typedef T value_type;
    typedef typename base_type::size_type              size_type;

public:
    string_builder ();
    
    template <typename StringT>
    string_builder & push_back (pfs::mpl::string<StringT> const & s)
    {
        this->push_back(s.native());
    }
    
    template <typename StringT>
    string_builder & push_back (StringT const & s);
    
    string_builder & push_back (char const * s);
    string_builder & push_back (char const * s, size_type n);
    string_builder & push_back (value_type c);
    string_builder & push_back (size_type n, value_type c);
    
    template <typename StringT>
    StringT str () const;
};

}}

#endif /* __PFS_MPL_STRING_BUILDER_HPP__ */

