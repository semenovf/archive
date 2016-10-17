/**
 * @file atomic.cpp
 * @author wladt
 * @date Mar 17, 2014
 */

#include <pfs/test/test.hpp>
#include <pfs/atomic.hpp>
#include <pfs/thread.hpp>
#include <iostream>

static int nonAtomicCounter = 0;
static pfs::atomic_int counter;       // forward counter
static pfs::atomic_int backCounter;   // backward counter
static pfs::atomic_int threadCounter; // thread counter
static pfs::atomic_int sw0;
static pfs::atomic_int sw1;
static const int __niters = 1000000;
static const int __nthreads = 50;
static const int __totalCounter = __niters * __nthreads;

class TestThread : public pfs::thread
{
public:
	virtual void run()
	{
		++threadCounter;
        
		for (int i = 0; i < __niters; ++i) {
			++counter;
			--backCounter;
			++nonAtomicCounter;
		}
	}
};

/*
class AtomicSwapThread : public pfs::thread
{
public:
	virtual void run()
	{
		for (int i = 0; i < __niters; ++i) {
			sw0.swap(sw1);
		}
	}
};
*/


void test_atomic_ref ()
{
	TestThread ** threads = new TestThread * [__nthreads];

	nonAtomicCounter = 0;
	threadCounter = 0;
	counter = 0;
	backCounter = __totalCounter;

	// Initialize threads
	for(int i = 0; i < __nthreads; ++i) {
		threads[i] = new TestThread();
		PFS_ASSERT(threads[i]);
	}

	for(int i = 0; i < __nthreads; ++i) {
		if (threads[i]) {
			threads[i]->start(pfs::thread::inherit_priority);
		}
	}

	for(int i = 0; i < __nthreads; ++i) {
		if (threads[i] && threads[i]->is_running()) {
			threads[i]->wait();
		}
	}

	TEST_OK(threadCounter.load() == __nthreads);
	TEST_OK(counter.load() == __totalCounter);
	TEST_OK(backCounter.load() == 0);

	if (nonAtomicCounter == __totalCounter) {
		std::cout << "Non atomic counter is equal to atomic counter: " << nonAtomicCounter << "==" << counter.load() << std::endl;
	} else {
		std::cout << "Non atomic counter is not equal to atomic counter: " << nonAtomicCounter << "!=" << counter.load() << std::endl;
	}
}

/*
void test_atomic_swap ()
{
	AtomicSwapThread ** threads = new AtomicSwapThread * [__nthreads];

	sw0.store(123);
	sw1.store(321);

	// Initialize threads
	for(int i = 0; i < __nthreads; ++i) {
		threads[i] = new AtomicSwapThread();
		PFS_ASSERT(threads[i]);
	}

	for(int i = 0; i < __nthreads; ++i) {
		if (threads[i]) {
			threads[i]->start(pfs::thread::InheritPriority);
		}
	}

	for(int i = 0; i < __nthreads; ++i) {
		if (threads[i] && threads[i]->isRunning()) {
			threads[i]->wait();
		}
	}

	if (__totalCounter % 2 == 0) { // even
		TEST_OK(sw0.load() == 123);
		TEST_OK(sw1.load() == 321);
	} else { // odd
		TEST_OK(sw0.load() == 321);
		TEST_OK(sw1.load() == 123);
	}
}
*/

int main (int argc, char *argv[])
{
	PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(3);

	test_atomic_ref();
//	test_atomic_swap ();

	return END_TESTS;
}



