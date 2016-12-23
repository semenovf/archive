/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_cmdline.hpp
 * Author: wladt
 *
 * Created on December 14, 2016, 12:11 PM
 */

#ifndef __TEST_CMDLINE_HPP__
#define __TEST_CMDLINE_HPP__

#include <pfs/test/test.hpp>
#include <pfs/cli/cmdline.hpp>
#include <cstring>

namespace test {
namespace cli {

static char const * __argv[] = {
    "/path/to/program.sh"
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

static int __argc = sizeof(__argv)/sizeof(__argv[0]);

void test_iterator ()
{
    typedef pfs::cli::details::cmdline<char, char const *, char const **> cmdline;
    
//    char const * shortopt_prefix    = "-";
//    char const * longopt_prefix     = "--";
//    char const * argument_separator = "--";
//    char const * optarg_delim       = "=";
    
//    typedef pfs::cli::iterator<char const *> iterator;
    ADD_TESTS(12);
    cmdline cl(__argc, __argv);

    cmdline::iterator it  = cl.begin();
    cmdline::iterator end = cl.end();

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

void test_tokenizer ()
{
    ADD_TESTS(13);

    typedef pfs::cli::details::cmdline<char, char const *, char const **> cmdline;
    typedef typename cmdline::range_type range_type;
    
    cmdline cl(__argc, __argv);
    cl.set_delimiter_char('=');
    cl.set_shortopt_prefix("-", 1);
    cl.set_longopt_prefix("--", 2);
    cl.set_argument_separator("--", 2);

    cmdline::iterator it  = cl.begin();
    cmdline::iterator end = cl.end();

    TEST_OK(it != end);

    cmdline::token_type t[10];

    for (int i = 1; i < __argc; i++) {
       t[i - 1] = cl.split(*it, *it + strlen(*it));
       ++it;
    }

    TEST_OK(t[0].prefix    == range_type("", size_t(0))
            && t[0].option == range_type("", size_t(0))   
            && t[0].arg    == range_type("domain", 6));
    
    TEST_OK(t[1].prefix    == range_type("", size_t(0))
            && t[1].option == range_type("", size_t(0))
            && t[1].arg    == range_type("command", 7));
    
    TEST_OK(t[2].prefix    == range_type("-", 1)
            && t[2].option == range_type("a", 1)
            && t[2].arg    == range_type("", size_t(0)));
    
    TEST_OK(t[3].prefix    == range_type("-", 1)
            && t[3].option == range_type("bcd", 3)
            && t[3].arg    == range_type("", size_t(0)));
    
    TEST_OK(t[4].prefix    == range_type("--", 2)
            && t[4].option == range_type("option1", 7)
            && t[4].arg    == range_type("", size_t(0)));
    
    TEST_OK(t[5].prefix    == range_type("--", 2)
            && t[5].option == range_type("option2", 7)
            && t[5].arg    == range_type("option2_arg", 11));
    
    TEST_OK(t[6].prefix    == range_type("--", 2)
            && t[6].option == range_type("option 3", 8)
            && t[6].arg    == range_type("option3 arg", 11));
    
    TEST_OK(t[7].prefix    == range_type("--", 2)
            && t[7].option == range_type("", size_t(0))
            && t[7].arg    == range_type("", size_t(0)));
    
    TEST_OK(cl.is_argument_separator(t[7]));
    
    TEST_OK(t[8].prefix    == range_type("", size_t(0))
            && t[8].option == range_type("", size_t(0))
            && t[8].arg    == range_type("arg1", 4));
    
    TEST_OK(t[9].prefix    == range_type("--", size_t(2))
            && t[9].option == range_type("arg2", 4)
            && t[9].arg    == range_type("", size_t(0)));

    TEST_OK(it == end);   
}

}}

#endif /* __TEST_CMDLINE_HPP__ */

