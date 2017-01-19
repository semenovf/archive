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

namespace test_access_01 {

template <template <typename, typename> class MapT>
void test ()
{
    ADD_TESTS(4);
    
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

} // test_access_01

namespace test_access_02 {

struct value
{
    int val;
    bool valid;
    
    value () : val(0), valid(true) {}
    value (int v) : val(v), valid(true) {}
    value & operator = (int v)
    {
      val = v;
      valid = true;
      return *this;
    }
};

inline bool operator == (value const & lhs, value const & rhs)
{
    return lhs.val == rhs.val;
}

inline bool operator < (value const & lhs, value const & rhs)
{
    return lhs.val < rhs.val;
}

template <template <typename, typename> class MapT>
void test ()
{
    ADD_TESTS(10);
    
    typedef pfs::mpl::map<value, value, MapT> map_type;

    map_type m;
    TEST_OK(m.empty());

    m[value(1)] = value(17);
    TEST_OK(m.size() == 1);
    TEST_OK((m.begin().key()).val == 1);
    TEST_OK((m.begin().value()).val == 17);
    TEST_OK(m[value(1)].val == 17);

    m[value(2)] = value(9);
    TEST_OK(m.size() == 2);
    TEST_OK(m[value(2)].val == 9);

    m[value(1)] = value(5);
    TEST_OK(m.size() == 2);
    TEST_OK(m[value(1)].val == 5);
    TEST_OK(m[value(2)].val == 9);
}

} // test_access_02

template <template <typename, typename> class MapT>
void test_access ()
{
    test_access_01::test<MapT>();
    test_access_02::test<MapT>();
}

#endif /* __PFS_TEST_ACCESS_HPP__ */
