/*
 * mutex.hpp
 *
 *  Created on: Oct 23, 2016
 *      Author: wladt
 */

#ifndef __PFS_CXX_CXX98_MUTEX_HPP__
#define __PFS_CXX_CXX98_MUTEX_HPP__

#include <pfs/bits/mutex.h>

namespace pfs {

/// Do not acquire ownership of the mutex.
struct defer_lock_t { };

/// Try to acquire ownership of the mutex without blocking.
struct try_to_lock_t { };

/// Assume the calling thread has already obtained mutex ownership and manage it.
struct adopt_lock_t { };

#define DEFER_LOCK  pfs::defer_lock_t()
#define TRY_TO_LOCK pfs::try_to_lock_t()
#define ADOPT_LOCK  pfs::adopt_lock_t()

#if defined(PFS_SINGLE_THREADED)

typedef fake_mutex mutex;

#elif defined(PFS_WIN32_THREADS) || defined(PFS_POSIX_THREADS)
/*
#	define PFS_DEFAULT_MT_POLICY pfs::mutex
*/

class mutex
{
public:
	typedef native_mutex_t * native_handle_type;

private:
	pfs_mutex_def(_mutex);

private:
    mutex (const mutex &);
    mutex & operator = (const mutex &);

public:
	mutex () { pfs_mutex_init(_mutex); }
	~mutex () { pfs_mutex_destroy(_mutex); }

	void lock ()
	{
		pfs_mutex_lock(_mutex);
	}

	bool try_lock ()
	{
		return !pfs_mutex_try_lock(_mutex);
	}

	void unlock ()
	{
		pfs_mutex_unlock(_mutex);
	}

	native_handle_type native_handle ()
	{
		return & _mutex;
	}
};

/// recursive_mutex
class recursive_mutex
{
public:
	typedef native_mutex_t * native_handle_type;

private:
	pfs_recursive_mutex_def(_mutex);

private:
	recursive_mutex (const mutex &);
	recursive_mutex & operator = (const mutex &);

public:
	recursive_mutex () { pfs_recursive_mutex_init(_mutex); }
	~recursive_mutex () { pfs_recursive_mutex_destroy(_mutex); }

	void lock ()
	{
		pfs_recursive_mutex_lock(_mutex);
	}

	bool try_lock()
	{
		return ! pfs_recursive_mutex_try_lock(_mutex);
	}

	void unlock()
	{
		pfs_recursive_mutex_unlock(_mutex);
	}

	native_handle_type native_handle ()
	{
		return & _mutex;
	}
};

template <typename Lockable>
class lock_guard
{
public:
    typedef Lockable mutex_type;

private:
    mutex_type & _mtx;

private:
    lock_guard (const lock_guard & );
    lock_guard & operator = (const lock_guard &);

public:
    explicit lock_guard (mutex_type & mtx)
    	: _mtx(mtx)
    {
    	_mtx.lock();
    }

    lock_guard (mutex_type & mtx, adopt_lock_t)
    	: _mtx(mtx)
    { } // calling thread owns mutex

    ~lock_guard ()
    {
    	_mtx.unlock();
    }
};

template <typename Lockable>
class unique_lock
{
public:
	typedef Lockable mutex_type;

private:
	mutex_type * _mtx;
	bool         _owns; // XXX use atomic_bool

private: // noncopyable
    unique_lock (const unique_lock &);
    unique_lock & operator = (const unique_lock &);

public:
	unique_lock() //XXX noexcept
		: _mtx(0)
		, _owns(false)
	{ }

	explicit unique_lock (mutex_type & m)
		: _mtx(& m), _owns(false)
	{
		lock();
		_owns = true;
	}

	unique_lock (mutex_type & m, defer_lock_t) // XXX noexcept
		: _mtx(& m), _owns(false)
	{ }

	unique_lock (mutex_type & m, try_to_lock_t)
		: _mtx(& m), _owns(_mtx->try_lock())
	{ }

	unique_lock (mutex_type & m, adopt_lock_t)
		: _mtx(& m), _owns(true)
	{
		// XXX calling thread owns mutex
	}

//  TODO Implement
//	template<typename _Clock, typename _Duration>
//	unique_lock(mutex_type& __m,
//	const chrono::time_point<_Clock, _Duration>& __atime)
//	: _mtx(&__m), _owns(_mtx->try_lock_until(__atime))
//	{ }

	//  TODO Implement
//	template<typename _Rep, typename _Period>
//	unique_lock(mutex_type& __m,
//	const chrono::duration<_Rep, _Period>& __rtime)
//	: _mtx(&__m), _owns(_mtx->try_lock_for(__rtime))
//	{ }

	~unique_lock ()
	{
		if (_owns)
			unlock();
	}

	void lock ()
	{
		PFS_ASSERT(_mtx);  // Operation not permitted (POSIX.1)
		PFS_ASSERT(!_owns); // Resource deadlock avoided (POSIX.1)
		_mtx->lock();
		_owns = true;
	}

	bool try_lock()
	{
		PFS_ASSERT(_mtx);  // EPERM
		PFS_ASSERT(!_owns); // EDEADLK
		_owns = _mtx->try_lock();
		return _owns;
	}

//  TODO Implement
//	template<typename _Clock, typename _Duration>
//	bool
//	try_lock_until(const chrono::time_point<_Clock, _Duration>& __atime)
//	{
//	if (!_mtx)
//	__throw_system_error(int(errc::operation_not_permitted));
//	else if (_owns)
//	__throw_system_error(int(errc::resource_deadlock_would_occur));
//	else
//	{
//	  _owns = _mtx->try_lock_until(__atime);
//	  return _owns;
//	}
//	}

//  TODO Implement
//	template<typename _Rep, typename _Period>
//	bool
//	try_lock_for(const chrono::duration<_Rep, _Period>& __rtime)
//	{
//	if (!_mtx)
//	__throw_system_error(int(errc::operation_not_permitted));
//	else if (_owns)
//	__throw_system_error(int(errc::resource_deadlock_would_occur));
//	else
//	{
//	_owns = _mtx->try_lock_for(__rtime);
//	return _owns;
//	}
//	}

	void unlock ()
	{
		PFS_ASSERT(_owns);
		if (_mtx) {
			_mtx->unlock();
			_owns = false;
		}
	}

//  TODO Implement
//	void swap (unique_lock & u) // XXX noexcept
//	{
//		std::swap(_mtx, u._mtx);
//		std::swap(_owns, u._owns);
//	}

	mutex_type * release () // XXX noexcept
	{
		mutex_type * ret = _mtx;
		_mtx = 0;
		_owns = false;
		return ret;
	}

	bool owns_lock() const // XXX noexcept
	{
		return _owns;
	}

	/*explicit */operator bool () const // XXX noexcept
	{
		return owns_lock();
	}

	mutex_type * mutex () const // XXX noexcept
	{
		return _mtx;
	}
};

#endif

}

#endif /* __PFS_CXX_CXX98_MUTEX_HPP__ */
