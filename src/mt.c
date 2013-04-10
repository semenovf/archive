/**
 * @file   mt.c
 * @author wladt
 * @date   Feb 21, 2013 4:02:42 PM
 *
 * @brief
 */


#include <cwt/mt.h>

/* This is a template for usage as global mutex
 */
#ifdef __CWT_USAGE_EXAMPLE__

#ifndef CWT_SINGLE_THREADED
static mt_def(__g_mutex);
static bool_t __g_mutex_initialized = false;

DLL_API mutex_t* cwt_global_mutex(void)
{
	if(!__g_mutex_initialized)
		mf_init(__g_mutex);
	__g_mutex_initialized = true;
	return &__g_mutex;
}
#endif /* CWT_SINGLE_THREADED */

#endif
