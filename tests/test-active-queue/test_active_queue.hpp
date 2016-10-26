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
#include <pfs/thread.hpp>
#include <pfs/active_queue.hpp>

#include <iostream>

using std::cout;
using std::endl;

namespace test {
namespace active_queue {

typedef pfs::active_queue_mt<void> active_queue_type;
active_queue_type q;

static pfs::atomic_int is_finish(0);
static pfs::atomic_int result(0);

void func1 ()
{
    pfs::thread::msleep(100);
}

void func2 ()
{
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


void test4 ()
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
    
    TEST_OK2(result == 0, "'pfs::active_queue_mt<void>' test is OK");
}

}}

#endif /* __TEST_ACTIVE_QUEUE_HPP__ */

