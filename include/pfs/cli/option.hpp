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

namespace pfs {
namespace cli {

char const no_short_name = '\0';
char const * no_long_name = 0;

namespace details {

template <typename CharType, typename StringType>
struct option_traits
{
    typedef CharType   char_type;
    typedef StringType string_type;
};

template <typename T, typename Traits>
class option
{
protected:
    typedef typename Traits::char_type   char_type;
    typedef typename Traits::string_type string_type;
    
public:
    option (char_type short_name, string_type long_name);
    option (char_type short_name, string_type long_name, T const & default_value);
};

} // details

}} // pfs::cli

#endif /* __PFS_CLI_OPTION_HPP__ */

