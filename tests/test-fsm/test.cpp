/**
 * @file   fsm-char.c
 * @author wladt
 * @date   Oct 10, 2012 11:40:55 AM
 *
 * @brief
 */


#include <pfs/test/test.hpp>
#include <pfs/string.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/vector.hpp>
#include <pfs/fsm.hpp>
#include <pfs/fsm/aliases.hpp>

#include <iostream>

using std::cout;
using std::endl;

static const pfs::byte_string __alpha_bytes("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
static const pfs::byte_string __digit_bytes("0123456789");

static const pfs::string __alpha_chars(_u8("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"));
static const pfs::string __digit_chars("0123456789");

static const pfs::string __rpt_chars("_ABC_ABC_ABC_ABC");

static void test_byte_helpers()
{
	size_t i, nalphas, ndigits;

	nalphas = __alpha_bytes.length();
	ndigits = __digit_bytes.length();

	for(i = 0; i < nalphas; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::byte_string>::belongs_char(
			  __alpha_bytes[i]
			, __alpha_bytes.cbegin()
			, __alpha_bytes.cend()));
	}

	for(i = 0; i < ndigits; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::byte_string>::belongs_char(
			  __digit_bytes[i]
			, __digit_bytes.cbegin()
			, __digit_bytes.cend()));
	}

	TEST_OK (pfs::fsm::fsm<pfs::byte_string>::contains_chars(
			  __alpha_bytes.cbegin()
			, __alpha_bytes.cend()
			, __alpha_bytes.cbegin()
			, __alpha_bytes.cend()).first);

	TEST_OK(!pfs::fsm::fsm<pfs::byte_string>::contains_chars(
			  __alpha_bytes.cend()
			, __alpha_bytes.cend()
			, __alpha_bytes.cbegin()
			, __alpha_bytes.cend()).first);

	TEST_OK(!pfs::fsm::fsm<pfs::byte_string>::contains_chars(
			  __alpha_bytes.cbegin()
			, __alpha_bytes.cend()
			, __alpha_bytes.cend()
			, __alpha_bytes.cend()).first);
}

static void test_int_helpers()
{
	typedef pfs::vector<int> int_vector_type;

	size_t i, nints;
	int __ints[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int_vector_type __integers(__ints, __ints + 10);

	nints = sizeof(__ints)/sizeof(__ints[0]);

	for(i = 0; i < nints; i++) {
		TEST_OK(pfs::fsm::fsm<int_vector_type>::belongs_char(
			  __integers[i]
			, __integers.cbegin()
			, __integers.cend()));
	}

	TEST_OK (pfs::fsm::fsm<int_vector_type>::contains_chars(
			  __integers.cbegin()
			, __integers.cend()
			, __integers.cbegin()
			, __integers.cend()).first);

	TEST_OK(!pfs::fsm::fsm<int_vector_type>::contains_chars(
			  __integers.cend()
			, __integers.cend()
			, __integers.cbegin()
			, __integers.cend()).first);

	TEST_OK(!pfs::fsm::fsm<int_vector_type>::contains_chars(
			  __integers.cbegin()
			, __integers.cend()
			, __integers.cend()
			, __integers.cend()).first);
}

static void test_char_helpers()
{
	size_t nalphas = __alpha_chars.length();
	size_t ndigits = __digit_chars.length();

	for (size_t i = 0; i < nalphas; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::string>::belongs_char(
			  __alpha_chars.at(i)
			, __alpha_chars.cbegin()
			, __alpha_chars.cend()));
	}

	for (size_t i = 0; i < ndigits; i++) {
		TEST_OK(pfs::fsm::fsm<pfs::string>::belongs_char(
			  __digit_chars.at(i)
			, __digit_chars.cbegin()
			, __digit_chars.cend()));
	}

	TEST_OK (pfs::fsm::fsm<pfs::string>::contains_chars(
			  __alpha_chars.cbegin()
			, __alpha_chars.cend()
			, __alpha_chars.cbegin()
			, __alpha_chars.cend()).first);

	TEST_OK(!pfs::fsm::fsm<pfs::string>::contains_chars(
			  __alpha_chars.cend()
			, __alpha_chars.cend()
			, __alpha_chars.cbegin()
			, __alpha_chars.cend()).first);

	TEST_OK(!pfs::fsm::fsm<pfs::string>::contains_chars(
			  __alpha_chars.cbegin()
			, __alpha_chars.cend()
			, __alpha_chars.cend()
			, __alpha_chars.cend()).first);
}

typedef pfs::fsm::m<pfs::string> M;

/* DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
static pfs::string _DIGITS("0123456789");
static pfs::string _HEXDIGITS("ABCDEFabcdef");
static pfs::fsm::transition<pfs::string> HEXDIG_FSM[] = {
	  {-1, 1, M::chr(_DIGITS).m    , FSM_ACCEPT, 0, 0 }
    , {-1,-1, M::chr(_HEXDIGITS).m , FSM_ACCEPT, 0, 0 }
};

static void test_alternatives_simple()
{
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	pfs::string hexdig("F");
	pfs::string digit("9");
	pfs::string notdigit("w");
	pfs::fsm::fsm<pfs::string> fsm(HEXDIG_FSM, 0);

	TEST_FAIL(fsm.exec(0, hexdig.cbegin(), hexdig.cbegin()).first == false);

	pfs::string::const_iterator it_end;
	result_type r;

	it_end = hexdig.cbegin();
	std::advance(it_end, 1);
	r = fsm.exec(0, hexdig.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hexdig.cbegin(), r.second) == 1);

	it_end = digit.cbegin();
	std::advance(it_end, 1);
	r = fsm.exec(0, digit.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(digit.cbegin(), r.second) == 1);

	it_end = notdigit.cbegin();
	std::advance(it_end, 1);
	r = fsm.exec(0, notdigit.cbegin(), it_end);
	TEST_FAIL(!r.first && r.second == it_end);
}

/* 0*DIGIT */
static pfs::fsm::transition<pfs::string> decimal0more_fsm[] = {
	  { 0, 1, M::chr(_DIGITS).m, FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::nothing().m   , FSM_ACCEPT, 0, 0 }
};

static void test_repetition_0more()
{
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	pfs::string dec("1972");
	pfs::string notdec("x1972");
	pfs::fsm::fsm<pfs::string> fsm(decimal0more_fsm, 0);

	pfs::string::const_iterator it_end;
	result_type r;

	it_end = dec.cbegin();
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 0);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 1);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 2);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 4);

	it_end = notdec.cbegin();
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(notdec.cbegin(), r.second) == 0);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(notdec.cbegin(), r.second) == 0);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(notdec.cbegin(), r.second) == 0);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(notdec.cbegin(), r.second) == 0);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(notdec.cbegin(), r.second) == 0);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(notdec.cbegin(), r.second) == 0);
}

/* 1*DIGIT */
static pfs::fsm::transition<pfs::string> decimal1more_fsm[] = {
	  { 0,-1, M::chr(_DIGITS).m, FSM_ACCEPT, 0, 0 }
};

/* 2*DIGIT */
static pfs::fsm::transition<pfs::string> decimal2more_fsm[] = {
	  { 1,-1, M::chr(_DIGITS).m, FSM_NORMAL, 0, 0 }
	, { 1,-1, M::chr(_DIGITS).m, FSM_ACCEPT, 0, 0 }
};


/* 1*HEXDIG_FSM */
static pfs::fsm::transition<pfs::string> hex_fsm[] = {
      { 0,-1, M::tr(HEXDIG_FSM).m, FSM_ACCEPT, 0, 0 }
};

static void test_repetition_1or2more(void)
{
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	pfs::string dec("1972");
	pfs::string notdec("x1972");
	pfs::string hex("BEAF");
	pfs::string nothex("BEAR");

	pfs::fsm::fsm<pfs::string> fsm(decimal1more_fsm, 0);

	pfs::string::const_iterator it_end;
	result_type r;

	it_end = dec.cbegin();
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(!r.first && r.second == it_end);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 1);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 2);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 4);

	it_end = notdec.cbegin();
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	std::advance(it_end, 1);
	r = fsm.exec(0, notdec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	fsm.set_transition_table(decimal2more_fsm);

	it_end = dec.cbegin();
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(!r.first);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 2);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, dec.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(dec.cbegin(), r.second) == 4);

	fsm.set_transition_table(hex_fsm);

	it_end = hex.cbegin();
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(!r.first);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 1);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 2);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 4);

	it_end = nothex.cbegin();
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(!r.first);

	std::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(nothex.cbegin(), r.second) == 1);

	std::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(nothex.cbegin(), r.second) == 2);

	std::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(nothex.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, nothex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(nothex.cbegin(), r.second) == 3);
}

/* NON-ZERO_DIGIT *DIGIT */
static pfs::fsm::transition<pfs::string> non_zero_decimal_fsm[] = {
	  { 1,-1, M::chr(_u8("123456789")).m, FSM_ACCEPT, 0, 0 }
	, { 1,-1, M::chr(_DIGITS).m         , FSM_ACCEPT, 0, 0 }
};


/* (non-zero-dec dec)  / ( "0" ("x" / "X") hex ) */
static pfs::fsm::transition<pfs::string> number_fsm[] = {
	  {-1, 1, M::tr(non_zero_decimal_fsm).m, FSM_ACCEPT, 0, 0 }
	, { 2,-1, M::str(_u8("0")).m  , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::chr(_u8("xX")).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::tr(hex_fsm).m    , FSM_ACCEPT, 0, 0 }
};

static void test_alternatives(void)
{
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	pfs::string hex("0xDEAD");
	pfs::string decimal("1972");
	pfs::string notnumber("[number]");
	pfs::fsm::fsm<pfs::string> fsm(number_fsm, 0);

	pfs::string::const_iterator it_end;
	result_type r;

	it_end = hex.cbegin();
	TEST_FAIL(fsm.exec(0, hex.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, hex.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 4);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 5);

	std::advance(it_end, 1);
	r = fsm.exec(0, hex.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(hex.cbegin(), r.second) == 6);

	it_end = decimal.cbegin();
	TEST_FAIL(fsm.exec(0, decimal.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(decimal.cbegin(), r.second) == 1);

	std::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(decimal.cbegin(), r.second) == 2);

	std::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(decimal.cbegin(), r.second) == 3);

	std::advance(it_end, 1);
	r = fsm.exec(0, decimal.cbegin(), it_end);
	TEST_FAIL(r.first && std::distance(decimal.cbegin(), r.second) == 4);

	it_end = notnumber.cbegin();
	std::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 1);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));

	std::advance(it_end, 5);
	TEST_FAIL(fsm.exec(0, notnumber.cbegin(), it_end) == result_type(false, it_end));
}

static pfs::fsm::transition<pfs::string> alpha_seq_fsm[] = {
	  { 1,-1, M::seq(1).m , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::seq(2).m , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::seq(3).m , FSM_NORMAL, 0, 0 }
	, { 4,-1, M::seq(4).m , FSM_NORMAL, 0, 0 }
	, { 5,-1, M::seq(5).m , FSM_NORMAL, 0, 0 }
	, { 6,-1, M::seq(6).m , FSM_NORMAL, 0, 0 }
	, { 7,-1, M::seq(7).m , FSM_NORMAL, 0, 0 }
	, { 8,-1, M::seq(8).m , FSM_NORMAL, 0, 0 }
	, { 9,-1, M::seq(9).m , FSM_NORMAL, 0, 0 }
	, {10,-1, M::seq(7).m , FSM_ACCEPT, 0, 0 }
	, {-1,-1, M::seq(33).m, FSM_ACCEPT, 0, 0 }
};

static pfs::fsm::transition<pfs::string> z_pos_fsm[] = {
	  { 1,-1, M::seq(25).m       , FSM_NORMAL, 0, 0 }
	, { 2,-1, M::str(_u8("Z")).m , FSM_NORMAL, 0, 0 }
	, { 3,-1, M::seq(25).m       , FSM_NORMAL, 0, 0 }
	, { 4,-1, M::str(_u8("z")).m , FSM_NORMAL, 0, 0 }
	, {-1,-1, M::seq(33).m       , FSM_ACCEPT, 0, 0 }
};


void test_sequence ()
{
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	pfs::fsm::fsm<pfs::string> fsm1(alpha_seq_fsm, 0);
	pfs::fsm::fsm<pfs::string> fsm2(z_pos_fsm, 0);

	TEST_FAIL(fsm1.exec(0, __alpha_chars.cbegin(), __alpha_chars.cend())
			== result_type(true, __alpha_chars.cend()));

	TEST_FAIL(fsm2.exec(0, __alpha_chars.cbegin(), __alpha_chars.cend())
			== result_type(true, __alpha_chars.cend()));
}

static pfs::fsm::transition<pfs::string> rpt_fsm[] = {
	  {-1,-1, M::rpt_str(_u8("_ABC"), 0, 10).m , FSM_ACCEPT, 0, 0 }
};

void test_rpt ()
{
	typedef pfs::fsm::fsm<pfs::string>::result_type result_type;

	pfs::fsm::fsm<pfs::string> fsm(rpt_fsm, 0);
	TEST_FAIL(fsm.exec(0, __rpt_chars.cbegin(), __rpt_chars.cend())
			== result_type(true, __rpt_chars.cend()));
}

int main(int argc, char *argv[])
{
	PFS_UNUSED(argc);
	PFS_UNUSED(argv);

	BEGIN_TESTS(229);

	test_byte_helpers();
	test_int_helpers();
	test_char_helpers();
	test_alternatives_simple();
	test_repetition_0more();
	test_repetition_1or2more();
	test_alternatives();
	test_sequence();
	test_rpt();

	return END_TESTS;
}
