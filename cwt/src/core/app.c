/*
 * app.c
 *
 *  Created on: Oct 5, 2012
 *      Author: wladt
 */


#include <cwt/app.h>
#include <cwt/event/kbd.h>
#include <cwt/event/clock.h>
#include <cwt/event/signal.h>
#include <cwt/event/channel.h>

static void __app_main (void);
static BOOL __app_registerEventSource(CwtEventSource*);

static CwtApp __app = {
	  __app_main
	, __app_registerEventSource
};

static CwtEventNS *__eventNS = NULL;

DLL_API_EXPORT CwtApp* cwtApp(void)
{
	if( !__eventNS ) {
		__eventNS = cwt_event_ns();
	}
	return &__app;
}


static void __app_main (void)
{
	__eventNS->loop();
	__eventNS->unregisterAllSources();
}


static BOOL __app_registerEventSource(CwtEventSource *source)
{
	CWT_UNUSED(source);
	return FALSE;
}
