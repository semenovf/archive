/*
 * cwt/event/clock.c
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */


#include <string.h>
#include <cwt/event/clock.h>
#include <time.h>
#include <cwt/string.h>
#include <cwt/stack.h>
#include <cwt/event/event.h>


struct CwtEventClockTime
{
	CwtEvent base;
	int hour;
	int min;
	int sec;
};
typedef struct CwtEventClockTime CwtEventClockTime;

struct CwtEventClockDate
{
	CwtEvent base;
	int day;
	int mon;
	int year;
};
typedef struct CwtEventClockDate CwtEventClockDate;

static CwtStack __cwt_evt_time_handlers = { NULL };
static CwtStack __cwt_evt_date_handlers = { NULL };
static void     __cwtEventClockPoll(void);

static CwtEventSource __cwt_evt_clock_source = { __cwtEventClockPoll };
static struct tm __tm = {
	0, 0, 0, 0, 0, 0, 0, 0
#ifdef CWT_CC_GNUC
	, 0     /* tm_isdst - DST.[-1/0/1] */
	, 0L    /* long int tm_gmtoff - Seconds east of UTC */
	, NULL  /* __const char *tm_zone */
#endif
};

CwtEventSourcePtr cwtEventClockSource(void)
{
	return &__cwt_evt_clock_source;
}

Stack* cwtEventClockTimeHandler(void)
{
	return &__cwt_evt_time_handlers;
}

Stack* cwtEventClockDateHandler(void)
{
	return &__cwt_evt_date_handlers;
}

static void __cwtEventClockPoll(void)
{
	struct tm *time_now;
	time_t secs_now;
	BOOL update_time = FALSE;
	BOOL update_date = FALSE;

	time(&secs_now);

	/* TODO !!!Critical section */
	time_now = localtime(&secs_now);

	if( time_now->tm_year != __tm.tm_year
			|| time_now->tm_mon != __tm.tm_mon
			|| time_now->tm_mday != __tm.tm_mday ) {
		update_date = TRUE;
	}

	if( time_now->tm_hour != __tm.tm_hour
			|| time_now->tm_min != __tm.tm_min
			|| time_now->tm_sec != __tm.tm_sec ) {
		update_time = TRUE;
	}

	if( update_time || update_date ) {
		memcpy(&__tm, time_now, sizeof(struct tm));

		if( update_time ) {
			cwtEventPostClockTime(__tm.tm_hour, __tm.tm_min, __tm.tm_sec);
		}

		if( update_date ) {
			cwtEventPostClockDate(__tm.tm_year + 1900, __tm.tm_mon + 1, __tm.tm_mday);
		}
	}
}


void cwtEventPeekClockTime(CwtEventPtr pevt, int *phour, int *pmin, int *psec)
{
	CwtEventClockTime *pevt_time = (CwtEventClockTime*)pevt;
	CWT_ASSERT(pevt);

	if( phour ) *phour = pevt_time->hour;
	if( pmin ) *pmin = pevt_time->min;
	if( psec ) *psec = pevt_time->sec;
}

void cwtEventPeekClockDate(CwtEventPtr pevt, int *pday, int *pmon, int *pyear)
{
	CwtEventClockDate *pevt_time = (CwtEventClockDate*)pevt;
	CWT_ASSERT(pevt);

	if( pday ) *pday = pevt_time->day;
	if( pmon ) *pmon = pevt_time->mon;
	if( pyear ) *pyear = pevt_time->year;
}


void cwtEventPostClockTime(int hour, int min, int sec)
{
	CwtEventClockTime *pevt = CWT_MALLOC(CwtEventClockTime);
	pevt->hour = hour;
	pevt->min  = min;
	pevt->sec  = sec;

	__cwtEventInitEvent((CwtEventPtr)pevt, &__cwt_evt_time_handlers, cwtEventDefaultDestructor);
	cwtEventPost((CwtEventPtr)pevt);
}


void cwtEventPostClockDate(int day, int mon, int year)
{
	CwtEventClockDate *pevt = CWT_MALLOC(CwtEventClockDate);
	pevt->year = year;
	pevt->mon  = mon;
	pevt->day  = day;

	__cwtEventInitEvent((CwtEventPtr)pevt, &__cwt_evt_date_handlers, cwtEventDefaultDestructor);
	cwtEventPost((CwtEventPtr)pevt);
}


/**
 * ���樨��� ������� ᮡ�⨩ �� �ᮢ
 *
 */
void cwtEventClockRefresh(void)
{
	__tm.tm_hour = -1;
	__tm.tm_mday = -1;
	__cwtEventClockPoll();
}

