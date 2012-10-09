#include <stdio.h>
#include <cwt/app.h>


/*static void __poll(void) {}*/

/*
static CwtEventSource __event_source_0 = {__poll, NULL};
static CwtEventSource __event_source_1 = {__poll, NULL};
static CwtEventSource __event_source_2 = {__poll, NULL};
static CwtEventSource __event_source_3 = {__poll, NULL};
static CwtEventSource __event_source_4 = {__poll, NULL};
*/

int main(int argc, char *argv[])
{
	CwtApp *app;
/*	CwtEventNS *eventNS = cwtEventNS();*/

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

/*
	eventNS->registerSource(&__event_source_0);
	eventNS->registerSource(&__event_source_1);
	eventNS->registerSource(&__event_source_2);
	eventNS->registerSource(&__event_source_3);
	eventNS->registerSource(&__event_source_4);
*/

	app = cwtApp();
/*
	if( !app->selectEvents(Cwt_Event_Kbd
			| Cwt_Event_PosixSignal
			| Cwt_Event_Clock ) )
		return -1;
*/

	app->main();

	return 0;
}


