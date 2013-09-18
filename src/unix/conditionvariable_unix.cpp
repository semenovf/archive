/*
 * conditionvariable_unix.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include "../conditionvariable_p.hpp"
#include "../../include/cwt/logger.hpp"
#include <pthread.h>
#include <cerrno>

CWT_NS_BEGIN

class PosixConditionVariableImpl : public ConditionVariableImpl
{
public:
	PosixConditionVariableImpl();
	~PosixConditionVariableImpl();
	bool wait (Mutex * lockedMutex, ulong_t timeout = CWT_ULONG_MAX);
	void wakeOne ();
	void wakeAll ();

private:
	bool wait (ulong_t timeout);
	void calculateAbsTime(ulong_t timeout, timespec * ts);

private:
	Mutex          m_mutex;
	pthread_cond_t m_cond;
	int            m_waiters;
	int            m_wakeups;
};


inline PosixConditionVariableImpl::PosixConditionVariableImpl ()
	: ConditionVariableImpl()
	, m_mutex()
	, m_waiters(0)
	, m_wakeups(0)
{
	int rc = pthread_cond_init(& m_cond, nullptr);
	if (rc)
		Logger::error(rc, _Tr("Unable to initialize condition variable"));
}

inline PosixConditionVariableImpl::~PosixConditionVariableImpl ()
{
	int rc = pthread_cond_destroy(& m_cond);
	if (rc)
		Logger::error(rc, _Tr("Failed to destroy condition variable"));
}

inline void PosixConditionVariableImpl::wakeOne ()
{
	m_mutex.lock();
	m_wakeups = CWT_MIN(m_wakeups + 1, m_waiters);
	int rc = pthread_cond_signal(& m_cond);
	if (rc)
		Logger::error(rc, _Tr("Failed to signal a condition"));
	m_mutex.unlock();
}

inline void PosixConditionVariableImpl::wakeAll ()
{
	m_mutex.lock();
	m_wakeups = m_waiters;
	int rc = pthread_cond_broadcast(& m_cond);
	if (rc)
		Logger::error(rc, _Tr("Failed to broadcast a condition"));
	m_mutex.unlock();
}

inline void PosixConditionVariableImpl::calculateAbsTime(ulong_t timeout, timespec * ts)
{
    struct timeval tv;
    gettimeofday(& tv, nullptr);

    ts->tv_nsec = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;
    ts->tv_sec = tv.tv_sec + (timeout / 1000) + (ts->tv_nsec / 1000000000);
    ts->tv_nsec %= 1000000000;
}

// see section "Timed Condition Wait" in pthread_cond_timedwait(P) manual page.
//
bool PosixConditionVariableImpl::wait (ulong_t timeout)
{
    int rc;

    if (timeout != CWT_ULONG_MAX) {
    	timespec abstime;

    	calculateAbsTime(timeout, & abstime);

    	while (m_wakeups == 0 && rc == 0)
    		rc = pthread_cond_timedwait(& m_cond, m_mutex.handlePtr(), & abstime);
    } else {
    	while (m_wakeups == 0 && rc == 0)
    		rc = pthread_cond_wait(& m_cond, m_mutex.handlePtr());
    }

    CWT_ASSERT(m_waiters > 0);
    --m_waiters;

    if (rc == 0) {
        CWT_ASSERT(m_wakeups > 0);
        --m_wakeups;
    } else if (rc != ETIMEDOUT) {
    	Logger::error(rc, _Tr("Condition variable waiting failure"));
    }

    return (rc == 0);
}

bool PosixConditionVariableImpl::wait (Mutex * lockedMutex, ulong_t timeout)
{
    if (! lockedMutex)
        return false;

    m_mutex.lock();

    ++m_waiters;
    lockedMutex->unlock();
    bool r = wait(timeout);
    lockedMutex->lock();

    m_mutex.unlock();

    return r;
}

ConditionVariable::ConditionVariable () : pimpl (new PosixConditionVariableImpl) {}
bool ConditionVariable::wait (Mutex * lockedMutex, ulong_t timeout) { return pimpl->wait(lockedMutex, timeout); }
void ConditionVariable::wakeOne () { pimpl->wakeOne(); }
void ConditionVariable::wakeAll () { pimpl->wakeAll(); }


CWT_NS_END
