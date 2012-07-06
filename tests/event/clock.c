#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/event/event.h>
#include <cwt/event/clock.h>

static int ntimes = 0;

static BOOL quit_event(CwtEventPtr pevt)
{
	CWT_UNUSED(pevt);

	if( ntimes >= 3 )
		cwtEventQuit();
	return FALSE;
}

static BOOL on_clock_time(CwtEventPtr pevt)
{
	int hour, min, sec;
	cwtEventPeekClockTime(pevt, &hour, &min, &sec);

	ntimes++;
	printf_info("Clock time event: %02d:%02d:%02d", hour, min, sec);
	return FALSE;
}

static BOOL on_clock_date(CwtEventPtr pevt)
{
	int day, mon, year;

	cwtEventPeekClockDate(pevt, &day, &mon, &year);

	printf_info("Clock date event: %02d:%02d:%02d", day, mon, year);
	return FALSE;
}


static void cwt_evt_test_0(void)
{
	CWT_TEST_OK(cwtEventRegisterSource(cwtEventClockSource()));
	CWT_TEST_OK(cwtEventIsRegisteredSource(cwtEventClockSource()));

	cwtEventPushHandler(cwtEventClockTimeHandler(), on_clock_time);
	cwtEventPushHandler(cwtEventClockTimeHandler(), quit_event);
	cwtEventPushHandler(cwtEventClockDateHandler(), on_clock_date);

	cwtEventLoop();
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(2);

	cwt_evt_test_0();

	CWT_END_TESTS;
}


