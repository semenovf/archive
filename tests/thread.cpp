/*
 * time.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: wladt
 */

#include <pfs/test/test.hpp>
#include "pfs/thread.hpp"
#include "pfs/random.hpp"
#include <pfs/string.hpp>
#include <pfs/safeformat.hpp>
#include <iostream>
#include <cstdlib>

//#include <QThread>

class TestThread : public pfs::thread
{
public:
	TestThread() : pfs::thread(),  m_name(pfs::string("unknown")) {}
	TestThread(const pfs::string & name) : m_name(name) {}

	virtual void run()
	{
		PFS_ASSERT(isRunning());

		pfs::random r (12345/*time(nullptr)*/);
		int t = r.rand();
		t = t < 1000000
				? 1000000
				: t > 3000000 ? 3000000 : t;

		std::cout << "pfs::thread [" << m_name.c_str() << _Tr("]: Sleeping for ") << t << " microseconds" << std::endl;
		msleep(500);
	}
private:
	pfs::string m_name;
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
	TEST_FAIL(threads);

	// Initialize threads
	for(int i = 0; i < nthreads; ++i) {
		threads[i] = new TestThread(pfs::safeformat("Thread_%04d")(i)());
		PFS_ASSERT(threads[i]);
	}

	for(int i = 0; i < nthreads; ++i) {
		if (threads[i]) {
			threads[i]->start(pfs::thread::InheritPriority);
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
	struct X : public pfs::thread
	{
		bool finished;
		X() : pfs::thread(), finished(false) {}

		virtual void run ()
		{
			sleep(2);
			finished = true;
		}
	};

	struct Y : public pfs::thread
	{
		bool finished;
		Y() : pfs::thread(), finished(false) { }

		virtual ~Y ()
		{}

		virtual void run ()
		{
			sleep(5);
			finished = true;
		}
	};

	struct Z : public pfs::thread
	{
		bool finished;
		Z() : pfs::thread(), finished(false) {}

		virtual void run ()
		{
			sleep(0);
			finished = true;
		}
	};

//	struct Q : public QThread
//	{
//		bool finished;
//		Q() : QThread(), finished(false) {}
//
//		virtual void run ()
//		{
//			sleep(5);
//			finished = true;
//		}
//	};

	X x;
	x.start();
	TEST_OK(x.wait(5000));  // ok => timeout > thread's execution time
	TEST_OK(x.finished);

	Y y;
	y.start();
	TEST_OK(!y.wait(2000)); // nok => timeout < thread's execution time
	TEST_OK(!y.finished);
	y.terminate();

	Z z;
	z.start();
	TEST_OK(z.wait()); // ok => thread's routine finished;
	TEST_OK(z.finished);

	Y y0;
	y0.start();
	//pfs::thread::sleep(1);
	y0.terminate();
	y0.wait();
	TEST_FAIL(y0.isFinished());

//	Q q;
//	q.start();
//	q.terminate();
//	q.wait();
//	TEST_FAIL(q.isFinished());

//  This code may abort the program
//	Y y1;
//	y1.start();
//	y1.terminate();

	Z z0;

//	Z z1;
//	z1.start(); // Will warn: "Attempt to destroy thread while it is still running"
//	            // at program exit

	Z z2;
	z2.start();
	z2.wait();
}

int main(int argc, char *argv[])
{
    PFS_UNUSED2(argc, argv);
	BEGIN_TESTS(8);

	do {
		if (1) test_thread();
		if (1) test_threads(350);
		if (1) test_wait_timeout();
	} while (argc > 1);

    END_TESTS; // exits with pfs::thread::exit
}




