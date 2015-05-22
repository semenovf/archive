/*
 * mt.h
 *
 *  Created on: Aug 24, 2011
 *  Modified on: Sep 25, 2012
 *      Author: wladt
 * 
 *  Great thanks to Sarah Thompson for her http://sigslot.sourceforge.net/
 */

#ifndef __PFS_MT_H__
#define __PFS_MT_H__

#include <pfs.h>

#if defined(PFS_NO_MT)
#	define PFS_SINGLE_THREADED
#elif defined(PFS_OS_WIN)
#	define PFS_WIN32_THREADS
#	include <windows.h>
#elif defined(PFS_HAVE_PTHREAD)
#	define PFS_POSIX_THREADS
#	include <pthread.h>
#else
#	define PFS_SINGLE_THREADED
#endif

#if defined(PFS_SINGLE_THREADED)
//#	pragma message("No threads")
#	define mt_def(m)
#	define mt_init(m)
#	define mt_destroy(m)
#	define mt_lock(m)
#	define mt_unlock(m)

#elif defined(PFS_WIN32_THREADS)
//#	pragma message("Using WIN32 threads")
	typedef CRITICAL_SECTION mutex_t;

#	define mt_def(m)      mutex_t m
#	define mt_init(m)	  InitializeCriticalSection(& m)
#	define mt_destroy(m)  DeleteCriticalSection(& m)
#	define mt_lock(m)     EnterCriticalSection(& m)
#	define mt_try_lock(m) TryEnterCriticalSection(& m);
#	define mt_unlock(m)   LeaveCriticalSection(& m)

#elif defined(PFS_POSIX_THREADS)
//#	pragma message("Using POSIX threads")
	typedef pthread_mutex_t mutex_t;

#	define mt_def(m)      mutex_t m
#	define mt_init(m)	  pthread_mutex_init(& m, nullptr)
#	define mt_destroy(m)  pthread_mutex_destroy(& m)
#	define mt_lock(m)     pthread_mutex_lock(& m);
#	define mt_try_lock(m) pthread_mutex_trylock(& m);
#	define mt_unlock(m)   pthread_mutex_unlock(& m)

#else
#	error "Unknown thread subsystem"
#endif


#ifdef PFS_SINGLE_THREADED
#	define MT_LOCK
#	define MT_UNLOCK
#else
#	define MT_LOCK   mt_lock(*mt_global_mutex())
#	define MT_UNLOCK mt_unlock(*mt_global_mutex())
/* DELETE this
EXTERN_C_BEGIN
DLL_API mutex_t * mt_global_mutex(void);
EXTERN_C_END
*/
#endif

#endif /* __PFS_MT_H__ */
