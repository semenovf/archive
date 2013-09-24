/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/string.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/thread.hpp>
#include <cwt/random.hpp>
#include <iostream>
#include <cstdlib>

using namespace cwt;

class TestThread : public Thread
{
public:
	TestThread() : Thread(),  m_name(String("unknown")) {}
	TestThread(const String & name) : m_name(name) {}

	virtual void run()
	{
		CWT_ASSERT(isRunning());

		Random r (12345/*time(nullptr)*/);
		int t = r.random();
		t = t < 1000000
				? 1000000
				: t > 3000000 ? 3000000 : t;

		std::cout << "Thread [" << m_name.c_str() << _Tr("]: Sleeping for ") << t << " microseconds" << std::endl;
		usleep(t);
	}
private:
	String m_name;
};

void test_thread ()
{
	TestThread thread;
	thread.start();
	thread.wait();
}

void test_threads(int nthreads)
{
	TestThread ** threads = new TestThread * [nthreads];

	// Initialize threads
	for(int i = 0; i < nthreads; ++i) {
		threads[i] = new TestThread(_F("Thread_%04d") % i);
		CWT_ASSERT(threads[i]);
	}

	for(int i = 0; i < nthreads; ++i) {
		if (threads[i]) {
			threads[i]->start(Thread::InheritPriority);
		}
	}

	for(int i = 0; i < nthreads; ++i) {
		if (threads[i] && threads[i]->isRunning()) {
			threads[i]->wait();
		}
	}


	for(int i = 0; i < nthreads; ++i) {
		if (threads[i])
			delete threads[i];
	}
	delete[] threads;
}


void test_wait_timeout ()
{
	struct X : public Thread {
		virtual void run ()
		{
			sleep(2);
		}
	};

	struct Y : public Thread {
		virtual void run ()
		{
			sleep(5);
		}
	};

	X x;
	x.start();
	CWT_TEST_OK(x.wait(5000000000));

	Y y;
	y.start();
	CWT_TEST_NOK(y.wait(2000000000));
}

int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(4);

	if (0) test_thread();
	if (0) test_threads(350);
	if (1) test_wait_timeout();

/*
	for (int i = 0; i < 10000; ++i) {
		Random r (time(nullptr));
		r.srandom(time(nullptr));
		int t = r.random();
		printf("random = %d\n", t);

	}
*/

    CWT_END_TESTS;
}




