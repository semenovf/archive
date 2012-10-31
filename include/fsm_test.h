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

#include <cwt/fsm.h>

#define VHEADER(fsm) _T(#fsm), fsm
#define VNULL   NULL
#define VFILL_1 NULL
#define VFILL_2 NULL, NULL
#define VFILL_3 NULL, NULL, NULL
#define VFILL_4 NULL, NULL, NULL, NULL
#define INULL   {0, NULL}
#define IFILL_1 {0, NULL}
#define IFILL_2 {0, NULL}, {0, NULL}
#define IFILL_3 {0, NULL}, {0, NULL}, {0, NULL}
#define IFILL_4 {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}
#define IFILL_5 {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}, {0, NULL}

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

#endif /* __CWT_FSM_TEST_H__ */
