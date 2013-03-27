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
//
// This unit tests base on BOOST's shared_ptr unit tests.
//
struct Foo
{
	Foo(int aVal) :
        m_val(aVal)
    {
        ++g_instances;
    }
    ~Foo(void)
    {
        --g_instances;
    }
    void incr(void)
    {
        ++m_val;
    }
    void decr(void)
    {
        --m_val;
    }

    int m_val;
    static int g_instances;
};

int Foo::g_instances = 0;


void test_empty_ptr()
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Foo> xPtr;

    CWT_TEST_OK(false == xPtr);
    CWT_TEST_OK(false == xPtr.unique());
    CWT_TEST_OK(0 == xPtr.use_count());
    CWT_TEST_OK(NULL == xPtr.get());

    if (xPtr) {
        CWT_ERROR("bool cast operator error");
    }

    // Reset to NULL (ie. do nothing)
    xPtr.reset();

    CWT_TEST_OK(false == xPtr);
    CWT_TEST_OK(false == xPtr.unique());
    CWT_TEST_OK(0 == xPtr.use_count());
    CWT_TEST_OK(NULL == xPtr.get());

    // sub-scope
    {
        // Copy construct the empty (ie. NULL) shared_ptr
        shared_ptr<Foo> yPtr(xPtr);

        CWT_TEST_OK(false == xPtr);
        CWT_TEST_OK(false == xPtr.unique());
        CWT_TEST_OK(0 == xPtr.use_count());
        CWT_TEST_OK(NULL == xPtr.get());
        CWT_TEST_OK(false == yPtr.unique());
        CWT_TEST_OK(0 == yPtr.use_count());
        CWT_TEST_OK(NULL == yPtr.get());

        // Assign the empty (ie. NULL) shared_ptr
        shared_ptr<Foo> zPtr;
        zPtr = xPtr;

        CWT_TEST_OK(false == xPtr);
        CWT_TEST_OK(false == xPtr.unique());
        CWT_TEST_OK(0 == xPtr.use_count());
        CWT_TEST_OK(NULL == xPtr.get());
        CWT_TEST_OK(false == zPtr.unique());
        CWT_TEST_OK(0 == zPtr.use_count());
        CWT_TEST_OK(NULL == zPtr.get());
    }
    // end of scope

    CWT_TEST_OK(false == xPtr);
    CWT_TEST_OK(false == xPtr.unique());
    CWT_TEST_OK(0 == xPtr.use_count());
    CWT_TEST_OK(NULL == xPtr.get());
}

void test_basic_ptr()
{
    {
        // Create a shared_ptr
        shared_ptr<Foo> xPtr(new Foo(123));

        CWT_TEST_OK(true == xPtr);
        CWT_TEST_OK(true == xPtr.unique());
        CWT_TEST_OK(1 == xPtr.use_count());
        CWT_TEST_OK(NULL != xPtr.get());

        if (xPtr)
        {
            CWT_TEST_OK(123 == xPtr->m_val);
            CWT_TEST_OK(1 == xPtr->g_instances);
            CWT_TEST_OK(1 == Foo::g_instances);

            // call a function
            xPtr->incr();
            CWT_TEST_OK(124 == xPtr->m_val);
            (*xPtr).incr();
            CWT_TEST_OK(125 == (*xPtr).m_val);
            xPtr->decr();
            xPtr->decr();

            // Copy construct the shared_ptr
            shared_ptr<Foo> yPtr(xPtr);

            CWT_TEST_OK(xPtr == yPtr);
            CWT_TEST_OK(true == xPtr);
            CWT_TEST_OK(false == xPtr.unique());
            CWT_TEST_OK(2 == xPtr.use_count());
            CWT_TEST_OK(NULL != xPtr.get());
            CWT_TEST_OK(123 == xPtr->m_val);
            CWT_TEST_OK(1 == Foo::g_instances);
            CWT_TEST_OK(true == yPtr);
            CWT_TEST_OK(false == yPtr.unique());
            CWT_TEST_OK(2 == yPtr.use_count());
            CWT_TEST_OK(NULL != yPtr.get());
            CWT_TEST_OK(123 == yPtr->m_val);
            CWT_TEST_OK(1 == Foo::g_instances);

            if (yPtr)
            {
                // Assign the shared_ptr
                shared_ptr<Foo> zPtr;
                zPtr = xPtr;

                CWT_TEST_OK(xPtr == zPtr);
                CWT_TEST_OK(true == xPtr);
                CWT_TEST_OK(false == xPtr.unique());
                CWT_TEST_OK(3 == xPtr.use_count());
                CWT_TEST_OK(NULL != xPtr.get());
                CWT_TEST_OK(123 == xPtr->m_val);
                CWT_TEST_OK(1 == Foo::g_instances);
                CWT_TEST_OK(true == yPtr);
                CWT_TEST_OK(false == yPtr.unique());
                CWT_TEST_OK(3 == yPtr.use_count());
                CWT_TEST_OK(NULL != yPtr.get());
                CWT_TEST_OK(123 == yPtr->m_val);
                CWT_TEST_OK(1 == Foo::g_instances);
                CWT_TEST_OK(true == zPtr);
                CWT_TEST_OK(false == zPtr.unique());
                CWT_TEST_OK(3 == zPtr.use_count());
                CWT_TEST_OK(NULL != zPtr.get());
                CWT_TEST_OK(123 == zPtr->m_val);
                CWT_TEST_OK(1 == Foo::g_instances);
            }

            CWT_TEST_OK(true == xPtr);
            CWT_TEST_OK(false == xPtr.unique());
            CWT_TEST_OK(2 == xPtr.use_count());
            CWT_TEST_OK(NULL != xPtr.get());
            CWT_TEST_OK(123 == xPtr->m_val);
            CWT_TEST_OK(1 == Foo::g_instances);
        }
        else
        {
            CWT_ERROR("bool cast operator error");
        }

        CWT_TEST_OK(true == xPtr);
        CWT_TEST_OK(true == xPtr.unique());
        CWT_TEST_OK(1 == xPtr.use_count());
        CWT_TEST_OK(NULL != xPtr.get());
        CWT_TEST_OK(123 == xPtr->m_val);
        CWT_TEST_OK(1 == Foo::g_instances);
    }

    CWT_TEST_OK(0 == Foo::g_instances);
}

void test_reset_ptr()
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Foo> xPtr;

    // Reset it with a new pointer
    xPtr.reset(new Foo(123));

    CWT_TEST_OK(true == xPtr);
    CWT_TEST_OK(true == xPtr.unique());
    CWT_TEST_OK(1 == xPtr.use_count());
    CWT_TEST_OK(NULL != xPtr.get());
    CWT_TEST_OK(123 == xPtr->m_val);
    CWT_TEST_OK(1 == Foo::g_instances);
    Foo* pX = xPtr.get();

    // Reset it with another new pointer
    xPtr.reset(new Foo(234));

    CWT_TEST_OK(true == xPtr);
    CWT_TEST_OK(true == xPtr.unique());
    CWT_TEST_OK(1 == xPtr.use_count());
    CWT_TEST_OK(NULL != xPtr.get());
    CWT_TEST_OK(234 == xPtr->m_val);
    CWT_TEST_OK(1 == Foo::g_instances);
    CWT_TEST_OK(pX != xPtr.get());

    // Reset to NULL
    xPtr.reset();

    CWT_TEST_OK(false == xPtr.unique());
    CWT_TEST_OK(0 == xPtr.use_count());
    CWT_TEST_OK(NULL == xPtr.get());
    CWT_TEST_OK(0 == Foo::g_instances);
}

void test_compare_ptr()
{
    // Create a shared_ptr
    shared_ptr<Foo> xPtr(new Foo(123));

    CWT_TEST_OK(true == xPtr);
    CWT_TEST_OK(true == xPtr.unique());
    CWT_TEST_OK(1 == xPtr.use_count());
    CWT_TEST_OK(NULL != xPtr.get());
    CWT_TEST_OK(123 == xPtr->m_val);
    CWT_TEST_OK(1 == Foo::g_instances);
    Foo* pX = xPtr.get();

    // Create another shared_ptr
    shared_ptr<Foo> yPtr(new Foo(234));

    CWT_TEST_OK(true == xPtr);
    CWT_TEST_OK(true == xPtr.unique());
    CWT_TEST_OK(1 == xPtr.use_count());
    CWT_TEST_OK(NULL != xPtr.get());
    CWT_TEST_OK(123 == xPtr->m_val);
    CWT_TEST_OK(2 == Foo::g_instances);

    CWT_TEST_OK(true == yPtr);
    CWT_TEST_OK(true == yPtr.unique());
    CWT_TEST_OK(1 == yPtr.use_count());
    CWT_TEST_OK(NULL != yPtr.get());
    CWT_TEST_OK(234 == yPtr->m_val);
    Foo* pY = yPtr.get();

    CWT_TEST_OK(xPtr != pY);
    CWT_TEST_OK(xPtr != yPtr);
    if (pX < pY)
    {
        CWT_TEST_OK(xPtr < pY);
        CWT_TEST_OK(xPtr < yPtr);
        CWT_TEST_OK(xPtr <= pY);
        CWT_TEST_OK(xPtr <= yPtr);
        CWT_TEST_OK(yPtr > pX);
        CWT_TEST_OK(yPtr > xPtr);
        CWT_TEST_OK(yPtr >= pX);
        CWT_TEST_OK(yPtr >= xPtr);
    }
    else // (pX > pY)
    {
        CWT_TEST_OK(xPtr > pY);
        CWT_TEST_OK(xPtr > yPtr);
        CWT_TEST_OK(xPtr >= pY);
        CWT_TEST_OK(xPtr >= yPtr);
        CWT_TEST_OK(yPtr < pX);
        CWT_TEST_OK(yPtr < xPtr);
        CWT_TEST_OK(yPtr <= pX);
        CWT_TEST_OK(yPtr <= xPtr);
    }

    // Copy a shared_ptr
    shared_ptr<Foo> zPtr = xPtr;
    Foo* pZ = zPtr.get();

    CWT_TEST_OK(pX == pZ);
    CWT_TEST_OK(xPtr == pZ);
    CWT_TEST_OK(xPtr == zPtr);
    CWT_TEST_OK(zPtr == pX);
    CWT_TEST_OK(zPtr == xPtr);
    CWT_TEST_OK(xPtr >= pZ);
    CWT_TEST_OK(xPtr >= zPtr);
    CWT_TEST_OK(xPtr <= pZ);
    CWT_TEST_OK(xPtr <= zPtr);
}

void test_swap_ptr()
{
    // Create a shared_ptr
    shared_ptr<Foo> xPtr(new Foo(123));

    CWT_TEST_OK(true == xPtr);
    CWT_TEST_OK(true == xPtr.unique());
    CWT_TEST_OK(1 == xPtr.use_count());
    CWT_TEST_OK(NULL != xPtr.get());
    CWT_TEST_OK(123 == xPtr->m_val);
    CWT_TEST_OK(1 == Foo::g_instances);
    Foo* pX = xPtr.get();

    // Create another shared_ptr
    shared_ptr<Foo> yPtr(new Foo(234));

    CWT_TEST_OK(true == yPtr);
    CWT_TEST_OK(true == yPtr.unique());
    CWT_TEST_OK(1 == yPtr.use_count());
    CWT_TEST_OK(NULL != yPtr.get());
    CWT_TEST_OK(234 == yPtr->m_val);
    CWT_TEST_OK(2 == Foo::g_instances);
    Foo* pY = yPtr.get();

    if (pX < pY)
    {
        CWT_TEST_OK(xPtr < yPtr);
        xPtr.swap(yPtr);
        CWT_TEST_OK(xPtr > yPtr);
        CWT_TEST_OK(true == xPtr);
        CWT_TEST_OK(true == yPtr);
    }
    else // (pX > pY)
    {
        CWT_TEST_OK(xPtr > yPtr);
        xPtr.swap(yPtr);
        CWT_TEST_OK(xPtr < yPtr);
        CWT_TEST_OK(true == xPtr);
        CWT_TEST_OK(true == yPtr);
    }
}


#ifdef __COMMENT__
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

void test_00()
{
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
#endif

void test_unique()
{
    shared_ptr<int> sp1(new int(5));
    CWT_TEST_OK(sp1.unique() == true);

    shared_ptr<int> sp2(sp1);
    CWT_TEST_OK(sp1.unique() == false);

}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(151);

	test_empty_ptr();
	test_basic_ptr();
	test_reset_ptr();
	test_compare_ptr();
	test_swap_ptr();
	test_unique();

//	test_00();

	CWT_END_TESTS;
}
