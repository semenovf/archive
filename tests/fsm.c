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

static const char *__alpha_cwt_chars = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static const char *__digit_cwt_chars = _T("0123456789");

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


size_t parse_seq(const CWT_CHAR *data, size_t len, size_t from, size_t to, BOOL (*is_elem_fn)(CWT_CHAR) )
{
	size_t n = 0;
	while( n <= to && len-- && is_elem_fn(*data++) ) {
		n++;
	}
	return n >= from && n <= to	? n	: 0;
}

static void set_weekday(void *context, void *action_args)
{
  struct set_weekday_args *swa = (struct set_weekday_args*)action_args;
  struct tm *tmp = (struct tm*)context;
  tmp->tm_wday = swa->wday;
}

static BOOL parse_uint_digits(const CWT_CHAR *s, size_t len, int radix, UINT *d)
{
	CwtStrNS *strNS = cwtStrNS();
	BOOL ok;
	CWT_CHAR buf[64];
	UINT n;

	strNS->strNcpy(buf, s, len);
	buf[len] = 0;
	n = strNS->toUINT(buf, radix, &ok);

	if( ok && d )
		*d = n;
	return ok;
}

static size_t parse_hour(void *context, const void *data, size_t len)
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
	return 0;
}

static size_t parse_min(void *context, const void *data, size_t len)
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
	return 0;
}

static size_t parse_sec(void *context, const void *data, size_t len)
{
	if( len >= 2 ) {
		struct tm *tm = (struct tm*)context;
		UINT sec;

		if( parse_uint_digits((const CWT_CHAR *)data, 2, 10, &sec)
				&& sec < 60 ) {
			tm->tm_min = (int)sec;
			return 2;
		}
	}
	return 0;
}


static CwtFsmTransition wday_fsm[] = {
	  {-1, FSM_MATCH_STR("Mon",3), FSM_ACCEPT, set_weekday, (void*)&(struct set_weekday_args){1} }
	, {-1, FSM_MATCH_STR("Tue",3), FSM_ACCEPT, set_weekday, (void*)&(struct set_weekday_args){2} }
	, {-1, FSM_MATCH_STR("Wed",3), FSM_ACCEPT, set_weekday, (void*)&(struct set_weekday_args){3} }
	, {-1, FSM_MATCH_STR("Thu",3), FSM_ACCEPT, set_weekday, (void*)&(struct set_weekday_args){4} }
	, {-1, FSM_MATCH_STR("Fri",3), FSM_ACCEPT, set_weekday, (void*)&(struct set_weekday_args){5} }
	, {-1, FSM_MATCH_STR("Sat",3), FSM_ACCEPT, set_weekday, (void*)&(struct set_weekday_args){6} }
	, {-1, FSM_MATCH_STR("Sun",3), FSM_REJECT, set_weekday, (void*)&(struct set_weekday_args){0} }
};

static CwtFsmTransition time_fsm[] = {
/* state 0: HOUR */
	  { 1, FSM_MATCH_FUNC(parse_hour), FSM_REJECT, NULL, NULL }
	, { 2, FSM_MATCH_CHAR(":",1), FSM_REJECT, NULL, NULL }
	, { 3, FSM_MATCH_FUNC(parse_min), FSM_REJECT, NULL, NULL }
	, { 4, FSM_MATCH_CHAR(":",1), FSM_REJECT, NULL, NULL }
	, {-1, FSM_MATCH_FUNC(parse_sec), FSM_REJECT, NULL, NULL }
};

/*
static CwtFsmTransition datetime_fsm[][3] = {
	  { { 1, FSM_MATCH_FUNC(parse_hour), NULL, NULL }, FSM_NIL_ENTRY, FSM_NIL_ENTRY }
	, { FSM_NIL_ENTRY, { 4, FSM_MATCH_CHAR(" ",1), NULL, NULL }, FSM_NIL_ENTRY }
};
*/



static void test_parse_date(void)
{
	CwtFsmNS *fsmNS = cwtFsmNS();
	const char *date_str = _T("Mon");
	const char *time_str = _T("12:34:56");
	struct tm date;
	CwtFsm fsm;

	FSM_INIT(fsm, wday_fsm, &date, cwtBelongCwtChar, cwtExactCwtChar);

	CWT_TEST_OK(fsmNS->exec(&fsm, 0, date_str, cwtStrNS()->strLen(date_str)) > 0);

	fsm.trans_tab = time_fsm;
	CWT_TEST_OK(fsmNS->exec(&fsm, 0, time_str, cwtStrNS()->strLen(time_str)) > 0);
}



int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(176);

	test_char_helpers();
	test_cwt_char_helpers();
	test_int_helpers();
	test_parse_date();

	CWT_END_TESTS;
}


