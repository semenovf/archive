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

template <typename T, typename Traits>
class option : public mapped_type
{
protected:
    typedef T                                value_type;
//    typedef Traits                           traits_type;
//    typedef typename traits_type::short_name_type short_name_type;
//    typedef typename traits_type::long_name_type  long_name_type;
    
protected:
    value_type _value;
    
public:
    option ()
        : _value()
    {}
        
    void set_default_value (value_type const & value)
    {
        _value = value;
    }
    
    short_name_type const & short_name () const
    {
        return _short_name;
    }

    long_name_type const & long_name () const
    {
        return _long_name;
    }
};

} // details

}} // pfs::cli

#endif /* __PFS_CLI_OPTION_HPP__ */

