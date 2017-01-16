/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_basic.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 1:08 PM
 */

#ifndef __PFS_TEST_BASIC_HPP__
#define __PFS_TEST_BASIC_HPP__

template <typename T, template <typename> class ListT>
void test_basic ()
{
    typedef pfs::mpl::list<T, ListT> list_type;
    list_type list;
}

#endif /* __PFS_TEST_BASIC_HPP__ */

