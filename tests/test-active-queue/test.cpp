/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include <pfs/active_queue.hpp>
#include <pfs/safeformat.hpp>

#include <iostream>

using std::cout;
using std::endl;

namespace test0 {

typedef pfs::active_queue<2048, void> active_queue_type;
static int counter = 0;

void func ()
{
    ++counter;
}

void test ()
{
    ADD_TESTS(1);
    
    active_queue_type q;
    size_t max_count = 100;
    
    for (size_t i = 0; i < max_count; ++i) {
        if (! q.push_function(& func)) {
            break;
        }
    }
    
    max_count = q.count();
    q.call_all();
    
    TEST_OK2(max_count > 0 && counter == max_count
            , _Sf("counter(%u) == max_count(%u)")(counter)(max_count).str().c_str());
}

}

namespace test1 {

struct A 
{
    static size_t max_count;
    size_t counter;
    
    A () : counter(0) {}
    
    void method ()
    {
        ++counter;
    }
};

typedef pfs::active_queue<2048, void> active_queue_type;
size_t A::max_count = 100;

void test ()
{
    ADD_TESTS(1);
    
    active_queue_type q;
    A a;
    
    for (size_t i = 0; i < A::max_count; ++i) {
        if (! q.push_method(& a, & A::method)) {
            break;
        }
    }
    
    A::max_count = q.count();
    q.call_all();
    
    TEST_OK2(A::max_count > 0 && a.counter == A::max_count
            , _Sf("counter(%u) == max_count(%u)")(a.counter)(A::max_count).str().c_str());
}

}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

    test0::test();
    test1::test();
    
	return END_TESTS;
}

