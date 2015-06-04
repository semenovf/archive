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
#include <cstdio>

#define VHEADER(fsm) #fsm, fsm
#define VNULL   NULL
#define INULL   {0, NULL}

struct FsmInvalidEntry
{
	ssize_t ret;
	const char *str;
};

struct FsmTestEntry
{
	const char * name;
	pfs::fsm::transition<pfs::string> *trans_tab;
	const char * valid_str[5];
	struct FsmInvalidEntry invalid_entries[5];
};


static void fsm_test_entries(pfs::fsm::fsm<pfs::string> & fsm, FsmTestEntry * entry)
{
	const char * fsmname = entry->name;
	const char * const * valid_str  = & entry->valid_str[0];
	FsmInvalidEntry *invalid_entries = & entry->invalid_entries[0];

	fprintf(stdout, "Test '%s'...\n", fsmname);
	fsm.setTransitionTable(entry->trans_tab);

	while( *valid_str != NULL ) {
		char desc[128];
		pfs::string input(pfs::string::fromUtf8(*valid_str));
		ssize_t result = fsm.exec(0, input.begin(), input.end());
		sprintf(desc, "result == input.length(): %d == %d", (int)result, (int)input.length());
		TEST_FAIL2(result == (ssize_t)input.length(), desc);
		++valid_str;
	}

	while( invalid_entries->str != NULL ) {
		char desc[128];
		pfs::string input(pfs::string::fromUtf8(invalid_entries->str));
		ssize_t result = fsm.exec(0, input.begin(), input.end());
		sprintf(desc, "result == invalid_entries->ret: %d == %d", (int)result, (int)invalid_entries->ret);
		TEST_FAIL2(result == invalid_entries->ret, desc);
		++invalid_entries;
	}
}


#endif /* __PFS_FSM_TEST_HPP__ */
