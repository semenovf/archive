/**
 * @file   mt.hpp
 * @author wladt
 * @date   Mar 1, 2013 4:11:37 PM
 *
 * @brief
 */

#ifndef __PFS_MT_HPP__
#define __PFS_MT_HPP__

#error "Use pfs/mutex.hpp instead"

#if __DEPRECATED__

#include <pfs/bits/mutex.h>

namespace pfs {

#if defined(PFS_SINGLE_THREADED)
#	define PFS_DEFAULT_MT_POLICY pfs::single_threaded

class DLL_API single_threaded {
public:
	single_threaded() { ; }
	virtual ~single_threaded() { ; }
	void lock()	{ ;	}
	void tryLock() { ; }
	void unlock() {	; }

	mutex_t * handlePtr() const { return nullptr; }
};

typedef single_threaded mt_policy_t;
typedef single_threaded mutex;

#elif defined(PFS_WIN32_THREADS) || defined(PFS_POSIX_THREADS)

class DLL_API multi_threaded_local
{
public:
	multi_threaded_local() { mt_init(_mutex); }
	//multi_threaded_local(const multi_threaded_local&) { InitializeCriticalSection(&m_critsec); }
	virtual ~multi_threaded_local() { mt_destroy(_mutex); }

	void lock()    { mt_lock(_mutex); }
	void tryLock() { mt_try_lock(_mutex); }
	void unlock()  { mt_unlock(_mutex); }

	mutex_t * handlePtr() { return & _mutex; }
private:
	mt_def(_mutex);
};

#	define PFS_DEFAULT_MT_POLICY pfs::multi_threaded_local

typedef multi_threaded_local mt_policy_t;
typedef multi_threaded_local mutex;
#else
// catched in mt.h
#endif

/**
 *
 * @code
 * AutoLock<>()
 */
template<class mt_policy = PFS_DEFAULT_MT_POLICY>
class auto_lock
{
public:
	auto_lock(mt_policy * mtx) : _mutex(mtx) { _mutex->lock(); }
	~auto_lock() { _mutex->unlock(); }

	void lock()    { _mutex->lock(); }
	void tryLock() { _mutex->tryLock(); }
	void unlock()  { _mutex->unlock(); }

	mt_policy * mutexPtr() const { return _mutex; }
	mt_policy & mutexRef() const { return *_mutex; }
private:
	mt_policy * _mutex;
};

template<class mt_policy = PFS_DEFAULT_MT_POLICY>
class auto_unlock
{
public:
	auto_unlock(mt_policy * mtx) : _mutex(mtx) { _mutex->unlock(); }
	~auto_unlock() { _mutex->lock(); }

	void lock()    { _mutex->lock(); }
	void tryLock() { _mutex->tryLock(); }
	void unlock()  { _mutex->unlock(); }

	mt_policy * mutexPtr() const { return _mutex; }
	mt_policy & mutexRef() const { return *_mutex; }

private:
	mt_policy * _mutex;
};

//
// The AutoLock class is a helper that will hold the lock while in scope.
//
#define PFS_IMPLEMENT_LOCKING(ClassName)                        \
protected:                                                      \
	class auto_lock                                             \
	{                                                           \
	public:                                                     \
	auto_lock(ClassName * base)                                 \
			: _base(base) { _base->_mutex.lock(); }             \
		~auto_lock() { _base->_mutex.unlock(); }                \
		pfs::mutex * handlePtr() const { return & _base->_mutex; }  \
		                                                        \
		void lock()    { _base->_mutex.lock(); }                \
		void tryLock() { _base->_mutex.tryLock(); }             \
		void unlock()  { _base->_mutex.unlock(); }              \
                                                                \
	private:                                                    \
		ClassName * _base;                                      \
	};                                                          \
private:                                                        \
	pfs::mutex _mutex;

} // pfs

#endif

#endif /* __PFS_MT_HPP__ */
