/*
 * mt.h
 *
 *  Created on: Aug 24, 2011
 *  Modified on: Sep 25, 2012
 *      Author: wladt
 * 
 *  Great thanks to Sarah Thompson for her http://sigslot.sourceforge.net/
 */

#ifndef __PFS_BITS_MUTEX_H__
#define __PFS_BITS_MUTEX_H__

#include <pfs.h>

#if !defined(PFS_NO_MT)
/*#	define PFS_SINGLE_THREADED*/
#	if defined(PFS_OS_WIN)
#		define PFS_WIN32_THREADS
#		include <windows.h>
#	elif defined(PFS_HAVE_PTHREAD)
#		define PFS_POSIX_THREADS
#		include <pthread.h>
#	else
#		error "No native support for threads"
#	endif
#endif

#if defined(PFS_NO_MT)
#	define pfs_mutex_def(m)
#	define pfs_mutex_init(m)
#	define pfs_mutex_destroy(m)
#	define pfs_mutex_lock(m)
#	define pfs_mutex_unlock(m)

#elif defined(PFS_WIN32_THREADS)
	typedef CRITICAL_SECTION native_mutex_t;

#	define pfs_mutex_def(m)      native_mutex_t m
#	define pfs_mutex_init(m)	 InitializeCriticalSection(& m)
#	define pfs_mutex_destroy(m)  DeleteCriticalSection(& m)
#	define pfs_mutex_lock(m)     EnterCriticalSection(& m)
#	define pfs_mutex_try_lock(m) TryEnterCriticalSection(& m)
#	define pfs_mutex_unlock(m)   LeaveCriticalSection(& m)

#	define pfs_recursive_mutex_init(m) error @ {} // __FIXME__ need to implement

#elif defined(PFS_POSIX_THREADS)
	typedef pthread_mutex_t native_mutex_t;

#	define pfs_mutex_def(m)      native_mutex_t m
#	define pfs_mutex_init(m)     pthread_mutex_init(& m, 0)
#	define pfs_mutex_destroy(m)  pthread_mutex_destroy(& m)
#	define pfs_mutex_lock(m)     pthread_mutex_lock(& m)
#	define pfs_mutex_try_lock(m) pthread_mutex_trylock(& m)
#	define pfs_mutex_unlock(m)   pthread_mutex_unlock(& m)

#	define pfs_recursive_mutex_def(m)  native_mutex_t m
#	define pfs_recursive_mutex_init(m) {                                 \
		pthread_mutexattr_t mutex_attr;                                  \
		pthread_mutexattr_init(& mutex_attr);                            \
		pthread_mutexattr_settype(& mutex_attr, PTHREAD_MUTEX_RECURSIVE);\
		pthread_mutex_init(& m, & mutex_attr);                           \
	}
#	define pfs_recursive_mutex_destroy(m)  pfs_mutex_destroy(m)
#	define pfs_recursive_mutex_lock(m)     pfs_mutex_lock(m)
#	define pfs_recursive_mutex_try_lock(m) pfs_mutex_try_lock(m)
#	define pfs_recursive_mutex_unlock(m)   pfs_mutex_unlock(m)

#endif


/* Delete it
#ifdef PFS_SINGLE_THREADED
#	define MT_LOCK
#	define MT_UNLOCK
#elses
#	define MT_LOCK   mt_lock(*mt_global_mutex())
#	define MT_UNLOCK mt_unlock(*mt_global_mutex())
#endif
*/

#endif /* __PFS_BITS_MUTEX_H__ */
