/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_trivial_forward_list.hpp
 * Author: wladt
 *
 * Created on October 26, 2016, 3:06 PM
 */

#ifndef __TEST_TRIVIAL_LIST_HPP__
#define __TEST_TRIVIAL_LIST_HPP__

#include <pfs/test/test.hpp>
#include <pfs/tl/trivial_list.hpp>

namespace test {
namespace trivial_list {

struct A
{
    int a;
    
    A (int x) : a(x)
    {}
    
    ~A ()
    {}
};

typedef pfs::tl::trivial_list<A> list_type;

void test_basic ()
{
    ADD_TESTS(1);
    
    list_type l;
    
    l.emplace_back(10);
    TEST_OK(l.back().a == 10);
}

}}

#endif /* __TEST_TRIVIAL_LIST_HPP__ */

