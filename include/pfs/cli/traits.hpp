/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   traits.hpp
 * Author: wladt
 *
 * Created on December 16, 2016, 11:53 AM
 */

#ifndef __PFS_CLI_TRAITS_HPP__
#define __PFS_CLI_TRAITS_HPP__

namespace pfs {
namespace cli {

namespace details {

struct mapped_type {};

template <typename Iterator
    , template <typename Key, typename T> class MapType
    , template <typename T> class ListType>
struct traits
{
    typedef Iterator                          iterator;
    typedef typename iterator::string_type    string_type;
    typedef typename string_type::value_type  char_type;
    typedef typename MapType<char_type
        , mapped_type const *>::type          short_map_type;
    typedef typename MapType<string_type
        , mapped_type const *>::type          long_map_type;
    typedef typename ListType<mapped_type *>::type list_type;
    
//    static char_type invalid_short_name ()
//    {
//        return short_name_type();
//    }
//    
//    static long_name_type invalid_long_name ()
//    {
//        return long_name_type();
//    }
};

} // details

}} // pfs::cli


#endif /* __PFS_CLI_TRAITS_HPP__ */

