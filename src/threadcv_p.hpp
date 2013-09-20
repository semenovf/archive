/*
 * threadcv_p.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#ifndef __CWT_THREADCV_P_HPP__
#define __CWT_THREADCV_P_HPP__

#include "../include/cwt/threadcv.hpp"

CWT_NS_BEGIN

class ThreadCVImpl
{
public:
	ThreadCVImpl() {}
};

#ifdef __CWT_INIT_FROM_PIMPL

void ThreadCV::wait (Mutex & lockedMutex) { pimpl->wait(lockedMutex); }
bool ThreadCV::wait (Mutex & lockedMutex, ulong_t timeout) { return pimpl->wait(lockedMutex, timeout); }
void ThreadCV::wakeOne () { pimpl->wakeOne(); }
void ThreadCV::wakeAll () { pimpl->wakeAll(); }

#endif

CWT_NS_END

#endif /* __CWT_THREADCV_P_HPP__ */
