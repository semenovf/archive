/*
 * synchronized.hpp
 *
 *  Created on: Sep 28, 2015
 *      Author: wladt
 */

#ifndef __PFS_SYNCHRONIZED_HPP__
#define __PFS_SYNCHRONIZED_HPP__

#include <pfs/mutex.hpp>
#include <pfs/noncopyable.hpp>

namespace pfs {

/**
 *
 * @example
 * 		template <typename T>
 * 		typedef synchronized<vector<T> > synchronized_vector;
 *
 *      ...
 *      {
 *			synchronized_vector<int> sync_vector(new vector);
 *			lock_guard<synchronized_vector<int> > locker;
 *			sync_vector->push_back(10);
 *			...
 *		}
 *
 */
template <typename T, typename Mtx = pfs::mutex>
class synchronized : noncopyable
{
	T *  _ptr;
	bool _destroy;
	Mtx  _mutex;

private:
	synchronized ();

public:
	synchronized (T & o)
		: _destroy(false)
		, _ptr(& o)
	{}

	synchronized (T * p)
		: _destroy(true)
		, _ptr(p)
	{
		PFS_ASSERT(p);
	}

	~synchronized ()
	{
		if (_destroy)
			delete _ptr;
	}

	void lock ()
	{
		_mutex.lock();
	}

	void unlock ()
	{
		_mutex.unlock();
	}

	const T * operator -> () const
	{
		return _ptr;
	}

	T * operator -> ()
	{
		return _ptr;
	}
};

} // pfs


#endif /* __PFS_SYNCHRONIZED_HPP__ */
