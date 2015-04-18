/**
 * @file conditionvariable.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 *
 * @brief Implements Conditional Variable
 */

#ifndef __PFS_THREADCV_HPP__
#define __PFS_THREADCV_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/mt.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace pfs {

class thread_cv_impl;

class DLL_API thread_cv
{
	pfs::pimpl _d;

public:
	thread_cv ();
    ~thread_cv ();

    bool wait (pfs::mutex & lockedMutex, uintegral_t time = PFS_UINTEGRAL_MAX);
//    bool wait(QReadWriteLock *lockedReadWriteLock, unsigned long time = ULONG_MAX);

    void wakeOne ();
    void wakeAll ();

private:
//    thread_cv (const thread_cv &);

//public:
//	thread_cv ();
//
//	bool wait (pfs::mutex & lockedMutex);
//	bool wait (pfs::mutex & lockedMutex, uintegral_t timeout);
//	void wakeOne ();
//	void wakeAll ();
//
//    template <typename predicate_type>
//    void wait (pfs::mutex & lockedMutex, predicate_type predicate)
//    {
//        while (!predicate())
//        	wait(lockedMutex);
//    }
//
//    template <typename predicate_type>
//    bool wait (pfs::mutex & lockedMutex, uintegral_t timeout, predicate_type predicate)
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

#endif /* __PFS_THREADCV_HPP__ */
