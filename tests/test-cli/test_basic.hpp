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
    pfs::cli::option<bool>        bool_long_opt(_u8("bool")); // Has default value, so it is optional
    bool_long_opt.set_default_value(true);
    pfs::cli::option<bool>        bool_short_opt('b');            // Has no default value, so it is mandatory
    pfs::cli::option<short>       short_int_opt(_u8("short"));
    pfs::cli::option<int>         int_opt('i', _u8("int"));
    pfs::cli::option<long>        long_int_opt(_u8("long"));
    pfs::cli::option<intmax_t>    intmax_opt(_u8("intmax"));
    pfs::cli::option<float>       float_opt('f', _u8("float"));
    pfs::cli::option<double>      double_opt('d', _u8("double"));
    pfs::cli::option<real_t>      real_opt(_u8("real"));
    pfs::cli::option<pfs::string> string_opt('s', _u8("string"));
    
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

