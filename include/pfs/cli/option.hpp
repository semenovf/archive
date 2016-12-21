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

template <typename Traits>
class option
{
    typedef Traits                            traits_type;
    typedef typename traits_type::string_type string_type;
    typedef typename traits_type::char_type   char_type;

protected:
    char_type   _short_name;
    string_type _long_name;
    bool        _required;
    string_type _description;
    
protected:
    option (char_type short_name
            , string_type const & long_name
            , bool required
            , string_type const & description)
        : _short_name(short_name)
        , _long_name(long_name)
        , _required(required)
        , _description(description)
    {}

public:        
    virtual ~option () 
    {}
    
    char_type short_name () const
    {
        return _short_name;
    }

    string_type long_name () const
    {
        return _long_name;
    }

};

} // details

template <typename T, typename Traits>
class option : public details::option<Traits>
{
    typedef details::option<Traits>           base_class;
    typedef Traits                            traits_type;
    typedef typename traits_type::string_type string_type;
    typedef typename traits_type::char_type   char_type;

protected:
    T * _pvalue;
    
public:
    option (T * pvalue
            , char_type short_name
            , string_type const & long_name
            , bool required
            , string_type const & description)
        : base_class(short_name, long_name, required, description)
        , _pvalue(pvalue)
    {}
};

}} // pfs::cli

#endif /* __PFS_CLI_OPTION_HPP__ */

