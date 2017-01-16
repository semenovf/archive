/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_basic.hpp
 * Author: wladt
 *
 * Created on January 16, 2017, 3:11 PM
 */

#ifndef __PFS_TEST_BASIC_HPP__
#define __PFS_TEST_BASIC_HPP__

template <typename Key, typename T, template <typename, typename> class MapT>
void test_basic ()
{
    typedef pfs::mpl::map<Key, T, MapT> map_type;
    map_type map;
}

#endif /* __PFS_TEST_BASIC_HPP__ */
