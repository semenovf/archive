/*
 * thread.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#include "thread_p.hpp"
#include "../include/cwt/logger.hpp"

// Sources:
// * POSIX Threads Programming
//	https://computing.llnl.gov/tutorials/pthreads/
// * Завершение работы нитей
//	http://pic.dhe.ibm.com/infocenter/aix/v6r1/topic/com.ibm.aix.genprogc/doc/genprogc/term_threads.htm#term_threads__cleanup

CWT_NS_BEGIN

void Thread::setPriority (Thread::Priority priority)
{
	pimpl->setPriority(priority);
}

void Thread::start (Priority priority, size_t stackSize)
{
	pimpl->start(priority, stackSize);
}

void Thread::terminate ()
{
	pimpl->terminate();
}

bool Thread::wait (ulong_t timeout)
{
	return pimpl->wait(timeout);
}

void Thread::sleep(ulong_t secs)
{
    Thread::Impl::sleep(secs);
}

void Thread::msleep(ulong_t msecs)
{
	Thread::Impl::msleep(msecs);
}

void Thread::usleep(ulong_t usecs)
{
	Thread::Impl::usleep(usecs);
}

CWT_NS_END
