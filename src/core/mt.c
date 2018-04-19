/*
 * mt.c
 *
 *  Created on: Sep 25, 2012
 *      Author: wladt
 */

#include <cwt/types.h>
#include <cwt/mt.h>

extern CwtThreaded* mt_global  (void);
extern CwtThreaded* mt_local   (void);
extern void         mt_init    (CwtThreaded *);
extern void         mt_destroy (CwtThreaded *);
extern void         mt_free  (CwtThreaded *th);
extern void         mt_lock  (CwtThreaded *th);
extern void         mt_unlock(CwtThreaded *th);

extern void         mt_init_helper(void);

static CwtMtNS __cwtMtNS = {
	  mt_global
	, mt_local
	, mt_init
	, mt_destroy
	, mt_free
	, mt_lock
	, mt_unlock
};

DLL_API CwtMtNS* cwt_mt_ns(void)
{
	mt_init_helper();
	return &__cwtMtNS;
}

#if defined(CWT_SINGLE_THREADED)

static CwtThreaded __th = {
	TRUE
};

void         mt_init_helper(void)         {}
CwtThreaded* mt_global  (void)            { return &__th; }
CwtThreaded* mt_local   (void)            { return &__th; }
void         mt_init    (CwtThreaded *th) { CWT_UNUSED(th); }
void         mt_destroy (CwtThreaded *th) { CWT_UNUSED(th); }
void         mt_free    (CwtThreaded *th) { CWT_UNUSED(th); }
void         mt_lock    (CwtThreaded *th) { CWT_UNUSED(th); }
void         mt_unlock  (CwtThreaded *th) { CWT_UNUSED(th); }

#endif
