/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_route.hpp
 * Author: wladt
 *
 * Created on December 20, 2016, 9:50 AM
 */

#ifndef __TEST_ROUTE_HPP__
#define __TEST_ROUTE_HPP__

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/cli.hpp>
#include <pfs/mpl/cli/route.hpp>
#include <pfs/mpl/impl/stdc/string.hpp>
#include <pfs/mpl/impl/stdcxx/list.hpp>
#include <pfs/mpl/impl/stdcxx/multimap.hpp>
//#include <iostream>

namespace test {
namespace cli {

struct stdcxx_cli_traits
{
    typedef pfs::mpl::string<char const *>  string_type;
    typedef pfs::mpl::multimap<char, >typename traits_type::shortoptmap_traits shortoptmap_traits;
    typedef typename traits_type::longoptmap_traits  longoptmap_traits;
    typedef typename traits_type::optlist_traits     optlist_traits;

    typedef typename traits_type::option_type        value_type;
    typedef typename traits_type::string_type        string_type;
    typedef typename traits_type::char_type          char_type;
    typedef typename traits_type::shortoptmap_type   shortoptmap_type;
    typedef typename traits_type::longoptmap_type    longoptmap_type;
    typedef typename traits_type::optlist_type       optlist_type;
};

void test_route ()
{
    ADD_TESTS(1);
    
    char const * __argv[] = { "/path/to/program.sh"
            , "domain"
            , "command"
            , "-a"
            , "-bcd"
            , "--option1"
            , "--option2=option2_arg"
            , "--option 3=option3 arg"
            , "--option-multi"
            , "--option-multi=123"
            , "--option-multi=3.14"
            , "--option-multi=string"
            , "--"
            , "arg1"
            , "--arg2"
    };
    
    int __argc = sizeof(__argv)/sizeof(__argv[0]);
   
    typedef pfs::mpl::cli::cmdline<char const *> cmdline;
    
    //pfs::cli::no_long_option
    
//    pfs::cli::route r;
//    
//    bool const  required     = true;
//    //bool const  optional     = false;
//    
//    bool        yes          = true;
//    short       short_value  = 0;
//    int         int_value    = 0;
//    long        long_value   = 0;
//    intmax_t    intmax_value = 0;
//    float       float_value  = 0.0f;
//    double      double_value = 0.0f;
//    real_t      real_value   = 0.0f;   
//    pfs::string string_value;
//    
//    r.add(& yes          ,      _u8("bool")  , required);
//    r.add(& short_value  ,      _u8("short") , required);
//    r.add(& int_value    , 'i', _u8("int")   , required);
//    r.add(& long_value   ,      _u8("long")  , required);
//    r.add(& intmax_value ,      _u8("intmax"), required);
//    r.add(& float_value  , 'f'               , required);
//    r.add(& double_value , 'd', _u8("double"), required);
//    r.add(& real_value   ,      _u8("real")  , required);
//    r.add(& string_value , 's', _u8("string"), required);
    
    TEST_OK(r.parse(__argc, __argv, pfs::cli::relaxed_flags));
}

}}

#endif /* __TEST_ROUTE_HPP__ */