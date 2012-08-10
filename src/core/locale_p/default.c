/*
 * default.c
 *
 *  Created on: 10.08.2012
 *      Author: user
 */


#ifndef __CWT_LOCALE_H__
#	error Do not use this file directly
#endif

static const CWT_CHAR* __weekDays[]  = {
	  _T("Sunday")
	, _T("Monday")
	, _T("Tuesday")
	, _T("Wednesday")
	, _T("Thursday")
	, _T("Friday")
	, _T("Saturday")
	, NULL
};


static const CWT_CHAR* __weekDaysAbr[]  = {
	  _T("Sun")
	, _T("Mon")
	, _T("Tue")
	, _T("Wed")
	, _T("Thu")
	, _T("Fri")
	, _T("Sat")
	, NULL
};

static const CWT_CHAR* __months[] = {
	  _T("January")
	, _T("February")
	, _T("March")
	, _T("April")
	, _T("May")
	, _T("June")
	, _T("July")
	, _T("August")
	, _T("September")
	, _T("October")
	, _T("November")
	, _T("December")
	, NULL
};

static const CWT_CHAR* __monthsAbr[] = {
	  _T("Jan")
	, _T("Feb")
	, _T("Mar")
	, _T("Apr")
	, _T("May")
	, _T("Jun")
	, _T("Jul")
	, _T("Aug")
	, _T("Sep")
	, _T("Oct")
	, _T("Nov")
	, _T("Dec")
	, NULL
};

static const CWT_CHAR* __amPm[] = { _T("AM") , _T("PM"), NULL };

static const CWT_CHAR* __loc_timeFormat(void)
{
	return _T("%I:%M:%S");
}

static const CWT_CHAR* __loc_dateFormat(void)
{
	return _T("%d/%m/%Y");
}

static const CWT_CHAR* __loc_dateTimeFormat(void)
{
	return _T("%a %d %b %Y %I:%M:%S %p %Z");
}

static const CWT_CHAR* __loc_amPmTimeFormat (void)
{
	return _T("%I:%M:%S %p");
}

static const CWT_CHAR* const* __loc_weekDays(void)
{
	return __weekDays;
}

static const CWT_CHAR* const* __loc_weekDaysAbr(void)
{
	return __weekDaysAbr;
}

static const CWT_CHAR* const* __loc_months(void)
{
	return __months;
}

static const CWT_CHAR* const* __loc_monthsAbr(void)
{
	return __monthsAbr;
}

static const CWT_CHAR* const* __loc_amPm(void)
{
	return __amPm;
}
