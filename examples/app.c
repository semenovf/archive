#include <stdio.h>
#include <cwt/app.h>


static void __poll(void) {}

static CwtEventSource __event_source_0 = {__poll, NULL};
static CwtEventSource __event_source_1 = {__poll, NULL};
static CwtEventSource __event_source_2 = {__poll, NULL};
static CwtEventSource __event_source_3 = {__poll, NULL};
static CwtEventSource __event_source_4 = {__poll, NULL};

static void test_event_sources(void)
{
	CwtEventNS *eventNS = cwtEventNS();

	CWT_TEST_OK(eventNS->registerSource(&__event_source_0));
	CWT_TEST_OK(eventNS->registerSource(&__event_source_1));
	CWT_TEST_OK(eventNS->registerSource(&__event_source_2));
	CWT_TEST_OK(eventNS->registerSource(&__event_source_3));
	CWT_TEST_OK(eventNS->registerSource(&__event_source_4));

	CWT_TEST_OK(eventNS->isRegisteredSource(&__event_source_0));
	CWT_TEST_OK(eventNS->isRegisteredSource(&__event_source_1));
	CWT_TEST_OK(eventNS->isRegisteredSource(&__event_source_2));
	CWT_TEST_OK(eventNS->isRegisteredSource(&__event_source_3));
	CWT_TEST_OK(eventNS->isRegisteredSource(&__event_source_4));

	eventNS->unregisterSource(&__event_source_0);
	CWT_TEST_NOK(eventNS->isRegisteredSource(&__event_source_0));

	eventNS->unregisterAllSources();
}

int main(int argc, char *argv[])
{
	CwtApp *app;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	app = cwtApp();
	if( !app->selectEvents(Cwt_Event_Kbd
			| Cwt_Event_PosixSignal
			| Cwt_Event_Clock ) )
		return -1;

	app->main();

	return 0;
}


