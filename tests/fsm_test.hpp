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
		String input(String::fromUtf8(*valid_str));
		CWT_TEST_FAIL(fsm.exec(0, input.data(), input.length()) == (ssize_t)input.length());
		valid_str++;
	}

	while( invalid_entries->str != NULL ) {
		String input(String::fromUtf8(invalid_entries->str));
		CWT_TEST_FAIL(fsm.exec(0, input.data(), input.length()) == invalid_entries->ret);
		invalid_entries++;
	}
}


#endif /* __CWT_FSM_TEST_HPP__ */
