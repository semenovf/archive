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
#include <cwt/fsm_common.h>

static CwtFsmNS *__fsmNS = NULL;

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


static void test_alternatives_simple(void)
{
	const CWT_CHAR *hexdig = _T("F");
	const CWT_CHAR *digit = _T("9");
	const CWT_CHAR *notdigit = _T("w");
	CwtFsm fsm;

	FSM_INIT(fsm, CWT_CHAR, HEXDIG_FSM, NULL, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hexdig, 0) == -1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hexdig, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, digit, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdigit, 1) < 0);
}


/* 0*DIGIT */
static CwtFsmTransition decimal0more_fsm[] = {
	  { 0, 1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING,               FSM_ACCEPT, NULL, NULL }
};

static void test_repetition_0more(void) {
	CwtFsm fsm;
	const CWT_CHAR *dec = _T("1972");
	const CWT_CHAR *notdec = _T("x1972");

	FSM_INIT(fsm, CWT_CHAR, decimal0more_fsm, NULL, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 0) == 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 2) == 2);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 4) == 4);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 0) == 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 1) == 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 2) == 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 3) == 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 4) == 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 5) == 0);
}

/* 1*DIGIT */
static CwtFsmTransition decimal_fsm[] = {
	  { 0,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
};

/* 2*DIGIT */
static CwtFsmTransition decimal2_fsm[] = {
	  { 1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_NORMAL, NULL, NULL }
	, { 1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL), FSM_ACCEPT, NULL, NULL }
};


/* 1*HEXDIG_FSM */
static CwtFsmTransition hex_fsm[] = {
      { 0,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
};

static void test_repetition_1more(void)
{
	CwtFsm fsm;
	const CWT_CHAR *dec =    _T("1972");
	const CWT_CHAR *notdec = _T("x1972");
	const CWT_CHAR *hex =    _T("BEAF");
	const CWT_CHAR *nothex = _T("BEAR");

	FSM_INIT(fsm, CWT_CHAR, decimal_fsm, NULL, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 2) == 2);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 4) == 4);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 1) < 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notdec, 2) < 0);

	fsm.trans_tab = decimal2_fsm;
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 1) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 2) == 2);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, dec, 4) == 4);


	fsm.trans_tab = hex_fsm;
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 2) == 2);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 4) == 4);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, nothex, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, nothex, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, nothex, 2) == 2);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, nothex, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, nothex, 4) == 3);
}

/* NON-ZERO_DIGIT *DIGIT */
static CwtFsmTransition non_zero_decimal_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_T("123456789"), 9), FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_INLINE(DIGIT_FSM_INL),    FSM_ACCEPT, NULL, NULL }
};

/* ( "0" ("x" / "X") hex ) / (non-zero-dec dec) */
static CwtFsmTransition number_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(non_zero_decimal_fsm), FSM_ACCEPT, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_T("0"), 1),           FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_CHAR(_T("xX"), 2),         FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(hex_fsm),              FSM_ACCEPT, NULL, NULL }
};


static void test_alternatives(void)
{
	const CWT_CHAR *hex = _T("0xDEAD");
	const CWT_CHAR *decimal = _T("1972");
	const CWT_CHAR *notnumber = _T("[number]");
	CwtFsm fsm;

	FSM_INIT(fsm, CWT_CHAR, number_fsm, NULL, cwtBelongCwtChar, cwtExactCwtChar, cwtRangeCwtChar);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 1) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 2) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 4) == 4);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 5) == 5);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, hex, 6) == 6);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, decimal, 0) ==-1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, decimal, 1) == 1);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, decimal, 2) == 2);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, decimal, 3) == 3);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, decimal, 4) == 4);

	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notnumber, 1) < 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notnumber, 2) < 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notnumber, 3) < 0);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, notnumber, 8) < 0);
}


static CwtFsmTransition alpha_seq_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(1), FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_SEQ(2), FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_SEQ(3), FSM_NORMAL, NULL, NULL }
	, { 4,-1, FSM_MATCH_SEQ(4), FSM_NORMAL, NULL, NULL }
	, { 5,-1, FSM_MATCH_SEQ(5), FSM_NORMAL, NULL, NULL }
	, { 6,-1, FSM_MATCH_SEQ(6), FSM_NORMAL, NULL, NULL }
	, { 7,-1, FSM_MATCH_SEQ(7), FSM_NORMAL, NULL, NULL }
	, { 8,-1, FSM_MATCH_SEQ(8), FSM_NORMAL, NULL, NULL }
	, { 9,-1, FSM_MATCH_SEQ(9), FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_SEQ(7), FSM_ACCEPT, NULL, NULL }
};

static CwtFsmTransition z_pos_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(25),    FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR("Z",1), FSM_ACCEPT, NULL, NULL }
	, { 3,-1, FSM_MATCH_SEQ(25),    FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR("z",1), FSM_ACCEPT, NULL, NULL }
};


void test_sequence(void)
{
	CwtFsm fsm;
	size_t nalphas;

	nalphas = strlen(__alpha_chars);

	FSM_INIT(fsm, char, alpha_seq_fsm, NULL, cwtBelongChar, cwtExactChar, cwtRangeChar);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, __alpha_chars, nalphas) == (ssize_t)nalphas);

	FSM_INIT(fsm, char, z_pos_fsm, NULL, cwtBelongChar, cwtExactChar, cwtRangeChar);
	CWT_TEST_FAIL(__fsmNS->exec(&fsm, 0, __alpha_chars, nalphas) == (ssize_t)nalphas);
}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	__fsmNS = cwtFsmNS();

	CWT_BEGIN_TESTS(232);

	test_char_helpers();
	test_cwt_char_helpers();
	test_int_helpers();

	test_alternatives_simple();
	test_repetition_0more();
	test_repetition_1more();
	test_alternatives();
	test_sequence();

	CWT_END_TESTS;
}


