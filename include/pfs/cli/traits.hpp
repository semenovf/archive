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

template <typename StringType
    , template <typename Key, typename T> class MapType
    , template <typename T> class ListType>
struct traits
{
    struct mapped_type {};
    
    typedef StringType                             string_type;
//    typedef typename string_type::value_type       char_type;
//    typedef typename MapType<char_type
//        , mapped_type const *>::type               shortopt_map_type;
//    typedef typename MapType<string_type
//        , mapped_type const *>::type               longopt_map_type;
//    typedef typename ListType<mapped_type *>::type opt_list_type;
    
//    static bool is_space (char_type ch);
    
//    static bool is_quote (char_type ch);
//    
//    static bool is_prefix (char_type ch);
//    
//    static bool is_escape (char_type ch);
};

} // details

}} // pfs::cli


#endif /* __PFS_CLI_TRAITS_HPP__ */

