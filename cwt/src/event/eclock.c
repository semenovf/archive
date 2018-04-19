/*
 * cwt/event/clock.c
 *
 *  Created on: 24.06.2012
 *      Author: wladt
 */


#include <string.h>
#include <time.h>
#include <cwt/string.h>
#include <cwt/event/event.h>
#include <cwt/event/clock.h>


typedef struct _CwtEventClockTime
{
	CwtEvent __base;
	int hour;
	int min;
	int sec;
} CwtEventClockTime;


typedef struct _CwtEventClockDate
{
	CwtEvent __base;
	int day;
	int mon;
	int year;
} CwtEventClockDate;


static CwtList *__time_handlers = NULL;
static CwtList *__date_handlers = NULL;

static void __pollClock(void);
static void __finish(CwtEventSource *source);

static CwtEventSource  __clock_source = { __pollClock, __finish };

static struct tm __tm = {
	0, 0, 0, 0, 0, 0, 0, 0
#ifdef CWT_CC_GNUC
	, 0     /* tm_isdst - DST.[-1/0/1] */
	, 0L    /* long int tm_gmtoff - Seconds east of UTC */
	, NULL  /* __const char *tm_zone */
#endif
};

static CwtEventSource* __source (void);
static void     __refresh       (void);
static CwtList* __timeHandler   (void);
static CwtList* __dateHandler   (void);
static void     __peekTime      (CwtEvent *pevt, int *phour, int *pmin, int *psec);
static void 	__peekDate      (CwtEvent *pevt, int *pday, int *pmon, int *pyear);
static void     __postTime      (int hour, int min, int sec);
static void     __postDate      (int day, int mon, int year);

static CwtEventClockNS __cwtEventClockNS = {
	  __source
	, __refresh
	, __timeHandler
	, __dateHandler
	, __peekTime
	, __peekDate
	, __postTime
	, __postDate
};

static CwtListNS    *__listNS = NULL;
static CwtEventNS   *__eventNS = NULL;

DLL_API_EXPORT CwtEventClockNS* cwt_event_clock_ns(void)
{
	if( !__listNS ) {
		__listNS = cwt_list_ns();
		__eventNS = cwt_event_ns();
	}

	if( !__time_handlers ) {
		__time_handlers = __listNS->createPtr(NULL);
		__date_handlers = __listNS->createPtr(NULL);
	}
	return &__cwtEventClockNS;
}


static inline CwtEventSource* __source(void)
{
	return &__clock_source;
}


static void __refresh(void)
{
	__tm.tm_hour = -1;
	__tm.tm_mday = -1;
	__pollClock();
}


static inline CwtList* __timeHandler(void)
{
	return __time_handlers;
}

static inline CwtList* __dateHandler(void)
{
	return __date_handlers;
}

static void __peekTime(CwtEvent *pevt, int *phour, int *pmin, int *psec)
{
	CwtEventClockTime *pevt_time = (CwtEventClockTime*)pevt;
	CWT_ASSERT(pevt);

	if( phour ) *phour = pevt_time->hour;
	if( pmin ) *pmin = pevt_time->min;
	if( psec ) *psec = pevt_time->sec;
}

static void __peekDate(CwtEvent *pevt, int *pday, int *pmon, int *pyear)
{
	CwtEventClockDate *pevt_time = (CwtEventClockDate*)pevt;
	CWT_ASSERT(pevt);

	if( pday ) *pday = pevt_time->day;
	if( pmon ) *pmon = pevt_time->mon;
	if( pyear ) *pyear = pevt_time->year;
}


static void __postTime(int hour, int min, int sec)
{
	CwtEventClockTime *pevt = CWT_MALLOCT(CwtEventClockTime);
	pevt->hour = hour;
	pevt->min  = min;
	pevt->sec  = sec;

	__eventNS->initEvent((CwtEvent*)pevt, __time_handlers, __eventNS->defaultDestructor);
	__eventNS->post((CwtEvent*)pevt);
}


static void __postDate(int day, int mon, int year)
{
	CwtEventClockDate *pevt = CWT_MALLOCT(CwtEventClockDate);
	pevt->year = year;
	pevt->mon  = mon;
	pevt->day  = day;

	__eventNS->initEvent((CwtEvent*)pevt, __date_handlers, __eventNS->defaultDestructor);
	__eventNS->post((CwtEvent*)pevt);
}


static void __pollClock(void)
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
			__postTime(__tm.tm_hour, __tm.tm_min, __tm.tm_sec);
		}

		if( update_date ) {
			__postDate(__tm.tm_year + 1900, __tm.tm_mon + 1, __tm.tm_mday);
		}
	}
}


static void __finish(CwtEventSource *source)
{
	CWT_UNUSED(source);

	if( !__time_handlers ) {
		 __listNS->free(__time_handlers);
		 __time_handlers = NULL;
	}

	if( !__date_handlers ) {
		 __listNS->free(__date_handlers);
		 __date_handlers = NULL;
	}
}
