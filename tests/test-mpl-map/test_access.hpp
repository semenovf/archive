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

#ifndef __PFS_TEST_ACCESS_HPP__
#define __PFS_TEST_ACCESS_HPP__

template </*typename Key, typename T, */template <typename, typename> class MapT>
void test_access ()
{
    typedef pfs::mpl::map<int, double, MapT> map_type;

    {
        map_type m;
        m[0] = 1.5;

        double & rd = m.at(0);
        TEST_OK(rd == 1.5);
        
        try {
            m.at(1);
        } catch (pfs::out_of_range & obj) {
            // Expected
            TEST_OK2(true, "`out_of_range` exception")
        } catch (...) {
            // Failed
            TEST_OK2(false, "`out_of_range` exception")
        }
    }

    {
        map_type m;
        m[1] = 2.5;
        const map_type cm(m);

        const double & crd = cm.at(1);
        TEST_OK(crd == 2.5);
        
        try {
            cm.at(0);
        } catch (pfs::out_of_range & obj) {
            // Expected
            TEST_OK2(true, "`out_of_range` exception")
        } catch (...) {
            // Failed.
            TEST_OK2(true, "`out_of_range` exception")
        }
    }
}

#endif /* __PFS_TEST_ACCESS_HPP__ */
