/*
 * threadcv_win.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include <pfs/mt.hpp>
#include <list> // FIXME need pfs::list implementation
//#include "pfs/logger.hpp"
//#include <cerrno>

// see Condition Variables (http://msdn.microsoft.com/en-us/library/windows/desktop/ms682052%28v=vs.85%29.aspx)
//
// XXX Valid since Vista and Windows Server 2008.
// Windows Server 2003 and Windows XP:  Condition variables are not supported.

namespace pfs {

struct tread_cv_event // QWaitConditionEvent
{
    int    _priority;
    bool   _wokenUp;
    HANDLE _event;

	tread_cv_event() : _priority(0), _wokenUp(false)
    {
        _event = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    ~tread_cv_event() { CloseHandle(_event); }
};

typedef std::list<tread_cv_event *> cv_event_queue_t;

class thread_cv_impl
{
public:
    pfs::mutex _mtx;
    cv_event_queue_t _queue;
    cv_event_queue_t _freeQueue;

    tread_cv_event * pre ();
    bool wait (tread_cv_event * cve, uintegral_t time);
    void post (tread_cv_event * cve, bool ret);
};

tread_cv_event * thread_cv_impl::pre ()
{
    _mtx.lock();
    tread_cv_event * r = nullptr;

    if (_freeQueue.empty()) {
         r = new tread_cv_event;
    } else {
    	r = _freeQueue.front();
    	_freeQueue.pop_front();
    }

    r->_priority = GetThreadPriority(GetCurrentThread());

    // insert 'r' into the queue (sorted by priority)
    //int index = 0;
    cv_event_queue_t::iterator it = _queue.begin();
    cv_event_queue_t::iterator itEnd = _queue.end();

    //for (; index < _queue.size(); ++index) {
    for (; it != itEnd; ++it) {
    	//tread_cv_event * current = _queue.at(index);
    	tread_cv_event * current = *it;

        if (current->_priority < r->_priority)
            break;
    }
    //_queue.insert(index, r);
    _queue.insert(it, r);
    _mtx.unlock();

    return r;
}

bool thread_cv_impl::wait (tread_cv_event * cve, uintegral_t time)
{
    // wait for the event
    bool r = false;

    switch (WaitForSingleObject(cve->_event, time)) {
    default: break;
    case WAIT_OBJECT_0:
        r = true;
        break;
    }
    return r;
}

void thread_cv_impl::post (tread_cv_event * cve, bool ret)
{
    _mtx.lock();

    // remove all occupancies of 'cve' from the queue
    _queue.remove(cve);

    ResetEvent(cve->_event);
    _freeQueue.push_back(cve);

    // wakeups delivered after the timeout should be forwarded to the next waiter
    if (!ret && cve->_wokenUp && !_queue.empty()) {
    	tread_cv_event * other = _queue.front();
        SetEvent(other->_event);
        other->_wokenUp = true;
    }

    _mtx.unlock();
}


thread_cv::thread_cv() : _d(new thread_cv_impl)
{}

static void delete_all_events (cv_event_queue_t & q)
{
    cv_event_queue_t::iterator it = q.begin();
    cv_event_queue_t::iterator itEnd = q.end();

    for (; it != itEnd; ++it) {
    	tread_cv_event * current = *it;
        if (current)
        	delete current;
    }
}

thread_cv::~thread_cv()
{
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
    if (!d->_queue.empty()) {
    	PFS_DEBUG(fprintf(stderr
    		, "thread_cv: destroyed while threads are still waiting\n"));
        delete_all_events(d->_queue);
    }
    delete_all_events(d->_freeQueue);
}

bool thread_cv::wait (pfs::mutex & lockedMutex, uintegral_t time)
{
// TODO
//    if (mtx->isRecursive()) {
//        qWarning("QWaitCondition::wait: Cannot wait on recursive mutexes");
//        return false;
//    }

    thread_cv_impl * d = _d.cast<thread_cv_impl>();
    tread_cv_event * cve = d->pre();
    lockedMutex.unlock();

    bool r = d->wait(cve, time);

    lockedMutex.lock();
    d->post(cve, r);

    return r;
}

//bool thread_cv::wait (QReadWriteLock * readWriteLock, unsigned long time)
//{
//    if (!readWriteLock || readWriteLock->d->accessCount == 0)
//        return false;
//    if (readWriteLock->d->accessCount < -1) {
//        qWarning("QWaitCondition: cannot wait on QReadWriteLocks with recursive lockForWrite()");
//        return false;
//    }
//
//    QWaitConditionEvent *wce = d->pre();
//    int previousAccessCount = readWriteLock->d->accessCount;
//    readWriteLock->unlock();
//
//    bool returnValue = d->wait(wce, time);
//
//    if (previousAccessCount < 0)
//        readWriteLock->lockForWrite();
//    else
//        readWriteLock->lockForRead();
//    d->post(wce, returnValue);
//
//    return returnValue;
//}

void thread_cv::wakeOne ()
{
    // wake up the first waiting thread in the queue
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
    auto_lock<>(& d->_mtx);

    cv_event_queue_t::iterator it = d->_queue.begin();
    cv_event_queue_t::iterator itEnd = d->_queue.end();

    //for (int i = 0; i < d->queue.size(); ++i) {
    for (; it != itEnd; ++it) {
    	tread_cv_event * current = *it; //d->queue.at(i);
        if (current->_wokenUp)
            continue;
        SetEvent(current->_event);
        current->_wokenUp = true;
        break;
    }
}

void thread_cv::wakeAll ()
{
    // wake up the all threads in the queue
	thread_cv_impl * d = _d.cast<thread_cv_impl>();
    auto_lock<>(& d->_mtx);

    cv_event_queue_t::iterator it = d->_queue.begin();
    cv_event_queue_t::iterator itEnd = d->_queue.end();

    //for (int i = 0; i < d->queue.size(); ++i) {
    for (; it != itEnd; ++it) {
    	tread_cv_event * current = *it; //d->queue.at(i);
        SetEvent(current->_event);
        current->_wokenUp = true;
    }
}

#ifdef __COMMENT__

class thread_cv_impl
{
public:
	impl ();
	~impl ();
	bool wait (pfs::mutex & lockedMutex);
	bool wait (pfs::mutex & lockedMutex, uintegral_t timeout);
	void wakeOne ();
	void wakeAll ();

private:
    QMutex mtx;
    EventQueue queue;
    EventQueue freeQueue;

    QWaitConditionEvent *pre();
    bool wait(QWaitConditionEvent *wce, unsigned long time);
    void post(QWaitConditionEvent *wce, bool ret);

	pfs::mutex         m_mutex;
	CONDITION_VARIABLE m_cond;
	int                m_waiters;
	int                m_wakeups;
};

/*
class WinThreadCVImpl : public ThreadCVImpl
{
public:
	WinThreadCVImpl();
	~WinThreadCVImpl();
	bool wait (pfs::mutex * lockedMutex, uintegral_t timeout = PFS_ULONG_MAX);
	void wakeOne ();
	void wakeAll ();

private:
	bool wait (uintegral_t timeout);

private:
};
*/


inline thread_cv::impl::impl ()
	: m_mutex()
	, m_waiters(0)
	, m_wakeups(0)
{
	InitializeConditionVariable(& m_cond);
}

inline thread_cv::impl::~impl ()
{
}

inline void thread_cv::impl::wakeOne ()
{
	m_mutex.lock();
	m_wakeups = PFS_MIN(m_wakeups + 1, m_waiters);
	WakeConditionVariable(& m_cond);
	m_mutex.unlock();
}

inline void thread_cv::impl::wakeAll ()
{
	m_mutex.lock();
	m_wakeups = m_waiters;
	WakeAllConditionVariable(& m_cond);
	m_mutex.unlock();
}

bool thread_cv::impl::wait (pfs::mutex & lockedMutex, uintegral_t timeout)
{
    m_mutex.lock();

    ++m_waiters;

    // Sleeps on the specified condition variable and releases the specified critical section as an atomic operation.
    // TODO need to see into SleepConditionVariableCS call (and compare with POSIX implementation )
    BOOL r = SleepConditionVariableCS(& m_cond
    		, lockedMutex.handlePtr()
    		, timeout == PFS_ULONG_MAX ? INFINITE : DWORD(timeout));

    m_mutex.unlock();

    return r != 0;
}

thread_cv::thread_cv () : _pimpl (new thread_cv::impl) {}
bool thread_cv::wait (pfs::mutex & lockedMutex, uintegral_t timeout) { return _pimpl->wait(lockedMutex, timeout); }
void thread_cv::wakeOne () { _pimpl->wakeOne(); }
void thread_cv::wakeAll () { _pimpl->wakeAll(); }

#endif

} // pfs
