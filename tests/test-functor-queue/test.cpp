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
#include <pfs/functor_queue.hpp>
#include <iostream>

using std::cout;
using std::endl;

typedef pfs::functor_queue<195, void, pfs::mutex> CallbackQueue;

CallbackQueue callbackQueue;
static const int Count = 10000;
static const int ProducerCount = 4;
pfs::atomic_int quit_flag(ProducerCount);
pfs::atomic_int counter(0);

void func1 ()
{
	counter.ref();
}

class producer_thread : public pfs::thread
{
public:
	producer_thread () : pfs::thread() {}

	virtual void run ()
	{
		for (int i = 0; i < Count; ++i) {
			while (!callbackQueue.push(& func1)) ;
		}

		quit_flag.deref();
	}
};

class consumer_thread : public pfs::thread
{
public:
	consumer_thread () : pfs::thread() {}

	virtual void run ()
	{
		while (not (quit_flag.load() == 0 && callbackQueue.empty())) {
			callbackQueue.call_all();
		}
	}
};


int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(1);

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
	return END_TESTS;
}

