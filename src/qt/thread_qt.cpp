/*
 * thread_qt.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: wladt
 */

#include "../../include/cwt/thread.hpp"
#include <QThread>

CWT_NS_BEGIN

class Thread::Impl : public QThread {
public:
	Impl(Thread *p) : QThread(), owner(p) {}
	virtual void run() { owner->run(); }
	Thread *owner;
};

static QThread::Priority __toQThreadPriority(Thread::Priority p)
{
	switch(p) {
	case Thread::IdlePriority: return QThread::IdlePriority;
	case Thread::LowestPriority: return QThread::LowestPriority;
	case Thread::LowPriority: return QThread::LowPriority;
	case Thread::NormalPriority: return QThread::NormalPriority;
	case Thread::HighPriority: return QThread::HighPriority;
	case Thread::HighestPriority: return QThread::HighestPriority;
	case Thread::TimeCriticalPriority: return QThread::TimeCriticalPriority;
	case Thread::InheritPriority: return QThread::InheritPriority;
	}
	return QThread::InheritPriority;
}

static Thread::Priority  __fromQThreadPriority(QThread::Priority p)
{
	switch(p) {
	case QThread::IdlePriority: return Thread::IdlePriority;
	case QThread::LowestPriority: return Thread::LowestPriority;
	case QThread::LowPriority: return Thread::LowPriority;
	case QThread::NormalPriority: return Thread::NormalPriority;
	case QThread::HighPriority: return Thread::HighPriority;
	case QThread::HighestPriority: return Thread::HighestPriority;
	case QThread::TimeCriticalPriority: return Thread::TimeCriticalPriority;
	case QThread::InheritPriority: return Thread::InheritPriority;
	}
	return Thread::InheritPriority;
}


Thread::Thread() : pimpl(new Impl(this)) {}
void	 Thread::exit (int returnCode) { pimpl->exit(returnCode); }
bool	 Thread::isFinished () const { return pimpl->isFinished(); }
bool	 Thread::isRunning () const { return pimpl->isRunning(); }
Thread::Priority Thread::priority () const { return  __fromQThreadPriority(pimpl->priority()); }
void     Thread::setPriority (Thread::Priority priority) { pimpl->setPriority(__toQThreadPriority(priority)); }
void	 Thread::setStackSize (uint_t stackSize) { pimpl->setStackSize(stackSize); }
uint_t   Thread::stackSize () const { return pimpl->stackSize(); }
bool	 Thread::wait (ulong_t time)
{
	if (time == CWT_ULONG_MAX)
		return pimpl->wait();

	return pimpl->wait((unsigned long)time);
}
void	 Thread::quit () { pimpl->quit(); }
void	 Thread::start (Thread::Priority priority) { pimpl->start(__toQThreadPriority(priority)); }
void	 Thread::terminate () { pimpl->terminate(); }
//Thread*  Thread::currentThread () { return dynamic_cast<Thread::Impl*>(Thread::Impl::currentThread())->owner; }

CWT_NS_END

