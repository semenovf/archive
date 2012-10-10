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
		CWT_TEST_OK(cwtBelongChar((void*)&__alpha_cwt_chars[i], __alpha_cwt_chars, nalphas));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(cwtBelongChar((void*)&__digit_cwt_chars[i], __digit_cwt_chars, ndigits));
	}

	CWT_TEST_OK(cwtExactChar(__alpha_cwt_chars, nalphas, __alpha_cwt_chars, nalphas));
	CWT_TEST_NOK(cwtExactChar(__alpha_cwt_chars, nalphas, __alpha_cwt_chars+1, nalphas-1));
	CWT_TEST_NOK(cwtExactChar(NULL, 0, __alpha_cwt_chars, nalphas));
	CWT_TEST_NOK(cwtExactChar(__alpha_cwt_chars, nalphas, NULL, 0));
}

static void test_int_helpers(void)
{
	size_t i, nints;

	nints = sizeof(__ints);

	for(i = 0; i < nints; i++) {
		CWT_TEST_OK(cwtBelongChar((void*)&__ints[i], __ints, nints));
	}

	CWT_TEST_OK(cwtExactChar(__ints, nints, __ints, nints));
	CWT_TEST_NOK(cwtExactChar(__ints, nints, &__ints[1], nints-1));
	CWT_TEST_NOK(cwtExactChar(NULL, 0, &__ints, nints));
	CWT_TEST_NOK(cwtExactChar(&__ints, nints, NULL, 0));
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

static void set_weekday(void *context, void *action_args)
{
  struct set_weekday_args *swa = (struct set_weekday_args*)action_args;
  struct tm *tmp = (struct tm*)context;
  tmp->tm_wday = swa->wday;
}

struct {
	int i;
union {
	  struct { int h; } height;
      char birthday[9];
      int age;
      float weight;
      } man;
} people = { 10, .man = { .height = { 14 }} };

struct {
	int next_tid; /* next transition id */
	CwtFsmMatchType match_type;
	union {
		struct { size_t len; void *chars; } str;
		struct _CwtFsmTransition *trans_tab;
		ssize_t (*trans_fn)();
	} condition;

	void (*action)(void *context, void *action_args);
	void *action_args;
} tr[] = {{ -1, Cwt_Fsm_Match_Str
	, .condition = {.trans_tab = NULL}
	, NULL, NULL}};

static CwtFsmTransition wday1_fsm[][] = {
	{{	-1
		, Cwt_Fsm_Match_Str
		, .condition = { .trans_tab = NULL }
		, set_weekday
		, NULL }}
};


static CwtFsmTransition wday_fsm[] = {
/* state 0 */
	{ {-1, FSM_MATCH_STR("Mon",3), set_weekday, (void*)&(struct set_weekday_args){1} }
	, {-1, FSM_MATCH_STR("Tue",3), set_weekday, (void*)&(struct set_weekday_args){2} }
	, {-1, FSM_MATCH_STR("Wed",3), set_weekday, (void*)&(struct set_weekday_args){3} }
	, {-1, FSM_MATCH_STR("Thu",3), set_weekday, (void*)&(struct set_weekday_args){4} }
	, {-1, FSM_MATCH_STR("Fri",3), set_weekday, (void*)&(struct set_weekday_args){5} }
	, {-1, FSM_MATCH_STR("Sat",3), set_weekday, (void*)&(struct set_weekday_args){6} }
	, {-1, FSM_MATCH_STR("Sun",3), set_weekday, (void*)&(struct set_weekday_args){0} }}
};

static void test_parse_date(void)
{
	CwtFsmNS *fsmNS = cwtFsmNS();
	const char *date_str = "Mon";
	struct tm date = {0};
	CwtFsm fsm = {wday_fsm, &date, cwtBelongChar, cwtExactChar};


	CWT_TEST_OK(fsmNS->exec(&fsm, date_str) >= (ssize_t)0);
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


