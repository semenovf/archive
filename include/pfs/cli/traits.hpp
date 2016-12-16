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

template <typename ShortNameType
    , typename LongNameType
    , BoolType
    , StringType
    , template <typename Key, typename T> class MapType>
struct traits
{
    typedef ShortNameType  short_name_type;
    typedef LongNameType   long_name_type;
    typedef BoolType       bool_type;
    typedef StringType     string_type;
    typedef typename MapType<ShortNameType, mapped_type const *>::type short_map_type;
    typedef typename MapType<LongNameType, mapped_type const *>::type long_map_type;
    
    static short_name_type invalid_short_name ()
    {
        return short_name_type();
    }
    
    static long_name_type invalid_long_name ()
    {
        return long_name_type();
    }
};

} // details

}} // pfs::cli


#endif /* __PFS_CLI_TRAITS_HPP__ */

