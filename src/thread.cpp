/*
 * thread.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#include "thread_p.hpp"
#include "../include/cwt/logger.hpp"

CWT_NS_BEGIN

Thread::Thread() : pimpl(new Thread::Impl(this)) { }

Thread::~Thread()
{
	CWT_TRACE("Thread::~Thread()");
}

bool Thread::isFinished () const
{
	return pimpl->isFinished();
}

bool Thread::isRunning () const
{
	return pimpl->isRunning();
}

Thread::Priority Thread::priority () const
{
	return pimpl->priority();
}

void Thread::setPriority (Thread::Priority priority)
{
	pimpl->setPriority(priority);
}

size_t Thread::stackSize () const
{
	return pimpl->stackSize();
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
