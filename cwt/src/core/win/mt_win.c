/*
 * mt.c
 *
 *  Created on: Sep 25, 2012
 *      Author: wladt
 */

#include <cwt/types.h>
#include <cwt/mt.h>

#ifdef CWT_WIN32_THREADS

static BOOL        __is_initialised = FALSE;
static CwtThreaded __th_global;

void mt_init_helper(void)
{
	if (!__is_initialised) {
		__th_global.is_global = TRUE;
		InitializeCriticalSection(&__th_global.critsec);
		__is_initialised = TRUE;
	}
}

void mt_lock(CwtThreaded *th)
{
	EnterCriticalSection(&th->critsec);
}

void mt_unlock(CwtThreaded *th)
{
	LeaveCriticalSection(&th->critsec);
}


CwtThreaded* mt_global(void)
{
	return &__th_global;
}


CwtThreaded* mt_local (void)
{
	CwtThreaded *th = CWT_MALLOC(CwtThreaded);
	th->is_global = FALSE;
	InitializeCriticalSection(&th->critsec);
	return th;
}


void mt_init (CwtThreaded *th)
{
	if (th != &__th_global) {
		th->is_global = FALSE;
		InitializeCriticalSection(&th->critsec);
	}
}

void mt_destroy (CwtThreaded *th)
{
	DeleteCriticalSection(&th->critsec);
}


void mt_free (CwtThreaded *th)
{
	DeleteCriticalSection(&th->critsec);
	if (!th->is_global) {
		CWT_FREE(th);
	}
}

#endif /* CWT_WIN32_THREADS */
