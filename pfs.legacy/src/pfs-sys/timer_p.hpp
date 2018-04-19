/*
 * timer_p.hpp
 *
 *  Created on: Sep 17, 2015
 *      Author: wladt
 */

#ifndef __PFS_TIMER_P_HPP__
#define __PFS_TIMER_P_HPP__

#include "pfs/timer.hpp"

/**
 * Timer support on Linux:
 *
 * - getitimer, setitimer.
 *         Conforming to POSIX.1-2001, SVr4, 4.4BSD
 *         (this call first appeared in 4.2BSD).
 *         POSIX.1-2008 marks this calls obsolete, recommending the
 *         use of the POSIX timers API (timer_gettime(2),
 *         timer_settime(2), etc.) instead.
 *
 * - timer_create, timer_settime, timer_gettime, timer_getoverrun, timer_delete.
 *         Conforming to POSIX.1-2001.
 */

#endif /* __PFS_TIMER_P_HPP__ */
