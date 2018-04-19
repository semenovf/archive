/*
 * esignal.c
 *
 *  Created on: Oct 5, 2012
 *      Author: wladt
 */

#include <cwt/unistd.h>
#include <cwt/logger.h>
#include <cwt/event/event.h>
#include <cwt/event/signal.h>

static void __finish(CwtEventSource *source);

static CwtEventSource  __signal_source = { NULL, __finish };
static CwtList *__signal_handlers = NULL;

static CwtEventSource* __source (void);
static CwtList* __handlers(void);
static BOOL     __accept (CwtSignal signum);
static BOOL     __raise  (CwtSignal signum);
static int      __signum(CwtEvent *pevt) { return ((CwtEventSignal*)pevt)->signum; }

static CwtEventSignalNS __cwtEventSignalNS = {
	  __source
	, __handlers
	, __accept
	, __raise
	, __signum
};

static CwtListNS    *__listNS = NULL;
static CwtEventNS   *__eventNS = NULL;


DLL_API_EXPORT CwtEventSignalNS* cwt_event_signal_ns(void)
{
	if( !__listNS ) {
		__listNS = cwt_list_ns();
		__eventNS = cwt_event_ns();
	}

	return &__cwtEventSignalNS;
}

static inline CwtEventSource* __source(void)
{
	return &__signal_source;
}

static CwtList* __handlers(void)
{
	if( !__signal_handlers ) {
		__signal_handlers = __listNS->createPtr(NULL);
	}
	return __signal_handlers;
}

static BOOL __accept (CwtSignal signum)
{
	CWT_UNUSED(signum);
	return TRUE;
}


static BOOL __raise(CwtSignal signum)
{
	CwtEventSignal *pevt = CWT_MALLOCT(CwtEventSignal);
	pevt->signum = signum;
	__eventNS->initEvent((CwtEvent*)pevt, __signal_handlers, __eventNS->defaultDestructor);
	__eventNS->post((CwtEvent*)pevt);
	return TRUE;
}



static void __finish(CwtEventSource *source)
{
	CWT_UNUSED(source);

	if( !__signal_handlers ) {
		 __listNS->free(__signal_handlers);
		 __signal_handlers = NULL;
	}
}
