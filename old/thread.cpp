/*
 * thread.cpp
 *
 *  Created on: Sep 20, 2013
 *      Author: wladt
 */

#include "thread_p.hpp"
#include "pfs/logger.hpp"

// Sources:
// * POSIX Threads Programming
//	https://computing.llnl.gov/tutorials/pthreads/
// * Завершение работы нитей
//	http://pic.dhe.ibm.com/infocenter/aix/v6r1/topic/com.ibm.aix.genprogc/doc/genprogc/term_threads.htm#term_threads__cleanup

namespace pfs {

thread::thread () : base_class(new thread_impl)
{
	base_class::cast()->_thread = this;
}

thread::~thread ()
{
	impl_class * d = base_class::cast();
	pfs::auto_lock<> locker(& d->_mutex);

    if (d->_state == ThreadFinishing) {
        locker.unlock();
        wait();
        locker.tryLock();
    }

    if (! PFS_VERIFY_X(d->_state != ThreadRunning
    		, _Tr("Attempt to destroy thread while it is still running"))) {
    	locker.unlock();
    	terminate();
    	wait();
    	locker.tryLock();
    }

	d->_thread = nullptr; // detach thread
}

bool thread::isFinished () const
{
	const impl_class * d = base_class::cast();
    return d->_state == ThreadFinished || d->_state == ThreadFinishing;
}

bool thread::isRunning () const
{
	const impl_class * d = base_class::cast();
    return d->_state == ThreadRunning;
}

thread::priority_type thread::priority () const
{
	const impl_class * d = base_class::cast();
    return d->_priority;
}

size_t thread::stackSize () const
{
	const impl_class * d = base_class::cast();
    return d->_stackSize;
}

void thread::setPriority (thread::priority_type priority)
{
	base_class::cast()->setPriority(priority);
}

void thread::start (thread::priority_type priority, size_t stackSize)
{
	base_class::cast()->start(priority, stackSize);
}

void thread::terminate ()
{
	base_class::cast()->terminate();
}

bool thread::wait (uintegral_t timeout)
{
	return base_class::cast()->wait(timeout);
}

void thread::sleep (uintegral_t secs)
{
    thread_impl::sleep(secs);
}

void thread::msleep (uintegral_t msecs)
{
	thread_impl::msleep(msecs);
}

void thread::usleep (uintegral_t usecs)
{
	thread_impl::usleep(usecs);
}

} // pfs
