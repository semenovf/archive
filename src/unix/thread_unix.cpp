/*
 * thread_unix.cpp
 *
 *  Created on: Sep 16, 2013
 *      Author: wladt
 */

#include <pfs/mt.hpp>
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

//#define CWT_TRACE_ENABLE
#include "../../include/cwt/trace.hpp"

namespace cwt {

#ifdef CWT_HAVE_TLS
	__thread thread::data * thread::data::currentthreadData = nullptr;
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


thread::impl::impl (/*thread::data * threadData*/)
	: _mutex()
	, _stackSize(0)
	, _priority(thread::InheritPriority)
	, _state(ThreadNotRunning)
	, _threadFinished()
	, _thread(nullptr)
	, _data(nullptr)
{
	CWT_TRACE_METHOD();
}

thread::impl::~impl()
{
	CWT_TRACE_METHOD();

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

thread::thread() : _pimpl(new thread::impl)
{
	CWT_TRACE_METHOD();
	_pimpl->_thread = this;
}

thread::~thread ()
{
	CWT_TRACE_METHOD();

	pfs::auto_lock<> locker(& _pimpl->_mutex);
    if (_pimpl->_state == ThreadFinishing) {
        locker.unlock();
        wait();
        locker.tryLock();
    }

    if (_pimpl->_state == ThreadRunning) {
    	PFS_WARN(_Tr("Attempt to destroy thread while it is still running"));
    	locker.unlock();
    	terminate();
    	wait();
    	locker.tryLock();
    }

	_pimpl->_thread = nullptr; // detach thread
}


bool thread::isFinished () const
{
	pfs::auto_lock<>(& _pimpl->_mutex);
    return _pimpl->_state == ThreadFinished || _pimpl->_state == ThreadFinishing;
}

bool thread::isRunning () const
{
	pfs::auto_lock<>(& _pimpl->_mutex);
    return _pimpl->_state == ThreadRunning;
}

thread::priority_type thread::priority () const
{
	pfs::auto_lock<>(& _pimpl->_mutex);
    return _pimpl->_priority;
}

size_t thread::stackSize () const
{
	pfs::auto_lock<>(& _pimpl->_mutex);
    return _pimpl->_stackSize;
}

void thread::yieldCurrentThread ()
{
	sched_yield();
}

bool thread::impl::setStackSize (pthread_attr_t & attr, size_t stackSize)
{
	int rc = 0;

    if (stackSize > 0) {
    	size_t page_size = getpagesize();

#ifdef PTHREAD_STACK_MIN
    	if (stackSize < PTHREAD_STACK_MIN)
    		stackSize = PTHREAD_STACK_MIN;
#endif

    	stackSize = ((stackSize + page_size - 1) / page_size) * page_size;
    	PFS_VERIFY_ERRNO(!(rc = pthread_attr_setstacksize(& attr, stackSize)), rc);
    }

    _stackSize = 0;
    PFS_VERIFY_ERRNO(!(rc = pthread_attr_getstacksize(& attr, & _stackSize)), rc);

    return (rc == 0);
}


void thread::impl::start (thread::priority_type priority, size_t stackSize)
{
	CWT_TRACE_METHOD();
	int rc = 0;
	pfs::auto_lock<> locker(& _mutex);

	if (_state == ThreadFinishing)
		_threadFinished.wait(*locker.handlePtr());

	if (_state == ThreadRunning ) {
		return;
	}

	_state = ThreadRunning;

	pthread_attr_t attr;

	while (true) {
		PFS_VERIFY_ERRNO(!(rc = pthread_attr_init(& attr)), rc);
		if (rc)
			break;

		// Only threads that are created as joinable can be joined.
		// If a thread is created as detached, it can never be joined.
		PFS_VERIFY_ERRNO(!(rc = pthread_attr_setdetachstate(& attr, PTHREAD_CREATE_DETACHED)), rc);
		if (rc)
			break;

		_priority = priority;

		if (priority == thread::InheritPriority) {
			PFS_VERIFY_ERRNO(!(rc = pthread_attr_setinheritsched(& attr, PTHREAD_INHERIT_SCHED)), rc);
			if (rc)
				break;
		} else {
#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING
			int posixPolicy;
			PFS_VERIFY_ERRNO(!(rc = pthread_attr_getschedpolicy(& attr, & posixPolicy)), rc);

            if (rc) {
            	rc = 0; // ignore this error
            	break;
            }

            int posixPriority;

            if (! __map_to_posix_priority(priority, & posixPolicy, & posixPriority)) {
            	PFS_WARN(_Tr("Can not map to POSIX priority"));
            	break;
            }

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

			PFS_VERIFY_ERRNO(!(rc = pthread_create(& pth, & attr, & thread::impl::thread_routine, this)), rc);

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
void * thread::impl::thread_routine (void * arg)
{
	CWT_TRACE_FUNC();
	PFS_ASSERT(arg);

	// If a cancellation request is received,
    // it is blocked until cancelability is enabled.
    PFS_VERIFY(!pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr));

	thread::impl * threadImpl = static_cast<thread::impl *>(arg);
	threadImpl->_data->threadId = pthread_self();

    pthread_cleanup_push(& thread::impl::finalize, threadImpl);

    thread::data::set(threadImpl->_data); // set thread-specific data

    PFS_VERIFY(0 == pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr));
    PFS_VERIFY(0 == pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr));

    pthread_testcancel();

	thread * thisthread = threadImpl->_thread;

	// This is the last checking if thread instance is a live (not destroyed)
	// before call the 'run' routine.
	if (! thisthread) {
		// This order of deletion is important to prevent from double free.
		thread::data * d = threadImpl->_data;
		threadImpl->_data = nullptr;
		delete d;
		PFS_ASSERT_X(thisthread, _Tr("thread destroyed, may be you forget to wait() it")); // thread detached (container already destroyed)
	}

   	thisthread->run(); // Do the job

    pthread_cleanup_pop(true); // calls finalize() routine

    return nullptr;
}

void thread::impl::finalize (void * arg)
{
	CWT_TRACE_FUNC();
	PFS_ASSERT(arg);

	thread::impl * threadImpl = static_cast<thread::impl *>(arg);

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
	CWT_TRACE_FUNC();
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
void thread::impl::terminate ()
{
//	CWT_TRACE_METHOD();
    pfs::auto_lock<> locker(& _mutex);

    if (_data && _data->threadId) {
    	int rc = 0;
    	_state = ThreadFinishing;
    	PFS_VERIFY_ERRNO(!(rc = pthread_cancel(_data->threadId)), rc); // thread termination error
    }
}

bool thread::impl::wait (ulong_t timeout)
{
//	CWT_TRACE_METHOD();

    pfs::auto_lock<> locker(& _mutex);

    if (_data) {
		if (_data->threadId == pthread_self()) {
			PFS_ERROR(_Tr("thread attempt to wait on itself"));
			return false;
		}

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


void thread::impl::setPriority(thread::priority_type priority)
{
    pfs::auto_lock<> locker(& _mutex);

    if (_state != ThreadRunning) {
    	PFS_WARN("Unable to set thread priority: thread must be in running state");
        return;
    }

    _priority = priority;

#ifdef CWT_HAVE_THREAD_PRIORITY_SCHEDULING

    int posixPolicy;
    sched_param param;

    int rc;

    PFS_VERIFY_ERRNO(!(rc = pthread_getschedparam(_data->threadId, & posixPolicy, & param)), rc);
    if (rc)
        return;

    int posixPriority;

    if (! __map_to_posix_priority(priority, & posixPolicy, & posixPriority)) {
    	PFS_WARN(_Tr("Can not map to POSIX priority"));
    	return;
    }

    param.sched_priority = posixPriority;
    PFS_VERIFY_ERRNO(!(rc = pthread_setschedparam(_data->threadId, posixPolicy, & param)), rc);

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

void thread::impl::sleep(ulong_t secs)
{
	struct timespec ts =  __make_timespec(secs, 0);
    __nanosleep(ts);
}

void thread::impl::msleep(ulong_t msecs)
{
	struct timespec ts = __make_timespec(msecs / 1000, msecs % 1000 * 1000 * 1000);
    __nanosleep(ts);
}

void thread::impl::usleep(ulong_t usecs)
{
	struct timespec ts = __make_timespec(usecs / 1000 / 1000, usecs % (1000*1000) * 1000);
    __nanosleep(ts);
}


void thread::exit ()
{
	pthread_exit(nullptr);
}

} // cwt
