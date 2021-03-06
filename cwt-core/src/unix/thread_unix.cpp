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

//#define CWT_TRACE_DISABLE
#include <cwt/trace.hpp>

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

ThreadData::ThreadData()
	: threadImpl()
	, threadId(0)
{
	CWT_TRACE_METHOD();
}

ThreadData::~ThreadData ()
{
	CWT_TRACE_METHOD();
	// automatically delete m_threadImpl
}


Thread::Impl::Impl (/*ThreadData * threadData*/)
	: m_mutex()
	, m_stackSize(0)
	, m_priority(Thread::InheritPriority)
	, m_state(ThreadNotRunning)
	, m_threadFinished()
	, m_thread(nullptr)
	, m_data(nullptr)
{
	CWT_TRACE_METHOD();
}

Thread::Impl::~Impl()
{
	CWT_TRACE_METHOD();

	// deleting m_data will destroy this instance

	ThreadData * d = m_data;
	m_data = nullptr;
	if (d) // if is in non-running state or already destroyed from ThreadData::destroy()
		delete d;

	Thread * t = m_thread;
	m_thread = nullptr;
	if (t) {
		delete t;
	}
}

Thread::Thread() : pimpl(new Thread::Impl)
{
	CWT_TRACE_METHOD();
	pimpl->m_thread = this;
}

Thread::~Thread ()
{
	CWT_TRACE_METHOD();

	AutoLock<> locker(& pimpl->m_mutex);
    if (pimpl->m_state == ThreadFinishing) {
        locker.unlock();
        wait();
        locker.tryLock();
    }

    if (pimpl->m_state == ThreadRunning) {
    	CWT_WARN(_Tr("Attempt to destroy thread while it is still running"));
    	locker.unlock();
    	terminate();
    	wait();
    	locker.tryLock();
    }

	pimpl->m_thread = nullptr; // detach thread
}


bool Thread::isFinished () const
{
	AutoLock<>(& pimpl->m_mutex);
    return pimpl->m_state == ThreadFinished || pimpl->m_state == ThreadFinishing;
}

bool Thread::isRunning () const
{
	AutoLock<>(& pimpl->m_mutex);
    return pimpl->m_state == ThreadRunning;
}

Thread::Priority Thread::priority () const
{
	AutoLock<>(& pimpl->m_mutex);
    return pimpl->m_priority;
}

size_t Thread::stackSize () const
{
	AutoLock<>(& pimpl->m_mutex);
    return pimpl->m_stackSize;
}

void Thread::yieldCurrentThread ()
{
	sched_yield();
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
    	CWT_VERIFY_ERRNO(!(rc = pthread_attr_setstacksize(& attr, stackSize)), rc);
    }

    m_stackSize = 0;
    CWT_VERIFY_ERRNO(!(rc = pthread_attr_getstacksize(& attr, & m_stackSize)), rc);

    return (rc == 0);
}


void Thread::Impl::start (Thread::Priority priority, size_t stackSize)
{
	CWT_TRACE_METHOD();
	int rc = 0;
	AutoLock<> locker(& m_mutex);

	if (m_state == ThreadFinishing)
		m_threadFinished.wait(*locker.handlePtr());

	if (m_state == ThreadRunning ) {
		return;
	}

	m_state = ThreadRunning;

	pthread_attr_t attr;

	while (true) {
		CWT_VERIFY_ERRNO(!(rc = pthread_attr_init(& attr)), rc);
		if (rc)
			break;

		// Only threads that are created as joinable can be joined.
		// If a thread is created as detached, it can never be joined.
		CWT_VERIFY_ERRNO(!(rc = pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED)), rc);
		if (rc)
			break;

		m_priority = priority;

		if (priority == Thread::InheritPriority) {
			CWT_VERIFY_ERRNO(!(rc = pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED)), rc);
			if (rc)
				break;
		} else {
#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING
			int posixPolicy;
			CWT_VERIFY_ERRNO(!(rc = pthread_attr_getschedpolicy(& attr, & posixPolicy)), rc);

            if (rc) {
            	rc = 0; // ignore this error
            	break;
            }

            int posixPriority;

            if (! __map_to_posix_priority(priority, & posixPolicy, & posixPriority)) {
            	CWT_WARN(_Tr("Can not map to POSIX priority"));
            	break;
            }

            sched_param sp;
            sp.sched_priority = posixPriority;

            if (pthread_attr_setinheritsched(& attr, PTHREAD_EXPLICIT_SCHED) != 0
            		|| pthread_attr_setschedpolicy(& attr, posixPolicy) != 0
	                || pthread_attr_setschedparam(& attr, & sp) != 0) {

            	pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
            	m_priority = priority;
            }
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING
		}
		break;
	}

	if (rc == 0) {
		while (true) {
			if (! setStackSize(attr, stackSize))
				break;

			pthread_t pth = 0;

			m_data = new ThreadData;
			m_data->threadImpl = m_thread->pimpl;

			CWT_VERIFY_ERRNO(!(rc = pthread_create(& pth, & attr, & Thread::Impl::thread_routine, this)), rc);

			if (rc) {
				m_state = ThreadNotRunning;
				if (m_data) {
					delete m_data;
					m_data = nullptr;
				}
#ifdef __COMMENT__
#ifdef CWT_OS_LINUX
				if (rc == EAGAIN) {
					struct rlimit rlim;
					getrlimit(RLIMIT_NPROC, & rlim);
//					String msg(_Tr("May be the limit of threads (processes) has been exceeded: soft limit = %u, hard limit = %u")
//							% ulong_t(rlim.rlim_cur) % ulong_t(rlim.rlim_max));
					CWT_ERROR(_Tr("May be the limit of threads (processes) has been exceeded"));
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


/*
 * 1. Cancelability should only be disabled on  entry  to  an  object,
 * 	  never  explicitly  enabled.  On  exit from an object, the cancelability
 * 	  state should always be restored to its value on entry to the object.
 *
 * 2. The cancelability type may be explicitly set to either deferred
 *    or asynchronous upon entry to an object.  But as with the cancelability
 *    state, on exit from an object the cancelability type should  always  be
 *    restored to its value on entry to the object.
 *
 * 3. Only functions that are cancel-safe may be called from a
 *    thread that is asynchronously cancelable.
 */
void * Thread::Impl::thread_routine (void * arg)
{
	CWT_TRACE_FUNC();
	CWT_ASSERT(arg);

	// If a cancellation request is received,
    // it is blocked until cancelability is enabled.
    CWT_VERIFY(!pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr));

	Thread::Impl * threadImpl = static_cast<Thread::Impl *>(arg);
	threadImpl->m_data->threadId = pthread_self();

    pthread_cleanup_push(& Thread::Impl::finalize, threadImpl);

    ThreadData::set(threadImpl->m_data); // set thread-specific data

    CWT_VERIFY(0 == pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr));
    CWT_VERIFY(0 == pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr));

    pthread_testcancel();

	Thread * thisThread = threadImpl->m_thread;

	// This is the last checking if Thread instance is a live (not destroyed)
	// before call the 'run' routine.
	if (! thisThread) {
		// This order of deletion is important to prevent from double free.
		ThreadData * d = threadImpl->m_data;
		threadImpl->m_data = nullptr;
		delete d;
		CWT_ASSERT_X(thisThread, _Tr("Thread destroyed, may be you forget to wait() it")); // thread detached (container already destroyed)
	}

   	thisThread->run(); // Do the job

    pthread_cleanup_pop(true); // calls finalize() routine

    return nullptr;
}

void Thread::Impl::finalize (void * arg)
{
	CWT_TRACE_FUNC();
	CWT_ASSERT(arg);

	Thread::Impl * threadImpl = static_cast<Thread::Impl *>(arg);

    AutoLock<> locker(& threadImpl->m_mutex);

    CWT_ASSERT(threadImpl->m_data->threadId);

    threadImpl->m_state = ThreadFinishing;

    // Delete thread storage data
/*
    locker.unlock();
    QThreadStorageData::finish((void **)data);
    locker.relock();
*/

    threadImpl->m_data->threadId = 0;
    threadImpl->m_state = ThreadFinished;
    threadImpl->m_threadFinished.wakeAll();
}

void ThreadData::destroy (void * pdata)
{
	CWT_TRACE_FUNC();
	CWT_ASSERT(pdata);
	pthread_setspecific(threadKey, pdata);

	ThreadData * threadData = static_cast<ThreadData *>(pdata);
	threadData->threadImpl->m_data = nullptr;
	delete threadData;

    pthread_setspecific(threadKey, nullptr);
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
	CWT_TRACE_METHOD();
    AutoLock<> locker(& m_mutex);

    if (m_data && m_data->threadId) {
    	int rc = 0;
    	m_state = ThreadFinishing;
    	CWT_VERIFY_ERRNO(!(rc = pthread_cancel(m_data->threadId)), rc); // Thread termination error
    }
}

bool Thread::Impl::wait (ulong_t timeout)
{
	CWT_TRACE_METHOD();

    AutoLock<> locker(& m_mutex);

    if (m_data) {
		if (m_data->threadId == pthread_self()) {
			CWT_ERROR(_Tr("Thread attempt to wait on itself"));
			return false;
		}

		while (m_state == ThreadRunning) {
			if (timeout == CWT_ULONG_MAX) {
				if (! m_threadFinished.wait(m_mutex)) {
					return false;
				}
			} else {
				if (! m_threadFinished.wait(m_mutex, timeout)) {
					return false;
				}
			}
		}
    }
    return true;
}


void Thread::Impl::setPriority(Thread::Priority priority)
{
    AutoLock<> locker(& m_mutex);

    if (m_state != ThreadRunning) {
    	CWT_WARN("Unable to set thread priority: thread must be in running state");
        return;
    }

    m_priority = priority;

#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING

    int posixPolicy;
    sched_param param;

    int rc;

    CWT_VERIFY_ERRNO(!(rc = pthread_getschedparam(m_data->threadId, & posixPolicy, & param)), rc);
    if (rc)
        return;

    int posixPriority;

    if (! __map_to_posix_priority(priority, & posixPolicy, & posixPriority)) {
    	CWT_WARN(_Tr("Can not map to POSIX priority"));
    	return;
    }

    param.sched_priority = posixPriority;
    CWT_VERIFY_ERRNO(!(rc = pthread_setschedparam(m_data->threadId, posixPolicy, & param)), rc);

#	ifdef SCHED_IDLE
    if (rc < 0 && posixPolicy == SCHED_IDLE && errno == EINVAL) {
    	// Set native thread's priority to minimal value
        pthread_getschedparam(m_data->threadId, & posixPolicy, & param);
        param.sched_priority = sched_get_priority_min(posixPolicy);
        pthread_setschedparam(m_data->threadId, posixPolicy, & param);
    }
#	endif
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING
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


void Thread::exit ()
{
	pthread_exit(nullptr);
}

CWT_NS_END
