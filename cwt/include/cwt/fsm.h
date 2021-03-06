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
	  Cwt_Fsm_Match_Nothing
	, Cwt_Fsm_Match_Nl           /* Match new line */
	, Cwt_Fsm_Match_Seq
	, Cwt_Fsm_Match_Str
	, Cwt_Fsm_Match_Char
	, Cwt_Fsm_Match_Range
	, Cwt_Fsm_Match_Fsm
	, Cwt_Fsm_Match_Func
	, Cwt_Fsm_Match_Rpt
} CwtFsmMatchType;

/* MSC does not support such union initializer
#	define FSM_MATCH_STR(s,n)      Cwt_Fsm_Match_Str,  { .str = {n, s} }
#	define FSM_MATCH_CHAR(s,n)     Cwt_Fsm_Match_Char, { .str = {n, s} }
#	define FSM_MATCH_FSM(f)        Cwt_Fsm_Match_Fsm,  { .trans_tab = f }
#	define FSM_MATCH_FUNC(f)       Cwt_Fsm_Match_Func, { .trans_fn = f }
#	define FSM_MATCH_NOTHING       Cwt_Fsm_Match_Nothing, { .str = {0, NULL} }
*/
#define FSM_MATCH_NL            Cwt_Fsm_Match_Nl      , { {0, NULL, NULL} }
#define FSM_MATCH_SEQ(n)        Cwt_Fsm_Match_Seq     , { {n, NULL, NULL} }
#define FSM_MATCH_STR(s,n)      Cwt_Fsm_Match_Str     , { {n, s, NULL} }
#define FSM_MATCH_CHAR(s,n)     Cwt_Fsm_Match_Char    , { {n, s, NULL} }
#define FSM_MATCH_RANGE(f,t)    Cwt_Fsm_Match_Range   , { {0, (void*)(f), (void*)(t)} }
#define FSM_MATCH_FSM(f)        Cwt_Fsm_Match_Fsm     , { {0, (void*)(f), NULL} }
#define FSM_MATCH_FUNC(f,pcont) Cwt_Fsm_Match_Func    , { {0, (void*)(f), pcont} }
#define FSM_MATCH_NOTHING       Cwt_Fsm_Match_Nothing , { {0, NULL, NULL} }
#define FSM_MATCH_RPT(f,r)      Cwt_Fsm_Match_Rpt     , { {0, (void*)(f), (void*)(r)} }
#define FSM_MATCH_INLINE(inl)   inl

/*
#define FSM_INIT(fsm, char_type, tab, pcontext, belong_fn, exact_fn, range_fn) \
		(fsm).trans_tab = tab;                            \
		(fsm).sizeof_char = sizeof(char_type);            \
		(fsm).context   = pcontext;                       \
		(fsm).belong    = belong_fn;                      \
		(fsm).exact     = exact_fn;                       \
		(fsm).range     = range_fn;
*/

#define FSM_NORMAL  0
#define FSM_REJECT  1
#define FSM_ACCEPT  2

typedef struct _CwtFsmRptBounds {
	int from, to;
} CwtFsmRptBounds;


struct _CwtFsm;
typedef union _CwtFsmCondition {
	struct { size_t len; void *chars; void *unused; } str;
	struct { size_t unused; void *from; void *to; } range;
	struct { size_t unused; struct _CwtFsmTransition *tab; void *bounds; } trans_tab;
	struct { size_t unused; ssize_t (*fn)(struct _CwtFsm *fsm, void *fn_context, const void *data, size_t len); void *fn_context; } trans_fn;
} CwtFsmCondition;

typedef struct _CwtFsmTransition {
	int state_next;
	int state_fail;

	CwtFsmMatchType match_type;
	CwtFsmCondition condition;

	int status; /* last entry in the chain of ...*/

	void (*action)(const void *data, size_t len, void *context, void *action_args);
	void *action_args;
} CwtFsmTransition;

typedef BOOL (*CwtBelongCharFn) (const void *ch, const void *subset, size_t n);
typedef BOOL (*CwtExactCharFn)  (const void *s, size_t n1, const void *seq, size_t n2);
typedef BOOL (*CwtRangeCharFn)  (const void *ch, const void *from, const void *to);


typedef struct _CwtFsm {
	CwtFsmTransition *trans_tab;
	CWT_CHAR *nl;
	int   sizeof_char;           /* sizeof character */
	void *context;               /* result context */

	/* checks if character ch belongs to the set of characters subset */
	CwtBelongCharFn belong_char_fn;

	/* checks if string s exactly equals to sequence of characters seq */
	CwtExactCharFn exact_char_fn;

	/* checks if character ch belongs to the specified range */
	CwtRangeCharFn range_char_fn;
} CwtFsm;


/*
typedef struct _CwtFsmRepetitionContext {
	CwtFsmTransition *trans_tab;
	int from, to;
} CwtFsmRepetitionContext;
*/

typedef struct _CwtFsmNS {
	void    (*init)             (
		  CwtFsm *fsm
		, int sizeof_char_type
		, CwtFsmTransition *initial_tab
		, void *context
		, CwtBelongCharFn belong_char_fn
		, CwtExactCharFn exact_char_fn
		, CwtRangeCharFn range_char_fn);
	void    (*destroy)          (CwtFsm *fsm);
	ssize_t (*exec)             (CwtFsm *fsm, int state_cur, const void *data, size_t len);
	void    (*reproduce)        (CwtFsm *fsm_copy, CwtFsm *fsm_orig, CwtFsmTransition *initial_tab);
	void    (*reproduceWithContext) (CwtFsm *fsm_copy, CwtFsm *fsm_orig, CwtFsmTransition *initial_tab, void *context);
	void    (*setNewLine)       (CwtFsm *fsm, CwtNewLineEnum nl);
	void    (*setCustomNewLine) (CwtFsm *fsm, const CWT_CHAR *nl);
} CwtFsmNS;

EXTERN_C_BEGIN
DLL_API_EXPORT CwtFsmNS* cwt_fsm_ns(void);

DLL_API_EXPORT BOOL cwt_fsm_belong_char(const void *ch, const void *subset, size_t n);
DLL_API_EXPORT BOOL cwt_fsm_exact_char(const void *s, size_t n1, const void *seq, size_t n2);
DLL_API_EXPORT BOOL cwt_fsm_range_char(const void *ch, const void *from, const void *to);
DLL_API_EXPORT BOOL cwt_fsm_belong_cwtchar(const void *ch, const void *subset, size_t n);
DLL_API_EXPORT BOOL cwt_fsm_exact_cwtchar(const void *s, size_t n1, const void *seq, size_t n2);
DLL_API_EXPORT BOOL cwt_fsm_range_cwtchar(const void *ch, const void *from, const void *to);
DLL_API_EXPORT BOOL cwt_fsm_belong_int(const void *ch, const void *subset, size_t n);
DLL_API_EXPORT BOOL cwt_fsm_exact_int(const void *s, size_t n1, const void *seq, size_t n2);
DLL_API_EXPORT BOOL cwt_fsm_range_int(const void *ch, const void *from, const void *to);

DLL_API_EXPORT ssize_t cwt_fsm_repetition(struct _CwtFsm *fsm, void *fn_context, const void *data, size_t len);

EXTERN_C_END

#endif /* __CWT_FSM_H__ */
