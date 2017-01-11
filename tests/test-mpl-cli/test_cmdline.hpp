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

#include <cstring>
#include <pfs/test/test.hpp>
#include <pfs/mpl/stdc/string.hpp>
#include <pfs/mpl/cli/cmdline.hpp>

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
    typedef pfs::mpl::cli::cmdline<char const *> cmdline;
    typedef typename cmdline::string_type string_type;
    
    ADD_TESTS(12);
    cmdline cl(__argc, __argv);

    cmdline::iterator it  = cl.begin();
    cmdline::iterator end = cl.end();

    TEST_OK(it != end);
    TEST_OK(*it++ == string_type("domain"))
    TEST_OK(*it++ == string_type("command"));
    TEST_OK(*it++ == string_type("-a"));
    TEST_OK(*it++ == string_type("-bcd"));
    TEST_OK(*it++ == string_type("--option1"));
    TEST_OK(*it++ == string_type("--option2=option2_arg"));
    TEST_OK(*it++ == string_type("--option 3=option3 arg"));
    TEST_OK(*it++ == string_type("--"));
    TEST_OK(*it++ == string_type("arg1"));
    TEST_OK(*it++ == string_type("--arg2"));
    TEST_OK(it == end);
}

void test_tokenizer ()
{
    ADD_TESTS(12);

    typedef pfs::mpl::cli::cmdline<char const *> cmdline;
    typedef typename cmdline::string_type string_type;
    
    cmdline cl(__argc, __argv);
    
    cl.set_delimiter_char('=');
    cl.set_shortopt_prefix(string_type("-"));
    cl.set_longopt_prefix(string_type("--"));
    cl.set_argument_separator(string_type("--"));

    cmdline::iterator it  = cl.begin();
    cmdline::iterator end = cl.end();

    TEST_OK(it != end);

    cmdline::token_type t[10];

    for (int i = 1; i < __argc; i++) {
       t[i - 1] = cl.split(*it);
       ++it;
    }

    TEST_OK(t[0].prefix    == string_type("")
            && t[0].option == string_type("")   
            && t[0].arg    == string_type("domain"));
    
    TEST_OK(t[1].prefix    == string_type("")
            && t[1].option == string_type("")
            && t[1].arg    == string_type("command"));
    
    TEST_OK(t[2].prefix    == string_type("-")
            && t[2].option == string_type("a")
            && t[2].arg    == string_type(""));
    
    TEST_OK(t[3].prefix    == string_type("-")
            && t[3].option == string_type("bcd")
            && t[3].arg    == string_type(""));
    
    TEST_OK(t[4].prefix    == string_type("--")
            && t[4].option == string_type("option1")
            && t[4].arg    == string_type(""));
    
    TEST_OK(t[5].prefix    == string_type("--")
            && t[5].option == string_type("option2")
            && t[5].arg    == string_type("option2_arg"));
    
    TEST_OK(t[6].prefix    == string_type("--")
            && t[6].option == string_type("option 3")
            && t[6].arg    == string_type("option3 arg"));
    
    TEST_OK(t[7].prefix    == string_type("--")
            && t[7].option == string_type("")
            && t[7].arg    == string_type(""));
    
    TEST_OK(t[8].prefix    == string_type("")
            && t[8].option == string_type("")
            && t[8].arg    == string_type("arg1"));
    
    TEST_OK(t[9].prefix    == string_type("--")
            && t[9].option == string_type("arg2")
            && t[9].arg    == string_type(""));

    TEST_OK(it == end);
}

}}

#endif /* __TEST_CMDLINE_HPP__ */
