/*
 * thread_unix.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: wladt
 */

#include "thread_unix.hpp"
#include "pfs/threadcv.hpp"
#include "pfs/platform.hpp"
#include <pfs/mt.hpp>
#include <pfs/safeformat.hpp>
#include <pthread.h>
#include <sched.h>

#ifdef __COMMENT__
#ifdef PFS_OS_LINUX
#	include <sys/time.h>
#	include <sys/resource.h> // for getrlimit(2)
#endif
#endif

//#define CWT_TRACE_ENABLE
#include "../../include/pfs/trace.hpp"

namespace pfs {

#ifdef CWT_HAVE_TLS
	__thread thread::data * thread::data::currentThreadData = nullptr;
#else
	pthread_once_t thread::data::threadKeyOnce = PTHREAD_ONCE_INIT;
	pthread_key_t  thread::data::threadKey;
#endif

#ifdef 	CWT_HAVE_THREAD_PRIORITY_SCHEDULING
// The  range  of  scheduling  priorities  may  vary  on  other POSIX systems,
// thus it is a good idea for portable applications to use a virtual priority
// range and map it to the interval given by sched_get_priority_max()
// and sched_get_priority_min() (see sched_get_priority_max(2)).
//
static bool __map_to_posix_priority (thread::priority_type priority, int * posixPolicyPtr, int * posixPriorityPtr)
{
#ifdef SCHED_IDLE
    if (priority == thread::IdlePriority) {
        *posixPolicyPtr = SCHED_IDLE;
        *posixPriorityPtr = 0;
        return true;
    }
    const int lowestPriority = thread::LowestPriority;
#else
    const int lowestPriority = thread::IdlePriority;
#endif
    const int highestPriority = thread::TimeCriticalPriority;

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
    *posixPriorityPtr = pfs::max(prio_min, pfs::min(prio_max, *posixPriorityPtr));
    return true;
}
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING

thread::data::data()
	: threadImpl()
	, threadId(0)
{
	CWT_TRACE_METHOD();
}

thread::data::~data ()
{
	CWT_TRACE_METHOD();
	// automatically delete _threadImpl
}


thread_impl::thread_impl (/*thread::data * threadData*/)
	: _mutex()
	, _stackSize(0)
	, _priority(thread::InheritPriority)
	, _state(ThreadNotRunning)
	, _threadFinished()
	, _thread(nullptr)
	, _data(nullptr)
{
}

thread_impl::~thread_impl ()
{
	// deleting _data will destroy this instance

	thread::data * d = _data;
	_data = nullptr;
	if (d) // if is in non-running state or already destroyed from thread::data::destroy()
		delete d;

	thread * t = _thread;
	_thread = nullptr;
	if (t) {
		delete t;
	}
}

void thread::yieldCurrentThread ()
{
	sched_yield();
}

bool thread_impl::setStackSize (pthread_attr_t & attr, size_t stackSize)
{
	int rc = 0;

    if (stackSize > 0) {
    	size_t page_size = getpagesize();

#ifdef PTHREAD_STACK_MIN
    	if (stackSize < PTHREAD_STACK_MIN)
    		stackSize = PTHREAD_STACK_MIN;
#endif

    	stackSize = ((stackSize + page_size - 1) / page_size) * page_size;
    	rc = CWT_VERIFY_ERRNO(pthread_attr_setstacksize(& attr, stackSize));
    }

    _stackSize = 0;
    rc = CWT_VERIFY_ERRNO(pthread_attr_getstacksize(& attr, & _stackSize));

    return (rc == 0);
}


void thread_impl::start (thread::priority_type priority, size_t stackSize)
{
	int rc = 0;
	pfs::auto_lock<> locker(& _mutex);

	if (_state == ThreadFinishing)
		_threadFinished.wait(*locker.handlePtr());

	if (_state == ThreadRunning) {
		return;
	}

	_state = ThreadRunning;

	pthread_attr_t attr;

	while (true) {
		rc = CWT_VERIFY_ERRNO(pthread_attr_init(& attr));
		if (rc)
			break;

		// Only threads that are created as joinable can be joined.
		// If a thread is created as detached, it can never be joined.
		rc = CWT_VERIFY_ERRNO(pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED));
		if (rc)
			break;

		_priority = priority;

		if (priority == thread::InheritPriority) {
			rc = CWT_VERIFY_ERRNO(pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED));
			if (rc)
				break;
		} else {

#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING
			int posixPolicy;
			rc = CWT_VERIFY_ERRNO(pthread_attr_getschedpolicy(& attr, & posixPolicy));
            if (rc) {
            	rc = 0; // ignore this error
            	break;
            }

            int posixPriority;
            bool ok;
            PFS_VERIFY_X((ok = __map_to_posix_priority(priority, & posixPolicy, & posixPriority))
            		, _Tr("Can not map to POSIX priority"));
            if (!ok)
            	break;

            sched_param sp;
            sp.sched_priority = posixPriority;

            if (pthread_attr_setinheritsched(& attr, PTHREAD_EXPLICIT_SCHED) != 0
            		|| pthread_attr_setschedpolicy(& attr, posixPolicy) != 0
	                || pthread_attr_setschedparam(& attr, & sp) != 0) {

            	pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED);
            	_priority = priority;
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

			_data = new thread::data;
			_data->threadImpl = _thread->_pimpl;

			rc = CWT_VERIFY_ERRNO(pthread_create(& pth, & attr, & thread_impl::thread_routine, this));

			if (rc) {
				_state = ThreadNotRunning;
				if (_data) {
					delete _data;
					_data = nullptr;
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
void * thread_impl::thread_routine (void * arg)
{
	PFS_ASSERT(arg);

	// If a cancellation request is received,
    // it is blocked until cancelability is enabled.
    CWT_VERIFY_ERRNO(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr));

	thread_impl * threadImpl = static_cast<thread_impl *>(arg);
	threadImpl->_data->threadId = pthread_self();

    pthread_cleanup_push(& thread_impl::finalize, threadImpl);

    thread::data::set(threadImpl->_data); // set thread-specific data

    CWT_VERIFY_ERRNO(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr));
    CWT_VERIFY_ERRNO(pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr));

    pthread_testcancel();

	thread * thisthread = threadImpl->_thread;

	// This is the last checking if thread instance is a live (not destroyed)
	// before call the 'run' routine.
	if (! thisthread) {
		// This order of deletion is important to prevent from double free.
		thread::data * d = threadImpl->_data;
		threadImpl->_data = nullptr;
		delete d;
		PFS_ASSERT_X(thisthread, _Tr("Thread destroyed, may be you forget to wait() it")); // thread detached (container already destroyed)
	}

   	thisthread->run(); // Do the job

    pthread_cleanup_pop(true); // calls finalize() routine

    return nullptr;
}

void thread_impl::finalize (void * arg)
{
	PFS_ASSERT(arg);

	thread_impl * threadImpl = static_cast<thread_impl *>(arg);

    pfs::auto_lock<> locker(& threadImpl->_mutex);

    PFS_ASSERT(threadImpl->_data->threadId);

    threadImpl->_state = ThreadFinishing;

    // Delete thread storage data
/*
    locker.unlock();
    QthreadStorageData::finish((void **)data);
    locker.relock();
*/

    threadImpl->_data->threadId = 0;
    threadImpl->_state = ThreadFinished;
    threadImpl->_threadFinished.wakeAll();
}

void thread::data::destroy (void * pdata)
{
	PFS_ASSERT(pdata);
	pthread_setspecific(threadKey, pdata);

	thread::data * threadData = static_cast<thread::data *>(pdata);
	threadData->threadImpl->_data = nullptr;
	delete threadData;

    pthread_setspecific(threadKey, nullptr);
}


/*
 *   |              thread                    pthread
 *   | terminate()     |                         |
 *   ----------------->|     pthread_cancel()    |
 *                     |------------------------>|
 *                                               |----
 *                                               |   | 1. Cancellation clean-up handlers are popped
 *                                               |   |    (in the reverse of the order in which they were pushed)
 *                                           ----|   |    and called (see pthread_cleanup_push(3))
 *       2. thread-specific data destructors |   |<---
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
void thread_impl::terminate ()
{
    pfs::auto_lock<> locker(& _mutex);

    if (_data && _data->threadId) {
    	_state = ThreadFinishing;
    	CWT_VERIFY_ERRNO(pthread_cancel(_data->threadId)); // thread termination error
    }
}

bool thread_impl::wait (uintegral_t timeout)
{
    pfs::auto_lock<> locker(& _mutex);

    if (_data) {
    	bool ok;
    	PFS_VERIFY_X((ok = (_data->threadId != pthread_self()))
    			, _Tr("Thread attempt to wait on itself"));
		if (!ok)
			return false;

		while (_state == ThreadRunning) {
			if (timeout == PFS_ULONG_MAX) {
				if (! _threadFinished.wait(_mutex)) {
					return false;
				}
			} else {
				if (! _threadFinished.wait(_mutex, timeout)) {
					return false;
				}
			}
		}
    }
    return true;
}


void thread_impl::setPriority(thread::priority_type priority)
{
    pfs::auto_lock<> locker(& _mutex);

    if (! PFS_VERIFY_X(_state == ThreadRunning
    		, _Tr("Unable to set thread priority: thread must be in running state"))) {
        return;
    }

    _priority = priority;

#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING

    int posixPolicy;
    sched_param param;

    int rc = CWT_VERIFY_ERRNO(pthread_getschedparam(_data->threadId, & posixPolicy, & param));
    if (rc)
        return;

    int posixPriority;
    bool ok = PFS_VERIFY_X(__map_to_posix_priority(priority, & posixPolicy, & posixPriority)
    		, _Tr("Can not map to POSIX priority"));
    if (! ok)
    	return;

    param.sched_priority = posixPriority;
    rc = CWT_VERIFY_ERRNO(pthread_setschedparam(_data->threadId, posixPolicy, & param));

#	ifdef SCHED_IDLE
    if (rc < 0 && posixPolicy == SCHED_IDLE && errno == EINVAL) {
    	// Set native thread's priority to minimal value
        pthread_getschedparam(_data->threadId, & posixPolicy, & param);
        param.sched_priority = sched_get_priority_min(posixPolicy);
        pthread_setschedparam(_data->threadId, posixPolicy, & param);
    }
#	endif
#endif // CWT_HAVE_THREAD_PRIORITY_SCHEDULING
}

inline struct timespec __make_timespec(time_t secs, integral_t nsecs)
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

void thread_impl::sleep (uintegral_t secs)
{
	struct timespec ts =  __make_timespec(secs, 0);
    __nanosleep(ts);
}

void thread_impl::msleep (uintegral_t msecs)
{
	struct timespec ts = __make_timespec(msecs / 1000, msecs % 1000 * 1000 * 1000);
    __nanosleep(ts);
}

void thread_impl::usleep (uintegral_t usecs)
{
	struct timespec ts = __make_timespec(usecs / 1000 / 1000, usecs % (1000*1000) * 1000);
    __nanosleep(ts);
}


void thread::exit ()
{
	pthread_exit(nullptr);
}

} // pfs
