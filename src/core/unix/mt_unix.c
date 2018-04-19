/*
 * mt.c
 *
 *  Created on: Sep 25, 2012
 *      Author: wladt
 */

#include <cwt/types.h>
#include <cwt/mt.h>

#ifdef CWT_POSIX_THREADS

static BOOL        __is_initialised = FALSE;
static CwtThreaded __th_global;

void mt_init_helper(void)
{
	if (!__is_initialised) {
		__th_global.is_global = TRUE;
		pthread_mutex_init(&__th_global.critsec, NULL);
		__is_initialised = TRUE;
	}
}

void mt_lock(CwtThreaded *th)
{
	pthread_mutex_lock(&th->critsec);
}

void mt_unlock(CwtThreaded *th)
{
	pthread_mutex_unlock(&th->critsec);
}

CwtThreaded* mt_global(void)
{
	return &__th_global;
}

CwtThreaded* mt_local (void)
{
	CwtThreaded *th = CWT_MALLOC(CwtThreaded);
	th->is_global = FALSE;
	pthread_mutex_init(&th->critsec, NULL);
	return th;
}


void mt_init (CwtThreaded *th)
{
	if (th != &__th_global) {
		th->is_global = FALSE;
		pthread_mutex_init(&th->critsec, NULL);
	}
}

void mt_destroy (CwtThreaded *th)
{
	pthread_mutex_destroy(&th->critsec);
}


void mt_free (CwtThreaded *th)
{
	pthread_mutex_destroy(&th->critsec);
	if (!th->is_global) {
		CWT_FREE(th);
	}
}
#endif
