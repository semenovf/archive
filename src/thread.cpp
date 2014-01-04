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

namespace cwt {

void thread::setPriority (thread::priority_type priority)
{
	_pimpl->setPriority(priority);
}

void thread::start (thread::priority_type priority, size_t stackSize)
{
	_pimpl->start(priority, stackSize);
}

void thread::terminate ()
{
	_pimpl->terminate();
}

bool thread::wait (ulong_t timeout)
{
	return _pimpl->wait(timeout);
}

void thread::sleep(ulong_t secs)
{
    thread::impl::sleep(secs);
}

void thread::msleep(ulong_t msecs)
{
	thread::impl::msleep(msecs);
}

void thread::usleep(ulong_t usecs)
{
	thread::impl::usleep(usecs);
}

} // cwt
