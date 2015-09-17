/*
 * timer_posix.cpp
 *
 *  Created on: Sep 17, 2015
 *      Author: wladt
 */

#include <signal.h>
#include <time.h>
#include <errno.h>

#include "pfs/atomic.hpp"
#include "../timer_p.hpp"

/**
 * @see [timers in linux in c](http://stackoverflow.com/questions/12764771/timers-in-linux-in-c)
 * @see [Loops/timers in C](http://stackoverflow.com/questions/5540245/loops-timers-in-c)
 * @see [High Resolution Timers](http://elinux.org/High_Resolution_Timers)
 */

#if defined(_POSIX_C_SOURCE) &&  _POSIX_C_SOURCE >= 199309L
#	define PFS_HAVE_TIMER_CREATE 1
#else
#	error Expected support of timer_create() POSIX call
#endif

namespace pfs {

/**
 * @note This class implemented using source code represented
 *       by [Nominal Animal](http://stackoverflow.com/users/1475978/nominal-animal)
 *       at [SrackOverflow](http://stackoverflow.com/questions/12764771/timers-in-linux-in-c)
 */
struct timer_impl
{
/*
	static const int TIMEOUTS = 16;
	static const int TIMEOUT_SIGNAL = SIGRTMIN + 0;

	static const int TIMEOUT_USED   = 1;
	static const int TIMEOUT_ARMED  = 2;
	static const int TIMEOUT_PASSED = 4;
*/

    timer_impl();
};

} // pfs
