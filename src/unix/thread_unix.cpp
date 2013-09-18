/*
 * thread_unix.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: wladt
 */

#include "../thread_p.hpp"
#include "../../include/cwt/mt.hpp"
#include "../../include/cwt/logger.hpp"
#include "../../include/cwt/safeformat.hpp"
#include "../../include/cwt/conditionvariable.hpp"
#include <pthread.h>
#include <sched.h>
#include <unistd.h> // for _POSIX_PRIORITY_SCHEDULING macro

CWT_NS_BEGIN

struct ThreadData
{
	ThreadData()
		: threadPtr(nullptr)
	{}

	Thread * threadPtr;
};

class PosixThreadImpl : public ThreadImpl
{
public:
	PosixThreadImpl()
		: ThreadImpl()
		, m_threadId(0)
		, m_threadFinished()
		, m_threadDataPtr(nullptr)
	{}

	~PosixThreadImpl();

	void start (Thread::Priority priority = Thread::InheritPriority);
	void setPriority(Thread::Priority priority);
	void terminate ();
	bool wait (ulong_t time = CWT_ULONG_MAX);

	static void * start_routine (void * arg);

private:

#ifdef 	_POSIX_PRIORITY_SCHEDULING
	bool mapToPosixPriority (Thread::Priority priority, int * posixPolicyPtr, int * posixPriorityPtr);
#endif

private:
	pthread_t         m_threadId;
	ConditionVariable m_threadFinished;
	ThreadData *      m_threadDataPtr;
};

PosixThreadImpl::~PosixThreadImpl ()
{
    AutoLock locker(this);
    if (isFinishingState()) {
        locker.mutexPtr()->unlock();
        wait();
        locker.mutexPtr()->tryLock();
    }
    if (isRunningState() && ! isFinished()) {
    	CWT_SYS_WARN(_Tr("Attempt to destroy thread while it is still running"));
    }

    m_threadDataPtr->threadPtr = nullptr;
}


#ifdef 	_POSIX_PRIORITY_SCHEDULING
// The  range  of  scheduling  priorities  may  vary  on  other POSIX systems,
// thus it is a good idea for portable applications to use a virtual priority
// range and map it to the interval given by sched_get_priority_max()
// and sched_get_priority_min() (see sched_get_priority_max(2)).
//
bool PosixThreadImpl::mapToPosixPriority (Thread::Priority priority, int * posixPolicyPtr, int * posixPriorityPtr)
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
    // x' = a*x + c __
    //                \
    // x0' = a*x0 + c |               (x - x0) * (x1' - x0')
    //                 >  x' = x0' + ----------------------
    // x1' = a*x1 + c |                     (x1 - x0)
    //              __/
    //
                                                                // XXX Qt5.0 ignores lowestPriority here---
                                                                //                                         |
                                                                //                                         v
    *posixPriorityPtr = prio_min + ((priority - lowestPriority) * (prio_max - prio_min) / (highestPriority - lowestPriority));
    *posixPriorityPtr = CWT_MAX(prio_min, CWT_MIN(prio_max, *posixPriorityPtr));
    return true;
}
#endif // _POSIX_PRIORITY_SCHEDULING

void PosixThreadImpl::start (Thread::Priority priority)
{
	int rc = 0;
	AutoLock locker(this);

	if (isFinishingState())
		m_threadFinished.wait(locker.mutexPtr());

	if (isRunningState())
		return;

	// TODO set status

	pthread_attr_t attr;

	while (true) {
		rc = pthread_attr_init(& attr);
		if (rc) {
			CWT_SYS_ERROR_RC(rc, _Tr("Failed to initialize thread attributes object"));
			break;
		}

		// Only threads that are created as joinable can be joined.
		// If a thread is created as detached, it can never be joined.
		rc = pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED);
		if (rc) {
			CWT_SYS_ERROR_RC(rc, _Tr("pthread_attr_setdetachstate"));
			break;
		}

		m_priority = priority;

		if (priority == Thread::InheritPriority) {
			rc = pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
			if (rc) {
				CWT_SYS_ERROR_RC(rc, _Tr("pthread_attr_setinheritsched"));
				break;
			}
		} else {
#ifdef _POSIX_PRIORITY_SCHEDULING
			int posixPolicy;
			rc = pthread_attr_getschedpolicy(& attr, & posixPolicy);

            if (rc) {
            	CWT_SYS_WARN_RC(rc, _Fr("Unable to get POSIX's thread policy"));
            	rc = 0; // ignore this error
            	break;
            }

            int posixPriority;

            if (!mapToPosixPriority(priority, & posixPolicy, & posixPriority)) {
            	CWT_SYS_WARN(_Tr("Can not map to POSIX priority"));
            	break;
            }

            sched_param sp;
            sp.sched_priority = posixPriority;

            if (pthread_attr_setinheritsched(& attr, PTHREAD_EXPLICIT_SCHED) != 0
            		|| pthread_attr_setschedpolicy(& attr, posixPolicy) != 0
	                || pthread_attr_setschedparam(& attr, & sp) != 0) {

            	pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
            	m_priority = priority;
            	// TODO | ThreadPriorityResetFlag);
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
					CWT_SYS_ERROR_RC(rc, _Tr("Failed to set stack size for thread"));
					break;
				}
			}

			rc = pthread_create(& m_threadId, & attr, start_routine, this);
			if (rc) {
				CWT_SYS_ERROR_RC(rc, _Tr("Failed to create thread"));
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

void PosixThreadImpl::setPriority(Thread::Priority priority)
{
    AutoLock locker(this);

    if (!isRunningState()) {
    	CWT_SYS_WARN("Unable to set thread priority: thread must be in running state");
        return;
    }

    m_priority = priority;

#ifdef _POSIX_PRIORITY_SCHEDULING
    int posixPolicy;
    sched_param param;

    int rc = pthread_getschedparam(m_threadId, & posixPolicy, & param);

    if (rc) {
    	CWT_SYS_WARN_RC(rc, _Tr("Cannot get scheduler parameters"));
        return;
    }

    int posixPriority;

    if (!mapToPosixPriority(priority, & posixPolicy, & posixPriority)) {
    	CWT_SYS_WARN(_Tr("Can not map to POSIX priority"));
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

/*
 *   |              Thread                    pthread
 *   | terminate()     |                         |
 *   ----------------->|     pthread_cancel()    |
 *                     |------------------------>|
 *                                               |----
 *                                               |   | 1. Cancellation clean-up handlers are popped
 *                                               |   |    (in the reverse of the order in which they were pushed)
 *                                           ----|   |    and called (see pthread_cleanup_push(3))
 *       2. Thread-specific data destructors |   |<---
 *          are called, in an unspecified    |   |
 *          order (see pthread_key_create(3))|   |----
 *                                           --->|   |
 *                                               |   | 3. The thread is terminated (see pthread_exit(3))
 *                                               |   |
 *                                               |<---
 *                                               |
 *
 *  The above steps happen asynchronously.
 */
inline void PosixThreadImpl::terminate ()
{
    AutoLock locker(this);

    if (!m_threadId)
        return;

    int rc = pthread_cancel(m_threadId);
    if (rc) {
    	CWT_SYS_ERROR_RC(rc, _Tr("Thread termination error"));
    }
}

inline bool PosixThreadImpl::wait (ulong_t timeout)
{
    AutoLock locker(this);

    if (m_threadId == pthread_self()) {
    	CWT_SYS_ERROR(_Tr("PosixThreadImpl::wait(): Attempt to wait on itself"));
        return false;
    }

    if (isFinishedState() || ! isRunningState())
        return true;

    while (isRunningState()) {
        if (! m_threadFinished.wait(locker.mutexPtr(), timeout))
            return false;
    }
    return true;
}


Thread::Thread() : pimpl(new PosixThreadImpl) { }
bool Thread::isFinished () const { return pimpl->isFinished(); }
bool Thread::isRunning () const  { return pimpl->isRunning(); }
Thread::Priority Thread::priority () const { return pimpl->priority(); }
void Thread::setPriority (Thread::Priority priority) { pimpl->setPriority(priority); }
void Thread::setStackSize (size_t stackSize) { pimpl->setStackSize(stackSize); }
size_t Thread::stackSize () const { return pimpl->stackSize(); }
//void Thread::start (Thread::Priority priority) { pimpl->start(priority); }
void Thread::terminate () { pimpl->terminate(); }
bool Thread::wait (ulong_t timeout) { return pimpl->wait(timeout); }
//void	 Thread::exit (int returnCode = 0)
//void	 Thread::quit ();


CWT_NS_END
