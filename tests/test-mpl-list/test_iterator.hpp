/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_iterator.hpp
 * Author: wladt
 *
 * Created on January 19, 2017, 2:15 PM
 */

#ifndef __PFS_TEST_ITERATOR_HPP__
#define __PFS_TEST_ITERATOR_HPP__

namespace test_iterator_00 {

template <template <typename> class ListT>
void test ()
{
    ADD_TESTS(4);
    
    typedef pfs::mpl::list<int, ListT> list_type;
    list_type l;
    
    TEST_OK(l.begin() == l.end());
    TEST_OK(l.cbegin() == l.cend());
    TEST_OK(l.rbegin() == l.rend());
    TEST_OK(l.crbegin() == l.crend());
    
//    for (int i = 0; i < 10; ++i)
//        l.push_back(i);
}

} // test_iterator_00

template <template <typename> class ListT>
void test_iterator ()
{
    test_iterator_00::test<ListT>();
}

#endif /* __PFS_TEST_ITERATOR_HPP__ */

