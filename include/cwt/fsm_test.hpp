/**
 * @file   fsm_test.h
 * @author wladt
 * @date   Oct 31, 2012 4:48:26 PM
 *
 * @brief Used for testing FSM rules. Simply include this file
 *        in test source file.
 */

#ifndef __CWT_FSM_TEST_HPP__
#define __CWT_FSM_TEST_HPP__

#include <cwt/test.h>
#include <cwt/fsm.hpp>
#include <cwt/string.hpp>
#include <cstdio>

#define VHEADER(fsm) #fsm, fsm
#define VNULL   NULL
#define INULL   {0, NULL}

using namespace cwt;

struct FsmInvalidEntry {
	ssize_t ret;
	const char *str;
};

struct FsmTestEntry {
	const char *name;
	FsmTransition *trans_tab;
	const char *valid_str[5];
	struct FsmInvalidEntry invalid_entries[5];
};


static void fsm_test_entries(Fsm<Char> &fsm, FsmTestEntry *entry)
{
	const char *fsmname = entry->name;
	const char * const *valid_str  = &entry->valid_str[0];
	FsmInvalidEntry *invalid_entries = &entry->invalid_entries[0];

	fprintf(stdout, _Tr("Test '%s'...\n"), fsmname);
	fsm.setTransitionTable(entry->trans_tab);

	while( *valid_str != NULL ) {
		char desc[128];
		String input(String::fromUtf8(*valid_str));
		ssize_t result = fsm.exec(0, input.data(), input.length());
		sprintf(desc, "result == input.length(): %ld == %ld", result, (ssize_t)input.length());
		CWT_TEST_FAIL2(result == (ssize_t)input.length(), desc);
		valid_str++;
	}

	while( invalid_entries->str != NULL ) {
		char desc[128];
		String input(String::fromUtf8(invalid_entries->str));
		ssize_t result = fsm.exec(0, input.data(), input.length());
		sprintf(desc, "result == invalid_entries->ret: %ld == %ld", result, invalid_entries->ret);
		CWT_TEST_FAIL2(result == invalid_entries->ret, desc);
		invalid_entries++;
	}
}


#endif /* __CWT_FSM_TEST_HPP__ */
