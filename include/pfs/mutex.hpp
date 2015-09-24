/*
 * mutex.hpp
 *
 *  Created on: Sep 23, 2015
 *      Author: wladt
 */

#ifndef __PFS_MUTEX_HPP__
#define __PFS_MUTEX_HPP__

#if __cplusplus >= 201103L
#	include <mutex>
#else
#	include <pfs/bits/mutex.h>
#	include <pfs/exception.hpp>
#endif

namespace pfs {

#if __cplusplus >= 201103L

using mutex = std::mutex;
#define DEFER_LOCK  std::defer_lock
#define TRY_TO_LOCK std::try_to_lock
#define ADOPT_LOCK  std::adopt_lock

#else

#if defined(PFS_SINGLE_THREADED)

/*
#	define PFS_DEFAULT_MT_POLICY pfs::mutex
*/

class DLL_API mutex
{
public:
	typedef native_mutex_t * native_handle_type;

private:
    mutex (const mutex &);
    mutex & operator = (const mutex &);

public:
	mutex () { ; }
	~mutex () { ; }
	void lock ()	{ ;	}
	void try_lock() { ; }
	void unlock() {	; }

	native_handle_type native_handle () { return (native_handle_type *)0; }
};

/*
typedef single_threaded mt_policy_t;
typedef single_threaded mutex;
*/

#elif defined(PFS_WIN32_THREADS) || defined(PFS_POSIX_THREADS)
/*
#	define PFS_DEFAULT_MT_POLICY pfs::mutex
*/

class /*DLL_API */mutex
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
		int ex = pfs_mutex_lock(_mutex);

		if (ex)
			PFS_THROW_SYSERR(ex);
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
		int ex = pfs_recursive_mutex_lock(_mutex);

		if (ex)
			PFS_THROW_SYSERR(ex);
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

/// Do not acquire ownership of the mutex.
struct defer_lock_t { };

/// Try to acquire ownership of the mutex without blocking.
struct try_to_lock_t { };

/// Assume the calling thread has already obtained mutex ownership and manage it.
struct adopt_lock_t { };

#define DEFER_LOCK  defer_lock_t()
#define TRY_TO_LOCK try_to_lock_t()
#define ADOPT_LOCK  adopt_lock_t()

template<typename Mtx>
class lock_guard
{
public:
    typedef Mtx mutex_type;

private:
    mutex_type &  _mtx;

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

/*
typedef multi_threaded_local mt_policy_t;
typedef multi_threaded_local mutex;
*/
#endif

#endif // __cplusplus < 201103L

} // pfs

#endif /* __PFS_MUTEX_HPP__ */
