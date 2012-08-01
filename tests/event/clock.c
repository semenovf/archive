#include <stdio.h>
#include <cwt/logger.h>
#include <cwt/types.h>
#include <cwt/test.h>
#include <cwt/event/event.h>
#include <cwt/event/clock.h>

static int ntimes = 0;

static CwtEventNS        *__eventNS = NULL;
static CwtEventClockNS   *__eventClockNS = NULL;


static BOOL quit_event(CwtEvent *pevt)
{
	CWT_UNUSED(pevt);

	if( ntimes >= 3 )
		__eventNS->quit();
	return FALSE;
}

static BOOL on_clock_time(CwtEvent *pevt)
{
	int hour, min, sec;

	__eventClockNS->peekTime(pevt, &hour, &min, &sec);

	ntimes++;
	printf_info(_T("Clock time event: %02d:%02d:%02d"), hour, min, sec);
	return FALSE;
}

static BOOL on_clock_date(CwtEvent *pevt)
{
	int day, mon, year;

	__eventClockNS->peekDate(pevt, &day, &mon, &year);

	printf_info(_T("Clock date event: %02d:%02d:%02d"), day, mon, year);
	return FALSE;
}


static void cwt_evt_test_0(void)
{
	__eventNS->registerSource(__eventClockNS->source());
	CWT_TEST_OK(__eventNS->isRegisteredSource(__eventClockNS->source()));

	__eventNS->pushHandler(__eventClockNS->timeHandler(), on_clock_time);
	__eventNS->pushHandler(__eventClockNS->timeHandler(), quit_event);
	__eventNS->pushHandler(__eventClockNS->dateHandler(), on_clock_date);

	__eventNS->loop();
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	__eventNS = cwtEventNS();
	__eventClockNS = cwtEventClockNS();

	CWT_BEGIN_TESTS(1);

	cwt_evt_test_0();

	CWT_END_TESTS;
}


