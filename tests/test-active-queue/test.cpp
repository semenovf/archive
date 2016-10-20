/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs.hpp>
#include <pfs/random.hpp>
#include <pfs/active_queue.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/thread.hpp>
#include <pfs/mutex.hpp>

#include <iostream>

using std::cout;
using std::endl;

struct null_ostream
{
    template <typename T>
    null_ostream & operator << (T a) { return *this; }
};

static null_ostream nout;

#define COUT nout
//#define COUT cout

namespace test0 {

typedef pfs::active_queue_st<void> active_queue_type;
static int counter = 0;

void func ()
{
    ++counter;
}

void test ()
{
    ADD_TESTS(2);
    active_queue_type q(2028, 2048, 10);
    active_queue_type q1(pfs::max_value<size_t>());
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
    
    max_count = 100;
    
    for (size_t i = 0; i < max_count; ++i) {
        if (! q1.push_function(& func)) {
            break;
        }
    }
    
    TEST_OK2(q1.count() == max_count
        , _Sf("q1.count()=> %u == %u")(q1.count())(max_count).str().c_str());
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

typedef pfs::active_queue_st<void> active_queue_type;
size_t A::max_count = 100;

void test ()
{
    ADD_TESTS(2);
    
    active_queue_type q(2048, 2048, 1);
    active_queue_type q1(pfs::max_value<size_t>());
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

    A::max_count = 100;
    
    for (size_t i = 0; i < A::max_count; ++i) {
        if (! q1.push_method(& a, & A::method)) {
            break;
        }
    }
    
    TEST_OK2(q1.count() == A::max_count
        , _Sf("q1.count()=> %u == %u")(q1.count())(A::max_count).str().c_str());
}

}

namespace test2 {

typedef pfs::active_queue_st<void> active_queue_type;
active_queue_type q(pfs::max_value<size_t>());

static int counter = 0;

void func1 ()
{
    ++counter;
}

void func2 (int i)
{
    COUT << "func2(" << i << ")\n";
}

void func3 (int a, char b)
{
    COUT << "func3(" << a << ", " << b << ")\n";
}

void test ()
{
    pfs::random rnd;
    size_t limit = double(rnd.rand())/double(pfs::max_value<uint32_t>()) * 100;
    
    int max = pfs::max_value<uint16_t>();
    
    for (int i = 0; i < max; ++i) {
        //COUT << "Push: " << i << endl;
        if (! q.push_function(& func1)) {
            break;
        }
        
        if (! q.push_function(& func2, i)) {
            break;
        }

        if (! q.push_function(& func3, i, 'W')) {
            break;
        }
        
        if (q.count() > limit) {
            q.call_all();
            limit = double(rnd.rand())/double(pfs::max_value<uint32_t>()) * 100;
        }
    }
    
    q.call_all();
}

}

namespace test3 {

typedef pfs::active_queue_mt<void> active_queue_type;
active_queue_type q(0, pfs::max_value<size_t>(), 1);
//active_queue_type q(100000);

static int counter = 0;
static bool is_finish = 0;

class thread : public pfs::thread
{
public:
	thread () 
        : pfs::thread()
    {}

	virtual void run()
	{
		while (! is_finish) {
            q.call_all();
        }
	}
};

void func1 ()
{
    ++counter;
}

void func2 (int i)
{
    COUT << "func2(" << i << ")\n";
}

void func3 (int a, char b)
{
    COUT << "func3(" << a << ", " << b << ")\n";
}

void finish ()
{
    COUT << "Finish\n";
    is_finish = true;
}

void test ()
{
    ADD_TESTS(1);
    
    bool ok = true;
    thread thr;
    thr.start();
    
    for (int i = 0; i < 300000; ++i) {
        COUT << "Push: " << i << "\n";
        
        if (! q.push_function(& func1)) {
            ok = false;
            break;
        }
        
        if (! q.push_function(& func2, i)) {
            ok = false;
            break;
        }

        if (! q.push_function(& func3, i, 'W')) {
            ok = false;
            break;
        }
    }
    
    if (ok) {
        if (!q.push_function(& finish))
            thr.terminate();
        else
            thr.wait();
    } else {
        thr.terminate();
    }
    
    TEST_OK2(ok, "'pfs::active_queue_mt<void>' test is OK");
}

}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

    test0::test();
    test1::test();
    test2::test();
    test3::test();
    
	return END_TESTS;
}

