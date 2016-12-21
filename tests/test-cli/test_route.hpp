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
//#include <iostream>

namespace test {
namespace cli {

void test_route ()
{
    ADD_TESTS(1);
    
    char const * argv[] = { "/path/to/program.sh"
            , "domain"
            , "command"
            , "-a"
            , "-bcd"
            , "--option1"
            , "--option2=option2_arg"
            , "--option 3=option3 arg"
            , "--"
            , "arg1"
            , "--arg2"
    };
    
    int argc = sizeof(argv)/sizeof(argv[0]);
   
    //pfs::cli::no_long_option
    
    pfs::cli::route r;
    
    bool        yes          = true;
    short       short_value  = 0;
    int         int_value    = 0;
    long        long_value   = 0;
    intmax_t    intmax_value = 0;
    float       float_value  = 0.0f;
    double      double_value = 0.0f;
    real_t      real_value   = 0.0f;   
    pfs::string string_value;
    
    r.add(& yes, '\0', _u8("bool"), true);
//    r.add(     "short"  , & short_value);
//    r.add('i', "int"    , & int_value);
//    r.add(     "long"   , & long_value);
//    r.add(     "intmax" , & intmax_value);
//    r.add('f', "float"  , & float_value);
//    r.add('d', "double" , & double_value);
//    r.add(   , "real"   , & real_value);
//    r.add('s', "string" , & string_value);
    
    TEST_OK(r.parse(argc, argv, pfs::cli::relaxed_flags));
}

}}

#endif /* __TEST_ROUTE_HPP__ */

