/**
 * @file conditionvariable.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 *
 * @brief Implements Conditional Variable
 */

#ifndef __CWT_THREADCV_HPP__
#define __CWT_THREADCV_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/mt.hpp>

#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

namespace cwt {

class DLL_API thread_cv
{
	class impl;
	pfs::pimpl _d;

public:
	thread_cv ();

	bool wait (pfs::mutex & lockedMutex);
	bool wait (pfs::mutex & lockedMutex, ulong_t timeout);
	void wakeOne ();
	void wakeAll ();

    template <typename predicate_type>
    void wait (pfs::mutex & lockedMutex, predicate_type predicate)
    {
        while (!predicate())
        	wait(lockedMutex);
    }

    template <typename predicate_type>
    bool wait (pfs::mutex & lockedMutex, ulong_t timeout, predicate_type predicate)
    {
        while (!predicate()) {
            if(!wait(lockedMutex, timeout))
                return predicate();
        }
        return true;
    }
};

} // cwt

#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif

#endif /* __CWT_THREADCV_HPP__ */
