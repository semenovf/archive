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
#include <pfs/cli/iterator.hpp>
#include <pfs/cli.hpp>
//#include <iostream>

namespace test {
namespace cli {

void test_iterator ()
{
    typedef pfs::cli::iterator<pfs::cli::traits> iterator;
    
    char const * argv[] = { "/path/to/program.sh"
            , "domain"
            , "command1"
            , "-a"
            , "-bcd"
            , "--option1"
            , "--option2=option2_arg"
            , "--opt\"ion3=\"=\"option3 arg\""
            , "--"
            , "arg1"
            , "--arg2"
    };
    
    int argc = sizeof(argv)/sizeof(argv[0]);

    {
        ADD_TESTS(12);
        
        iterator it  = pfs::cli::begin<pfs::cli::traits>(argc, argv);
        iterator end = pfs::cli::end<pfs::cli::traits>(argc, argv);

        TEST_OK(it != end);
        TEST_OK(*it++ == "domain");
        TEST_OK(*it++ == "command1");
        TEST_OK(*it++ == "-a");
        TEST_OK(*it++ == "-bcd");
        TEST_OK(*it++ == "--option1");
        TEST_OK(*it++ == "--option2=option2_arg");
        TEST_OK(*it++ == "--opt\"ion3=\"=\"option3 arg\"");
        TEST_OK(*it++ == "--");
        TEST_OK(*it++ == "arg1");
        TEST_OK(*it++ == "--arg2");
        TEST_OK(it == end);
    }
    
    {
        ADD_TESTS(12);
        
        iterator it  = pfs::cli::begin<pfs::cli::traits>(argc, argv);
        iterator end = pfs::cli::end<pfs::cli::traits>(argc, argv);

        TEST_OK(it != end);
    }
}

}}

#endif /* __TEST_ITERATOR_HPP__ */

