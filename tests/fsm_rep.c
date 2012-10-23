/**
 * @file   fsm_rep.c
 * @author wladt
 * @date   Oct 18, 2012 12:24:37 PM
 *
 * @brief Testing CwtFsm::repetition functionality
 */

#include <cwt/fsm.h>
#include <cwt/str.h>
#include <cwt/test.h>
#include <cwt/fsm_common.h>
#include <cwt/logger.h>

static CwtFsmNS *__fsmNS = NULL;

/* "%" HEXDIG HEXDIG */
static CwtFsmTransition pct_encoded_fsm[] = {
      { 1,-1, FSM_MATCH_STR(_T("%"), 1), FSM_NORMAL, NULL, NULL }
    , { 2,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_NORMAL, NULL, NULL }
    , {-1,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
};
/*  */

static CwtFsmRepetitionContext __rep_context = {
	pct_encoded_fsm, -1, -1
};

static void action(const void *data, size_t len, void *context, void *action_args)
{
	CWT_UNUSED(data);
	CWT_UNUSED(len);
	CWT_UNUSED(context);
	CWT_UNUSED(action_args);

	cwtLoggerNS()->trace(_T("PCT-encoded element found"));
}

static CwtFsmTransition pct_encoded_str_fsm[] = {
	{-1,-1, FSM_MATCH_REP(&__rep_context), FSM_ACCEPT, action, NULL }
};

static struct _FsmRepTestEntry {
	int from, count;
	ssize_t ret;
} __fsmRepTestEntries[] = {
	  {-1,-1,18 }
	, { 0, 0, 0 }
	, { 1, 1, 3 }
	, { 1, 2, 6 }
	, { 5, 2, 3 }
	, {20,30,-1 }
};



static void test_fsm_rep(void)
{
	CwtFsmNS *fsmNS = cwtFsmNS();
	const CWT_CHAR *pct_str = _T("%AB%CD%EF%01%02%03");
	int i, n;
	CwtFsm fsm;
/*
	const CWT_CHAR *date_str_incorrect_0 = _T("Cat Apr 29 12:34:56 1972");   invalid weekday
	const CWT_CHAR *date_str_incorrect_1 = _T("Sat Apt 29 12:34:56 1972");   invalid month
	const CWT_CHAR *date_str_incorrect_2 = _T("Sat Apr 29 32:34:56 1972");   invalid hour
	const CWT_CHAR *date_str_incorrect_3 = _T("Sat Apr 29 12:34:56  1972");  extra space char before year
	const CWT_CHAR *date_str_incorrect_4 = _T("Sat Apr 29 12:3:56 1972");    too little digits for minutes
*/


	FSM_INIT(fsm, CWT_CHAR, pct_encoded_str_fsm, NULL, cwtBelongCwtChar, cwtExactCwtChar);

	n = sizeof(__fsmRepTestEntries)/sizeof(__fsmRepTestEntries[0]);

	for( i = 0; i < n; i++ ) {
		__rep_context.from  = __fsmRepTestEntries[i].from;
		__rep_context.count = __fsmRepTestEntries[i].count;
		CWT_TEST_FAIL(fsmNS->exec(&fsm, 0, pct_str, cwtStrNS()->strLen(pct_str)) == __fsmRepTestEntries[i].ret);
	}
}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);
	CWT_UNUSED(SP_FSM);
	CWT_UNUSED(ALPHA_FSM);

	__fsmNS = cwtFsmNS();

	CWT_BEGIN_TESTS(12);

	test_fsm_rep();

	CWT_END_TESTS;
}
