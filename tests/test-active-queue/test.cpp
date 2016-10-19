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
#include <pfs/active_queue_1.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/thread.hpp>
#include <pfs/mutex.hpp>

#include <iostream>

using std::cout;
using std::endl;

namespace test0 {

typedef pfs::active_queue<void> active_queue_type;
static int counter = 0;

void func ()
{
    ++counter;
}

void test ()
{
    ADD_TESTS(2);
    active_queue_type q(2028, 2048, 10);
    active_queue_type q1;
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

#if __COMMENT__

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

typedef pfs::active_queue<void> active_queue_type;
size_t A::max_count = 100;

void test ()
{
    ADD_TESTS(2);
    
    active_queue_type q(2048, 2048, 1);
    active_queue_type q1;
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

#endif

namespace test2 {

typedef pfs::active_queue<void, pfs::mutex> active_queue_type;
active_queue_type q;

static int counter = 0;

void func1 ()
{
    ++counter;
}

void func2 (int i)
{
    cout << "func2(" << i << ')' << endl;
}

void func3 (int a, char b)
{
    cout << "func3(" << a << ", " << b << ')' << endl;
}

void test ()
{
    pfs::random rnd;
    size_t limit = double(rnd.rand())/double(pfs::max_value<uint32_t>()) * 1000;
    
    int max = pfs::max_value<int>();
    
    for (int i = 0; i < max; ++i) {
        cout << "Push: " << i << endl;
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
            limit = double(rnd.rand())/double(pfs::max_value<uint32_t>()) * 1000;
        }
    }
}

}

namespace test3 {

typedef pfs::active_queue<void, pfs::mutex> active_queue_type;
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
    cout << "func2(" << i << ')' << endl;
}

void func3 (int a, char b)
{
    cout << "func3(" << a << ", " << b << ')' << endl;
}

void finish ()
{
    cout << "Finish" << endl;
    is_finish = true;
}

void test ()
{
    bool ok = true;
    thread thr;
    thr.start();
    
    for (int i = 0; i < 300000; ++i) {
        cout << "Push: " << i << endl;
        
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
}

}

namespace test4 {

typedef pfs::details::ring_queue<> ring_queue_type;

static int const magic_number = 0xDEADBEAF;

struct A 
{
    size_t magic;
    A () : magic(magic_number) {}
    virtual ~A () { magic = 0xDEADDEAD; }
};

struct B : public A
{
    int b;
    B (int x) : A(), b(x) {}
};

struct C : public B
{
    char c;
    C (int x, char y) : B(x), c(y) {}
};

struct D : public C
{
    double d;
    D (int x, char y, double z) : C(x, y), d(z) {}
};

void test ()
{
    ADD_TESTS(10);
    pfs::fake_mutex mutex;
    ring_queue_type rq(mutex);
    
    TEST_OK(rq.push<A>());
    TEST_OK(rq.push<B>(314159));
    TEST_OK(rq.push<C>(314159, 'W'));
    TEST_OK(rq.push<D>(314159, 'W', 3.14159f));
    
    TEST_OK(rq.count() == 4);
    
    TEST_OK(rq.front<A>().magic == magic_number);
    rq.pop<A>();

    TEST_OK(rq.front<B>().magic == magic_number
            && rq.front<B>().b == 314159);
    rq.pop<B>();

    TEST_OK(rq.front<C>().magic == magic_number
            && rq.front<C>().b == 314159
            && rq.front<C>().c == 'W');
    rq.pop<C>();

    TEST_OK(rq.front<D>().magic == magic_number
            && rq.front<D>().b == 314159
            && rq.front<D>().c == 'W'
            && rq.front<D>().d == 3.14159f);
    rq.pop<D>();
    
    TEST_OK(rq.count() == 0);
}

}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(0);

//    test0::test();
//    test1::test();
//    test2::test();
    test3::test();
//    test4::test();
    
	return END_TESTS;
}

