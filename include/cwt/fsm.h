/**
 * @file   fsm.h
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Header file for Finite State Machine definitions.
 */

#ifndef __CWT_FSM_H__
#define __CWT_FSM_H__

#include <cwt/types.h>

typedef enum _CwtFsmMatchType {
	  Cwt_Fsm_Match_Invalid
	, Cwt_Fsm_Match_Str
	, Cwt_Fsm_Match_Char
	, Cwt_Fsm_Match_Fsm
	, Cwt_Fsm_Match_Func
} CwtFsmMatchType;

#define FSM_MATCH_STR(s,n) Cwt_Fsm_Match_Str,  { .str = {n, s} }
#define FSM_MATCH_CHAR(c)  Cwt_Fsm_Match_Char, { .str = {1, &c} }
#define FSM_MATCH_FSM(f)   Cwt_Fsm_Match_Fsm,  { .trans_tab = f }
#define FSM_MATCH_FUNC(f)  Cwt_Fsm_Match_Func, { .trans_fn = f }

typedef struct _CwtFsmTransition {
	int next_tid; /* next transition id */
	CwtFsmMatchType match_type;
	union {
		struct { size_t len; void *chars; } str;
		struct _CwtFsmTransition *trans_tab;
		ssize_t (*trans_fn)();
	} condition;
	void (*action)(void *context, void *action_args);
	void *action_args;
} CwtFsmTransition;

typedef struct _CwtFsm {
	CwtFsmTransition *transTable;
	void *context;

	/* checks if character ch belongs to the set of characters subset */
	BOOL (*belong)(void *ch, void *subset, size_t n);

	/* checks if string s exactly equals to sequence of characters seq */
	BOOL (*exact)(void *s, size_t n1, void *seq, size_t n2);
} CwtFsm;

typedef struct CwtFsmNS {
	ssize_t (*exec)(CwtFsm *fsm, const void *data, size_t len);
} CwtFsmNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtFsmNS* cwtFsmNS(void);

DLL_API_EXPORT BOOL cwtBelongChar(void *ch, const void *subset, size_t n);
DLL_API_EXPORT BOOL cwtExactChar(const void *s, size_t n1, const void *seq, size_t n2);
DLL_API_EXPORT BOOL cwtBelongCwtChar(void *ch, const void *subset, size_t n);
DLL_API_EXPORT BOOL cwtExactCwtChar(const void *s, size_t n1, const void *seq, size_t n2);
DLL_API_EXPORT BOOL cwtBelongInt(void *ch, const void *subset, size_t n);
DLL_API_EXPORT BOOL cwtExactInt(const void *s, size_t n1, const void *seq, size_t n2);

EXTERN_C_END

#endif /* __CWT_FSM_H__ */
