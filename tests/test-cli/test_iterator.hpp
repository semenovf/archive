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

namespace test {
namespace cli {

void test_iterator ()
{
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

    {
        ADD_TESTS(12);
        
        pfs::cli::iterator it  = pfs::cli::begin<pfs::cli::traits>(argc, argv);
        pfs::cli::iterator end = pfs::cli::end<pfs::cli::traits>(argc, argv);

        TEST_OK(it != end);
        TEST_OK(*it++ == "domain");
        TEST_OK(*it++ == "command");
        TEST_OK(*it++ == "-a");
        TEST_OK(*it++ == "-bcd");
        TEST_OK(*it++ == "--option1");
        TEST_OK(*it++ == "--option2=option2_arg");
        TEST_OK(*it++ == "--option 3=option3 arg");
        TEST_OK(*it++ == "--");
        TEST_OK(*it++ == "arg1");
        TEST_OK(*it++ == "--arg2");
        TEST_OK(it == end);
    }
    
    {
        ADD_TESTS(12);
        
        pfs::cli::iterator it  = pfs::cli::begin<pfs::cli::traits>(argc, argv);
        pfs::cli::iterator end = pfs::cli::end<pfs::cli::traits>(argc, argv);

        TEST_OK(it != end);
        
        pfs::cli::tuple t[10];
        
        for (int i = 1; i < argc; i++) {
            t[i - 1] = it.split();
            ++it;
        }
        
        TEST_OK(t[0].prefix == ""   && t[0].option == ""         && t[0].arg == "domain");
        TEST_OK(t[1].prefix == ""   && t[1].option == ""         && t[1].arg == "command");
        TEST_OK(t[2].prefix == "-"  && t[2].option == "a"        && t[2].arg == "");
        TEST_OK(t[3].prefix == "-"  && t[3].option == "bcd"      && t[3].arg == "");
        TEST_OK(t[4].prefix == "--" && t[4].option == "option1"  && t[4].arg == "");
        TEST_OK(t[5].prefix == "--" && t[5].option == "option2"  && t[5].arg == "option2_arg");
        TEST_OK(t[6].prefix == "--" && t[6].option == "option 3" && t[6].arg == "option3 arg");
        TEST_OK(t[7].prefix == "--" && t[7].option == ""         && t[7].arg == "");
        TEST_OK(t[8].prefix == ""   && t[8].option == ""         && t[8].arg == "arg1");
        TEST_OK(t[9].prefix == ""   && t[9].option == ""         && t[9].arg == "--arg2");

        TEST_OK(it == end);
    }
}

}}

#endif /* __TEST_ITERATOR_HPP__ */

