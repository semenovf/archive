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
    pfs::cli::option<bool>        bool_long_opt(pfs::cli::no_short_name, "bool", true); // Has default value, so it is optional
    pfs::cli::option<bool>        bool_short_opt('b', pfs::cli::no_long_name);            // Has no default value, so it is mandatory
    pfs::cli::option<short>       short_int_opt(pfs::cli::no_short_name, "short");
    pfs::cli::option<int>         int_opt('i', "int");
    pfs::cli::option<long>        long_int_opt(pfs::cli::no_short_name, "long");
    pfs::cli::option<intmax_t>    intmax_opt(pfs::cli::no_short_name, "intmax");
    pfs::cli::option<float>       float_opt('f', "float");
    pfs::cli::option<double>      double_opt('d', "double");
    pfs::cli::option<real_t>      real_opt(pfs::cli::no_short_name, "real");
    pfs::cli::option<pfs::string> string_opt('s', "string");
    
    route1.add(bool_long_opt);
    route1.add(bool_short_opt);
    route1.add(short_int_opt);
    route1.add(int_opt);
    route1.add(long_int_opt);
    route1.add(intmax_opt);
    route1.add(float_opt);
    route1.add(double_opt);
    route1.add(real_opt);
    route1.add(string_opt);
    
    //TEST_OK(pfs::cli::parse(route1, parse_flag, argc, argv));
}

}}

#endif /* __TEST_BASIC_HPP__ */

