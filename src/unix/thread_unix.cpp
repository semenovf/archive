/*
 * thread_unix.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: wladt
 */

#include "../../include/cwt/mt.hpp"
#include "../../include/cwt/logger.hpp"
#include "../../include/cwt/safeformat.hpp"
#include "../../include/cwt/threadcv.hpp"
#include <pthread.h>
#include <sched.h>
#include <unistd.h> // for _POSIX_PRIORITY_SCHEDULING macro

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#	define CWT_HAS_THREAD_PRIORITY_SCHEDULING
#endif

#define __CWT_INIT_FROM_PIMPL
#include "thread_unix.hpp"

CWT_NS_BEGIN


class PosixThreadImpl : public ThreadImpl
{
public:
	PosixThreadImpl()
		: ThreadImpl()
		, m_threadId(0)
		, m_threadFinished()
		//, m_threadDataPtr(nullptr)
	{}

	~PosixThreadImpl();

	void start (Thread::Priority priority = Thread::InheritPriority, size_t stackSize = 0);
	void setPriority(Thread::Priority priority);

	void terminate ();
	bool wait (ulong_t time = CWT_ULONG_MAX);

	static void * start_routine (void * arg);
	static void finish_routine (void *);

private:
	bool setStackSize (pthread_attr_t & attr, size_t stackSize = 0);

#ifdef 	CWT_HAS_THREAD_PRIORITY_SCHEDULING
	bool mapToPosixPriority (Thread::Priority priority, int * posixPolicyPtr, int * posixPriorityPtr);
#endif

private:
	pthread_t     m_threadId;
	ThreadCV      m_threadFinished;
	//ThreadData *  m_threadDataPtr;

	//friend class ThreadData;
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

    //m_threadDataPtr->threadPtr = nullptr;
}


bool PosixThreadImpl::setStackSize (pthread_attr_t & attr, size_t stackSize)
{
	int rc = 0;

    if (stackSize > 0) {
    	size_t page_size = getpagesize();

#ifdef PTHREAD_STACK_MIN
    	if (stackSize < PTHREAD_STACK_MIN)
    		stackSize = PTHREAD_STACK_MIN;
#endif

    	stackSize = ((stackSize + page_size - 1) / page_size) * page_size;
    	rc = pthread_attr_setstacksize(& attr, stackSize);

    	if (rc)
    		CWT_SYS_ERROR_RC(rc, _Tr("Failed to set thread's stack size"));
    }

    m_stackSize = 0;
    rc = pthread_attr_getstacksize(& attr, & m_stackSize);

    if (rc)
    	CWT_SYS_ERROR_RC(rc, _Tr("Failed to get thread's stack size"));

    return (rc == 0);
}

#ifdef 	CWT_HAS_THREAD_PRIORITY_SCHEDULING
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
#endif // CWT_HAS_THREAD_PRIORITY_SCHEDULING

void PosixThreadImpl::start (Thread::Priority priority, size_t stackSize)
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
#ifdef CWT_HAS_THREAD_PRIORITY_SCHEDULING
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
#endif // CWT_HAS_THREAD_PRIORITY_SCHEDULING
		}
		break;
	}

	if (rc == 0) {
		while (true) {
			if (! setStackSize(attr, stackSize))
				break;

			rc = pthread_create(& m_threadId, & attr, start_routine, this);
			if (rc) {
				m_threadId = 0;
				CWT_SYS_ERROR_RC(rc, _Tr("Failed to create thread"));
				break;
			}

			break;
		}
	}

	pthread_attr_destroy(& attr);
}

void PosixThreadImpl::setPriority(Thread::Priority priority)
{
    AutoLock locker(this);

    if (!isRunningState()) {
    	CWT_SYS_WARN("Unable to set thread priority: thread must be in running state");
        return;
    }

    m_priority = priority;

#ifdef CWT_HAS_THREAD_PRIORITY_SCHEDULING
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
#endif // CWT_HAS_THREAD_PRIORITY_SCHEDULING
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


void * PosixThreadImpl::start_routine(void * arg)
{
    CWT_VERIFY(0 == pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr));
    pthread_cleanup_push(PosixThreadImpl::finish_routine, arg);

    Thread * thisThread = reinterpret_cast<Thread *>(arg);
/*
    ThreadData * data = ThreadData::get2(thr);

    data->threadId = (Qt::HANDLE)pthread_self();
    set_thread_data(data);

    data->ref();
    {
        QMutexLocker locker(&thr->d_func()->mutex);
        data->quitNow = thr->d_func()->exited;
    }
*/

    CWT_VERIFY(0 == pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL));
    pthread_testcancel();

    thisThread->run(); // Do the job
    pthread_cleanup_pop(1);
    return nullptr;
}

void PosixThreadImpl::finish_routine (void * arg)
{
    Thread * thisThread = reinterpret_cast<Thread *>(arg);
    AutoLock locker(this);

    setFinishingState(true);

    // Delete thread storage data
/*
    d->priority = QThread::InheritPriority;
    void *data = &d->data->tls;
    locker.unlock();
    QThreadStorageData::finish((void **)data);
    locker.relock();
*/

    m_threadId = 0;
    setRunningState(false);
    setFinishingState(false);
    setFinishedState(true);
    m_threadFinished.wakeAll();
}


Thread::Thread() : pimpl(new PosixThreadImpl) { }

CWT_NS_END
