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

static void cwt_evt_test_event_sources(void)
{
	CWT_TEST_OK(cwtEventRegisterSource(&__event_source_0));
	CWT_TEST_OK(cwtEventRegisterSource(&__event_source_1));
	CWT_TEST_OK(cwtEventRegisterSource(&__event_source_2));
	CWT_TEST_OK(cwtEventRegisterSource(&__event_source_3));
	CWT_TEST_OK(cwtEventRegisterSource(&__event_source_4));

	CWT_TEST_OK(cwtEventIsRegisteredSource(&__event_source_0));
	CWT_TEST_OK(cwtEventIsRegisteredSource(&__event_source_1));
	CWT_TEST_OK(cwtEventIsRegisteredSource(&__event_source_2));
	CWT_TEST_OK(cwtEventIsRegisteredSource(&__event_source_3));
	CWT_TEST_OK(cwtEventIsRegisteredSource(&__event_source_4));

	CWT_TEST_OK(cwtEventUnregisterSource(&__event_source_0));
	CWT_TEST_NOK(cwtEventIsRegisteredSource(&__event_source_0));

	cwtEventUnregisterAllSources();
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(12);

	cwt_evt_test_event_sources();

	CWT_END_TESTS;
}


