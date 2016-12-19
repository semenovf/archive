/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_iterator.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 12:11 PM
 */

#ifndef __TEST_ITERATOR_HPP__
#define __TEST_ITERATOR_HPP__

#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/cli.hpp>
//#include <iostream>

namespace test {
namespace cli {

void test_iterator ()
{
    ADD_TESTS(1);
    
    char const * argv[] = { "program"
            , "command"
            , "--option1"
            , "--option2=option_arg2"
            , "-o"
            , "--"
            , "hello" };
    int argc = sizeof(argv)/sizeof(argv[0]);
    
    pfs::cli::iterator it  = pfs::cli::begin(argc, argv);
    pfs::cli::iterator end = pfs::cli::end(argc, argv);
    
    int counter = 0;
    
    for (; counter < argc && it != end; ++counter) {
        if (*it != pfs::string(argv[counter]))
            break;
        
        //std::cout << *it << std::endl;
        ++it;
    }
    
    TEST_OK2(counter == argc, "Iterator test is OK");
    
#if __COMMENT__    
    
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
#endif    
}

}}

#endif /* __TEST_ITERATOR_HPP__ */

