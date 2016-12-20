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
    ADD_TESTS(12);
    
    typedef pfs::cli::iterator<pfs::cli::traits> iterator;
    
    char const * argv[] = { "/path/to/program.sh"
            , "domain"
            , "command1"
            , "-a"
            , "-bcd"
            , "--option1"
            , "--option2=option2_arg"
            , "--"
            , "arg1"
            , "--arg2"
    };
    
    int argc = sizeof(argv)/sizeof(argv[0]);
    
    iterator it  = pfs::cli::begin<pfs::cli::traits>(argc, argv);
    iterator end = pfs::cli::end<pfs::cli::traits>(argc, argv);
    
    TEST_OK(it != end);
    TEST_OK(*it++ == "/path/to/program.sh");
    TEST_OK(*it++ == "domain");
    TEST_OK(*it++ == "command1");
    TEST_OK(*it++ == "-a");
    TEST_OK(*it++ == "-bcd");
    TEST_OK(*it++ == "--option1");
    TEST_OK(*it++ == "--option2=option2_arg");
    TEST_OK(*it++ == "--");
    TEST_OK(*it++ == "arg1");
    TEST_OK(*it++ == "--arg2");
    TEST_OK(it == end);
}

//void test_iterator_pod_string ()
//{
//    ADD_TESTS(12);
//    
//    typedef pfs::cli::pod_string_iterator_tag iterator_tag;
//    typedef pfs::cli::iterator<pfs::cli::traits, iterator_tag> iterator;
//    
//    char const * cmdline = "/path/to/program.sh domain command1 -a -bcd --option1 --option2=option2_arg -- arg1 --arg2";
//
//    iterator it  = pfs::cli::begin<pfs::cli::traits>(cmdline);
//    iterator end = pfs::cli::end<pfs::cli::traits>(cmdline);
//
//    TEST_OK(it != end);
//    TEST_OK(*it++ == "/path/to/program.sh");
//    TEST_OK(*it++ == "domain");
//    TEST_OK(*it++ == "command1");
//    TEST_OK(*it++ == "-a");
//    TEST_OK(*it++ == "-bcd");
//    TEST_OK(*it++ == "--option1");
//    TEST_OK(*it++ == "--option2=option2_arg");
//    TEST_OK(*it++ == "--");
//    TEST_OK(*it++ == "arg1");
//    TEST_OK(*it++ == "--arg2");
//    TEST_OK(it == end);
//}
//
//void test_iterator_string ()
//{
//    ADD_TESTS(12);
//    
//    typedef pfs::cli::traits::string_type string_type;
//    typedef pfs::cli::string_iterator_tag iterator_tag;
//    typedef pfs::cli::iterator<pfs::cli::traits, iterator_tag> iterator;
//    
//    
//    
//    {
//        pfs::cli::traits::string_type cmdline("-o");
//        iterator it  = pfs::cli::begin<pfs::cli::traits>(cmdline);
//        iterator end = pfs::cli::end<pfs::cli::traits>(cmdline);
//        TEST_OK(*it == cmdline);
//    }
//
//    {
//        pfs::cli::traits::string_type cmdline("-abc");
//    }
////    pfs::cli::traits::string_type cmdline3("--option");
////    pfs::cli::traits::string_type cmdline4("--option=arg");
////    pfs::cli::traits::string_type cmdline5("\"double \\\" quoted string\"");
////    pfs::cli::traits::string_type cmdline6("\"double \\\"=quoted string\"");
////    pfs::cli::traits::string_type cmdline7("--option=\"double \\\"=quoted string\"");
////    
////    
////    pfs::cli::traits::string_type cmdline("/path/to/program.sh domain command1 "
////        "-a -bcd --option1 \"double \\\" quoted string\" "
////        "--option2=option2_arg -- 'quoted \" string' arg1 --arg2");
////
////    iterator it  = pfs::cli::begin<pfs::cli::traits>(cmdline);
////    iterator end = pfs::cli::end<pfs::cli::traits>(cmdline);
////
////    TEST_OK(it != end);
////    TEST_OK(*it++ == string_type("/path/to/program.sh"));
////    TEST_OK(*it++ == string_type("domain"));
////    TEST_OK(*it++ == string_type("command1"));
////    TEST_OK(*it++ == string_type("-a"));
////    TEST_OK(*it++ == string_type("-bcd"));
////    TEST_OK(*it++ == string_type("--option1"));
////    TEST_OK(*it++ == string_type("\"double \\\" quoted string\""));
////    TEST_OK(*it++ == string_type("--option2=option2_arg"));
////    TEST_OK(*it++ == string_type("--"));
////    TEST_OK(*it++ == string_type("'quoted \" string'"));
////    TEST_OK(*it++ == string_type("arg1"));
////    TEST_OK(*it++ == string_type("--arg2"));
////    TEST_OK(it == end);
//}

}}

#endif /* __TEST_ITERATOR_HPP__ */

