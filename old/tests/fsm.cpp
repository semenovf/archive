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
#include <cwt/bytearray.hpp>
#include <cwt/vector.hpp>

using namespace cwt;


static const ByteArray __alpha_bytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static const ByteArray __digit_bytes("0123456789");

static int _ints[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
static Vector<int> __integers(_ints, 10);

static const String __alpha_chars(String::fromUtf8("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
static const String __digit_chars("0123456789");

static const String __rpt_chars(String::fromUtf8("_ABC_ABC_ABC_ABC"));

static void test_byte_helpers()
{
	size_t i, nalphas, ndigits;

	nalphas = __alpha_bytes.length();
	ndigits = __digit_bytes.length();

	for(i = 0; i < nalphas; i++) {
		CWT_TEST_OK(Fsm<ByteArray>::belongsChar(__alpha_bytes[i], __alpha_bytes.cbegin(), __alpha_bytes.cend()));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(Fsm<ByteArray>::belongsChar(__digit_bytes[i], __digit_bytes.cbegin(), __digit_bytes.cend()));
	}

	CWT_TEST_OK (Fsm<ByteArray>::containsChars(__alpha_bytes.cbegin(), __alpha_bytes.cend(), __alpha_bytes.cbegin(), __alpha_bytes.cend()));
//	CWT_TEST_NOK(Fsm<ByteArray>::containsChars(__alpha_bytes, nalphas, __alpha_bytes+1, nalphas-1));
	CWT_TEST_NOK(Fsm<ByteArray>::containsChars(__alpha_bytes.cend(), __alpha_bytes.cend(), __alpha_bytes.cbegin(), __alpha_bytes.cend()));
	CWT_TEST_NOK(Fsm<ByteArray>::containsChars(__alpha_bytes.cbegin(), __alpha_bytes.cend(), __alpha_bytes.cend(), __alpha_bytes.cend()));
}

static void test_int_helpers()
{
	size_t i, nints;

	nints = sizeof(__integers)/sizeof(__integers[0]);

	for(i = 0; i < nints; i++) {
		CWT_TEST_OK(Fsm<Vector<int> >::belongsChar(__integers[i], __integers.cbegin(), __integers.cend()));
	}

	CWT_TEST_OK (Fsm<Vector<int> >::containsChars(__integers.cbegin(), __integers.cend(), __integers.cbegin(), __integers.cend()));
	CWT_TEST_NOK(Fsm<Vector<int> >::containsChars(__integers.cend(), __integers.cend(), __integers.cbegin(), __integers.cend()));
	CWT_TEST_NOK(Fsm<Vector<int> >::containsChars(__integers.cbegin(), __integers.cend(), __integers.cend(), __integers.cend()));
}

static void test_char_helpers()
{
	size_t i, nalphas, ndigits;

	nalphas = __alpha_chars.length();
	ndigits = __digit_chars.length();

	for(i = 0; i < nalphas; i++) {
		CWT_TEST_OK(Fsm<String>::belongsChar(__alpha_chars.charAt(i), __alpha_chars.begin(), __alpha_chars.end()));
	}
	for(i = 0; i < ndigits; i++) {
		CWT_TEST_OK(Fsm<String>::belongsChar(__digit_chars.charAt(i), __digit_chars.begin(), __digit_chars.end()));
	}

	CWT_TEST_OK (Fsm<String>::containsChars(__alpha_chars.begin(), __alpha_chars.end(), __alpha_chars.begin(), __alpha_chars.end()));
	CWT_TEST_NOK(Fsm<String>::containsChars(__alpha_chars.end(), __alpha_chars.end(), __alpha_chars.begin(), __alpha_chars.end()));
	CWT_TEST_NOK(Fsm<String>::containsChars(__alpha_chars.begin(), __alpha_chars.end(), __alpha_chars.end(), __alpha_chars.end()));
}


/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static String _DIGITS("0123456789");
static String _HEXDIGITS("ABCDEFabcdef");
static FsmTransition<String> HEXDIG_FSM[] = {
	  {-1, 1, FSM_MATCH_CHAR(_DIGITS)    , FSM_ACCEPT, 0, 0 }
    , {-1,-1, FSM_MATCH_CHAR(_HEXDIGITS) , FSM_ACCEPT, 0, 0 }
};

static void test_alternatives_simple()
{
	String hexdig("F");
	String digit("9");
	String notdigit("w");
	Fsm<String> fsm(HEXDIG_FSM, 0);

	CWT_TEST_FAIL(fsm.exec(0, hexdig.begin(), hexdig.begin()) == -1);
	CWT_TEST_FAIL(fsm.exec(0, hexdig.begin(), hexdig.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, digit.begin(), digit.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, notdigit.begin(), notdigit.begin() + 1) < 0);
}

/* 0*DIGIT */
static FsmTransition<String> decimal0more_fsm[] = {
	  { 0, 1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, 0, 0 }
	, {-1,-1, FSM_MATCH_NOTHING      , FSM_ACCEPT, 0, 0 }
};

static void test_repetition_0more()
{
	String dec("1972");
	String notdec("x1972");
	Fsm<String> fsm(decimal0more_fsm, 0);

	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin()) == 0);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 4) == 4);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin()) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 1) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 2) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 3) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 4) == 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 5) == 0);
}

/* 1*DIGIT */
static FsmTransition<String> decimal1more_fsm[] = {
	  { 0,-1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, 0, 0 }
};

/* 2*DIGIT */
static FsmTransition<String> decimal2more_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_DIGITS), FSM_NORMAL, 0, 0 }
	, { 1,-1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, 0, 0 }
};


/* 1*HEXDIG_FSM */
static FsmTransition<String> hex_fsm[] = {
      { 0,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, 0, 0 }
};

static void test_repetition_1or2more(void)
{
	String dec("1972");
	String notdec("x1972");
	String hex("BEAF");
	String nothex("BEAR");

	Fsm<String> fsm(decimal1more_fsm, 0);

	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 4) == 4);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 1) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notdec.begin(), notdec.begin() + 2) < 0);

	fsm.setTransitionTable(decimal2more_fsm);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 1) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, dec.begin(), dec.begin() + 4) == 4);

	fsm.setTransitionTable(hex_fsm);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 4) == 4);

	CWT_TEST_FAIL(fsm.exec(0, nothex.begin(), nothex.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, nothex.begin(), nothex.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, nothex.begin(), nothex.begin() + 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, nothex.begin(), nothex.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, nothex.begin(), nothex.begin() + 4) == 3);
}


/* NON-ZERO_DIGIT *DIGIT */
static FsmTransition<String> non_zero_decimal_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR("123456789"), FSM_ACCEPT, 0, 0 }
	, { 1,-1, FSM_MATCH_CHAR(_DIGITS) , FSM_ACCEPT, 0, 0 }
};


/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static FsmTransition<String> number_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(non_zero_decimal_fsm), FSM_ACCEPT, 0, 0 }
	, { 2,-1, FSM_MATCH_STR("0")             , FSM_NORMAL, 0, 0 }
	, { 3,-1, FSM_MATCH_CHAR("xX")           , FSM_NORMAL, 0, 0 }
	, {-1,-1, FSM_MATCH_FSM(hex_fsm)         , FSM_ACCEPT, 0, 0 }
};

static void test_alternatives(void)
{
	String hex("0xDEAD");
	String decimal("1972");
	String notnumber("[number]");
	Fsm<String> fsm(number_fsm, 0);

	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 1) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 2) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 4) == 4);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 5) == 5);
	CWT_TEST_FAIL(fsm.exec(0, hex.begin(), hex.begin() + 6) == 6);

	CWT_TEST_FAIL(fsm.exec(0, decimal.begin(), decimal.begin()) ==-1);
	CWT_TEST_FAIL(fsm.exec(0, decimal.begin(), decimal.begin() + 1) == 1);
	CWT_TEST_FAIL(fsm.exec(0, decimal.begin(), decimal.begin() + 2) == 2);
	CWT_TEST_FAIL(fsm.exec(0, decimal.begin(), decimal.begin() + 3) == 3);
	CWT_TEST_FAIL(fsm.exec(0, decimal.begin(), decimal.begin() + 4) == 4);

	CWT_TEST_FAIL(fsm.exec(0, notnumber.begin(), notnumber.begin() + 1) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notnumber.begin(), notnumber.begin() + 2) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notnumber.begin(), notnumber.begin() + 3) < 0);
	CWT_TEST_FAIL(fsm.exec(0, notnumber.begin(), notnumber.begin() + 8) < 0);
}


static FsmTransition<String> alpha_seq_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(1) , FSM_NORMAL, 0, 0 }
	, { 2,-1, FSM_MATCH_SEQ(2) , FSM_NORMAL, 0, 0 }
	, { 3,-1, FSM_MATCH_SEQ(3) , FSM_NORMAL, 0, 0 }
	, { 4,-1, FSM_MATCH_SEQ(4) , FSM_NORMAL, 0, 0 }
	, { 5,-1, FSM_MATCH_SEQ(5) , FSM_NORMAL, 0, 0 }
	, { 6,-1, FSM_MATCH_SEQ(6) , FSM_NORMAL, 0, 0 }
	, { 7,-1, FSM_MATCH_SEQ(7) , FSM_NORMAL, 0, 0 }
	, { 8,-1, FSM_MATCH_SEQ(8) , FSM_NORMAL, 0, 0 }
	, { 9,-1, FSM_MATCH_SEQ(9) , FSM_NORMAL, 0, 0 }
	, {10,-1, FSM_MATCH_SEQ(7) , FSM_ACCEPT, 0, 0 }
	, {-1,-1, FSM_MATCH_SEQ(33), FSM_ACCEPT, 0, 0 }
};

static FsmTransition<String> z_pos_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(25)  , FSM_NORMAL, 0, 0 }
	, { 2,-1, FSM_MATCH_STR("Z") , FSM_NORMAL, 0, 0 }
	, { 3,-1, FSM_MATCH_SEQ(25)  , FSM_NORMAL, 0, 0 }
	, { 4,-1, FSM_MATCH_STR("z") , FSM_NORMAL, 0, 0 }
	, {-1,-1, FSM_MATCH_SEQ(33)  , FSM_ACCEPT, 0, 0 }
};


void test_sequence(void)
{
	Fsm<String> fsm1(alpha_seq_fsm, 0);
	Fsm<String> fsm2(z_pos_fsm, 0);

	CWT_TEST_FAIL(fsm1.exec(0, __alpha_chars.begin(), __alpha_chars.end()) == ssize_t(__alpha_chars.length()));
	CWT_TEST_FAIL(fsm2.exec(0, __alpha_chars.begin(), __alpha_chars.end()) == ssize_t(__alpha_chars.length()));
}


static FsmTransition<String> rpt_fsm[] = {
	  {-1,-1, FSM_MATCH_RPT_STR("_ABC", 0, 10) , FSM_ACCEPT, 0, 0 }
};


void test_rpt(void)
{
	Fsm<String> fsm(rpt_fsm, 0);

	CWT_TEST_FAIL(fsm.exec(0, __rpt_chars.begin(), __rpt_chars.end()) == ssize_t(__rpt_chars.length()));
}


int main(int argc, char *argv[])
{
	CWT_CHECK_SIZEOF_TYPES;

	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(225);

	test_byte_helpers();
	test_int_helpers();
	test_char_helpers();
	test_alternatives_simple();
	test_repetition_0more();
	test_repetition_1or2more();
	test_alternatives();
	test_sequence();
	test_rpt();

	CWT_END_TESTS;
}


