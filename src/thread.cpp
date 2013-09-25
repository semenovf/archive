/*
 * thread.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#include "thread_p.hpp"

CWT_NS_BEGIN

Thread::Thread() : pimpl(new Thread::Impl(this)) { }

Thread::~Thread()
{
	CWT_TRACE("Thread::~Thread()");

    AutoLock<> locker(& pimpl->m_mutex);
    if (pimpl->isFinishing()) {
        locker.handlePtr()->unlock();
        wait();
        locker.handlePtr()->tryLock();
    }

    if (! isFinished()) {
    	CWT_SYS_WARN(_Tr("Attempt to destroy thread while it is still running"));
    }

    pimpl->m_threadDataPtr->m_thread = nullptr; // detach from Thread instance
}

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

//void	 Thread::exit (int returnCode = 0)
//void	 Thread::quit ();

CWT_NS_END
