/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   option.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 5:05 PM
 */

#ifndef __PFS_CLI_OPTION_HPP__
#define __PFS_CLI_OPTION_HPP__

#include <pfs/cli/traits.hpp>

namespace pfs {
namespace cli {

namespace details {

#if __COMMENT__
template <typename Traits>
class route;

template <typename T, typename Traits>
class option : public mapped_type
{
    friend class route<Traits>;

protected:
    typedef T                                     value_type;
    typedef Traits                                traits_type;
    typedef typename traits_type::short_name_type short_name_type;
    typedef typename traits_type::long_name_type  long_name_type;
    typedef typename traits_type::string_type     string_type;
    
protected:
    value_type *    _pvalue;
    short_name_type _short_name;
    long_name_type  _long_name;
    string_type     _description;
    
protected:
    option (value_type * pvalue
            , short_name_type const & short_name
            , long_name_type const & long_name
            , string_type const & description)
        : _pvalue(pvalue)
        , _short_name(short_name)
        , _long_name(long_name)
        , _description(description)
    {}
};

#endif

} // details

}} // pfs::cli

#endif /* __PFS_CLI_OPTION_HPP__ */

