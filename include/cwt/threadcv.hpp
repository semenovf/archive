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

#include <cwt/pimpl.hpp>
#include <cwt/mt.hpp>

CWT_NS_BEGIN

class DLL_API ThreadCV
{
	CWT_PIMPL_IMPL(ThreadCV);

public:
	ThreadCV();

	bool wait (Mutex & lockedMutex);
	bool wait (Mutex & lockedMutex, ulong_t timeout);
	void wakeOne ();
	void wakeAll ();

    template <typename predicate_type>
    void wait (Mutex & lockedMutex, predicate_type predicate)
    {
        while (!predicate())
        	wait(lockedMutex);
    }

    template <typename predicate_type>
    bool wait (Mutex & lockedMutex, ulong_t timeout, predicate_type predicate)
    {
        while (!predicate()) {
            if(!wait(lockedMutex, timeout))
                return predicate();
        }
        return true;
    }
};

CWT_NS_END

#endif /* __CWT_THREADCV_HPP__ */
