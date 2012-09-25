/*
 * mt.c
 *
 *  Created on: Sep 25, 2012
 *      Author: wladt
 */

#include <cwt/types.h>
#include <cwt/mt.h>

#if defined(CWT_SINGLE_THREADED)

static void __st_lock(CwtThreaded *th)   { CWT_UNUSED(th); }
static void __st_unlock(CwtThreaded *th) { CWT_UNUSED(th); }

static CwtThreaded __th_global = {
	  TRUE
	, __st_lock
	, __st_unlock
};

static CwtThreaded __th_local = {
	  FALSE
	, __st_lock
	, __st_unlock
};

static CwtThreaded* __st_global(void) { return &__th_global; }
static CwtThreaded* __st_local (void) { return &__th_local; }
static void         __st_free  (CwtThreaded *th) { CWT_UNUSED(th); }

static CwtMtNS __cwtMtNS = {
	  __st_global
	, __st_local
	, __st_free
};

DLL_API CwtMtNS* cwtMtNS(void)
{
	return &__cwtMtNS;
}


#elif defined(CWT_WIN32_THREADS)
	typedef struct _CwtMultiThreadedLocal {
		struct _CwtThreaded __base;
		CRITICAL_SECTION critsec;
	} CwtMultiThreadedLocal;

	typedef struct _CwtThreaded CwtMultiThreadedGlobal;

	static CRITICAL_SECTION __critsec;
	static BOOL __is_initialised = FALSE;

	static void __mt_global_lock(CwtThreaded *);
	static void __mt_global_unlock(CwtThreaded *);
	static void __mt_local_lock(CwtThreaded *);
	static void __mt_local_unlock(CwtThreaded *);

	static CwtThreaded* __mt_global(void);
	static CwtThreaded* __mt_local (void);
	static void         __mt_free  (CwtThreaded *);

	static CwtMultiThreadedGlobal __th_global = {
		  TRUE
		, __mt_global_lock
		, __mt_global_unlock
	}

	static CwtMtNS __cwtMtNS = {
		  __mt_global
		, __mt_local
		, __mt_free
	};

	DLL_API CwtMtNS* cwtMtNS(void)
	{
		if (!__is_initialised) {
			InitializeCriticalSection(&__critsec);
			__is_initialised = TRUE;
		}
		return &__cwtMtNS;
	}

	static void __mt_global_lock(CwtThreaded *th)
	{
		CWT_UNUSED(th);
		EnterCriticalSection(&__critsec);
	}

	static void __mt_global_unlock(CwtThreaded *th)
	{
		CWT_UNUSED(th);
		LeaveCriticalSection(&__critsec);
	}

	static void __mt_local_lock(CwtThreaded *th)
	{
		EnterCriticalSection(&((CwtMultiThreadedLocal *)th)->critsec);
	}

	static void __mt_local_unlock(CwtThreaded *th)
	{
		LeaveCriticalSection(&((CwtMultiThreadedLocal *)th)->critsec);
	}

	static CwtThreaded* __mt_global(void)
	{
		return &__th_global;
	}

	static CwtThreaded* __mt_local (void)
	{
		CwtMultiThreadedLocal *loc = CWT_MALLOC(CwtMultiThreadedLocal);
		loc->__base.is_global = FALSE;
		loc->__base.lock      = __mt_local_lock;
		loc->__base.unlock    = __mt_local_unlock;
		InitializeCriticalSection(&loc->critsec);
		return (CwtThreaded*)loc;
	}

	static void __mt_free (CwtThreaded *th)
	{
		if(!th->is_global) {
			DeleteCriticalSection(&((CwtMultiThreadedLocal*)th)->critsec);
			CWT_FREE(th);
		}
	}
#else /* defined(CWT_POSIX_THREADS) */
	typedef struct _CwtMultiThreadedLocal {
		struct _CwtThreaded __base;
		pthread_mutex_t critsec;
	} CwtMultiThreadedLocal;

	typedef struct _CwtThreaded CwtMultiThreadedGlobal;

	static BOOL __is_initialised = FALSE;
	static pthread_mutex_t __critsec;

	static void __mt_global_lock(CwtThreaded *);
	static void __mt_global_unlock(CwtThreaded *);
	static void __mt_local_lock(CwtThreaded *);
	static void __mt_local_unlock(CwtThreaded *);

	static CwtThreaded* __mt_global(void);
	static CwtThreaded* __mt_local (void);
	static void         __mt_free  (CwtThreaded *);

	static CwtMultiThreadedGlobal __th_global = {
		  TRUE
		, __mt_global_lock
		, __mt_global_unlock
	};

	static CwtMtNS __cwtMtNS = {
		  __mt_global
		, __mt_local
		, __mt_free
	};

	DLL_API CwtMtNS* cwtMtNS(void)
	{
		if (!__is_initialised) {
			pthread_mutex_init(&__critsec, NULL);
			__is_initialised = TRUE;
		}
		return &__cwtMtNS;
	}

	static void __mt_global_lock(CwtThreaded *th)
	{
		CWT_UNUSED(th);
		pthread_mutex_lock(&__critsec);
	}

	static void __mt_global_unlock(CwtThreaded *th)
	{
		CWT_UNUSED(th);
		pthread_mutex_unlock(&__critsec);
	}

	static void __mt_local_lock(CwtThreaded *th)
	{
		EnterCriticalSection(&((CwtMultiThreadedLocal *)th)->critsec);
	}

	static void __mt_local_unlock(CwtThreaded *th)
	{
		LeaveCriticalSection(&((CwtMultiThreadedLocal *)th)->critsec);
	}

	static CwtThreaded* __mt_global(void)
	{
		return &__th_global;
	}

	static CwtThreaded* __mt_local (void)
	{
		CwtMultiThreadedLocal *loc = CWT_MALLOC(CwtMultiThreadedLocal);
		loc->__base.is_global = FALSE;
		loc->__base.lock      = __mt_local_lock;
		loc->__base.unlock    = __mt_local_unlock;
		pthread_mutex_init(&loc->critsec, NULL);
		return (CwtThreaded*)loc;
	}

	static void __mt_free (CwtThreaded *th)
	{
		if(!th->is_global) {
			pthread_mutex_destroy(&((CwtMultiThreadedLocal*)th)->critsec);
			CWT_FREE(th);
		}
	}
#endif /* CWT_POSIX_THREADS */
