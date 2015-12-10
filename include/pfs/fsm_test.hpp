/**
 * @file   fsm_test.h
 * @author wladt
 * @date   Oct 31, 2012 4:48:26 PM
 *
 * @brief Used for testing FSM rules. Simply include this file
 *        in test source file.
 */

#ifndef __PFS_FSM_TEST_HPP__
#define __PFS_FSM_TEST_HPP__

#include <pfs/test/test.hpp>
#include <pfs/fsm.hpp>
#include <pfs/string.hpp>
#include <pfs/iostream.hpp>
#include <pfs/sstream.hpp>

#define VHEADER(fsm) #fsm, fsm
#define VNULL   NULL
#define INULL   {0, NULL}

namespace pfs { namespace fsm {

struct invalid_entry
{
	ssize_t ret;
	string  str;
};

struct test_entry
{
	string               name;
	transition<string> * trans_tab;
	string               valid_str[5];
	invalid_entry        invalid_entries[5];
};

namespace {

void test_entries (fsm<string> & f, test_entry * entry)
{
	const string fsmname(entry->name);
	const string * valid_str = & entry->valid_str[0];
	const invalid_entry * invalid = & entry->invalid_entries[0];

	cout << "Test '" << fsmname << "'..." << endl;
	f.setTransitionTable(entry->trans_tab);

	while (!valid_str->empty()) {
		ssize_t result = f.exec(0, valid_str->begin(), valid_str->end());

		ostringstream os;
		os << "result == input.length(): " << result << " == " << valid_str->length();

		TEST_FAIL2(result == static_cast<ssize_t>(valid_str->length()), os.str().c_str());
		++valid_str;
	}

	while (!invalid->str.empty()) {
		ssize_t result = f.exec(0, invalid->str.begin(), invalid->str.end());

		ostringstream os;
		os << "result == invalid->ret: " << result << " == " << invalid->ret;

		TEST_FAIL2(result == invalid->ret, os.str().c_str());

		++invalid;
	}
}

}

}} // pfs::fsm

#endif /* __PFS_FSM_TEST_HPP__ */
