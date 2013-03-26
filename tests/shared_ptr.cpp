/**
 * @file   shared_ptr.cpp
 * @author wladt
 * @date   Mar 26, 2013 3:17:03 PM
 *
 * @brief
 */

#include <cwt/test.h>
#include <cstdio>
#include <cwt/shared_ptr.hpp>


using namespace cwt;

void test_00()
{
	struct Foo {
		Foo() { printf("Foo...\n"); }
		~Foo() { printf("~Foo...\n"); }
	};

	struct D {
		void operator()(Foo* p) const {
			printf("Call delete for Foo object...\n");
			delete p;
		}
	};

    // constructor with no managed object
    shared_ptr<Foo> sh1;

    // constructor with object
    shared_ptr<Foo> sh2(new Foo);
    shared_ptr<Foo> sh3(sh2);

    CWT_TEST_OK(sh1.use_count() == 0);
    CWT_TEST_OK(sh2.use_count() == 2);
    CWT_TEST_OK(sh3.use_count() == 2);

    //constructor with object and deleter
    shared_ptr<Foo> sh4(new Foo, D());
}

void test_unique()
{
    shared_ptr<int> sp1(5);
    CWT_TEST_OK(sp1.unique() == true);

    shared_ptr<int> sp2 {sp1};
    CWT_TEST_OK(sp1.unique() == false);

}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(0);

	test_00();
	test_unique();

    CWT_END_TESTS;
}
