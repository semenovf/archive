/**
 * @file   fsm-char.c
 * @author wladt
 * @date   Oct 10, 2012 11:40:55 AM
 *
 * @brief
 */


#include <cwt/test.h>
#include <cwt/fsm.hpp>
#include <cwt/string.hpp>
/*
#include <stdio.h>
#include <string.h>
*/
/*#include <time.h>*/

using namespace cwt;

static const char *__alpha_bytes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char *__digit_bytes = "0123456789";

static const String __alpha_chars(_U("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
static const String __digit_chars(_U("0123456789"));

static int __integers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

static void test_byte_helpers(void)
{
	size_t i, nalphas, ndigits;

	nalphas = strlen(__alpha_bytes);
	ndigits = strlen(__digit_bytes);

	for(i = 0; i < nalphas; i++) {
		CWT_TEST_OK(belongCharType<byte_t>((void*)&__alpha_bytes[i], __alpha_bytes, nalphas));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(belongCharType<byte_t>((void*)&__digit_bytes[i], __digit_bytes, ndigits));
	}

	CWT_TEST_OK(exactCharType<byte_t>(__alpha_bytes, nalphas, __alpha_bytes, nalphas));
	CWT_TEST_NOK(exactCharType<byte_t>(__alpha_bytes, nalphas, __alpha_bytes+1, nalphas-1));
	CWT_TEST_NOK(exactCharType<byte_t>(NULL, 0, __alpha_bytes, nalphas));
	CWT_TEST_NOK(exactCharType<byte_t>(__alpha_bytes, nalphas, NULL, 0));
}

static void test_char_helpers(void)
{
	size_t i, nalphas, ndigits;

	nalphas = __alpha_chars.length();
	ndigits = __digit_chars.length();

	for(i = 0; i < nalphas; i++) {
		CWT_TEST_OK(belongCharType<Char>(__alpha_chars.data() + i, __alpha_chars.data(), nalphas));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(belongCharType<Char>(__digit_chars.data() + i, __digit_chars.data(), ndigits));
	}

	CWT_TEST_OK(exactCharType<Char>(__alpha_chars.data(), nalphas, __alpha_chars.data(), nalphas));
	CWT_TEST_NOK(exactCharType<Char>(NULL, 0, __alpha_chars.data(), nalphas));
	CWT_TEST_NOK(exactCharType<Char>(__alpha_chars.data(), nalphas, NULL, 0));
}


static void test_int_helpers(void)
{
	size_t i, nints;

	nints = sizeof(__integers);

	for(i = 0; i < nints; i++) {
		CWT_TEST_OK(belongCharType<int>((void*)&__integers[i], __integers, nints));
	}

	CWT_TEST_OK(exactCharType<int>(__integers, nints, __integers, nints));
	CWT_TEST_NOK(exactCharType<int>(NULL, 0, &__integers, nints));
	CWT_TEST_NOK(exactCharType<int>(&__integers, nints, NULL, 0));
}

#define FSM_MATCH_CHAR(s) FsmMatch(new FsmMatchStringChar(s))
#define FSM_MATCH_STR(s)  FsmMatch(new FsmMatchStringStr(s))

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static String _DIGITS(_U("0123456789"));
static String _HEXDIGITS(_U("ABCDEFabcdef"));
static FsmTransition HEXDIG_FSM[] = {
	  {-1, 1, FSM_MATCH_CHAR(_DIGITS)    , FSM_ACCEPT, NULL, NULL }
    , {-1,-1, FSM_MATCH_CHAR(_HEXDIGITS) , FSM_ACCEPT, NULL, NULL }
};

static void test_alternatives_simple(void)
{
	String hexdig(_U("F"));
	String digit(_U("9"));
	String notdigit(_U("w"));
	Fsm<Char> fsm(HEXDIG_FSM, NULL);

	CWT_TEST_FAIL(fsm.exec(0, hexdig.data(), 0) == -1);
	CWT_TEST_FAIL(fsm.exec(0, hexdig.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, digit.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, notdigit.data(), 1) < 0);
}

/* 0*DIGIT */
static FsmTransition decimal0more_fsm[] = {
	  { 0, 1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, NULL, NULL }
	, {-1,-1, FSM_MATCH_NOTHING      , FSM_ACCEPT, NULL, NULL }
};

static void test_repetition_0more(void)
{
	String dec(_U("1972"));
	String notdec(_U("x1972"));
	Fsm<Char> fsm(decimal0more_fsm, NULL);

	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 0) == 0);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 4) == 4);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 0) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 1) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 2) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 3) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 4) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 5) == 0);
}

/* 1*DIGIT */
static FsmTransition decimal1more_fsm[] = {
	  { 0,-1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, NULL, NULL }
};

/* 2*DIGIT */
static FsmTransition decimal2more_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_DIGITS), FSM_NORMAL, NULL, NULL }
	, { 1,-1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, NULL, NULL }
};


/* 1*HEXDIG_FSM */
static FsmTransition hex_fsm[] = {
      { 0,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, NULL, NULL }
};

static void test_repetition_1or2more(void)
{
	String dec(_U("1972"));
	String notdec(_U("x1972"));
	String hex(_U("BEAF"));
	String nothex(_U("BEAR"));

	Fsm<Char> fsm(decimal1more_fsm, NULL);

	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 4) == 4);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 1) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.data(), 2) < 0);

	fsm.setTransitionTable(decimal2more_fsm);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 1) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, dec.data(), 4) == 4);


	fsm.setTransitionTable(hex_fsm);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 4) == 4);

	CWT_TEST_FAIL(fsm.exec(0, nothex.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, nothex.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, nothex.data(), 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, nothex.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, nothex.data(), 4) == 3);
}

/* NON-ZERO_DIGIT *DIGIT */
static FsmTransition non_zero_decimal_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_U("123456789")), FSM_ACCEPT, NULL, NULL }
	, { 1,-1, FSM_MATCH_CHAR(_DIGITS) , FSM_ACCEPT, NULL, NULL }
};


/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static FsmTransition number_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(non_zero_decimal_fsm), FSM_ACCEPT, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR(_U("0"))             , FSM_NORMAL, NULL, NULL }
	, { 3,-1, FSM_MATCH_CHAR(_U("xX"))           , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_FSM(hex_fsm)             , FSM_ACCEPT, NULL, NULL }
};

static void test_alternatives(void)
{
	String hex(_U("0xDEAD"));
	String decimal(_U("1972"));
	String notnumber(_U("[number]"));
	Fsm<Char> fsm(number_fsm, NULL);

	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 1) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 2) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 4) == 4);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 5) == 5);
	CWT_TEST_FAIL(fsm.exec(0, hex.data(), 6) == 6);

	CWT_TEST_FAIL(fsm.exec(0, decimal.data(), 0) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, decimal.data(), 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, decimal.data(), 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, decimal.data(), 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, decimal.data(), 4) == 4);

	CWT_TEST_FAIL(fsm.exec(0, notnumber.data(), 1) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notnumber.data(), 2) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notnumber.data(), 3) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notnumber.data(), 8) < 0);
}


#undef FSM_MATCH_CHAR
#undef FSM_MATCH_STR

#define FSM_MATCH_CHAR(s,n) FsmMatch(new FsmMatchChar(s,n))
#define FSM_MATCH_STR(s,n)  FsmMatch(new FsmMatchStr(s,n))

static FsmTransition alpha_seq_fsm[] = {
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

static FsmTransition z_pos_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(25)     , FSM_NORMAL, NULL, NULL }
	, { 2,-1, FSM_MATCH_STR("Z", 1) , FSM_ACCEPT, NULL, NULL }
	, { 3,-1, FSM_MATCH_SEQ(25)     , FSM_NORMAL, NULL, NULL }
	, {-1,-1, FSM_MATCH_STR("z", 1) , FSM_ACCEPT, NULL, NULL }
};


void test_sequence(void)
{
	size_t nalphas;
	Fsm<byte_t> fsm1(alpha_seq_fsm, NULL);
	Fsm<byte_t> fsm2(z_pos_fsm, NULL);

	nalphas = strlen(__alpha_bytes);

	CWT_TEST_FAIL(fsm1.exec(0, __alpha_bytes, nalphas) == (ssize_t)nalphas);
	CWT_TEST_FAIL(fsm2.exec(0, __alpha_bytes, nalphas) == (ssize_t)nalphas);
}

int main(int argc, char *argv[])
{
	CWT_CHECK_SIZEOF_TYPES;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(263);

	test_byte_helpers();
	test_char_helpers();
	test_int_helpers();
	test_alternatives_simple();
	test_repetition_0more();
	test_repetition_1or2more();
	test_alternatives();
	test_sequence();

	CWT_END_TESTS;
}


