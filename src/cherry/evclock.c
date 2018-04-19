/*
 * ev_clock.c
 *
 *  Created on: 23.01.2012
 *      Author: wladt
 */


#include <cwt/cherry/event.h>
#include <cwt/string.h>
#include <cwt/logger.h>

/*
struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  char *__tm_zone;
  int __tm_gmtoff;
};
*/

static struct tm __tm = { 0, 0, 0, 0, 0, 0, 0, 0 };

void _cwt_process_clock(void)
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
			CWT_CLOCK_EVENT *evt = JQ_MALLOC(CWT_CLOCK_EVENT);
			evt->hour = __tm.tm_hour;
			evt->min  = __tm.tm_min;
			evt->sec  = __tm.tm_sec;
			cwt_post_event(CWT_EVT_CLOCK_TIME, evt, CWT_DEFAULT_FREE);
		}

		if( update_date ) {
			CWT_CLOCK_EVENT *evt = JQ_MALLOC(CWT_CLOCK_EVENT);
			evt->year = __tm.tm_year + 1900;
			evt->mon  = __tm.tm_mon + 1;
			evt->day  = __tm.tm_mday;
			cwt_post_event(CWT_EVT_CLOCK_DATE, evt, CWT_DEFAULT_FREE);
		}
	}
}

void cwt_update_clock(void)
{
	__tm.tm_hour = -1;
	__tm.tm_mday = -1;
	_cwt_process_clock();
}
