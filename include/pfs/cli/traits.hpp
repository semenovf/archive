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

template <typename Traits>
class option;

template <typename StringType
    , template <typename Key, typename T> class MapType
    , template <typename T> class ListType>
struct traits
{
    struct mapped_type {};
    
    typedef StringType                                string_type;
    typedef typename string_type::value_type          char_type;
    typedef typename MapType<char_type
        , option<traits> const *>::type               shortopt_map_type;
    typedef typename MapType<string_type
        , option<traits> const *>::type               longopt_map_type;
    typedef typename ListType<option<traits> *>::type opt_list_type;
    
    typedef typename shortopt_map_type::mapped_type   shortopt_mapped_type;
    typedef typename longopt_map_type::mapped_type    longopt_mapped_type;

    // used by iterator and route
    static bool empty (string_type const & s)
    {
        return s.empty();
    }

    // used by iterator
    static bool is_prefix_char (char_type ch)
    {
        return ch == traits::char_type('-');
    }
    
    // used by iterator
    static bool is_delim_char (char_type ch)
    {
        return ch == traits::char_type('=');
    }
    
    // used by iterator
    static bool is_arg_seperator (string_type const & s)
    {
        return s == "--";
    }
    
    // used by route
    static void insert (shortopt_map_type & m, option<traits> const * o)
    {
        m.insert(shortopt_mapped_type(o->short_name(), o));
    }

    // used by route
    static void insert (longopt_map_type & m, option<traits> const * o)
    {
        m.insert(longopt_mapped_type(o->long_name(), o));
    }
    
    // used by route
    static void push_back (opt_list_type & l, option<traits> const * o)
    {
        l.push_back(o);
    }
};

} // details

}} // pfs::cli

#endif /* __PFS_CLI_TRAITS_HPP__ */

