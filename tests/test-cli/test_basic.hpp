/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_basic.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 12:11 PM
 */

#ifndef __TEST_BASIC_HPP__
#define __TEST_BASIC_HPP__

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/cli.hpp>

namespace test {
namespace cli {

void test_basic ()
{
    ADD_TESTS(0);
    //pfs::cli::no_long_option
    
    int parse_flag = pfs::cli::combine_short_options | pfs::cli::single_dash_long_option;
    pfs::cli::route route1;
    
    bool     yes          = true;
    short    short_value  = 0;
    int      int_value    = 0;
    long     long_value   = 0;
    intmax_t intmax_value = 0;
    float    float_value  = 0.0f;
    double   double_value = 0.0f;
    real_t   real_value   = 0.0f;   
    pfs::string string_value;
    
    route1.add(     "bool"   , & yes);
    route1.add(     "short"  , & short_value);
    route1.add('i', "int"    , & int_value);
    route1.add(     "long"   , & long_value);
    route1.add(     "intmax" , & intmax_value);
    route1.add('f', "float"  , & float_value);
    route1.add('d', "double" , & double_value);
    route1.add(   , "real"   , & real_value);
    route1.add('s', "string" , & string_value);
    
    //TEST_OK(pfs::cli::parse(route1, parse_flag, argc, argv));
}

}}

#endif /* __TEST_BASIC_HPP__ */

