/*
 * threadcv_win.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include "../threadcv_p.hpp"
#include "pfs/logger.hpp"
#include <cerrno>

// see Condition Variables (http://msdn.microsoft.com/en-us/library/windows/desktop/ms682052%28v=vs.85%29.aspx)
//
// XXX Valid since Vista and Windows Server 2008.
// Windows Server 2003 and Windows XP:  Condition variables are not supported.

namespace pfs {

class thread_cv::impl
{
public:
	impl ();
	~impl ();
	bool wait (pfs::mutex & lockedMutex);
	bool wait (pfs::mutex & lockedMutex, uintegral_t timeout);
	void wakeOne ();
	void wakeAll ();

private:
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

} // pfs
