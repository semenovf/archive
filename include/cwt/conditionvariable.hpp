/*
 * conditionvariable.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_CONDITIONVARIABLE_HPP__
#define __CWT_CONDITIONVARIABLE_HPP__

#include <cwt/pimpl.hpp>

CWT_NS_BEGIN

class DLL_API ConditionVariable
{
	CWT_PIMPL_IMPL(ConditionVariable);
	CWT_DENY_COPY(ConditionVariable);
public:
	ConditionVariable();

	bool wait (Mutex * lockedMutex, ulong_t timeout = CWT_ULONG_MAX);
	void wakeOne ();
	void wakeAll ();
};

CWT_NS_END

#endif /* __CWT_CONDITIONVARIABLE_HPP__ */
