/**
 * @file   test.cpp
 * @author
 * @date
 *
 * @brief testing ...
 */

#include <pfs/test/test.hpp>
#include <pfs/thread.hpp>
#include <pfs/atomic.hpp>
#include <pfs/active_queue.hpp>
#include <iostream>

namespace test {
namespace active_queue {
namespace consumer_producer {

using std::cout;
using std::endl;

typedef pfs::active_queue_st<void> active_queue_type;

static active_queue_type callbackq;
static const int Count = 10000;
static const int ProducerCount = 4;
static pfs::atomic_int quit_flag(ProducerCount);
static pfs::atomic_int counter(0);

void func1 ()
{
    ++counter;
}

class producer_thread : public pfs::thread
{
public:
    producer_thread () : pfs::thread() {}

    virtual void run ()
    {
        for (int i = 0; i < Count; ++i) {
            while (!callbackq.push_function(& func1)) ;
        }

        --quit_flag;
    }
};

class consumer_thread : public pfs::thread
{
public:
    consumer_thread () : pfs::thread() {}

    virtual void run ()
    {
        while (not (quit_flag.load() == 0 && callbackq.empty())) {
            callbackq.call_all();
        }
    }
};


void test ()
{
    ADD_TESTS(1)
    producer_thread producers[ProducerCount];
    consumer_thread consumer;

    consumer.start();

    for (int i = 0; i < ProducerCount; ++i) {
        producers[i].start();
    }

    consumer.wait();

    for (int i = 0; i < ProducerCount; ++i) {
        producers[i].wait();
    }

    TEST_OK(counter.load() == Count * ProducerCount);
}

}}} // test::active_queue::consumer_producer

