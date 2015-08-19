/*
 * threadcv.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: wladt
 */

#include "pfs/threadcv.hpp"

/*
 * ThreadCV cond;
 * Mutex mutex;
 * bool data_ready = false;
 *
 * void consumer ()
 * {
 * 		AutoLock<> locker(& mutex);
 * 		while (! data_ready) {
 * 			cond.wait(mutex);
 * 		process_data();
 * 		data_ready = false;
 * } // unlock mutex implicitly
 *
 * void producer ()
 * {
 * 		read_data();
 * 		{
 * 			AutoLock<> locker(& mutex);
 * 			data_ready = true;
 * 		} // unlock mutex implicitly
 * 		cond.wakeOne();
 * }
 *
 *         u                                                                       u
 *         n                                                                       n
 *   l     l                                          l                            l
 *   o     o                                          o                            o
 *   c     c                                          c                            c
 *   k     k                                          k                            k
 *   |     |                 wait                     |   process_data             |
 *   |     |  --------------------------------------  |  ---------------    -----  |
 *   |     | /                                      \ | /               \  /     \ |
 *   v_____v/________________________________________\v/_________________\/_______\v
 *                                        ____                                ^
 *                                       |     data_ready = true              |___ data_ready = false
 * ______________________________________v_____________
 * \                               /^\       /^\      /
 *  \_____________________________/ | \_____/ | \____/
 *             read_data            |         | wakeOne
 *                                  |         |
 *                                  l         u
 *                                  o         n
 *                                  c         l
 *                                  k         o
 *                                            k
 *                                            c
 *
 */


namespace pfs {

/**
 * @fn bool ConditionVariable::wait (Mutex * lockedMutex, uintegral_t timeout)
 *
 * @param lockedMutex
 * @param timeout Timeout in milliseconds.
 * @return
 */

} // pfs
