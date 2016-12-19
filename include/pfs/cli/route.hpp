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

// Parse flags
enum {
      skip_argv_0                  = 0x0001 //!< Skip first element (program path) while parsing argv array.
    , combine_short_options        = 0x0002 //!< Combine short options (stacked options) using form -abc (-a -b -c).
    , shortopt_stacked             = combine_short_options //!< Synonim for combine_short_options.
    , longopt_single_dash          = 0x0004 //!< Allow single dash prefix for long options in form -option.
    , longopt_double_dash          = 0x0008 //!< Allow double dash prefix for long options in form --option.
    , longopt_slash                = 0x0010 //!< Allow slash prefix for long options in form /option.
    , shortopt_slash               = 0x0020 //!< Allow slash prefix for short options in form /o.
    , double_dash_pos_args         = 0x0040 //!< Allow double dash to separate options and positional arguments.
    , longopt_arg_eq_separator     = 0x0080 //!< Allow long option with argument in form {-- | /}option=ARG.
    , longopt_arg_colon_separator  = 0x0100 //!< Allow long option with argument in form {-- | /}option:ARG.
    , longopt_arg_space_separator  = 0x0200 //!< Allow long option with argument in form {-- | /}option ARG.
    , shortopt_arg_eq_separator    = 0x0400 //!< Allow short option with argument in form {- | /}o=ARG.
    , shortopt_arg_colon_separator = 0x0800 //!< Allow short option with argument in form {- | /}o:ARG.
    , shortopt_arg_space_separator = 0x1000 //!< Allow short option with argument in form {- | /}o ARG.
    , shortopt_arg_no_separator    = 0x2000 //!< Allow short option with argument in form {- | /}oARG.
};

#if __COMMENT__

namespace details {

template <typename T, typename Traits>
class option;

template <typename Traits>
class route 
{
protected:
    typedef Traits traits_type;
    typedef typename traits_type::short_name_type short_name_type;
    typedef typename traits_type::long_name_type  long_name_type;
    typedef typename traits_type::bool_type       bool_type;
    typedef typename traits_type::string_type     string_type;
    typedef typename traits_type::short_map_type  short_map_type;
    typedef typename traits_type::long_map_type   long_map_type;
    
protected:
    short_map_type _short_options;
    long_map_type  _long_options;
    
public:
    route () 
    {}
    
    template <typename T>
    void add (T * pvalue
        , short_name_type const & sname
        , long_name_type const & lname
        , string_type const & description = string_type());
};

template <typename Traits>
template <typename T>
void route<Traits>::add (T * pvalue
        , short_name_type const & sname
        , long_name_type const & lname
        , string_type const & description)
{
    if (sname != traits_type::invalid_short_name())
        _short_options.insert(typename short_map_type::value_type(sname, & opt));
    
    if (lname != traits_type::invalid_long_name())
        _long_options.insert(typename long_map_type::value_type(lname(), & opt));
}

} // details

#endif

}} // pfs::cli

#endif /* __PFS_CLI_ROUTE_HPP__ */

