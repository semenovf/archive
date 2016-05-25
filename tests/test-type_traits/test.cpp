/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <iostream>
#include <pfs.hpp>
#include <pfs/test/test.hpp>
#include <pfs/type_traits.hpp>

using std::cout;
using std::endl;

enum my_enum {
    e1,
    e2
};

void test_integral_constant ()
{
	ADD_TESTS(15);

    typedef pfs::integral_constant<int, 2> two_t;
    typedef pfs::integral_constant<int, 4> four_t;
    
    TEST_OK((pfs::is_same<two_t, four_t>()) == false);
    TEST_OK(two_t::value == 2);
    TEST_OK(four_t::value == 4);
    TEST_OK(two_t::value * 2 == four_t::value);
    
    typedef pfs::integral_constant<my_enum, e1> my_e_e1;
    typedef pfs::integral_constant<my_enum, e2> my_e_e2;

    TEST_OK(my_e_e1::value != my_e_e2::value);
    TEST_OK(my_e_e1::value == e1);
    TEST_OK(my_e_e2::value == e2);
    TEST_OK((pfs::is_same<my_e_e1, my_e_e1>::value) == true);
    TEST_OK((pfs::is_same<my_e_e2, my_e_e2>::value) == true);
    TEST_OK((pfs::is_same<my_e_e1, my_e_e2>::value) == false);
    
    typedef pfs::integral_constant<bool, true>  my_true_t;
    typedef pfs::integral_constant<bool, false> my_false_t;
    
    TEST_OK((pfs::is_same<my_true_t, my_false_t>::value)  == false);
    TEST_OK((pfs::is_same<my_true_t, my_true_t>::value)   == true);
    TEST_OK((pfs::is_same<my_false_t, my_false_t>::value) == true);
    TEST_OK(my_true_t::value == true);
    TEST_OK(my_false_t::value == false);
}

struct A { static int id () { return 1; } };
struct B { static int id () { return 2; } };

void test_conditional ()
{
    ADD_TESTS(2);
    
    typedef pfs::conditional<true, A, B>::type Type1;
    typedef pfs::conditional<false, A, B>::type Type2;
    
    TEST_OK2(Type1::id() == 1, "pfs::conditional<true, A, B>::type is A");
    TEST_OK2(Type2::id() == 2, "pfs::conditional<false, A, B>::type is B");
}

void test_remove_cv ()
{
    ADD_TESTS(5);
    
    typedef pfs::remove_cv<const int>::type type1;
    typedef pfs::remove_cv<volatile int>::type type2;
    typedef pfs::remove_cv<const volatile int>::type type3;
    typedef pfs::remove_cv<const volatile int *>::type type4;
    typedef pfs::remove_cv<int * const volatile>::type type5;
 
    TEST_OK((pfs::is_same<int, type1>::value));
    TEST_OK((pfs::is_same<int, type2>::value));
    TEST_OK((pfs::is_same<int, type3>::value));
    TEST_OK((pfs::is_same<const volatile int *, type4>::value));
    TEST_OK((pfs::is_same<int *, type5>::value));
}

void test_remove_reference ()
{
    ADD_TESTS(4);
    
    TEST_OK((pfs::is_same<int, int>()) == true);
    TEST_OK((pfs::is_same<int, int &>()) == false);

    TEST_OK((pfs::is_same<int, pfs::remove_reference<int>::type>()) == true);
    TEST_OK((pfs::is_same<int, pfs::remove_reference<int &>::type>()) == true);
}

void test_remove_pointer ()
{
    ADD_TESTS(9);
    
    TEST_OK((pfs::is_same<int, int>()) == true);
    TEST_OK((pfs::is_same<int, int *>()) == false);
    TEST_OK((pfs::is_same<int, int **>()) == false);
    TEST_OK((pfs::is_same<int, pfs::remove_pointer<int>::type>()) == true);
    TEST_OK((pfs::is_same<int, pfs::remove_pointer<int *>::type>()) == true);
    TEST_OK((pfs::is_same<int, pfs::remove_pointer<int **>::type>()) == false);
    TEST_OK((pfs::is_same<int, pfs::remove_pointer<int const *>::type>()) == true);
    TEST_OK((pfs::is_same<int, pfs::remove_pointer<int volatile * >::type>()) == true);
    TEST_OK((pfs::is_same<int, pfs::remove_pointer<int const volatile * >::type>()) == true);
}

void test_is_void ()
{
    ADD_TESTS(7);

	TEST_OK(pfs::is_void<void>() == true);
    TEST_OK(pfs::is_void<const void>() == true);
    TEST_OK(pfs::is_void<volatile void>() == true);
    TEST_OK(pfs::is_void<const volatile void>() == true);
    TEST_OK(pfs::is_void<volatile const void>() == true);
    TEST_OK(pfs::is_void<int>() == false);
    TEST_OK(pfs::is_void<A>() == false);
}

void test_is_integral ()
{
    ADD_TESTS(32);

    TEST_OK(pfs::is_integral<bool>::value);
    TEST_OK(pfs::is_integral<bool const>::value);
    TEST_OK(pfs::is_integral<bool volatile>::value);
    TEST_OK(pfs::is_integral<bool const volatile>::value);
    
    TEST_OK(pfs::is_integral<char>::value);
    TEST_OK(pfs::is_integral<char const>::value);
    TEST_OK(pfs::is_integral<char volatile>::value);
    TEST_OK(pfs::is_integral<char const volatile>::value);

    TEST_OK(pfs::is_integral<signed char>::value);
    TEST_OK(pfs::is_integral<signed char const>::value);
    TEST_OK(pfs::is_integral<signed char volatile>::value);
    TEST_OK(pfs::is_integral<signed char const volatile>::value);

    TEST_OK(pfs::is_integral<unsigned char>::value);
    TEST_OK(pfs::is_integral<unsigned char const>::value);
    TEST_OK(pfs::is_integral<unsigned char volatile>::value);
    TEST_OK(pfs::is_integral<unsigned char const volatile>::value);

    TEST_OK(pfs::is_integral<wchar_t>::value);
    TEST_OK(pfs::is_integral<wchar_t const>::value);
    TEST_OK(pfs::is_integral<wchar_t volatile>::value);
    TEST_OK(pfs::is_integral<wchar_t const volatile>::value);

    TEST_OK(pfs::is_integral<int>::value);
    TEST_OK(pfs::is_integral<int const>::value);
    TEST_OK(pfs::is_integral<int volatile>::value);
    TEST_OK(pfs::is_integral<int const volatile>::value);

    TEST_OK(pfs::is_integral<signed short>::value);
    TEST_OK(pfs::is_integral<signed short const>::value);
    TEST_OK(pfs::is_integral<signed short volatile>::value);
    TEST_OK(pfs::is_integral<signed short const volatile>::value);

    TEST_OK(pfs::is_integral<unsigned long>::value);
    TEST_OK(pfs::is_integral<unsigned long const>::value);
    TEST_OK(pfs::is_integral<unsigned long volatile>::value);
    TEST_OK(pfs::is_integral<unsigned long const volatile>::value);

    
#if PFS_HAVE_LONGLONG
    ADD_TESTS(4);
    
    TEST_OK(pfs::is_integral<signed long long>::value);
    TEST_OK(pfs::is_integral<signed long long const>::value);
    TEST_OK(pfs::is_integral<signed long long volatile>::value);
    TEST_OK(pfs::is_integral<signed long long const volatile>::value);
#endif

    ADD_TESTS(5);
    TEST_OK(pfs::is_integral<void>::value == false);
    TEST_OK(pfs::is_integral<void const>::value == false);
    TEST_OK(pfs::is_integral<float>::value == false);
    TEST_OK(pfs::is_integral<double>::value == false);
    TEST_OK(pfs::is_integral<A>() == false);
}

void test_is_floating_point ()
{
    ADD_TESTS(8);

    TEST_OK(pfs::is_floating_point<float>::value);
    TEST_OK(pfs::is_floating_point<float const>::value);
    TEST_OK(pfs::is_floating_point<float volatile>::value);
    TEST_OK(pfs::is_floating_point<float const volatile>::value);

    TEST_OK(pfs::is_floating_point<double>::value);
    TEST_OK(pfs::is_floating_point<double const>::value);
    TEST_OK(pfs::is_floating_point<double volatile>::value);
    TEST_OK(pfs::is_floating_point<double const volatile>::value);
    
#if PFS_HAVE_LONGLONG
    ADD_TESTS(4);
    
    TEST_OK(pfs::is_floating_point<long double>::value);
    TEST_OK(pfs::is_floating_point<long double const>::value);
    TEST_OK(pfs::is_floating_point<long double volatile>::value);
    TEST_OK(pfs::is_floating_point<long double const volatile>::value);
#endif

    ADD_TESTS(5);
    TEST_OK(pfs::is_floating_point<void>::value == false);
    TEST_OK(pfs::is_floating_point<void const>::value == false);
    TEST_OK(pfs::is_floating_point<bool>::value == false);
    TEST_OK(pfs::is_floating_point<int>::value == false);
    TEST_OK(pfs::is_floating_point<A>() == false);
}

void test_is_arithmetic ()
{
    ADD_TESTS(4);
    
    TEST_OK(pfs::is_arithmetic<bool>::value);
    TEST_OK(pfs::is_arithmetic<wchar_t const>::value);
    TEST_OK(pfs::is_arithmetic<unsigned long const volatile>::value);
    TEST_OK(pfs::is_arithmetic<float const volatile>::value);
        
    ADD_TESTS(3);
    TEST_OK(pfs::is_arithmetic<void>::value == false);
    TEST_OK(pfs::is_arithmetic<void const>::value == false);
    TEST_OK(pfs::is_arithmetic<A>() == false);
}

void test_is_fundamental ()
{
    ADD_TESTS(7);
    
    TEST_OK(pfs::is_fundamental<A>::value == false);
    TEST_OK(pfs::is_fundamental<int>::value == true);
    TEST_OK(pfs::is_fundamental<int &>::value == false);
    TEST_OK(pfs::is_fundamental<int *>::value == false);
    TEST_OK(pfs::is_fundamental<float>::value == true);
    TEST_OK(pfs::is_fundamental<float &>::value == false);
    TEST_OK(pfs::is_fundamental<float *>::value == false);
}

void test_is_signed ()
{
    ADD_TESTS(5);
    
    TEST_OK(pfs::is_signed<A>::value == false);
    TEST_OK(pfs::is_signed<bool>::value == false);
    TEST_OK(pfs::is_signed<float>::value == true);
    TEST_OK(pfs::is_signed<signed int>())
    TEST_OK(pfs::is_signed<unsigned int>() == false);
}

void test_is_unsigned ()
{
    ADD_TESTS(5);
    
    TEST_OK(pfs::is_unsigned<A>::value == false);
    TEST_OK(pfs::is_unsigned<bool>::value == true);
    TEST_OK(pfs::is_unsigned<float>::value == false);
    TEST_OK(pfs::is_unsigned<signed int>::value == false);
    TEST_OK(pfs::is_unsigned<unsigned int>::value);
}

void test_is_const ()
{
    ADD_TESTS(5);
    
    TEST_OK(pfs::is_const<int>::value         == false);
    TEST_OK(pfs::is_const<const int>::value   == true);
    TEST_OK(pfs::is_const<const int *>::value == false);
    TEST_OK(pfs::is_const<int * const>::value == true);
    TEST_OK(pfs::is_const<const int &>::value == false);
}

void test_is_volatile ()
{
    ADD_TESTS(5);
    
    TEST_OK(pfs::is_volatile<int>::value            == false);
    TEST_OK(pfs::is_volatile<volatile int>::value   == true);
    TEST_OK(pfs::is_volatile<volatile int *>::value == false);
    TEST_OK(pfs::is_volatile<int * volatile>::value == true);
    TEST_OK(pfs::is_volatile<volatile int &>::value == false);
}

int main (int argc, char * argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

    BEGIN_TESTS(0);
    
    test_integral_constant();
    test_conditional();
    test_remove_cv();
    test_remove_reference();
    test_remove_pointer();
    test_is_void();
    test_is_integral();
    test_is_floating_point();
    test_is_arithmetic();
    test_is_fundamental();
    test_is_signed();
    test_is_unsigned();
    test_is_const();
    test_is_volatile();
    
	return END_TESTS;
}
