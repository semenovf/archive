/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <cwt/test.h>
#include <cwt/string.hpp>
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
		Random r (time(nullptr));
		int t = r.random();
		std::cout << "Thread [" << m_name.c_str() << _Tr("]: Sleeping for ") << t << " nanoseconds" << std::endl;
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
		threads[i] = new TestThread();
	}

	for(int i = 0; i < nthreads; ++i) {
		threads[i]->start();
	}

	for(int i = 0; i < nthreads; ++i) {
		threads[i]->wait();
	}


	delete[] threads;
}


int main(int argc, char *argv[])
{
    CWT_CHECK_SIZEOF_TYPES;
    CWT_UNUSED2(argc, argv);
	CWT_BEGIN_TESTS(4);

	test_thread();
	if (0) test_threads(10);

    CWT_END_TESTS;
}




