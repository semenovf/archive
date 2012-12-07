/**
 * @file   fsm.c
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Implementation of
 */

#include <cwt/fsm.h>
#include <string.h>
#include <cwt/str.h>
#include <cwt/vector.h>

/*
CWT_BEGIN_DECL_VECTOR_NS (FsmOffsetArrayNS, FsmOffsetArray, size_t)
CWT_END_DECL_VECTOR_NS   (FsmOffsetArrayNS)
CWT_BEGIN_DEF_VECTOR_NS  (FsmOffsetArrayNS, FsmOffsetArray, size_t)
CWT_END_DEF_VECTOR_NS    (FsmOffsetArrayNS)
CWT_VECTOR_METHODS       (                  FsmOffsetArray, size_t)
*/

/*
   \r\n ; Windows/DOS
   \n\r ; MAC OS 9
   \n   ; Unix

   Use this FSM transition table instead of CRLF_FSM_INL
*/

static const CWT_CHAR *__nl_win = _T("\r\n");
static const CWT_CHAR *__nl_mac = _T("\n\r"); /* FIXME must be up to 9 version */
static const CWT_CHAR *__nl_nix = _T("\n");

static void    fsm_init          (
		  CwtFsm *fsm
		, int sizeof_char_type
		, CwtFsmTransition *initial_tab
		, void *context
		, CwtBelongCharFn belong_char_fn
		, CwtExactCharFn exact_char_fn
		, CwtRangeCharFn range_char_fn);
static void    fsm_destroy       (CwtFsm *fsm);
static ssize_t fsm_exec          (CwtFsm *fsm, int state_cur, const void *data, size_t len);
static void    fsm_reproduce     (CwtFsm *fsm_copy, CwtFsm *fsm_orig, CwtFsmTransition *trans);
static void    fsm_reproduce_with_context (CwtFsm *fsm_copy, CwtFsm *fsm_orig, CwtFsmTransition *initial_tab, void *context);
static void    fsm_set_nl        (CwtFsm *fsm, CwtNewLineEnum nl);
static void    fsm_set_custom_nl (CwtFsm *fsm, const CWT_CHAR *nl);


static CwtFsmNS __cwtFsmNS = {
	  fsm_init
	, fsm_destroy
	, fsm_exec
	, fsm_reproduce
	, fsm_reproduce_with_context
	, fsm_set_nl
	, fsm_set_custom_nl
};


DLL_API_EXPORT CwtFsmNS* cwt_fsm_ns(void)
{
	return &__cwtFsmNS;
}


static void fsm_init (CwtFsm *fsm
		, int sizeof_char_type
		, CwtFsmTransition *initial_tab
		, void *context
		, CwtBelongCharFn belong_char_fn
		, CwtExactCharFn exact_char_fn
		, CwtRangeCharFn range_char_fn)
{
	cwt_bzero(fsm, sizeof(*fsm));
	fsm->trans_tab         = initial_tab;
	fsm->sizeof_char       = sizeof_char_type;
	fsm->context           = context;
	fsm->belong_char_fn    = belong_char_fn;
	fsm->exact_char_fn     = exact_char_fn;
	fsm->range_char_fn     = range_char_fn;

	fsm_set_nl(fsm,
#if defined(CWT_OS_WIN)
			Cwt_NL_Win
#elif defined(CWT_OS_MAC9) /* FIXME must be recognized and defined in global._os.h */
			Cwt_NL_MacOS9
#elif defined(CWT_OS_UNIX)
			Cwt_NL_Unix
#else
			Cwt_NL_Unix
#endif
	);
}

static void fsm_destroy (CwtFsm *fsm)
{
	if (fsm) {
		if (fsm->nl) {
			CWT_FREE(fsm->nl);
			fsm->nl = NULL;
		}
	}
}


static void fsm_reproduce (CwtFsm *fsm_copy, CwtFsm *fsm_orig, CwtFsmTransition *trans_tab)
{
	CWT_ASSERT(fsm_orig);
	CWT_ASSERT(fsm_copy);

	cwt_str_ns()->memcpy(fsm_copy, fsm_orig, sizeof(*fsm_orig));
	fsm_copy->trans_tab = trans_tab;
	fsm_copy->nl = cwt_str_ns()->strDup(fsm_orig->nl);
}


static void fsm_reproduce_with_context (CwtFsm *fsm_copy, CwtFsm *fsm_orig, CwtFsmTransition *initial_tab, void *context)
{
	fsm_reproduce(fsm_copy, fsm_orig, initial_tab);
	fsm_copy->context = context;
}

static void fsm_set_nl (CwtFsm *fsm, CwtNewLineEnum nl)
{
	CWT_ASSERT(fsm);

	switch (nl) {
	case Cwt_NL_Win:
		fsm_set_custom_nl(fsm, __nl_win);
		break;
	case Cwt_NL_MacOS9:
		fsm_set_custom_nl(fsm, __nl_mac);
		break;
	case Cwt_NL_Unix:
	default:
		fsm_set_custom_nl(fsm, __nl_nix);
		break;
	}
}

static void fsm_set_custom_nl (CwtFsm *fsm, const CWT_CHAR *nl)
{
	CWT_ASSERT(fsm);

	if (fsm->nl) {
		CWT_FREE(fsm->nl);
		fsm->nl = NULL;
	}

	if (nl) {
		fsm->nl = cwt_str_ns()->strDup(nl);
	}
}

#define _BELONG_CHAR(char_type,ch,subset,n)                           \
	size_t i = 0;                                                     \
	while( i < (n) ) {                                                \
		if( ((const char_type*)(subset))[i++] == *((char_type*)(ch))) \
			return TRUE;                                              \
	}                                                                 \
	return FALSE;

#define _EXACT_STR(char_type,s1,n1,s2,n2)                       \
	if( (n1) != (n2) ) return FALSE;                            \
	return memcmp(s1, s2, (n1) * sizeof(char_type)) == 0        \
		? TRUE : FALSE;

#define _RANGE_CHAR(char_type,ch,from,to)                           \
	return *((const char_type*)(ch)) >= *((const char_type*)(from)) \
		&& *((const char_type*)(ch)) <= *((const char_type*)(to))   \
		? TRUE : FALSE;


DLL_API_EXPORT BOOL cwt_fsm_belong_char(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(char, ch, subset, n);
}

DLL_API_EXPORT BOOL cwt_fsm_exact_char(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(char, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwt_fsm_range_char(const void *ch, const void *from, const void *to)
{
	_RANGE_CHAR(char, ch, from, to);
}

DLL_API_EXPORT BOOL cwt_fsm_belong_cwtchar(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(CWT_CHAR, ch, subset, n);
}

DLL_API_EXPORT BOOL cwt_fsm_exact_cwtchar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(CWT_CHAR, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwt_fsm_range_cwtchar(const void *ch, const void *from, const void *to)
{
	_RANGE_CHAR(CWT_CHAR, ch, from, to);
}

DLL_API_EXPORT BOOL cwt_fsm_belong_int(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(int, ch, subset, n);
}

DLL_API_EXPORT BOOL cwt_fsm_exact_int(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(int, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwt_fsm_range_int(const void *ch, const void *from, const void *to)
{
	_RANGE_CHAR(int, ch, from, to);
}

/**
 *
 * @param fsm
 * @param fn_context
 * @param data
 * @param len
 * @return
 */
ssize_t cwt_fsm_repetition(CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CwtFsmRptBounds *bounds = (CwtFsmRptBounds*)fn_context;
	const char *ptr;
	int i;
	ssize_t nchars_processed;
	size_t nchars_total_processed;

	CWT_ASSERT(fn_context);
	CWT_ASSERT(fsm);

	ptr = (const char*)data;

	if( !ptr )
		return 0;

	if( bounds->from < 0 )
		bounds->from = 0;

	if( bounds->to < 0 )
		bounds->to = CWT_INT_MAX;

	CWT_ASSERT(bounds->from <= bounds->to);

	nchars_processed = (ssize_t)-1;
	nchars_total_processed = 0;

	for( i = 0; i < bounds->to && len > 0; i++ ) {

		nchars_processed = fsm_exec(fsm, 0, ptr, len);

		if( nchars_processed < 0 ) {
			break;
		}

		if( nchars_processed > 0) {
			len -= nchars_processed;
			ptr += (fsm->sizeof_char * nchars_processed);
			nchars_total_processed += (size_t)nchars_processed;
		}
	}

	CWT_ASSERT(nchars_total_processed <= CWT_SSIZE_T_MAX);

	if( i < bounds->from )
		return (ssize_t)-1;

	return (ssize_t)nchars_total_processed;
}

static ssize_t fsm_exec(CwtFsm *fsm, int state_cur, const void *data, size_t len)
{
	const char *ptr;
	ssize_t nchars_processed;
	size_t nchars_total_processed;
	size_t nchars_total_accepted;
	CwtFsmTransition *trans;
	BOOL accepted;

	ptr = (const char*)data;
	nchars_processed = (ssize_t)-1;
	nchars_total_processed = (size_t)0;
	nchars_total_accepted  = (size_t)0;
	accepted = FALSE;

	trans = &fsm->trans_tab[state_cur];

	CWT_ASSERT(trans);

	do /*while( TRUE )*/ {

		switch( trans->match_type ) {

		case Cwt_Fsm_Match_Nl:
			if (fsm->nl) {
				size_t nl_len = cwt_str_ns()->strLen(fsm->nl);

				if (fsm->exact_char_fn(ptr, CWT_MIN(len, nl_len), fsm->nl, nl_len)) {
					nchars_processed = nl_len;
				}
			} else {
				nchars_processed = 0;
			}
			break;


		case Cwt_Fsm_Match_Seq:
			if( len >= trans->condition.str.len )
				nchars_processed = (ssize_t)trans->condition.str.len;
			break;

		case Cwt_Fsm_Match_Str:
			if( len >= trans->condition.str.len
					&& fsm->exact_char_fn(ptr, CWT_MIN(len, trans->condition.str.len)
					, trans->condition.str.chars, trans->condition.str.len) ) {
				nchars_processed = (ssize_t)trans->condition.str.len;
			}
			break;

		case Cwt_Fsm_Match_Char:
			if( len > 0 && fsm->belong_char_fn(ptr, trans->condition.str.chars, trans->condition.str.len) ) {
				nchars_processed = 1;
			}
			break;

		case Cwt_Fsm_Match_Range:
			if( len > 0 && fsm->range_char_fn(ptr, trans->condition.range.from, trans->condition.range.to) ) {
				nchars_processed = 1;
			}
			break;

		case Cwt_Fsm_Match_Fsm: {
				CwtFsm inner_fsm;
				fsm_reproduce(&inner_fsm, fsm, trans->condition.trans_tab.tab);
				nchars_processed = fsm_exec(&inner_fsm, 0, ptr, len);
				fsm_destroy(&inner_fsm);
			}
			break;

		case Cwt_Fsm_Match_Func:
			nchars_processed = trans->condition.trans_fn.fn(fsm, trans->condition.trans_fn.fn_context, ptr, len);
			break;

		case Cwt_Fsm_Match_Rpt: {
				CwtFsm inner_fsm;

				memcpy(&inner_fsm, fsm, sizeof(inner_fsm));
				inner_fsm.trans_tab = trans->condition.trans_tab.tab;
				nchars_processed = cwt_fsm_repetition(&inner_fsm, trans->condition.trans_tab.bounds, ptr, len);
			}
			break;

		case Cwt_Fsm_Match_Nothing:
			nchars_processed = 0;
			break;
		}

		if( nchars_processed >= 0 ) {
			if( trans->action )
				trans->action(ptr
				, (size_t)(nchars_processed)
				, fsm->context
				, trans->action_args);

			if( trans->status == FSM_ACCEPT ) {
				accepted = TRUE;
			}

			ptr += (fsm->sizeof_char * nchars_processed);
			len -= nchars_processed;
			nchars_total_processed += nchars_processed;

			if( trans->status == FSM_ACCEPT )
				nchars_total_accepted = nchars_total_processed;

			state_cur = trans->state_next;
		} else {
			state_cur = trans->state_fail;

			if( trans->status != FSM_ACCEPT ) {
				accepted = FALSE;
			}

			ptr -= (fsm->sizeof_char * nchars_total_processed);
			len += nchars_total_processed;
			nchars_total_processed = nchars_total_accepted;
		}

		if( state_cur < 0 )
			break;

		trans = &fsm->trans_tab[state_cur];
		nchars_processed = (ssize_t)-1;

		if( trans->status == FSM_REJECT ) {
			if( trans->action )
				trans->action(ptr
				, len
				, fsm->context
				, trans->action_args);

			return (ssize_t)-1;
		}

	} while( TRUE );

	CWT_ASSERT(nchars_total_accepted <= CWT_SSIZE_T_MAX);

	return accepted
			? (ssize_t)nchars_total_accepted
			: (ssize_t)-1;
}
