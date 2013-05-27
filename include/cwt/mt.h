/*
 * mt.h
 *
 *  Created on: Aug 24, 2011
 *  Modified on: Sep 25, 2012
 *      Author: wladt
 * 
 *  Great thanks to Sarah Thompson for her http://sigslot.sourceforge.net/
 */

#ifndef __CWT_MT_H__
#define __CWT_MT_H__

#include <cwt/cwt.h>

#if !defined(CWT_MT)
#	define CWT_SINGLE_THREADED
#elif defined(CWT_OS_WIN)
#	define CWT_WIN32_THREADS
#	include <windows.h>
#elif defined(CWT_HAVE_PTHREAD)
#	define CWT_POSIX_THREADS
#	include <pthread.h>
#else
#	define CWT_SINGLE_THREADED
#endif

#if defined(CWT_SINGLE_THREADED)
#	pragma message("No threads")
#	define mt_def(mutex)
#	define mt_init(mutex)
#	define mt_destroy(mutex)
#	define mt_lock(mutex)
#	define mt_unlock(mutex)

#elif defined(CWT_WIN32_THREADS)
#	pragma message("Using WIN32 threads")
	typedef CRITICAL_SECTION mutex_t;

#	define mt_def(mutex)      mutex_t mutex
#	define mt_init(mutex)	  InitializeCriticalSection(&mutex)
#	define mt_destroy(mutex)  DeleteCriticalSection(&mutex)
#	define mt_lock(mutex)     EnterCriticalSection(&mutex)
#	define mt_unlock(mutex)   LeaveCriticalSection(&mutex)

#elif defined(CWT_POSIX_THREADS)
#	pragma message("Using POSIX threads")
	typedef pthread_mutex_t mutex_t;

#	define mt_def(mutex)      mutex_t mutex
#	define mt_init(mutex)	  pthread_mutex_init(&mutex, NULL)
#	define mt_destroy(mutex)  pthread_mutex_destroy(&mutex)
#	define mt_lock(mutex)     pthread_mutex_lock(&mutex);
#	define mt_unlock(mutex)   pthread_mutex_unlock(&mutex)

#else
#	error "Unknown thread subsystem"
#endif


#ifdef CWT_SINGLE_THREADED
#	define CWT_LOCK
#	define CWT_UNLOCK
#else
#	define CWT_LOCK   mt_lock(*cwt_global_mutex())
#	define CWT_UNLOCK mt_unlock(*cwt_global_mutex())
EXTERN_C_BEGIN
DLL_API mutex_t* cwt_global_mutex(void);
EXTERN_C_END
#endif

#endif /* __CWT_MT_H__ */
