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

#include "option.hpp"


namespace pfs {
namespace cli {

namespace details {

template <typename Traits>
class option;

template <typename Char>
struct char_traits
{
    typedef Char type;

    static bool is_prefix (type ch)
    {
        return ch == type('-');
    }
    
    static bool is_delim (type ch)
    {
        return ch == type('=');
    }
};

template <typename String>
struct string_traits
{
    typedef String type;
    typedef typename type::const_pointer   const_pointer;
    typedef typename type::const_iterator  const_iterator;
    typedef type const &                   const_reference;

    static bool empty (const_reference s)
    {
        return s.empty();
    }
    
    static type arg_seperator ()
    {
        return type("--");
    }
};

template <typename Key, typename T, template <typename, typename> class Map>
struct map_traits
{
#if __cplusplus >= 201103
    typedef Map<Key, T> type;
#else
    typedef typename Map<Key, T>::type type;
#endif
    typedef typename type::key_type    key_type;
    typedef typename type::mapped_type mapped_type;
    typedef typename type::value_type  value_type;

    static void insert (type & m
        , key_type const & key
        , mapped_type const & value)
    {
        m.insert(value_type(key, value));
    }
};

template <typename T, template <typename> class List>
struct list_traits
{
#if __cplusplus >= 201103
    typedef List<T> type;
#else
    typedef typename List<T>::type type;
#endif
    typedef typename type::value_type value_type;

    static void push_back (type & l, value_type const & value)
    {
        l.push_back(value);
    }
};

template <typename Char
    , typename String
    , template <typename, typename> class Map
    , template <typename> class List>
struct traits
{
    typedef details::option<traits>           option_type;
    typedef option_type *                     option_pointer;
    typedef option_type const *               option_const_pointer;
    
    typedef details::char_traits<Char>        char_traits;
    typedef typename char_traits::type        char_type;
    
    typedef details::string_traits<String>    string_traits;
    typedef typename string_traits::type      string_type;
    
    typedef typename details::map_traits<
              char_type
            , option_const_pointer
            , Map>                            shortoptmap_traits;
    typedef typename shortoptmap_traits::type shortoptmap_type;
    
    typedef typename details::map_traits<
              string_type
            , option_const_pointer
            , Map>                            longoptmap_traits;
    typedef typename longoptmap_traits::type  longoptmap_type;
    
    typedef typename details::list_traits<
              option_pointer
            , List>                           optlist_traits;
    typedef typename optlist_traits::type     optlist_type;
};

} // details

}} // pfs::cli

#endif /* __PFS_CLI_TRAITS_HPP__ */

