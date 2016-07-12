/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <iostream>
#include <pfs/ratio.hpp>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

    BEGIN_TESTS(0);
    
    {
        ADD_TESTS(5);
        
        typedef pfs::ratio<5,3> t1;
        TEST_OK(t1::num == 5 && t1::den == 3);

        typedef pfs::ratio<25,15> t2;
        TEST_OK(t2::num == 5 && t2::den == 3);

        typedef pfs::ratio<42,42> t3;
        TEST_OK(t3::num == 1 && t3::den == 1);

        typedef pfs::ratio<0> t4;
        TEST_OK(t4::num == 0 && t4::den == 1);

        typedef pfs::ratio<7,-3> t5;
        TEST_OK(t5::num == -7 && t5::den == 3);
    }
    
    // Test Comparison
    //
    {
        ADD_TESTS(8);
        TEST_OK((pfs::ratio_equal<pfs::ratio<2,3>,pfs::ratio<4,6> >::value));

        TEST_OK((pfs::ratio_not_equal<pfs::ratio<2,3>,pfs::ratio<1,3> >::value));

        TEST_OK((pfs::ratio_less<pfs::ratio<23,37>, pfs::ratio<57,90> >::value));

        TEST_OK((pfs::ratio_less_equal<pfs::ratio<23,37>, pfs::ratio<57,90> >::value));
        TEST_OK((pfs::ratio_less_equal<pfs::ratio<5,3>, pfs::ratio<25,15> >::value));

        TEST_OK((pfs::ratio_greater<pfs::ratio<57,90>, pfs::ratio<23,37> >::value));

        TEST_OK((pfs::ratio_greater_equal<pfs::ratio<57,90>, pfs::ratio<23,37> >::value));
        TEST_OK((pfs::ratio_greater_equal<pfs::ratio<5,3>, pfs::ratio<25,15> >::value));
    }
    
    // Aritmetic operations
    //
    {
        ADD_TESTS(1);
        typedef pfs::ratio<2, 3> t1;
        typedef pfs::ratio<1, 6> t2;
        typedef pfs::ratio_add<t1, t2> t3;
        TEST_OK(t3::num == 5 && t3::den == 6);
    }
    
	return END_TESTS;
}

