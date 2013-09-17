/*
 * thread_unix.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: wladt
 */

#include "../../include/cwt/thread.hpp"
#include "../../include/cwt/mt.hpp"
#include "../../include/cwt/logger.hpp"
#include "../../include/cwt/safeformat.hpp"
#include <pthread.h>
#include <sched.h>
#include <unistd.h> // for _POSIX_PRIORITY_SCHEDULING macro

CWT_NS_BEGIN

class ThreadImpl
{
	CWT_IMPLEMENT_LOCKING(ThreadImpl);

private:
	static const int StateMask = 0x0000000F;

	enum Status {
		  NotRunning
		, Running     = 0x00000001
/*
		, Finishing   = 0x00000002
		, Finished    = 0x00000004
		, Terminated  = 0x00000008
*/
	};

public:
	ThreadImpl()
		: m_threadId(0)
		, m_stackSize(0)
		, m_priority(Thread::InheritPriority)
		, m_status(NotRunning) {}

	void start (Thread::Priority priority = Thread::InheritPriority);
	void setPriority(Thread::Priority priority);

	static void * start_routine (void * arg);

private:
//	void setRunning (bool b) { m_status | StateMask) & Running; }
//	bool setFinished (bool b) const   { return (m_status & StateMask) & Finished; }
	//bool isTerminated () const { return (m_status & StateMask) & Terminated; }

	bool isRunning () const    { return (m_status & StateMask) & Running; }
/*
	bool isFinishing () const  { return (m_status & StateMask) & Finishing; }
	bool isFinished () const   { return (m_status & StateMask) & Finished; }
	bool isTerminated () const { return (m_status & StateMask) & Terminated; }
*/

#ifdef 	_POSIX_PRIORITY_SCHEDULING
	bool mapToPosixPriority (Thread::Priority priority, int * posixPolicyPtr, int * posixPriorityPtr);
#endif

private:
	pthread_t        m_threadId;
	size_t           m_stackSize;
	Thread::Priority m_priority;
	uint32_t         m_status;
};


#ifdef 	_POSIX_PRIORITY_SCHEDULING
// The  range  of  scheduling  priorities  may  vary  on  other POSIX systems,
// thus it is a good idea for portable applications to use a virtual priority
// range and map it to the interval given by sched_get_priority_max()
// and sched_get_priority_min() (see sched_get_priority_max(2)).
//
bool ThreadImpl::mapToPosixPriority (Thread::Priority priority, int * posixPolicyPtr, int * posixPriorityPtr)
{
#ifdef SCHED_IDLE
    if (priority == Thread::IdlePriority) {
        *posixPolicyPtr = SCHED_IDLE;
        *posixPriorityPtr = 0;
        return true;
    }
    const int lowestPriority = Thread::LowestPriority;
#else
    const int lowestPriority = Thread::IdlePriority;
#endif
    const int highestPriority = Thread::TimeCriticalPriority;

    int prio_min = sched_get_priority_min(*posixPolicyPtr);
    int prio_max = sched_get_priority_max(*posixPolicyPtr);

    if (prio_min < 0 || prio_max < 0)
        return false;

    // Scale priority using affine transformation:
    //
    // x' = a*x + c
    //              ___
    // x0' = a*x0 + c |              (x - x0) * (x1' - x0')
    //                |=> x' = x0' + ----------------------
    // x1' = a*x1 + c |                    (x1 - x0)
    //              --
    //
                                                                // XXX Qt5.0 ignores lowestPriority here---
                                                                //                                         |
                                                                //                                         v
    *posixPriorityPtr = prio_min + ((priority - lowestPriority) * (prio_max - prio_min) / (highestPriority - lowestPriority));
    *posixPriorityPtr = CWT_MAX(prio_min, CWT_MIN(prio_max, *posixPriorityPtr));
    return true;
}
#endif // _POSIX_PRIORITY_SCHEDULING

void ThreadImpl::start (Thread::Priority priority)
{
	int rc = 0;
	AutoLock locker(this);

	//if (m_status & Finishing);
	// TODO Check if in finishing state

	// TODO Check if is not running

	pthread_attr_t attr;

	while (true) {
		rc = pthread_attr_init(& attr);
		if (rc) {
			Logger::error(rc, _Tr("pthread_attr_init"));
			break;
		}

		// Only threads that are created as joinable can be joined.
		// If a thread is created as detached, it can never be joined.
		rc = pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED);
		if (rc) {
			Logger::error(rc, _Tr("pthread_attr_setdetachstate"));
			break;
		}

		m_priority = priority;

		if (priority == Thread::InheritPriority) {
			rc = pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
			if (rc) {
				Logger::error(rc, _Tr("pthread_attr_setinheritsched"));
				break;
			}
		} else {
#ifdef _POSIX_PRIORITY_SCHEDULING
			int posixPolicy;
			rc = pthread_attr_getschedpolicy(& attr, & posixPolicy);

            if (rc) {
            	Logger::warn(rc, _Fr("Thread::start: Unable to get POSIX's thread policy"));
            	rc = 0; // ignore this error
            	break;
            }

            int posixPriority;

            if (!mapToPosixPriority(priority, & posixPolicy, & posixPriority)) {
            	Logger::warn(_Tr("Thread::start: Can not map to POSIX priority"));
            	break;
            }

            sched_param sp;
            sp.sched_priority = posixPriority;

            if (pthread_attr_setinheritsched(& attr, PTHREAD_EXPLICIT_SCHED) != 0
            		|| pthread_attr_setschedpolicy(& attr, posixPolicy) != 0
	                || pthread_attr_setschedparam(& attr, & sp) != 0) {

            	pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
            	m_priority = priority;
            	//| ThreadPriorityResetFlag);
            }
#endif
		}
		break;
	}

	if (rc == 0) {
		while (true) {
			if (m_stackSize > 0) {
				rc = pthread_attr_setstacksize(& attr, m_stackSize);
				if (rc) {
					Logger::error(rc, _Tr("Failed to set stack size for thread"));
					break;
				}
			}

			rc = pthread_create(& m_threadId, & attr, start_routine, this);
			if (rc) {
				Logger::error(rc, _Tr("Failed to create thread"));
				break;
			}

			break;
		}
	}

	pthread_attr_destroy(& attr);

	if (rc) {
		m_threadId = 0;
	}
}

void ThreadImpl::setPriority(Thread::Priority priority)
{
    AutoLock locker(this);

    if (!isRunning()) {
        Logger::warn("Thread::setPriority: Unable to set thread priority: thread must be in running state");
        return;
    }

    m_priority = priority;

#ifdef _POSIX_PRIORITY_SCHEDULING
    int posixPolicy;
    sched_param param;

    int rc = pthread_getschedparam(m_threadId, & posixPolicy, & param);

    if (rc) {
    	Logger::warn(rc, _Tr("Thread::setPriority: Cannot get scheduler parameters"));
        return;
    }

    int posixPriority;

    if (!mapToPosixPriority(priority, & posixPolicy, & posixPriority)) {
    	Logger::warn(_Tr("Thread::setPriority: Can not map to POSIX priority"));
    	return;
    }

    param.sched_priority = posixPriority;
    rc = pthread_setschedparam(m_threadId, posixPolicy, & param);

#	ifdef SCHED_IDLE
    if (rc < 0 && posixPolicy == SCHED_IDLE && errno == EINVAL) {
    	// Set native thread's priority to minimal value
        pthread_getschedparam(m_threadId, & posixPolicy, & param);
        param.sched_priority = sched_get_priority_min(posixPolicy);
        pthread_setschedparam(m_threadId, posixPolicy, & param);
    }
#endif // _POSIX_PRIORITY_SCHEDULING
}


Thread::Thread() : pimpl(new ThreadImpl) { }
void Thread::start (Thread::Priority priority) { pimpl->start(priority); }
void Thread::setPriority (Thread::Priority priority) { pimpl->setPriority(priority); }

//void	 Thread::exit (int returnCode = 0)
//bool	 Thread::isFinished () const
//bool	 Thread::isRunning () const
//Thread::Priority Thread::priority () const

//void	 Thread::setStackSize (uint_t stackSize)
//uint_t	 Thread::stackSize () const
//bool	 Thread::wait (ulong_t time = CWT_ULONG_MAX)
//void	 Thread::quit ();
//void	 Thread::terminate ()


CWT_NS_END
