/**
 * @file   mt.c
 * @author wladt
 * @date   Feb 21, 2013 4:02:42 PM
 *
 * @brief
 */


#include "../include/cwt/mt.h"

#if !defined(CWT_SINGLE_THREADED)
static mt_def(__g_mutex);

DLL_API mutex_t* cwt_global_mutex(void)
{
	static bool __g_mutex_initialized = false;
	if(!__g_mutex_initialized)
		mt_init(__g_mutex);
	__g_mutex_initialized = true;
	return &__g_mutex;
}
#endif /* CWT_SINGLE_THREADED */
