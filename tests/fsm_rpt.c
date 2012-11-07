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
	{-1,-1, FSM_MATCH_RPT(&__rep_context), FSM_ACCEPT, action, NULL }
};

static struct _FsmRptTestEntry {
	int from, to;
	ssize_t ret;
} __fsmRptTestEntries[] = {
	  {-1,-1,18 }
	, { 0, 0, 0 }
	, { 0, 1, 3 }
	, { 0, 2, 6 }
	, { 0, 3, 9 }
	, { 0, 4,12 }
	, { 0, 5,15 }
	, { 0, 6,18 }
	, { 0, 7,18 }
	, { 0, 8,18 }
	, { 1, 1, 3 }
	, { 1, 2, 6 }
	, { 1, 3, 9 }
	, { 1, 4,12 }
	, { 2, 2, 6 }
	, { 2, 2, 6 }
	, {20,30,-1 }
};



static void test_fsm_rep(void)
{
	CwtFsmNS *fsmNS = cwtFsmNS();
	const CWT_CHAR *pct_str = _T("%AB%CD%EF%01%02%03");
	int i, n;
	CwtFsm fsm;

	FSM_INIT(fsm, CWT_CHAR, pct_encoded_str_fsm, NULL, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);

	n = sizeof(__fsmRptTestEntries)/sizeof(__fsmRptTestEntries[0]);

	for( i = 0; i < n; i++ ) {
		__rep_context.from  = __fsmRptTestEntries[i].from;
		__rep_context.to    = __fsmRptTestEntries[i].to;
		CWT_TEST_FAIL(fsmNS->exec(&fsm, 0, pct_str, cwtStrNS()->strLen(pct_str)) == __fsmRptTestEntries[i].ret);
	}
}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_UNUSED(fsm_unused_commons_fn);

	__fsmNS = cwtFsmNS();

	CWT_BEGIN_TESTS(17);

	test_fsm_rep();

	CWT_END_TESTS;
}
