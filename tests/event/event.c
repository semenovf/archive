#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/event/event.h>

static CwtEventSource __event_source_0;
static CwtEventSource __event_source_1;
static CwtEventSource __event_source_2;
static CwtEventSource __event_source_3;
static CwtEventSource __event_source_4;

static void test_event_sources(void)
{
	CwtEventNS *eventNS = cwtEventNS();
	eventNS->registerSource(&__event_source_0);
	eventNS->registerSource(&__event_source_1);
	eventNS->registerSource(&__event_source_2);
	eventNS->registerSource(&__event_source_3);
	eventNS->registerSource(&__event_source_4);

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
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(6);

	test_event_sources();

	CWT_END_TESTS;
}


