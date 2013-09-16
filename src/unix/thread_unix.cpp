/*
 * thread_unix.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: wladt
 */

#include "../../include/cwt/thread.hpp"
#include "../../include/cwt/mt.hpp"
#include "../../include/cwt/safeformat.hpp"
#include <pthread.h>

CWT_NS_BEGIN

class ThreadImpl
{
public:
	ThreadImpl()
		: m_threadId(0)
		, m_stackSize(0)
		, m_priority(Thread::InheritPriority) {}

	void start (Thread::Priority priority = Thread::InheritPriority);

	CWT_IMPLEMENT_LOCKING(ThreadImpl);

	pthread_t        m_threadId;
	size_t           m_stackSize;
	Thread::Priority m_priority;
};


void ThreadImpl::start (Thread::Priority priority)
{
	int rc = 0;
	ThreadImpl::AutoLock(this);

	// TODO Check if in finishing state

	// TODO Check if is not running

	pthread_attr_t attr;

	while (true) {
		rc = pthread_attr_init(& attr);
		if (rc) {
			Logger::error(rc, _Fr("pthread_attr_init"));
			break;
		}

		rc = pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED);
		if (rc) {
			Logger::error(rc, _Fr("pthread_attr_setdetachstate"));
			break;
		}

		m_priority = priority;

		switch (priority) {
		case Thread::InheritPriority:
			rc = pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
			if (rc) {
				Logger::error(rc, _Fr("pthread_attr_setinheritsched"));
			}
			break;
		default:
			// TODO set priority
			break;
		}

		break;
	}

// pthread_attr_setaffinity_np(3)
// pthread_attr_setdetachstate(3)
// pthread_attr_setguardsize(3)
// pthread_attr_setinheritsched(3)
// pthread_attr_setschedparam(3)
// pthread_attr_setschedpolicy(3)
// pthread_attr_setscope(3)
// pthread_attr_setstack(3)
// pthread_attr_setstackaddr(3)
// pthread_attr_setstacksize(3)
// pthread_create(3)
// pthread_getattr_np(3)
// pthreads(7)

	if (rc == 0) {
		while (true) {
			if (m_stackSize > 0) {
				rc = pthread_attr_setstacksize(& attr, m_stackSize);
				if (rc) {
					Logger::error(rc, _Fr("Failed to set stack size for thread"));
					break;
				}
			}

			pthread_create(& m_threadId, & attr, );
			break;
		}
	}
}

Thread::Thread() : pimpl(new ThreadImpl) { }
void Thread::start (Thread::Priority priority) { pimpl->start(priority); }
//void	 Thread::exit (int returnCode = 0)
//bool	 Thread::isFinished () const
//bool	 Thread::isRunning () const
//Thread::Priority Thread::priority () const
//void	 Thread::setPriority (Thread::Priority priority)
//void	 Thread::setStackSize (uint_t stackSize)
//uint_t	 Thread::stackSize () const
//bool	 Thread::wait (ulong_t time = CWT_ULONG_MAX)
//void	 Thread::quit ();
//void	 Thread::terminate ()


CWT_NS_END
