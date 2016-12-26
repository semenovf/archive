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

#ifndef __PFS_CLI_TRAITS_CSTRING_HPP__
#define __PFS_CLI_TRAITS_CSTRING_HPP__

//#include "option.hpp"
#include <pfs/cli/traits.hpp>
#include <cstring>

namespace pfs {
namespace cli {

template <>
struct string_traits<char const *>
{
    typedef char const *  string_type;
    typedef char          char_type;
    typedef char const *  char_iterator;
    typedef char const ** token_iterator;

    char_iterator char_begin (string_type const & s) const
    {
        return char_iterator(s);
    }
    
    char_iterator char_end (string_type const & s) const
    {
        return char_iterator(s) + std::strlen(s);
    }
};

//#if __COMMENT__
//
//template <typename Traits>
//class option;
//
//template <typename Key, typename T, template <typename, typename> class Map>
//struct map_traits
//{
//#if __cplusplus >= 201103
//    typedef Map<Key, T> type;
//#else
//    typedef typename Map<Key, T>::type type;
//#endif
//    typedef typename type::key_type    key_type;
//    typedef typename type::mapped_type mapped_type;
//    typedef typename type::value_type  value_type;
//
////    struct ref
////    {
////        type & m;
////        ref (type & x) : m(x) {}
////
////        void insert (type & m
////                , key_type const & key
////                , mapped_type const & value)
////        {
////            m.insert(value_type(key, value));
////        }
////    };
//};
//
//template <typename T, template <typename> class List>
//struct list_traits
//{
//    
//#if __cplusplus >= 201103
//    typedef List<T> type;
//#else
//    typedef typename List<T>::type type;
//#endif
//    typedef typename type::value_type value_type;
//    typedef typename type::iterator   iterator;
//};
//
//template <typename String
//    , template <typename, typename> class Map
//    , template <typename> class List>
//struct traits
//{
//#if __COMMENT__
//    typedef details::option<traits>           option_type;
//    typedef option_type *                     option_pointer;
//    typedef option_type const *               option_const_pointer;
//    
//    typedef String                            string_type;
//    typedef typename string_type::value_type  char_type;
//    
//    typedef typename details::map_traits<
//              char_type
//            , option_const_pointer
//            , Map>                            shortoptmap_traits;
//    typedef typename shortoptmap_traits::type shortoptmap_type;
//    
//    typedef typename details::map_traits<
//              string_type
//            , option_const_pointer
//            , Map>                            longoptmap_traits;
//    typedef typename longoptmap_traits::type  longoptmap_type;
//    
//    typedef typename details::list_traits<
//              option_pointer
//            , List>                           optlist_traits;
//    typedef typename optlist_traits::type     optlist_type;
//#endif    
//};
//
//#endif

}} // pfs::cli

#endif /* __PFS_CLI_TRAITS_CSTRING_HPP__ */

