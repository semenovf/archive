/*
 * mutex.hpp
 *
 *  Created on: Sep 23, 2015
 *      Author: wladt
 */

#ifndef __PFS_MUTEX_HPP__
#define __PFS_MUTEX_HPP__

#include <pfs/cxxversion.hpp>
#include PFS_CXX_HEADER(mutex)

namespace pfs {

class fake_mutex
{
	typedef int native_handle_type;

private:
	fake_mutex (const fake_mutex &);
	fake_mutex & operator = (const fake_mutex &);

public:
	fake_mutex ()  {}
	~fake_mutex () {}
	void lock ()	{}
	bool try_lock() { return true; }
	void unlock()   {}

	native_handle_type native_handle () { return native_handle_type(0); }
};

} // pfs or std


#if __PFS_CXX_VERSION__ >= __PFS_CXX11__ // need to avoid error: explicit specialization of non-template ‘lock_guard’

namespace std {
template <>
class lock_guard<pfs::fake_mutex>

#else

namespace pfs {
template <>
class lock_guard<fake_mutex>

#endif
{
public:
    typedef pfs::fake_mutex mutex_type;

private:
    lock_guard (const lock_guard & );
    lock_guard & operator = (const lock_guard &);

public:
    explicit lock_guard (mutex_type & ) {}
    lock_guard (mutex_type & , adopt_lock_t) {}
    ~lock_guard () {}
};

} // pfs or std

#if __PFS_CXX_VERSION__ >= __PFS_CXX11__ // need to avoid error: explicit specialization of non-template ‘unique_lock’

namespace std {
template <>
class unique_lock<pfs::fake_mutex>

#else

namespace pfs {
template <>
class unique_lock<fake_mutex>

#endif
{
public:
	typedef pfs::fake_mutex mutex_type;

private: // noncopyable
    unique_lock (unique_lock const &);
    unique_lock & operator = (unique_lock const &);

public:
	unique_lock() {}
	explicit unique_lock (mutex_type & ) {}
	unique_lock (mutex_type & m, defer_lock_t) {}
	unique_lock (mutex_type & m, try_to_lock_t) {}
	unique_lock (mutex_type & m, adopt_lock_t) {}

	~unique_lock () {}

	void lock () {}
	bool try_lock () { return true; }
	void unlock () {}
	mutex_type * release () { return 0; }
	bool owns_lock () const { return true; } // FIXME what value should return this method ?
	/*explicit */operator bool () const { return owns_lock(); }
	mutex_type * mutex () const { return 0; }
};

} // pfs

#endif /* __PFS_MUTEX_HPP__ */
