/*
 * thread.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#include "thread_p.hpp"

CWT_NS_BEGIN

Thread::Thread() : pimpl(new Thread::Impl(this)) { }

bool Thread::isFinished () const
{
	AutoLock<>(& pimpl->m_mutex);
	return pimpl->isFinished();
}
bool Thread::isRunning () const
{
	AutoLock<>(& pimpl->m_mutex);
	return pimpl->isRunning();
}

Thread::Priority Thread::priority () const
{
	AutoLock<>(& pimpl->m_mutex);
	return pimpl->priority();
}

void Thread::setPriority (Thread::Priority priority)
{
	pimpl->setPriority(priority);
}

size_t Thread::stackSize () const
{
	AutoLock<>(& pimpl->m_mutex);
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
    pimpl->sleep(secs);
}

void Thread::msleep(ulong_t msecs)
{
	pimpl->msleep(msecs);
}

void Thread::usleep(ulong_t usecs)
{
	pimpl->usleep(usecs);
}

//void	 Thread::exit (int returnCode = 0)
//void	 Thread::quit ();

CWT_NS_END
