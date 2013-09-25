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
#include "thread_unix.hpp"
#include <pthread.h>
#include <sched.h>

#ifdef __COMMENT__
#ifdef CWT_OS_LINUX
#	include <sys/time.h>
#	include <sys/resource.h> // for getrlimit(2)
#endif
#endif


CWT_NS_BEGIN

#ifdef CWT_HAVE_TLS
	__thread ThreadData * ThreadData::currentThreadData = nullptr;
#else
	pthread_once_t ThreadData::threadKeyOnce = PTHREAD_ONCE_INIT;
	pthread_key_t  ThreadData::threadKey;
#endif

#ifdef 	CWT_HAVE_THREAD_PRIORITY_SCHEDULING
// The  range  of  scheduling  priorities  may  vary  on  other POSIX systems,
// thus it is a good idea for portable applications to use a virtual priority
// range and map it to the interval given by sched_get_priority_max()
// and sched_get_priority_min() (see sched_get_priority_max(2)).
//
static bool __map_to_posix_priority (Thread::Priority priority, int * posixPolicyPtr, int * posixPriorityPtr)
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
    //                |
    // x0' = a*x0 + c |               (x - x0) * (x1' - x0')
    //                 >  x' = x0' + ----------------------
    // x1' = a*x1 + c |                     (x1 - x0)
    //              __|
    //
                                                                // XXX Qt5.0 ignores lowestPriority here---
                                                                //                                         |
                                                                //                                         v
    *posixPriorityPtr = prio_min + ((priority - lowestPriority) * (prio_max - prio_min) / (highestPriority - lowestPriority));
    *posixPriorityPtr = CWT_MAX(prio_min, CWT_MIN(prio_max, *posixPriorityPtr));
    return true;
}
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING

Thread::Impl::Impl (Thread * threadPtr)
	: m_mutex()
	, m_stackSize(0)
	, m_priority(Thread::InheritPriority)
	, m_state(ThreadNotRunning)
	, m_threadDataPtr(nullptr)
{
	m_threadDataPtr = new ThreadData(threadPtr);
}


Thread::Impl::~Impl ()
{
	CWT_TRACE("Thread::Impl::~Impl ()");

    AutoLock<> locker(& m_mutex);
    if (m_state == ThreadFinishing) {
        locker.handlePtr()->unlock();
        wait();
        locker.handlePtr()->tryLock();
    }

    if (! isFinished()) {
    	CWT_SYS_WARN(_Tr("Attempt to destroy thread while it is still running"));
    } else {
		if (m_threadDataPtr != nullptr) {
			m_threadDataPtr->m_threadId = 0;
			delete m_threadDataPtr;
			m_threadDataPtr = nullptr;
		}
    }

    m_threadDataPtr->m_thread = nullptr; // detach from Thread instance
}


bool Thread::Impl::setStackSize (pthread_attr_t & attr, size_t stackSize)
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


void Thread::Impl::start (Thread::Priority priority, size_t stackSize)
{
	int rc = 0;
	AutoLock<> locker(& m_mutex);

	if (m_state == ThreadFinishing)
		m_threadDataPtr->m_threadFinished.wait(*locker.handlePtr());

	if (m_state == ThreadRunning)
		return;

	m_state = ThreadRunning;

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
#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING
			int posixPolicy;
			rc = pthread_attr_getschedpolicy(& attr, & posixPolicy);

            if (rc) {
            	CWT_SYS_WARN_RC(rc, _Fr("Unable to get POSIX's thread policy"));
            	rc = 0; // ignore this error
            	break;
            }

            int posixPriority;

            if (! __map_to_posix_priority(priority, & posixPolicy, & posixPriority)) {
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
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING
		}
		break;
	}

	if (rc == 0) {
		while (true) {
			if (! setStackSize(attr, stackSize))
				break;

			rc = pthread_create(& m_threadDataPtr->m_threadId, & attr, & Thread::Impl::start_routine, this);
			if (rc) {
				m_state = ThreadNotRunning;
				m_threadDataPtr->m_threadId = 0;
				CWT_SYS_ERROR_RC(rc, _Tr("Failed to create thread"));
#ifdef __COMMENT__
#ifdef CWT_OS_LINUX
				if (rc == EAGAIN) {
					struct rlimit rlim;
					getrlimit(RLIMIT_NPROC, & rlim);
					String msg(_Fr("May be the limit of threads (processes) has been exceeded: soft limit = %u, hard limit = %u")
							% ulong_t(rlim.rlim_cur) % ulong_t(rlim.rlim_max));
					CWT_SYS_ERROR(msg);
				}
#endif
#endif
				break;
			}

			break;
		}
	}

	pthread_attr_destroy(& attr);
}


void * Thread::Impl::start_routine (void * arg)
{
    CWT_VERIFY(!pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr));

    pthread_cleanup_push(& Thread::Impl::finish_routine, arg);

    Thread::Impl * threadImpl = reinterpret_cast<Thread::Impl *>(arg);
    CWT_ASSERT(threadImpl);
    CWT_ASSERT(threadImpl->m_threadDataPtr);

    CWT_TRACE(String(_Fr("start_routine: BEGIN: threadImpl = %p, m_threadId = %d")
    		% threadImpl
    		% threadImpl->m_threadDataPtr->m_threadId).c_str());

    Thread * thisThread = threadImpl->m_threadDataPtr->m_thread;
    //ThreadData::set(thisThread->pimpl->m_threadDataPtr);

    // The cancelability state and type of any newly created threads,
    // including the thread in which main() was first invoked, shall be
    // PTHREAD_CANCEL_ENABLE and PTHREAD_CANCEL_DEFERRED respectively.
    CWT_VERIFY(0 == pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr));
    CWT_VERIFY(0 == pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr));
    pthread_testcancel();

    thisThread->run(); // Do the job

    CWT_TRACE(String(_Fr("start_routine: END: threadImpl = %p, m_threadId = %d")
    		% threadImpl
    		% threadImpl->m_threadDataPtr->m_threadId).c_str());

    pthread_cleanup_pop(1);

    return nullptr;
}

void Thread::Impl::finish_routine (void * arg)
{
	CWT_ASSERT(arg);
	Thread::Impl * threadImpl = reinterpret_cast<Thread::Impl *>(arg);

	CWT_ASSERT(threadImpl);
    CWT_ASSERT(threadImpl->m_threadDataPtr);

    AutoLock<> locker(& threadImpl->m_mutex);


    CWT_TRACE(String(_Fr("finish_routine: BEGIN: threadImpl = %p, m_threadId = %d")
    		% threadImpl
    		% threadImpl->m_threadDataPtr->m_threadId).c_str());

    threadImpl->m_state = ThreadFinishing;

    // Delete thread storage data
/*
    locker.unlock();
    QThreadStorageData::finish((void **)data);
    locker.relock();
*/
    CWT_ASSERT(threadImpl->m_threadDataPtr);
    threadImpl->m_state = ThreadFinished;
    threadImpl->m_threadDataPtr->m_threadFinished.wakeAll();

    threadImpl->m_threadDataPtr->m_threadId = 0;
    CWT_TRACE(String(_Fr("finish_routine: END: threadImpl = %p, m_threadId = %d")
    		% threadImpl
    		% threadImpl->m_threadDataPtr->m_threadId).c_str());
}


void Thread::Impl::setPriority(Thread::Priority priority)
{
    AutoLock<> locker(& m_mutex);

    if (m_state != ThreadRunning) {
    	CWT_SYS_WARN("Unable to set thread priority: thread must be in running state");
        return;
    }

    m_priority = priority;

#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING

    int posixPolicy;
    sched_param param;

    int rc = pthread_getschedparam(m_threadDataPtr->m_threadId, & posixPolicy, & param);

    if (rc) {
    	CWT_SYS_WARN_RC(rc, _Tr("Cannot get scheduler parameters"));
        return;
    }

    int posixPriority;

    if (! __map_to_posix_priority(priority, & posixPolicy, & posixPriority)) {
    	CWT_SYS_WARN(_Tr("Can not map to POSIX priority"));
    	return;
    }

    param.sched_priority = posixPriority;
    rc = pthread_setschedparam(m_threadDataPtr->m_threadId, posixPolicy, & param);

#	ifdef SCHED_IDLE
    if (rc < 0 && posixPolicy == SCHED_IDLE && errno == EINVAL) {
    	// Set native thread's priority to minimal value
        pthread_getschedparam(m_threadDataPtr->m_threadId, & posixPolicy, & param);
        param.sched_priority = sched_get_priority_min(posixPolicy);
        pthread_setschedparam(m_threadDataPtr->m_threadId, posixPolicy, & param);
    }
#	endif
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING
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
void Thread::Impl::terminate ()
{
    AutoLock<> locker(& m_mutex);

    CWT_TRACE(String(_Fr("Terminating: m_threadId = %d") % m_threadDataPtr->m_threadId).c_str());

    if (! m_threadDataPtr->m_threadId)
        return;

    CWT_TRACE(String(_Fr("Canceling: m_threadId = %d") % m_threadDataPtr->m_threadId).c_str());
    int rc = pthread_cancel(m_threadDataPtr->m_threadId);
    if (rc) {
    	CWT_SYS_ERROR_RC(rc, _Tr("Thread termination error"));
    }
}

bool Thread::Impl::wait (ulong_t timeout)
{
    AutoLock<> locker(& m_mutex);

    if (m_threadDataPtr->m_threadId == pthread_self()) {
    	CWT_SYS_ERROR(_Tr("PosixThreadImpl::wait(): Attempt to wait on itself"));
        return false;
    }

    while (m_state == ThreadRunning) {
    	if (timeout == CWT_ULONG_MAX) {
    		if (! m_threadDataPtr->m_threadFinished.wait(m_mutex))
    			return false;
    	} else {
    		if (! m_threadDataPtr->m_threadFinished.wait(m_mutex, timeout))
    			return false;
    	}
    }
    return true;
}


void ThreadData::destroy (void * pdata)
{
	CWT_ASSERT(pdata);
    // POSIX says the value in our key is set to zero before calling
    // this destructor function, so we need to set it back to the
    // right value...

	// TODO Check this sentence in debug
    //pthread_setspecific(m_key, pdata);

    //ThreadData * threadDataPtr = static_cast<ThreadData *>(pdata);

    //CWT_ASSERT(threadDataPtr->m_thread);
/*
    if (data->m_isAdopted) {
        Thread * thread = data->m_thread;
        CWT_ASSERT(thread);
        PosixThreadImpl * thread_p = static_cast<PosixThreadImpl *>(thread->pimpl);
        CWT_ASSERT(! thread_p->isFinished());
        thread_p->finish(thread);
    }
*/

    pthread_setspecific(threadKey, nullptr);
}


inline struct timespec __make_timespec(time_t secs, long_t nsecs)
{
    struct timespec ts;
    ts.tv_sec = secs;
    ts.tv_nsec = nsecs;
    return ts;
}

inline void __nanosleep (struct timespec & ts)
{
	int rc;
	while ((rc = nanosleep(& ts, & ts)) < 0 && errno == EINTR)
		;
}

void Thread::Impl::sleep(ulong_t secs)
{
	struct timespec ts =  __make_timespec(secs, 0);
    __nanosleep(ts);
}

void Thread::Impl::msleep(ulong_t msecs)
{
	struct timespec ts = __make_timespec(msecs / 1000, msecs % 1000 * 1000 * 1000);
    __nanosleep(ts);
}

void Thread::Impl::usleep(ulong_t usecs)
{
	struct timespec ts = __make_timespec(usecs / 1000 / 1000, usecs % (1000*1000) * 1000);
    __nanosleep(ts);
}


CWT_NS_END
