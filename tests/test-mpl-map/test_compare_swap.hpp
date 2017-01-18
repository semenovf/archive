/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_compare.hpp
 * Author: wladt
 *
 * Created on January 18, 2017, 9:20 AM
 */

#ifndef __PFS_TEST_COMPARE_HPP__
#define __PFS_TEST_COMPARE_HPP__

template <template <typename, typename> class MapT>
void test_compare ()
{
    ADD_TESTS(4);
    
    typedef pfs::mpl::map<int, int, MapT> map_type;
    
    map_type m1;
    map_type m2;
    map_type m3;
    map_type m4;
    
    m1[0] = 0;
    m1[1] = 1;
    m1[2] = 2;

    m2[0] = 2;
    m2[1] = 1;
    m2[2] = 0;

    m3[0] = 0;
    m3[1] = 1;
    m3[2] = 2;
    
    TEST_OK(m1 == m1);
    TEST_OK(m1 != m2);
    TEST_OK(m1 == m3);
    TEST_OK(m1 != m4);
}

template <template <typename, typename> class MapT>
void test_swap ()
{
    ADD_TESTS(9);
    
    typedef pfs::mpl::map<int, int, MapT> map_type;
    
    map_type m1;
    map_type m2;
    map_type m3;
    map_type m4;
    map_type m5;
    
    m1[0] = m2[0] = 0;
    m1[1] = m2[1] = 1;
    m1[2] = m2[2] = 2;

    m3[0] = m4[0] = 2;
    m3[1] = m4[1] = 1;
    m3[2] = m4[2] = 0;

    TEST_OK(m1 == m2);
    TEST_OK(m3 == m4);
    
    pfs::swap(m1, m3);
    TEST_OK(m1 == m4);
    TEST_OK(m3 == m2);

    TEST_OK(m5.empty());
    TEST_OK(!m1.empty());

    pfs::swap(m1, m5);

    TEST_OK(m1.empty());
    TEST_OK(!m5.empty());
    TEST_OK(m5.size() == 3);
}

#endif /* __PFS_TEST_COMPARE_HPP__ */

