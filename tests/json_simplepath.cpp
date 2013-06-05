/**
 * @file   json_simplepath.cpp
 * @author wladt
 * @date   Jun 05, 2013
 *
 * @brief JsonSimplePath testing
 */

#include "../include/cwt/json.hpp"
#include "../include/cwt/fsm_test.hpp"
#include "../src/json_simplepath.cpp"

using namespace cwt;

/*
static String jpath_0(_U("/store/book[0]/title"));
static String jpath_1(_U("/[1]/book[0]/title"));
static String jpath_2(_U("[1]/[2]/[3]/title"));
*/

static FsmTestEntry __fsmTestEntries[] = {
		  { VHEADER(subscript_fsm)
			, { "[1]", "[12]", "[9876]", VNULL }
			, {   {-1, "[0]" }
				, {-1, "[01234]" }
			    , {-1, "[1234" }
			    , {-1, "[]" }
			    , INULL }}

		, { VHEADER(key_subscript_fsm)
			, { "key[123]", "123-key[9]", "===[999]", "||||[987654321]", VNULL }
				, { { -1, "key[]" }
					, INULL }}

		, { VHEADER(elem_fsm)
			, { "key[123]", "===[999]", "key", VNULL }
				, { INULL }}

		, { VHEADER(jpath_fsm)
			, { "/root/elem[1]/elem[2]/elem3", "/root/element[1]/././..", "elem/parent[1]/../child",VNULL }
			, {   {-1, "[]" }
				, { 5, "/root[]" }
				, INULL }}
};


void test_json_simplepath_fsm()
{
	static Fsm<Char> fsm(NULL, NULL);
	int nentries = sizeof(__fsmTestEntries)/sizeof(__fsmTestEntries[0]);

	for( int i = 0; i < nentries; i++ )
		fsm_test_entries(fsm, &__fsmTestEntries[i]);
}


int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(94);

	test_json_simplepath_fsm();

	CWT_END_TESTS;
}
