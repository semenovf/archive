#include "pfs/thread.hpp"
#include "pfs/platform.hpp"
#include "../thread_p.hpp"
#include <pfs/pp/utility.h>

#include <process.h>
#include <iostream>

namespace pfs {

static DWORD current_thread_data_tls_index = TLS_OUT_OF_INDEXES;

static struct main_thread
{
	HANDLE _threadHandle;
	main_thread () { _threadHandle = GetCurrentThread(); }
	bool equalsTo (HANDLE otherThreadHandle) const
	{
		return _threadHandle == otherThreadHandle;
	}
} __main_thread;

static void create_tls ()
{
    if (current_thread_data_tls_index != TLS_OUT_OF_INDEXES)
        return;
    static mutex mtx;
    pfs::auto_lock<mutex> locker(& mtx);
    current_thread_data_tls_index = TlsAlloc();
}

static void free_tls()
{
    if (current_thread_data_tls_index != TLS_OUT_OF_INDEXES) {
        TlsFree(current_thread_data_tls_index);
        current_thread_data_tls_index = TLS_OUT_OF_INDEXES;
    }
}
PFS_AUTO_DTOR_FUNCTION(free_tls)

thread_data * thread_data::current ()
{
    create_tls();
    thread_data * threadData = reinterpret_cast<thread_data *>(::TlsGetValue(current_thread_data_tls_index));
    PFS_ASSERT(threadData);
    return threadData;
}


void thread_data::clear_current_thread_data ()
{
    TlsSetValue(current_thread_data_tls_index, 0);
}

bool thread_impl::is_main_thread () const
{
	return __main_thread.equalsTo(_threadHandle);
}

void thread_impl::setPriority (thread::priority threadPriority)
{
    // copied from start() with a few modifications:
    int prio;
    _priority = threadPriority;

    switch (_priority) {
    case thread::idle_priority:
        prio = THREAD_PRIORITY_IDLE;
        break;

    case thread::LowestPriority:
        prio = THREAD_PRIORITY_LOWEST;
        break;

    case thread::LowPriority:
        prio = THREAD_PRIORITY_BELOW_NORMAL;
        break;

    case thread::NormalPriority:
        prio = THREAD_PRIORITY_NORMAL;
        break;

    case thread::HighPriority:
        prio = THREAD_PRIORITY_ABOVE_NORMAL;
        break;

    case thread::HighestPriority:
        prio = THREAD_PRIORITY_HIGHEST;
        break;

    case thread::TimeCriticalPriority:
        prio = THREAD_PRIORITY_TIME_CRITICAL;
        break;

    case thread::InheritPriority:
    default:
    	PFS_DEBUG(std::cerr
    			<< "pfs::thread_impl::setPriority(): argument cannot be InheritPriority"
    			<< std::endl);
        return;
    }

    if (!SetThreadPriority(_threadHandle, prio)) {
    	PFS_DEBUG(std::cerr
    			<< "pfs::thread_impl::setPriority(): failed to set thread priority: "
    			<< platform::strerror(static_cast<int>(GetLastError()))
    			<< std::endl);
    }
}

unsigned int __stdcall thread_impl::start (void * arg)
{
    thread * thr = reinterpret_cast<thread *>(arg);
    thread_data * data = thread_data::get2(thr);

    create_tls();
    TlsSetValue(current_thread_data_tls_index, data);
    data->_threadHandle = GetCurrentThread();

    thread::set_termination_enabled(false);

    {
    	thread_impl * d = thr->_d.cast<thread_impl>();
    	pfs::auto_lock<> locker(& d->_mutex);
        data->_quitNow = d->_exited;
    }

    thread::set_termination_enabled(true);
    thr->run();

    finish(arg);
    return 0;
}

void thread_impl::finish (void * arg, bool /*lockAnyway*/)
{
    thread * thr = reinterpret_cast<thread *>(arg);
    thread_impl * d = thr->_d.cast<thread_impl>();

    pfs::auto_lock<> locker(& d->_mutex);

    d->_isInFinish = true;
    d->_priority = thread::InheritPriority;

    d->_running = false;
    d->_finished = true;
    d->_isInFinish = false;
    d->_interruptionRequested = false;

    if (!d->_waiters) {
        CloseHandle(d->_threadHandle);
        d->_threadHandle = 0;
    }

    d->_threadId = 0;
}

bool thread::wait (uintmax_t timeout)
{
	thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

    if (d->_threadId == GetCurrentThreadId()) {
    	PFS_DEBUG(std::cerr
    			<< "pfs::thread::wait(): thread tried to wait on itself"
    			<< std::endl);
        return false;
    }

    if (d->_finished || !d->_running)
        return true;

    ++d->_waiters;
    locker.mutexRef().unlock();

    bool ret = false;

    DWORD dwMilliseconds = (timeout <= PFS_UINT32_MAX
    		? static_cast<DWORD>(timeout)
    		: INFINITE);

    switch (WaitForSingleObject(d->_threadHandle, dwMilliseconds)) {
    case WAIT_OBJECT_0:
        ret = true;
        break;
    case WAIT_FAILED:
    	PFS_DEBUG(std::cerr
    			<< "pfs::thread::wait(): thread wait failure: "
    			<< strerror(static_cast<int>(GetLastError()))
    			<< std::endl);
        break;
    case WAIT_ABANDONED:
    case WAIT_TIMEOUT:
    default:
        break;
    }

    locker.mutexRef().lock();
    --d->_waiters;

    if (ret && !d->_finished) {
        // thread was terminated by someone else
        thread_impl::finish(this, false);
    }

    if (d->_finished && !d->_waiters) {
        CloseHandle(d->_threadHandle);
        d->_threadHandle = 0;
    }

    return ret;
}

void thread::set_termination_enabled (bool enabled)
{
    thread * thr = current_thread();

    PFS_ASSERT_X(thr != 0
    		, _Tr("pfs::thread::setTerminationEnabled(): current thread was not started with pfs::thread"));


    thread_impl * d = thr->_d.cast<thread_impl>();
    pfs::auto_lock<> locker(& d->_mutex);

    d->_terminationEnabled = enabled;
    if (enabled && d->_terminatePending) {
        thread_impl::finish(thr, false);
        locker.unlock(); // don't leave the mutex locked!
        _endthreadex(0);
    }
}

int thread::ideal_thread_count ()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

void thread::yield_current_thread ()
{
    SwitchToThread();
}

void thread::sleep (uintmax_t secs)
{
    ::Sleep(integral_cast_check<DWORD, uintmax_t>(secs * 1000));
}

void thread::msleep (uintmax_t msecs)
{
    ::Sleep(integral_cast_check<DWORD, uintmax_t>(msecs));
}

void thread::usleep (uintmax_t usecs)
{
    ::Sleep(integral_cast_check<DWORD, uintmax_t>((usecs / 1000) + 1));
}

void thread::start (priority priority)
{
    thread_impl * d = _d.cast<thread_impl>();
    pfs::auto_lock<> locker(& d->_mutex);

    if (d->_isInFinish) {
        locker.unlock();
        wait();
        locker.lock();
    }

    if (d->_running)
        return;

    d->_running = true;
    d->_finished = false;
    d->_exited = false;
    d->_returnCode = 0;
    d->_interruptionRequested = false;

    /*
      NOTE: we create the thread in the suspended state, set the
      priority and then resume the thread.

      since threads are created with normal priority by default, we
      could get into a case where a thread (with priority less than
      NormalPriority) tries to create a new thread (also with priority
      less than NormalPriority), but the newly created thread preempts
      its 'parent' and runs at normal priority.
    */
    d->_threadHandle = reinterpret_cast<HANDLE>(_beginthreadex(NULL
    		, d->_stackSize
    		, thread_impl::start
    		, this
    		, CREATE_SUSPENDED
    		, & d->_threadId));

    if (!d->_threadHandle) {
        PFS_DEBUG(std::cerr
        		<< "thread::start: failed to create thread: "
        		<< platform::strerror(errno));

        d->_running = false;
        d->_finished = true;
        return;
    }

    int prio;
    d->_priority = priority;

    switch (d->_priority) {
    case idle_priority:
        prio = THREAD_PRIORITY_IDLE;
        break;

    case LowestPriority:
        prio = THREAD_PRIORITY_LOWEST;
        break;

    case LowPriority:
        prio = THREAD_PRIORITY_BELOW_NORMAL;
        break;

    case NormalPriority:
        prio = THREAD_PRIORITY_NORMAL;
        break;

    case HighPriority:
        prio = THREAD_PRIORITY_ABOVE_NORMAL;
        break;

    case HighestPriority:
        prio = THREAD_PRIORITY_HIGHEST;
        break;

    case TimeCriticalPriority:
        prio = THREAD_PRIORITY_TIME_CRITICAL;
        break;

    case InheritPriority:
    default:
        prio = GetThreadPriority(GetCurrentThread());
        break;
    }

    if (!SetThreadPriority(d->_threadHandle, prio)) {
        PFS_DEBUG(std::cerr
        		<< "thread::start: failed to set thread priority: "
        		<< platform::strerror(static_cast<int>(GetLastError())));
    }

    if (ResumeThread(d->_threadHandle) == (DWORD) -1) {
        PFS_DEBUG(std::cerr
        		<< "thread::start: failed to resume new thread: "
        		<< platform::strerror(static_cast<int>(GetLastError())));
    }
}

void thread::terminate ()
{
    thread_impl * d = _d.cast<thread_impl>();
    pfs::auto_lock<> locker(& d->_mutex);

    if (!d->_running)
        return;

    if (!d->_terminationEnabled) {
        d->_terminatePending = true;
        return;
    }

    TerminateThread(d->_threadHandle, 0);
    thread_impl::finish(this, false);
}


//Qt::HANDLE QThread::currentThreadId() Q_DECL_NOTHROW
//{
//    return reinterpret_cast<Qt::HANDLE>(GetCurrentThreadId());
//}

} // pfs

