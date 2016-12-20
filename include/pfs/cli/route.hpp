/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   route.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 5:05 PM
 */

#ifndef __PFS_CLI_ROUTE_HPP__
#define __PFS_CLI_ROUTE_HPP__

#include <pfs/cli/traits.hpp>
#include <pfs/cli/option.hpp>

namespace pfs {
namespace cli {

namespace details {

//template <typename T, typename Traits>
//class option;

template <typename Traits>
class route 
{
protected:
//    typedef Traits traits_type;
//    typedef typename traits_type::short_name_type short_name_type;
//    typedef typename traits_type::long_name_type  long_name_type;
//    typedef typename traits_type::bool_type       bool_type;
//    typedef typename traits_type::string_type     string_type;
//    typedef typename traits_type::short_map_type  short_map_type;
//    typedef typename traits_type::long_map_type   long_map_type;
    
protected:
//    short_map_type _short_options;
//    long_map_type  _long_options;
    
public:
    route () 
    {}
    
//    template <typename T>
//    void add (T * pvalue
//        , short_name_type const & sname
//        , long_name_type const & lname
//        , string_type const & description = string_type());
};

//template <typename Traits>
//template <typename T>
//void route<Traits>::add (T * pvalue
//        , short_name_type const & sname
//        , long_name_type const & lname
//        , string_type const & description)
//{
//    if (sname != traits_type::invalid_short_name())
//        _short_options.insert(typename short_map_type::value_type(sname, & opt));
//    
//    if (lname != traits_type::invalid_long_name())
//        _long_options.insert(typename long_map_type::value_type(lname(), & opt));
//}

} // details

}} // pfs::cli

#endif /* __PFS_CLI_ROUTE_HPP__ */

