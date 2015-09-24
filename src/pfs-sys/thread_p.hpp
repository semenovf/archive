#ifndef __PFS_THREAD_P_HPP_
#define __PFS_THREAD_P_HPP_

#include "pfs/mutex.hpp"
#include "pfs/threadcv.hpp"
#include "pfs/atomic.hpp"
#include "pfs/vector.hpp"

namespace pfs {

class thread_data;

class thread_impl
{
	friend class thread;

private:
    thread_impl (const thread_impl &);
    thread_impl & operator = (const thread_impl &);

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

#ifdef PFS_CC_GCC

    pthread_t _threadHandle;
    thread_cv _threadDone;

    static void * start (void * arg);
    static void finish (void *);

#elif defined(PFS_CC_MSVC)

    static unsigned int __stdcall start (void *);
    static void finish (void *, bool lockAnyway = true);

    HANDLE _threadHandle;
    unsigned int _threadId;
    int _waiters;
    bool _terminationEnabled;
    bool _terminatePending;

#else
#   error "Unsupported platform"
#endif // PFS_CC_MSVC

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
#ifdef PFS_CC_GCC
    pthread_t _threadHandle;
#elif defined(PFS_CC_MSVC)
    HANDLE _threadHandle;
#endif
//    vector<void *> _tls;

    bool _quitNow;
    bool _canWait;

public:
	thread_data (int initialRefCount = 1);
    ~thread_data ();

    static thread_data * current ();
    static void clearCurrentThreadData ();
    static thread_data * get2 (thread * thr)
    {
    	PFS_ASSERT_X(thr != 0, "pfs::thread_data::get2(): internal error");
    	return thr->_d->_data;
    }

    void ref ();
    void deref ();
};

} // pfs

#endif // __PFS_THREAD_P_HPP_
