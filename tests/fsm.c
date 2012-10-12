/**
 * @file   fsm-char.c
 * @author wladt
 * @date   Oct 10, 2012 11:40:55 AM
 *
 * @brief
 */

#include <cwt/fsm.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <cwt/str.h>
#include <cwt/test.h>

static const char *__alpha_chars     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char *__digit_chars     = "0123456789";

static const CWT_CHAR *__alpha_cwt_chars = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static const CWT_CHAR *__digit_cwt_chars = _T("0123456789");

static int __ints[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static void test_char_helpers(void)
{
	size_t i, nalphas, ndigits;

	nalphas = strlen(__alpha_chars);
	ndigits = strlen(__digit_chars);

	for(i = 0; i < nalphas; i++) {
		CWT_TEST_OK(cwtBelongChar((void*)&__alpha_chars[i], __alpha_chars, nalphas));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(cwtBelongChar((void*)&__digit_chars[i], __digit_chars, ndigits));
	}

	CWT_TEST_OK(cwtExactChar(__alpha_chars, nalphas, __alpha_chars, nalphas));
	CWT_TEST_NOK(cwtExactChar(__alpha_chars, nalphas, __alpha_chars+1, nalphas-1));
	CWT_TEST_NOK(cwtExactChar(NULL, 0, __alpha_chars, nalphas));
	CWT_TEST_NOK(cwtExactChar(__alpha_chars, nalphas, NULL, 0));
}

static void test_cwt_char_helpers(void)
{
	size_t i, nalphas, ndigits;

	nalphas = cwtStrNS()->strLen(__alpha_cwt_chars);
	ndigits = cwtStrNS()->strLen(__digit_cwt_chars);

	for(i = 0; i < nalphas; i++) {
		CWT_TEST_OK(cwtBelongCwtChar((void*)&__alpha_cwt_chars[i], __alpha_cwt_chars, nalphas));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(cwtBelongCwtChar((void*)&__digit_cwt_chars[i], __digit_cwt_chars, ndigits));
	}

	CWT_TEST_OK(cwtExactCwtChar(__alpha_cwt_chars, nalphas, __alpha_cwt_chars, nalphas));
	CWT_TEST_NOK(cwtExactCwtChar(__alpha_cwt_chars, nalphas, __alpha_cwt_chars+1, nalphas-1));
	CWT_TEST_NOK(cwtExactCwtChar(NULL, 0, __alpha_cwt_chars, nalphas));
	CWT_TEST_NOK(cwtExactCwtChar(__alpha_cwt_chars, nalphas, NULL, 0));
}

static void test_int_helpers(void)
{
	size_t i, nints;

	nints = sizeof(__ints);

	for(i = 0; i < nints; i++) {
		CWT_TEST_OK(cwtBelongInt((void*)&__ints[i], __ints, nints));
	}

	CWT_TEST_OK(cwtExactInt(__ints, nints, __ints, nints));
	CWT_TEST_NOK(cwtExactInt(__ints, nints, &__ints[1], nints-1));
	CWT_TEST_NOK(cwtExactInt(NULL, 0, &__ints, nints));
	CWT_TEST_NOK(cwtExactInt(&__ints, nints, NULL, 0));
}


/*
 datetime = wday SP date SP time SP 4DIGIT
 date    = month SP ( 2DIGIT | ( SP 1DIGIT ))
 time    = 2DIGIT ":" 2DIGIT ":" 2DIGIT
 wday    = "Mon" | "Tue" | "Wed" | "Thu" | "Fri" | "Sat" | "Sun"
 month   = "Jan" | "Feb" | "Mar" | "Apr" | "May" | "Jun"
 	 	 | "Jul" | "Aug" | "Sep" | "Oct" | "Nov" | "Dec"
*/

struct set_weekday_args {
	int wday;
};

struct set_month_args {
	int mon;
};


size_t parse_seq(const CWT_CHAR *data, size_t len, size_t from, size_t to, BOOL (*is_elem_fn)(CWT_CHAR) )
{
	size_t n = 0;
	while( n <= to && len-- && is_elem_fn(*data++) ) {
		n++;
	}
	return n >= from && n <= to	? n	: 0;
}

static void set_weekday(const void *data, size_t len, void *context, void *action_args)
{
	struct set_weekday_args *swa = (struct set_weekday_args*)action_args;
	struct tm *tm = (struct tm*)context;
	CWT_UNUSED(data);
	CWT_UNUSED(len);
	tm->tm_wday = swa->wday;
}

static void set_month(const void *data, size_t len, void *context, void *action_args)
{
	struct set_month_args *sma = (struct set_month_args*)action_args;
	struct tm *tm = (struct tm*)context;
	CWT_UNUSED(data);
	CWT_UNUSED(len);
	tm->tm_mon = sma->mon;
}


static BOOL parse_uint_digits(const CWT_CHAR *s, size_t len, int radix, UINT *d)
{
	CwtStrNS *strNS = cwtStrNS();
	BOOL ok;
	CWT_CHAR buf[64];
	UINT n;
	size_t i;

	strNS->strNcpy(buf, s, len);
	buf[len] = 0;

	for( i = 0; i < len; i++ )
		if( !strNS->isDigit(buf[i]) )
			return FALSE;

	n = strNS->toUINT(buf, radix, &ok);

	if( ok && d )
		*d = n;
	return ok;
}

static ssize_t parse_mday(void *context, const void *data, size_t len)
{
	if( len >= 2 ) {
		struct tm *tm = (struct tm*)context;
		const CWT_CHAR *ptr = (const CWT_CHAR *)data;
		UINT mday;
		len = 2;

		if( cwtStrNS()->isSpace(ptr[0]) ) {
			ptr++;
			len--;
		}

		if( parse_uint_digits(ptr, len, 10, &mday)
				&& mday < 32 ) {
			tm->tm_mday = (int)mday;
			return (ssize_t)len;
		}
	}
	return (ssize_t)-1;
}


static ssize_t parse_hour(void *context, const void *data, size_t len)
{
	if( len >= 2 ) {
		struct tm *tm = (struct tm*)context;
		UINT hour;

		if( parse_uint_digits((const CWT_CHAR *)data, 2, 10, &hour)
				&& hour < 24 ) {
			tm->tm_hour = (int)hour;
			return 2;
		}
	}
	return (ssize_t)-1;
}

static ssize_t parse_min(void *context, const void *data, size_t len)
{
	if( len >= 2 ) {
		struct tm *tm = (struct tm*)context;
		UINT min;

		if( parse_uint_digits((const CWT_CHAR *)data, 2, 10, &min)
				&& min < 60 ) {
			tm->tm_min = (int)min;
			return 2;
		}
	}
	return (ssize_t)-1;
}

static ssize_t parse_sec(void *context, const void *data, size_t len)
{
	if( len >= 2 ) {
		struct tm *tm = (struct tm*)context;
		UINT sec;

		if( parse_uint_digits((const CWT_CHAR *)data, 2, 10, &sec)
				&& sec < 60 ) {
			tm->tm_sec = (int)sec;
			return 2;
		}
	}
	return (ssize_t)-1;
}

static ssize_t parse_year(void *context, const void *data, size_t len)
{
	if( len >= 4 ) {
		struct tm *tm = (struct tm*)context;
		UINT year;

		if( parse_uint_digits((const CWT_CHAR *)data, 4, 10, &year) ) {
			tm->tm_year = ((int)year) - 1900;
			return 4;
		}
	}
	return (ssize_t)-1;
}


static struct set_weekday_args sun = {0};
static struct set_weekday_args mon = {1};
static struct set_weekday_args tue = {2};
static struct set_weekday_args wed = {3};
static struct set_weekday_args thu = {4};
static struct set_weekday_args fri = {5};
static struct set_weekday_args sat = {6};

static CwtFsmTransition wday_fsm[] = {
	  {-1, FSM_MATCH_STR(_T("Mon"),3), FSM_CHAINED, set_weekday, &mon }
	, {-1, FSM_MATCH_STR(_T("Tue"),3), FSM_CHAINED, set_weekday, &tue }
	, {-1, FSM_MATCH_STR(_T("Wed"),3), FSM_CHAINED, set_weekday, &wed }
	, {-1, FSM_MATCH_STR(_T("Thu"),3), FSM_CHAINED, set_weekday, &thu }
	, {-1, FSM_MATCH_STR(_T("Fri"),3), FSM_CHAINED, set_weekday, &fri }
	, {-1, FSM_MATCH_STR(_T("Sat"),3), FSM_CHAINED, set_weekday, &sat }
	, {-1, FSM_MATCH_STR(_T("Sun"),3), FSM_TERM, set_weekday,    &sun }
};



/*
month   = "Jan" | "Feb" | "Mar" | "Apr" | "May" | "Jun"
 	 	 | "Jul" | "Aug" | "Sep" | "Oct" | "Nov" | "Dec"
*/
static struct set_month_args jan = {0};
static struct set_month_args feb = {1};
static struct set_month_args mar = {2};
static struct set_month_args apr = {3};
static struct set_month_args may = {4};
static struct set_month_args jun = {5};
static struct set_month_args jul = {6};
static struct set_month_args aug = {7};
static struct set_month_args sep = {8};
static struct set_month_args oct = {9};
static struct set_month_args nov = {10};
static struct set_month_args dec = {11};

static CwtFsmTransition month_fsm[] = {
	  {-1, FSM_MATCH_STR(_T("Jan"),3), FSM_CHAINED, set_month, &jan}
	, {-1, FSM_MATCH_STR(_T("Feb"),3), FSM_CHAINED, set_month, &feb}
	, {-1, FSM_MATCH_STR(_T("Mar"),3), FSM_CHAINED, set_month, &mar}
	, {-1, FSM_MATCH_STR(_T("Apr"),3), FSM_CHAINED, set_month, &apr}
	, {-1, FSM_MATCH_STR(_T("May"),3), FSM_CHAINED, set_month, &may}
	, {-1, FSM_MATCH_STR(_T("Jun"),3), FSM_CHAINED, set_month, &jun}
	, {-1, FSM_MATCH_STR(_T("Jul"),3), FSM_CHAINED, set_month, &jul}
	, {-1, FSM_MATCH_STR(_T("Aug"),3), FSM_CHAINED, set_month, &aug}
	, {-1, FSM_MATCH_STR(_T("Sep"),3), FSM_CHAINED, set_month, &sep}
	, {-1, FSM_MATCH_STR(_T("Oct"),3), FSM_CHAINED, set_month, &oct}
	, {-1, FSM_MATCH_STR(_T("Nov"),3), FSM_CHAINED, set_month, &nov}
	, {-1, FSM_MATCH_STR(_T("Dec"),3), FSM_TERM,    set_month, &dec}
};


/* date = month SP ( 2DIGIT | ( SP 1DIGIT )) */
static CwtFsmTransition date_fsm[] = {
	  { 1, FSM_MATCH_FSM(month_fsm), FSM_TERM, NULL, NULL }
	, { 2, FSM_MATCH_STR(" ", 1), FSM_TERM, NULL, NULL }
	, {-1, FSM_MATCH_FUNC(parse_mday), FSM_TERM, NULL, NULL }
};


static CwtFsmTransition time_fsm[] = {
	  { 1, FSM_MATCH_FUNC(parse_hour), FSM_TERM, NULL, NULL }
	, { 2, FSM_MATCH_CHAR(":",1), FSM_TERM, NULL, NULL }
	, { 3, FSM_MATCH_FUNC(parse_min), FSM_TERM, NULL, NULL }
	, { 4, FSM_MATCH_CHAR(":",1), FSM_TERM, NULL, NULL }
	, {-1, FSM_MATCH_FUNC(parse_sec), FSM_TERM, NULL, NULL }
};


static CwtFsmTransition datetime_fsm[] = {
	  { 1, FSM_MATCH_FSM(wday_fsm), FSM_TERM, NULL, NULL }
	, { 2, FSM_MATCH_STR(" ", 1), FSM_TERM, NULL, NULL }
	, { 3, FSM_MATCH_FSM(date_fsm), FSM_TERM, NULL, NULL }
	, { 4, FSM_MATCH_STR(" ", 1), FSM_TERM, NULL, NULL }
	, { 5, FSM_MATCH_FSM(time_fsm), FSM_TERM, NULL, NULL }
	, { 6, FSM_MATCH_STR(" ", 1), FSM_TERM, NULL, NULL }
	, {-1, FSM_MATCH_FUNC(parse_year), FSM_TERM, NULL, NULL }
};



static void test_parse_date(void)
{
	CwtFsmNS *fsmNS = cwtFsmNS();
	const CWT_CHAR *date_str = _T("Sat Apr 29 12:34:56 1972");
	const CWT_CHAR *date_str_incorrect_0 = _T("Sat Apt 29 12:34:56 1972"); /* invalid month */
	const CWT_CHAR *date_str_incorrect_1 = _T("Sat Apr 29 32:34:56 1972"); /* invalid hour */
	const CWT_CHAR *date_str_incorrect_2 = _T("Sat Apr 29 12:34:56  1972"); /* extra space char before year */
	struct tm tm;
	CwtFsm fsm;

	FSM_INIT(fsm, CWT_CHAR, datetime_fsm, &tm, cwtBelongCwtChar, cwtExactCwtChar);

	CWT_TEST_OK(fsmNS->exec(&fsm, 0, date_str, cwtStrNS()->strLen(date_str)) >= 0);

	CWT_TEST_OK(tm.tm_wday == 6);
	CWT_TEST_OK(tm.tm_mon == 3);
	CWT_TEST_OK(tm.tm_hour == 12);
	CWT_TEST_OK(tm.tm_min == 34);
	CWT_TEST_OK(tm.tm_sec == 56);
	CWT_TEST_OK(tm.tm_year + 1900 == 1972);

	CWT_TEST_NOK(fsmNS->exec(&fsm, 0, date_str_incorrect_0, cwtStrNS()->strLen(date_str_incorrect_0)) >= 0);
	CWT_TEST_NOK(fsmNS->exec(&fsm, 0, date_str_incorrect_1, cwtStrNS()->strLen(date_str_incorrect_1)) >= 0);
	CWT_TEST_NOK(fsmNS->exec(&fsm, 0, date_str_incorrect_2, cwtStrNS()->strLen(date_str_incorrect_2)) >= 0);

}



int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(186);

	test_char_helpers();
	test_cwt_char_helpers();
	test_int_helpers();
	test_parse_date();

	CWT_END_TESTS;
}


