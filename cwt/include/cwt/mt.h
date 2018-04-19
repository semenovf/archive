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

#ifndef __CWT_GLOBAL_H__
#	error "Do not direct include of <cwt/mt.h> header, only implicitly through inclusion <cwt/global.h>"
#else
#	if !defined(CWT_MT)
#		define CWT_SINGLE_THREADED
#	elif defined(CWT_OS_WIN)
#		define CWT_WIN32_THREADS
#		include <windows.h>
#	elif defined(__GNUG__)
#		define CWT_POSIX_THREADS
#		include <pthread.h>
#	else
#		define CWT_SINGLE_THREADED
#	endif

#ifdef CWT_SINGLE_THREADED
#	define CWT_LOCK
#	define CWT_UNLOCK
#else
#	define CWT_LOCK   cwt_mt_ns()->global()->lock()
#	define CWT_UNLOCK cwt_mt_ns()->global()->unlock()
#endif


#if defined(CWT_SINGLE_THREADED)

	typedef struct _CwtThreaded {
		BOOL is_global;
	} CwtThreaded;

#elif defined(CWT_WIN32_THREADS)

	typedef struct _CwtThreaded {
		BOOL is_global;
		CRITICAL_SECTION critsec;
	} CwtThreaded;

#elif defined(CWT_POSIX_THREADS)

	typedef struct _CwtThreaded {
		BOOL is_global;
		pthread_mutex_t critsec;
	} CwtThreaded;

#else
#	error Unknown thread subsystem
#endif

typedef struct _CwtMtNS {
	CwtThreaded* (*global) (void);
	CwtThreaded* (*local)  (void);
	void         (*init)   (CwtThreaded *);
	void         (*destroy)(CwtThreaded *);
	void         (*free)   (CwtThreaded *);
	void         (*lock)   (CwtThreaded *);
	void         (*unlock) (CwtThreaded *);
} CwtMtNS;

EXTERN_C_BEGIN
DLL_API CwtMtNS* cwt_mt_ns(void);
EXTERN_C_END

#endif  /*__CWT_GLOBAL_H__*/

#endif /* __CWT_MT_H__ */
