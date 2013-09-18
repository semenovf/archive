/*
 * conditionvariable_win.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include "../conditionvariable_p.hpp"
#include "../../include/cwt/logger.hpp"
#include <pthread.h>
#include <cerrno>

// see Condition Variables (http://msdn.microsoft.com/en-us/library/windows/desktop/ms682052%28v=vs.85%29.aspx)
//
// XXX Valid since Vista and Windows Server 2008.
// Windows Server 2003 and Windows XP:  Condition variables are not supported.

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

private:
	Mutex              m_mutex;
	CONDITION_VARIABLE m_cond;
	int                m_waiters;
	int                m_wakeups;
};


inline PosixConditionVariableImpl::PosixConditionVariableImpl ()
	: ConditionVariableImpl()
	, m_mutex()
	, m_waiters(0)
	, m_wakeups(0)
{
	InitializeConditionVariable(& m_cond);
}

inline PosixConditionVariableImpl::~PosixConditionVariableImpl ()
{
}

inline void PosixConditionVariableImpl::wakeOne ()
{
	m_mutex.lock();
	m_wakeups = CWT_MIN(m_wakeups + 1, m_waiters);
	WakeConditionVariable(& m_cond);
	m_mutex.unlock();
}

inline void PosixConditionVariableImpl::wakeAll ()
{
	m_mutex.lock();
	m_wakeups = m_waiters;
	WakeAllConditionVariable(& m_cond);
	m_mutex.unlock();
}

bool PosixConditionVariableImpl::wait (Mutex * lockedMutex, ulong_t timeout)
{
    if (! lockedMutex)
        return false;

    m_mutex.lock();

    ++m_waiters;

    // Sleeps on the specified condition variable and releases the specified critical section as an atomic operation.
    // TODO need to see into SleepConditionVariableCS call (and compare with POSIX implementation )
    BOOL r = SleepConditionVariableCS(& m_cond
    		, lockedMutex->handlePtr()
    		, timeout == CWT_ULONG_MAX ? INFINITE : DWORD(timeout));

    m_mutex.unlock();

    return r != 0;
}

ConditionVariable::ConditionVariable () : pimpl (new PosixConditionVariableImpl) {}
bool ConditionVariable::wait (Mutex * lockedMutex, ulong_t timeout) { return pimpl->wait(locakedMutex, timeout); }
void ConditionVariable::wakeOne () { pimpl->wakeOne(); }
void ConditionVariable::wakeAll () { pimpl->wakeAll(); }


CWT_NS_END
