#include "pfs/thread.hpp"
#include "pfs/mt.hpp"
#include "thread_p.hpp"
#include <iostream>

namespace pfs {

/**
 * @fn void thread::terminate ()
 *
 * @details Warning: This function is dangerous and its use is discouraged.
 * 		The thread can be terminated at any point in its code path.
 * 		Threads can be terminated while modifying data.
 * 		There is no chance for the thread to clean up after itself, unlock any held mutexes, etc.
 * 		In short, use this function only if absolutely necessary.
 */


thread_data::thread_data (int initialRefCount)
    : _ref(initialRefCount), _thread(0), _threadHandle(0),
      _quitNow(false), _canWait(true)
{}

thread_data::~thread_data()
{
    PFS_ASSERT(_ref.load() == 0);

    thread * t = _thread;
    _thread = 0;
    delete t;
}

void thread_data::ref()
{
    (void) _ref.ref();
    PFS_ASSERT(_ref.load() != 0);
}

void thread_data::deref()
{
    if (!_ref.deref())
        delete this;
}

thread_impl::thread_impl ()
    : _running(false)
	, _finished(false)
	, _isInFinish(false)
	, _interruptionRequested(false)
    , _exited(false)
	, _returnCode(-1)
	, _stackSize(0)
	, _priority(thread::InheritPriority)
	, _data(/*d*/0)
{
#if defined (PFS_CC_GCC)
    _threadHandle = 0;
#elif defined (PFS_CC_MSVC)
    _threadHandle = 0;
    _threadId = 0;
    _waiters = 0;
#endif

#if defined (PFS_CC_MSVC)
    _terminationEnabled = true;
    _terminatePending = false;
#endif

    if (!_data)
        _data = new thread_data;
}

thread_impl::~thread_impl()
{
    _data->deref();
}

thread * thread::currentThread()
{
    thread_data * data = thread_data::current();
    PFS_ASSERT(data != 0);
    return data->_thread;
}

thread::thread ()
    : _d(new thread_impl)
{
    _d.cast<thread_impl>()->_data->_thread = this;
}

thread::~thread()
{
	thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

	if (d->_isInFinish) {
		locker.unlock();
		wait();
		locker.lock();
	}

	if (d->_running && !d->_finished) {
		PFS_DEBUG(std::cerr
				<< "pfs::thread: destroyed while thread is still running\n");
	}

	d->_data->_thread = nullptr;
}

bool thread::isFinished() const
{
	const thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);
    return d->_finished || d->_isInFinish;
}

bool thread::isRunning() const
{
	const thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);
    return d->_running && !d->_isInFinish;
}

void thread::setStackSize (size_t stackSize)
{
	thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

    PFS_ASSERT_X(!d->_running
    		, _Tr("pfs:thread::setStackSize(): cannot change stack size while the thread is running"));
    d->_stackSize = stackSize;
}

size_t thread::stackSize() const
{
	const thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

	return d->_stackSize;
}

void thread::setPriority (Priority priority)
{
	thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

	if (!d->_running) {
		PFS_DEBUG(std::cerr
				<< "pfs::thread::setPriority(): cannot set priority, thread is not running\n");
        return;
    }
    d->setPriority(priority);
}

thread::Priority thread::priority() const
{
	const thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

    // mask off the high bits that are used for flags
    return Priority(d->_priority & 0xffff);
}

void thread::requestInterruption ()
{
	thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

	if (!d->_running || d->_finished || d->_isInFinish)
        return;

    if (d->isMainThread()) {
        PFS_DEBUG(std::cerr
        		<< "pfs::thread::requestInterruption has no effect on the main thread\n");
        return;
    }
    d->_interruptionRequested = true;
}

bool thread::isInterruptionRequested() const
{
	const thread_impl * d = _d.cast<thread_impl>();
	pfs::auto_lock<> locker(& d->_mutex);

	if (!d->_running || d->_finished || d->_isInFinish)
        return false;
    return d->_interruptionRequested;
}

} // pfs
