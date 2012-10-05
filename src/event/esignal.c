/*
 * esignal.c
 *
 *  Created on: Oct 5, 2012
 *      Author: wladt
 */


#include <cwt/event/event.h>
#include <cwt/event/signal.h>

typedef struct _CwtEventSignal
{
	CwtEvent __base;
	CwtSignal sig;
} CwtEventSignal;

static CwtEventSource  __signal_source = { NULL, NULL };

static CwtEventSource* __source (void);
static void     __postSignal    (CwtSignal sig);


static CwtEventClockNS __cwtEventClockNS = {
	  __source
	, __postSignal
};

static CwtListNS    *__listNS = NULL;
static CwtEventNS   *__eventNS = NULL;


DLL_API_EXPORT CwtEventClockNS* cwtEventSignalNS(void)
{
	return &__cwtEventClockNS;
}


static inline CwtEventSource* __source(void)
{
	return &__signal_source;
}

static void __postSignal(CwtSignal sig)
{
	CwtEventSignal *pevt = CWT_MALLOC(CwtEventSignal);
	pevt->sig = sig;
	__eventNS->initEvent((CwtEvent*)pevt, __date_handlers, __eventNS->defaultDestructor);
	pevt->year = year;
	pevt->mon  = mon;
	pevt->day  = day;
/*

	__eventNS->post((CwtEvent*)pevt);
*/
}

