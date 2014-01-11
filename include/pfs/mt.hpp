/**
 * @file   mt.hpp
 * @author wladt
 * @date   Mar 1, 2013 4:11:37 PM
 *
 * @brief
 */

#ifndef __PFS_MT_HPP__
#define __PFS_MT_HPP__

#include <pfs/mt.h>

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
#ifdef __DEPRECATED__ // TODO DELETE THIS CODE
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

	void lock()    { mt_lock(g_mutex); }
	void tryLock() { mt_try_lock(g_mutex); }
	void unlock()  { mt_unlock(g_mutex); }

	static multi_threaded_global & getPolicy() { static multi_threaded_global mtp; return mtp; }
	static mutex_t * handlePtr() { return & g_mutex; }

private:
	static mt_def(g_mutex);
};
#endif // __DEPRECATED__

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

	mt_policy * handlePtr() const { return _mutex; }
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

	mt_policy * handlePtr() const { return _mutex; }
private:
	mt_policy * _mutex;
};

#ifdef __DEPRECATED__ // TODO DELETE THIS CODE
class auto_lock_global
{
public:
#ifndef PFS_SINGLE_THREADED
	auto_lock_global() { multi_threaded_global::getPolicy().lock(); }
	~auto_lock_global() { multi_threaded_global::getPolicy().unlock(); }

	void lock()    { multi_threaded_global::getPolicy().lock(); }
	void tryLock() { multi_threaded_global::getPolicy().tryLock(); }
	void unlock()  { multi_threaded_global::getPolicy().unlock(); }

#else
	auto_lock_global() { ; }
	~auto_lock_global() { ; }
#endif
};
#endif // __DEPRECATED__
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

#endif /* __PFS_MT_HPP__ */
