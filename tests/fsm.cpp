/**
 * @file   fsm-char.c
 * @author wladt
 * @date   Oct 10, 2012 11:40:55 AM
 *
 * @brief
 */


#include <pfs/test/test.hpp>
#include <pfs/fsm.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/vector.hpp>

static const pfs::byte_string __alpha_bytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static const pfs::byte_string __digit_bytes("0123456789");

static const pfs::string __alpha_chars(_u8("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
static const pfs::string __digit_chars("0123456789");

static const pfs::string __rpt_chars(pfs::string::fromUtf8("_ABC_ABC_ABC_ABC"));

static void test_byte_helpers()
{
	size_t i, nalphas, ndigits;

	nalphas = __alpha_bytes.length();
	ndigits = __digit_bytes.length();

	for(i = 0; i < nalphas; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::byte_string>::belongsChar(__alpha_bytes[i], __alpha_bytes.cbegin(), __alpha_bytes.cend()));
	}
	for(i = 0; i < ndigits; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::byte_string>::belongsChar(__digit_bytes[i], __digit_bytes.cbegin(), __digit_bytes.cend()));
	}

	TEST_OK (pfs::fsm::fsm<pfs::byte_string>::containsChars(__alpha_bytes.cbegin(), __alpha_bytes.cend(), __alpha_bytes.cbegin(), __alpha_bytes.cend()));
//	TEST_NOK(pfs::fsm::fsm<ByteArray>::containsChars(__alpha_bytes, nalphas, __alpha_bytes+1, nalphas-1));
	TEST_OK(!pfs::fsm::fsm<pfs::byte_string>::containsChars(__alpha_bytes.cend(), __alpha_bytes.cend(), __alpha_bytes.cbegin(), __alpha_bytes.cend()));
	TEST_OK(!pfs::fsm::fsm<pfs::byte_string>::containsChars(__alpha_bytes.cbegin(), __alpha_bytes.cend(), __alpha_bytes.cend(), __alpha_bytes.cend()));
}

static void test_int_helpers()
{
	size_t i, nints;
	int _ints[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	pfs::vector<int> __integers(10, _ints);

	nints = sizeof(__integers)/sizeof(__integers[0]);

	for(i = 0; i < nints; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::vector<int> >::belongsChar(__integers[i], __integers.cbegin(), __integers.cend()));
	}

	TEST_OK (pfs::fsm::fsm<pfs::vector<int> >::containsChars(__integers.cbegin(), __integers.cend(), __integers.cbegin(), __integers.cend()));
	TEST_OK(!pfs::fsm::fsm<pfs::vector<int> >::containsChars(__integers.cend(), __integers.cend(), __integers.cbegin(), __integers.cend()));
	TEST_OK(!pfs::fsm::fsm<pfs::vector<int> >::containsChars(__integers.cbegin(), __integers.cend(), __integers.cend(), __integers.cend()));
}

static void test_char_helpers()
{
	size_t i, nalphas, ndigits;

	nalphas = __alpha_chars.length();
	ndigits = __digit_chars.length();

	for(i = 0; i < nalphas; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::string>::belongsChar(__alpha_chars.valueAt(i), __alpha_chars.cbegin(), __alpha_chars.cend()));
	}
	for(i = 0; i < ndigits; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::string>::belongsChar(__digit_chars.valueAt(i), __digit_chars.cbegin(), __digit_chars.cend()));
	}

	TEST_OK (pfs::fsm::fsm<pfs::string>::containsChars(__alpha_chars.cbegin(), __alpha_chars.cend(), __alpha_chars.cbegin(), __alpha_chars.cend()));
	TEST_OK(!pfs::fsm::fsm<pfs::string>::containsChars(__alpha_chars.cend(), __alpha_chars.cend(), __alpha_chars.cbegin(), __alpha_chars.cend()));
	TEST_OK(!pfs::fsm::fsm<pfs::string>::containsChars(__alpha_chars.cbegin(), __alpha_chars.cend(), __alpha_chars.cend(), __alpha_chars.cend()));
}


/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static pfs::string _DIGITS("0123456789");
static pfs::string _HEXDIGITS("ABCDEFabcdef");
static pfs::fsm::transition<pfs::string> HEXDIG_FSM[] = {
	  {-1, 1, FSM_MATCH_CHAR(_DIGITS)    , FSM_ACCEPT, nullptr, nullptr }
    , {-1,-1, FSM_MATCH_CHAR(_HEXDIGITS) , FSM_ACCEPT, nullptr, nullptr }
};

static void test_alternatives_simple()
{
	pfs::string hexdig("F");
	pfs::string digit("9");
	pfs::string notdigit("w");
	pfs::fsm::fsm<pfs::string> fsm(HEXDIG_FSM, nullptr);

	TEST_FAIL(fsm.exec(0, hexdig.cbegin(), hexdig.cbegin()) == -1);
	TEST_FAIL(fsm.exec(0, hexdig.cbegin(), hexdig.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, digit.cbegin(), digit.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, notdigit.cbegin(), notdigit.cbegin() + 1) < 0);
}

/* 0*DIGIT */
static pfs::fsm::transition<pfs::string> decimal0more_fsm[] = {
	  { 0, 1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_NOTHING      , FSM_ACCEPT, nullptr, nullptr }
};

static void test_repetition_0more()
{
	pfs::string dec("1972");
	pfs::string notdec("x1972");
	pfs::fsm::fsm<pfs::string> fsm(decimal0more_fsm, nullptr);

	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin()) == 0);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 2) == 2);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 4) == 4);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin()) == 0);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 1) == 0);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 2) == 0);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 3) == 0);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 4) == 0);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 5) == 0);
}

/* 1*DIGIT */
static pfs::fsm::transition<pfs::string> decimal1more_fsm[] = {
	  { 0,-1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, nullptr, nullptr }
};

/* 2*DIGIT */
static pfs::fsm::transition<pfs::string> decimal2more_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_DIGITS), FSM_NORMAL, nullptr, nullptr }
	, { 1,-1, FSM_MATCH_CHAR(_DIGITS), FSM_ACCEPT, nullptr, nullptr }
};


/* 1*HEXDIG_FSM */
static pfs::fsm::transition<pfs::string> hex_fsm[] = {
      { 0,-1, FSM_MATCH_FSM(HEXDIG_FSM), FSM_ACCEPT, nullptr, nullptr }
};

static void test_repetition_1or2more(void)
{
	pfs::string dec("1972");
	pfs::string notdec("x1972");
	pfs::string hex("BEAF");
	pfs::string nothex("BEAR");

	pfs::fsm::fsm<pfs::string> fsm(decimal1more_fsm, nullptr);

	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 2) == 2);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 4) == 4);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 1) < 0);
	TEST_FAIL(fsm.exec(0, notdec.cbegin(), notdec.cbegin() + 2) < 0);

	fsm.setTransitionTable(decimal2more_fsm);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 1) ==-1);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 2) == 2);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, dec.cbegin(), dec.cbegin() + 4) == 4);

	fsm.setTransitionTable(hex_fsm);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 2) == 2);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 4) == 4);

	TEST_FAIL(fsm.exec(0, nothex.cbegin(), nothex.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, nothex.cbegin(), nothex.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, nothex.cbegin(), nothex.cbegin() + 2) == 2);
	TEST_FAIL(fsm.exec(0, nothex.cbegin(), nothex.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, nothex.cbegin(), nothex.cbegin() + 4) == 3);
}


/* NON-ZERO_DIGIT *DIGIT */
static pfs::fsm::transition<pfs::string> non_zero_decimal_fsm[] = {
	  { 1,-1, FSM_MATCH_CHAR(_u8("123456789")), FSM_ACCEPT, nullptr, nullptr }
	, { 1,-1, FSM_MATCH_CHAR(_DIGITS) , FSM_ACCEPT, nullptr, nullptr }
};


/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static pfs::fsm::transition<pfs::string> number_fsm[] = {
	  {-1, 1, FSM_MATCH_FSM(non_zero_decimal_fsm), FSM_ACCEPT, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_STR(_u8("0"))        , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_CHAR(_u8("xX"))      , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_FSM(hex_fsm)         , FSM_ACCEPT, nullptr, nullptr }
};

static void test_alternatives(void)
{
	pfs::string hex("0xDEAD");
	pfs::string decimal("1972");
	pfs::string notnumber("[number]");
	pfs::fsm::fsm<pfs::string> fsm(number_fsm, nullptr);

	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 1) ==-1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 2) ==-1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 4) == 4);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 5) == 5);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), hex.cbegin() + 6) == 6);

	TEST_FAIL(fsm.exec(0, decimal.cbegin(), decimal.cbegin()) ==-1);
	TEST_FAIL(fsm.exec(0, decimal.cbegin(), decimal.cbegin() + 1) == 1);
	TEST_FAIL(fsm.exec(0, decimal.cbegin(), decimal.cbegin() + 2) == 2);
	TEST_FAIL(fsm.exec(0, decimal.cbegin(), decimal.cbegin() + 3) == 3);
	TEST_FAIL(fsm.exec(0, decimal.cbegin(), decimal.cbegin() + 4) == 4);

	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), notnumber.cbegin() + 1) < 0);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), notnumber.cbegin() + 2) < 0);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), notnumber.cbegin() + 3) < 0);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), notnumber.cbegin() + 8) < 0);
}

static pfs::fsm::transition<pfs::string> alpha_seq_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(1) , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_SEQ(2) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_SEQ(3) , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_SEQ(4) , FSM_NORMAL, nullptr, nullptr }
	, { 5,-1, FSM_MATCH_SEQ(5) , FSM_NORMAL, nullptr, nullptr }
	, { 6,-1, FSM_MATCH_SEQ(6) , FSM_NORMAL, nullptr, nullptr }
	, { 7,-1, FSM_MATCH_SEQ(7) , FSM_NORMAL, nullptr, nullptr }
	, { 8,-1, FSM_MATCH_SEQ(8) , FSM_NORMAL, nullptr, nullptr }
	, { 9,-1, FSM_MATCH_SEQ(9) , FSM_NORMAL, nullptr, nullptr }
	, {10,-1, FSM_MATCH_SEQ(7) , FSM_ACCEPT, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_SEQ(33), FSM_ACCEPT, nullptr, nullptr }
};

static pfs::fsm::transition<pfs::string> z_pos_fsm[] = {
	  { 1,-1, FSM_MATCH_SEQ(25)       , FSM_NORMAL, nullptr, nullptr }
	, { 2,-1, FSM_MATCH_STR(_u8("Z")) , FSM_NORMAL, nullptr, nullptr }
	, { 3,-1, FSM_MATCH_SEQ(25)       , FSM_NORMAL, nullptr, nullptr }
	, { 4,-1, FSM_MATCH_STR(_u8("z")) , FSM_NORMAL, nullptr, nullptr }
	, {-1,-1, FSM_MATCH_SEQ(33)       , FSM_ACCEPT, nullptr, nullptr }
};


void test_sequence(void)
{
	pfs::fsm::fsm<pfs::string> fsm1(alpha_seq_fsm, nullptr);
	pfs::fsm::fsm<pfs::string> fsm2(z_pos_fsm, nullptr);

	TEST_FAIL(fsm1.exec(0, __alpha_chars.cbegin(), __alpha_chars.cend()) == ssize_t(__alpha_chars.length()));
	TEST_FAIL(fsm2.exec(0, __alpha_chars.cbegin(), __alpha_chars.cend()) == ssize_t(__alpha_chars.length()));
}


static pfs::fsm::transition<pfs::string> rpt_fsm[] = {
	  {-1,-1, FSM_MATCH_RPT_STR(_u8("_ABC"), 0, 10) , FSM_ACCEPT, nullptr, nullptr }
};


void test_rpt(void)
{
	pfs::fsm::fsm<pfs::string> fsm(rpt_fsm, nullptr);
	TEST_FAIL(fsm.exec(0, __rpt_chars.cbegin(), __rpt_chars.cend()) == ssize_t(__rpt_chars.length()));
}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(224);

	test_byte_helpers();
	test_int_helpers();
	test_char_helpers();
	test_alternatives_simple();
	test_repetition_0more();
	test_repetition_1or2more();
	test_alternatives();
	test_sequence();
	test_rpt();

	END_TESTS;
}


