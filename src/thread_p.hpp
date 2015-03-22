#ifndef __PFS_THREAD_P_HPP_
#define __PFS_THREAD_P_HPP_

#include "pfs/mt.hpp"
#include "pfs/threadcv.hpp"
#include "pfs/atomic.hpp"
#include "pfs/vector.hpp"

namespace pfs {

class thread_data;

class thread_impl
{
	friend class thread;

public:
	thread_impl ();
    ~thread_impl ();

    void setPriority (thread::Priority prio);

    mutable pfs::mutex _mutex;
    atomic_integer<int> quitLockRef;

    bool _running;
    bool _finished;
    bool _isInFinish; //when in QThreadPrivate::finish
    bool _interruptionRequested;

    bool _exited;
    int  _returnCode;

    size_t _stackSize;
    thread::Priority _priority;

//    static thread * threadForId (int id);

    bool isMainThread () const;

#ifdef PFS_OS_UNIX
    pthread_t _threadId;
    thread_cv _threadDone;

    static void * start (void * arg);
    static void finish (void *);
#elif defined(PFS_OS_WIN)
    static unsigned int __stdcall start (void *);
    static void finish (void *, bool lockAnyway = true);

    Qt::HANDLE _handle;

    unsigned int _id;
    int _waiters;
    bool _terminationEnabled;
    bool _terminatePending;
#endif // PFS_OS_WIN

    thread_data * _data;

    void ref ()
    {
        quitLockRef.ref();
    }

    void deref ()
    {
        quitLockRef.deref();
    }
};

class thread_data
{
private:
    atomic_integer<int> _ref;

public:
    thread *   _thread;
#ifdef PFS_OS_UNIX
    pthread_t _threadId;
#elif defined(PFS_OS_WIN)
    Qt::HANDLE _threadId;
#endif
//    vector<void *> _tls;

    bool _quitNow;
    bool _canWait;

public:
	thread_data (int initialRefCount = 1);
    ~thread_data ();

    static thread_data * current ();
    //static void clearCurrentThreadData ();
    static thread_data * get2 (thread * thr)
    {
    	PFS_ASSERT_X(thr != 0, "pfs::thread_data::get2(): internal error");
    	return thr->_d.cast<thread_impl>()->_data;
    }

    void ref ();
    void deref ();
};

} // pfs

#endif // __PFS_THREAD_P_HPP_
