/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cli.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 5:04 PM
 */

#ifndef __PFS_CLI_HPP__
#define __PFS_CLI_HPP__

#include <pfs/string.hpp>
#include <pfs/unicode.hpp>
#include <pfs/cli/option.hpp>
#include <pfs/cli/route.hpp>

namespace pfs {
namespace cli {

typedef details::option_traits<pfs::unicode::char_t, pfs::string> option_traits;

template <typename T>
class option : public details::option<T, option_traits>
{
    typedef details::option<T, option_traits> base_type;
    typedef typename base_type::char_type   char_type;
    typedef typename base_type::string_type string_type;

public:
    option (char short_name, char const * long_name)
        : base_type(char_type(short_name), string_type(long_name))
    {}

    option (char_type short_name, string_type long_name)
        : base_type(short_name, long_name)
    {}

    option (char short_name, char const * long_name, T const & default_value)
        : base_type(char_type(short_name), string_type(long_name), default_value)
    {}
            
    option (char_type short_name, string_type long_name, T const & default_value)
        : base_type(short_name, long_name, default_value)
    {}
};

}} // pfs::cli

#endif /* __PFS_CLI_HPP__ */

