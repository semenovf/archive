/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_active_queue.hpp
 * Author: wladt
 *
 * Created on October 26, 2016, 4:19 PM
 */

#ifndef __TEST_ACTIVE_QUEUE_HPP__
#define __TEST_ACTIVE_QUEUE_HPP__

#include <pfs/test/test.hpp>
#include <pfs/safeformat.hpp>
#include <pfs/thread.hpp>
#include <pfs/random.hpp>
#include <pfs/active_queue.hpp>

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
//#define COUT std::cout

namespace test {
namespace active_queue {

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
    
    active_queue_type q1(pfs::max_value<size_t>());
    
    for (size_t i = 0; i < max_count; ++i) {
        if (! q1.push_function(& func)) {
            break;
        }
    }
    
    TEST_OK2(q1.count() == max_count
        , _Sf("q1.count()=> %u == %u")(q1.count())(max_count).str().c_str());
}

} // test0

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

} // test1


namespace test2 {

typedef pfs::active_queue_st<void> active_queue_type;
active_queue_type q(pfs::max_value<size_t>());

static int counter1 = 0;
static int counter2 = 0;
static int counter3 = 0;

void func1 ()
{
    ++counter1;
}

void func2 (int i)
{
    COUT << "func2(" << i << ")\n";
    ++counter2;
}

void func3 (int a, char b)
{
    COUT << "func3(" << a << ", " << b << ")\n";
    ++counter3;
}

void test ()
{
    ADD_TESTS(1);
    
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
    
    TEST_OK2(counter1 == max
                && counter2 == max
                && counter3 == max
            , "test::active_queue::test2 is ok");
}

} // test2


namespace test3 {

typedef pfs::active_queue_mt<void> active_queue_type;
active_queue_type q(0, pfs::max_value<size_t>(), 1);
//active_queue_type q(100000);

static pfs::atomic_int is_finish(0);
static pfs::atomic_int result(0);

static pfs::atomic_size_t prod1_counter(0);
static pfs::atomic_size_t prod2_counter(0);
static pfs::atomic_size_t prod3_counter(0);

static pfs::atomic_size_t cons1_counter(0);
static pfs::atomic_size_t cons2_counter(0);
static pfs::atomic_size_t cons3_counter(0);

void func1 ()
{
    ++cons1_counter;
}

void func2 (int i)
{
    COUT << "func2(" << i << ")\n";
    ++cons2_counter;
}

void func3 (int a, char b)
{
    COUT << "func3(" << a << ", " << b << ")\n";
    ++cons3_counter;
}

class consumer : public pfs::thread
{
public:
	consumer () 
        : pfs::thread()
    {}

	virtual void run()
	{
		while (! is_finish) {
            q.call_all();
        }
	}
};

class producer1 : public pfs::thread
{
public:
	producer1 () 
        : pfs::thread()
    {}

	virtual void run()
	{
		while (! is_finish) {
            if (! q.push_function(& func1)) {
                result |= 0x0001;
                break;
            }
            ++prod1_counter;
        }
	}
};

class producer2 : public pfs::thread
{
public:
	producer2 () 
        : pfs::thread()
    {}

	virtual void run()
	{
        int i = 0;
        
		while (! is_finish) {
            if (! q.push_function(& func2, i++)) {
                result |= 0x0002;
                break;
            }
            ++prod2_counter;
        }
	}
};

class producer3 : public pfs::thread
{
public:
	producer3 () 
        : pfs::thread()
    {}

	virtual void run()
	{
        int i = 0;
        
		while (! is_finish) {
            if (! q.push_function(& func3, i++, 'W')) {
                result |= 0x0004;
                break;
            }
            ++prod3_counter;
        }
	}
};


void test ()
{
    ADD_TESTS(4);
    
    consumer cons;
    producer1 prod1;
    producer2 prod2;
    producer3 prod3;
    
    cons.start();
    prod1.start();
    prod2.start();
    prod3.start();
    
    pfs::thread::sleep(2);
    
    is_finish = 1;
    
    cons.wait();
    prod1.wait();
    prod2.wait();
    prod3.wait();
    
    if (result != 0) {
        cout << "result = 0x" << std::hex << result.load() << endl;
    }
    
    TEST_OK2(result == 0
                && cons1_counter == prod1_counter
                && cons2_counter == prod2_counter
                && cons3_counter == prod3_counter
            , "test::active_queue::test3 is ok");
    
//    cout << "cons1_counter(" << cons1_counter.load() << ") == "
//         << "prod1_counter(" << prod1_counter.load() << ")" << endl;
//
//    cout << "cons2_counter(" << cons2_counter.load() << ") == "
//         << "prod2_counter(" << prod2_counter.load() << ")" << endl;
//
//    cout << "cons3_counter(" << cons3_counter.load() << ") == "
//         << "prod3_counter(" << prod3_counter.load() << ")" << endl;
}

} // test3

namespace test4 {

typedef pfs::active_queue_mt<void> active_queue_type;
static active_queue_type q;

static pfs::atomic_int is_finish(0);
static pfs::atomic_int result(0);

void func1 ()
{
    //pfs::thread::msleep(10);
}

void func2 ()
{
    //pfs::thread::msleep(100);
    q.push_function(& func1);
}

class consumer : public pfs::thread
{
public:
	consumer () 
        : pfs::thread()
    {}

	virtual void run()
	{
		while (! is_finish) {
            q.call_all();
        }
	}
};

class producer : public pfs::thread
{
public:
	producer () 
        : pfs::thread()
    {}

	virtual void run()
	{
		while (! is_finish) {
            if (! q.push_function(& func2)) {
                result |= 0x0001;
                break;
            }
        }
	}
};


void test ()
{
    ADD_TESTS(1);
    
    consumer cons;
    producer prod;
    
    cons.start();
    prod.start();
    
    pfs::thread::sleep(3);
    
    is_finish = 1;
    
    cons.wait();
    prod.wait();
    
    if (result != 0) {
        cout << "result = 0x" << std::hex << result.load() << endl;
    }
    
    TEST_OK2(result == 0
            , "test::active_queue::test4 is ok");
}

} // test4

}} //test::active_queue

#endif /* __TEST_ACTIVE_QUEUE_HPP__ */
