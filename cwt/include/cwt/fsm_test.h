/**
 * @file   fsm_test.h
 * @author wladt
 * @date   Oct 31, 2012 4:48:26 PM
 *
 * @brief Used for testing FSM rules. Simply include this file
 *        in test source file.
 */

#ifndef __CWT_FSM_TEST_H__
#define __CWT_FSM_TEST_H__

#include <cwt/test.h>
#include <cwt/fsm.h>
#include <cwt/str.h>

#define VHEADER(fsm) _T(#fsm), fsm
#define VNULL   NULL
#define INULL   {0, NULL}

struct _FsmInvalidEntry {
	ssize_t ret;
	const CWT_CHAR * const invalid_str;
};

struct _FsmTestEntry {
	const CWT_CHAR *name;
	CwtFsmTransition *trans_tab;
	const CWT_CHAR *const valid_str[5];
	struct _FsmInvalidEntry invalid_entries[5];
};


static void fsm_test_entries(CwtFsm *fsm, struct _FsmTestEntry *entry)
{
	CwtStrNS *strNS = cwt_str_ns();
	const CWT_CHAR *fsmname = entry->name;
	const CWT_CHAR * const *valid_str  = entry->valid_str;
	struct _FsmInvalidEntry *invalid_entries = entry->invalid_entries;

	cwt_logger_ns()->trace(_T("Test '%s'..."), fsmname);
	fsm->trans_tab = entry->trans_tab;

	while( *valid_str != NULL ) {
		CWT_TEST_FAIL(cwt_fsm_ns()->exec(fsm, 0, *valid_str, strNS->strLen(*valid_str)) == (ssize_t)strNS->strLen(*valid_str));
		valid_str++;
	}

	while( invalid_entries->invalid_str != NULL ) {
		CWT_TEST_FAIL(cwt_fsm_ns()->exec(fsm, 0
				, invalid_entries->invalid_str
				, strNS->strLen(invalid_entries->invalid_str)) == invalid_entries->ret);
		invalid_entries++;
	}

}


#endif /* __CWT_FSM_TEST_H__ */
