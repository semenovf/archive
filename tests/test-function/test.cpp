/**
 * @file function.cpp
 * @author wladt
 * @date Dec 5, 2013
 */


#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include <pfs/functional.hpp>
#include <iostream>

int func1 ()
{
	return 123;
}

void check1()
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
	TEST_OK(x == f2(& x));

	double y = double(1/3);
	TEST_OK(y == f3(& y));
}


struct Foo
{
	int smth(short x)
	{
		return x + 1;
	}
};


void check3()
{
	typedef pfs::function<int (Foo, short)> member_function_t;

	member_function_t f1(& Foo::smth);

	Foo foo;
	TEST_OK(f1(foo, 5) == 6);
}


int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(9);

	check1();
	check2();
    check3();

    return END_TESTS;
}
