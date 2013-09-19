/**
 * @file   mt.hpp
 * @author wladt
 * @date   Mar 1, 2013 4:11:37 PM
 *
 * @brief
 */

#ifndef __CWT_MT_HPP__
#define __CWT_MT_HPP__

#include <cwt/cwt.h>
#include <cwt/mt.h>

CWT_NS_BEGIN

#if defined(CWT_SINGLE_THREADED)
#	define CWT_DEFAULT_MT_POLICY cwt_single_threaded

class DLL_API cwt_single_threaded {
public:
	cwt_single_threaded() { ; }
	virtual ~cwt_single_threaded() { ; }
	virtual void lock()	{ ;	}
	virtual void unlock() {	; }

	cwt_mutex_t * handlePtr() const { return nullptr; }
};

typedef cwt_single_threaded mt_policy_t;
typedef cwt_single_threaded Mutex;

#elif defined(CWT_WIN32_THREADS) || defined(CWT_POSIX_THREADS)

class DLL_API multi_threaded_global
{
public:
	multi_threaded_global() {
		static bool is_initialised = false;

		if(!is_initialised) {
			mt_init(g_mutex);
			is_initialised = true;
		}
	}

	multi_threaded_global(const multi_threaded_global&) { ; }
	virtual ~multi_threaded_global() { mt_destroy(g_mutex); }

	virtual void lock()    { mt_lock(g_mutex); }
	virtual void tryLock() { mt_try_lock(g_mutex); }
	virtual void unlock()  { mt_unlock(g_mutex); }

	static multi_threaded_global & getPolicy() { static multi_threaded_global mtp; return mtp; }
	static cwt_mutex_t * handlePtr() { return & g_mutex; }

private:
	static mt_def(g_mutex);
};

class DLL_API multi_threaded_local
{
public:
	multi_threaded_local() { mt_init(m_mutex); }
	//multi_threaded_local(const multi_threaded_local&) { InitializeCriticalSection(&m_critsec); }
	virtual ~multi_threaded_local() { mt_destroy(m_mutex); }

	virtual void lock()    { mt_lock(m_mutex); }
	virtual void tryLock() { mt_try_lock(m_mutex); }
	virtual void unlock()  { mt_unlock(m_mutex); }

	cwt_mutex_t * handlePtr() const { return & m_mutex; }
private:
	mt_def(m_mutex);
};

#	define CWT_DEFAULT_MT_POLICY multi_threaded_local

typedef multi_threaded_local mt_policy_t;
typedef multi_threaded_local Mutex;
#else
// catched in mt.h
#endif

/**
 *
 * @code
 * AutoLock<>()
 */
template<class mt_policy = CWT_DEFAULT_MT_POLICY>
class AutoLock
{
public:
	AutoLock(mt_policy * mtx) : m_mutex(mtx) { m_mutex->lock(); }
	~AutoLock() { m_mutex->unlock(); }

	mt_policy * handlePtr() const { return m_mutex; }
private:
	mt_policy * m_mutex;
};

template<class mt_policy = CWT_DEFAULT_MT_POLICY>
class AutoUnlock
{
public:
	AutoUnlock(mt_policy * mtx) : m_mutex(mtx) { m_mutex->unlock(); }
	~AutoUnlock() { m_mutex->lock(); }

	mt_policy * handlePtr() const { return m_mutex; }
private:
	mt_policy * m_mutex;
};

class AutoLockGlobal
{
public:
#ifndef CWT_SINGLE_THREADED
	AutoLockGlobal() { multi_threaded_global::getPolicy().lock(); }
	~AutoLockGlobal() { multi_threaded_global::getPolicy().unlock(); }
#else
	AutoLockGlobal() { ; }
	~AutoLockGlobal() { ; }
#endif
};

//
// The AutoLock class is a helper that will hold the lock while in scope.
//
#define CWT_IMPLEMENT_LOCKING(ClassName)                        \
protected:                                                      \
	class AutoLock                                              \
	{                                                           \
	public:                                                     \
		AutoLock(ClassName * base)                              \
			: m_base(base) { m_base->m_mutex.lock(); }          \
		~AutoLock() { m_base->m_mutex.unlock(); }               \
		Mutex * mutexPtr() const { return & m_base->m_mutex; }  \
	private:                                                    \
		ClassName * m_base;                                     \
	};                                                          \
private:                                                        \
	Mutex m_mutex;

CWT_NS_END

#endif /* __CWT_MT_HPP__ */
