/**
 * @file condition_variable.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 *
 * @brief Implements Conditional Variable
 */

#ifndef __PFS_CONDITION_VARIABLE_HPP__
#define __PFS_CONDITION_VARIABLE_HPP__

#include <pfs.hpp>
#include <pfs/mutex.hpp>
#include <pfs/bits/condition_variable.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

/// cv_status
enum cv_status { no_timeout, timeout };

class DLL_API condition_variable
{
	details::condition_variable_data _d;

private:
	condition_variable (const condition_variable &);
	condition_variable & operator = (const condition_variable &);

public:
	condition_variable ();
    ~condition_variable ();

    void notify_one ();
    void notify_all ();

    void wait (unique_lock<mutex> & lock);

    template <typename Predicate>
    void wait (unique_lock<mutex> & lock, Predicate p)
    {
        while (!p())
            wait(lock);
    }
    
    // TODO DEPRECATED
    bool wait_until (unique_lock<mutex> & lock, uintmax_t timeout);

#if __FIXME__
    
        template<typename _Duration>
      cv_status
      wait_until(unique_lock<mutex>& __lock,
		 const chrono::time_point<__clock_t, _Duration>& __atime)
      { return __wait_until_impl(__lock, __atime); }

    template<typename _Clock, typename _Duration>
      cv_status
      wait_until(unique_lock<mutex>& __lock,
		 const chrono::time_point<_Clock, _Duration>& __atime)
      {
	// DR 887 - Sync unknown clock to known clock.
	const typename _Clock::time_point __c_entry = _Clock::now();
	const __clock_t::time_point __s_entry = __clock_t::now();
	const auto __delta = __atime - __c_entry;
	const auto __s_atime = __s_entry + __delta;

	return __wait_until_impl(__lock, __s_atime);
      }

    template<typename _Clock, typename _Duration, typename _Predicate>
      bool
      wait_until(unique_lock<mutex>& __lock,
		 const chrono::time_point<_Clock, _Duration>& __atime,
		 _Predicate __p)
      {
	while (!__p())
	  if (wait_until(__lock, __atime) == cv_status::timeout)
	    return __p();
	return true;
      }

    template<typename _Rep, typename _Period>
      cv_status
      wait_for(unique_lock<mutex>& __lock,
	       const chrono::duration<_Rep, _Period>& __rtime)
      { return wait_until(__lock, __clock_t::now() + __rtime); }

    template<typename _Rep, typename _Period, typename _Predicate>
      bool
      wait_for(unique_lock<mutex>& __lock,
	       const chrono::duration<_Rep, _Period>& __rtime,
	       _Predicate __p)
      { return wait_until(__lock, __clock_t::now() + __rtime, std::move(__p)); }

    native_handle_type
    native_handle()
    { return &_M_cond; }

  private:
    template<typename _Dur>
      cv_status
      __wait_until_impl(unique_lock<mutex>& __lock,
			const chrono::time_point<__clock_t, _Dur>& __atime)
      {
	auto __s = chrono::time_point_cast<chrono::seconds>(__atime);
	auto __ns = chrono::duration_cast<chrono::nanoseconds>(__atime - __s);

	__gthread_time_t __ts =
	  {
	    static_cast<std::time_t>(__s.time_since_epoch().count()),
	    static_cast<long>(__ns.count())
	  };

	__gthread_cond_timedwait(&_M_cond, __lock.mutex()->native_handle(),
				 &__ts);

	return (__clock_t::now() < __atime
		? cv_status::no_timeout : cv_status::timeout);
      }
    
#endif    
//    template <typename predicate_type>
//    bool wait (pfs::mutex & lockedMutex, uintmax_t timeout, predicate_type predicate)
//    {
//        while (!predicate()) {
//            if(!wait(lockedMutex, timeout))
//                return predicate();
//        }
//        return true;
//    }
};

} // pfs

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __PFS_CONDITION_VARIABLE_HPP__ */
