/**
 * @file function.cpp
 * @author wladt
 * @date Dec 5, 2013
 */


#include <cwt/test.hpp>
#include <pfs.hpp>
#include <pfs/functional.hpp>
#include <iostream>

int func1 ()
{
	return 123;
}

void check1_0()
{
        typedef pfs::function<int (void)> int_function_t;

        int_function_t f1(func1);
        TEST_OK(123 == f1());

        int_function_t f2;
        f2 = f1;
        TEST_OK(123 == f2());
        TEST_OK(f1() == f2());

        int_function_t f3(f2);
        TEST_OK(123 == f3());
        TEST_OK(f2() == f3());
        TEST_OK(f1() == f3());
}

void check1()
{
        typedef pfs::funktion<int (void)> int_function_t;

        int_function_t f1(func1);
        TEST_OK(123 == f1());

        int_function_t f2;
        f2 = f1;
        TEST_OK(123 == f2());
        TEST_OK(f1() == f2());

        int_function_t f3(f2);
        TEST_OK(123 == f3());
        TEST_OK(f2() == f3());
        TEST_OK(f1() == f3());
}

int func2 (const int * x)
{
	return (*x);
}

double func3 (const double * x)
{
	return (*x);
}

void check2()
{
        typedef pfs::function<int (const int *)> int_function_t;
        typedef pfs::function<double (const double *)> double_function_t;
        int_function_t f2(func2);
        double_function_t f3(func3);

        int x = 10;
        //std::cout << "calling function with signature int (const int *): " << f2(&x) << std::endl;
        TEST_OK(x == f2(& x));

        double y = double(1/3);
        TEST_OK(y == f3(& y));
}



#ifdef __COMMENT__

struct Foo
{
        int smth(int x)
        {
                return x + 1;
        }
};


void check3()
{
        typedef function<int (Foo, int)> member_function_t;

        member_function_t f1 = &Foo::smth;


        Foo foo;
        cout << "calling member function with signature int (int): " << f1(foo, 5) << endl;
}





struct Bar
{
        int callMe(int x)
        {
                return x * 2;
        }
};


void check4()
{
        typedef function<int (void)> int_function_t;

        int_function_t f1;

        Bar bar;

        f1 = std::bind(&Bar::callMe, bar, 10);
        cout << "calling binded member function with signature int (void): " << f1() << endl;
}

#endif

int main (int argc, char *argv[])
{
	PFS_CHECK_SIZEOF_TYPES;
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(8);

	check1();
	check1_0();
	check2();
//        check3();
//        check4();

    END_TESTS;
}
