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



static ssize_t __fsm_exec(CwtFsm *fsm, int state_cur, const void *data, size_t len);

static CwtFsmNS __cwtFsmNS = {
	  __fsm_exec
};


DLL_API_EXPORT CwtFsmNS* cwtFsmNS(void)
{
	return &__cwtFsmNS;
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


DLL_API_EXPORT BOOL cwtBelongChar(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(char, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactChar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(char, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtRangeChar(const void *ch, const void *from, const void *to)
{
	_RANGE_CHAR(char, ch, from, to);
}

DLL_API_EXPORT BOOL cwtBelongCwtChar(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(CWT_CHAR, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactCwtChar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(CWT_CHAR, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtRangeCwtChar(const void *ch, const void *from, const void *to)
{
	_RANGE_CHAR(CWT_CHAR, ch, from, to);
}

DLL_API_EXPORT BOOL cwtBelongInt(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(int, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactInt(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(int, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtRangeInt(const void *ch, const void *from, const void *to)
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
ssize_t cwtFsmRepetition(CwtFsm *fsm, void *fn_context, const void *data, size_t len)
{
	CwtFsmRepetitionContext *rep_context = (CwtFsmRepetitionContext*)fn_context;
	CwtFsm rep_fsm;
	const char *ptr;
	int i;
	ssize_t nchars_processed;
	size_t nchars_total_processed;

	CWT_ASSERT(fn_context);
	CWT_ASSERT(fsm);

	ptr = (const char*)data;

	if( !ptr )
		return 0;

	if( rep_context->from < 0 )
		rep_context->from = 0;

	if( rep_context->to < 0 )
		rep_context->to = CWT_INT_MAX;

	CWT_ASSERT(rep_context->from <= rep_context->to);

	memcpy(&rep_fsm, fsm, sizeof(CwtFsm));
	rep_fsm.trans_tab = rep_context->trans_tab;

	nchars_processed = (ssize_t)-1;
	nchars_total_processed = 0;

	for( i = 0; i < rep_context->to && len > 0; i++ ) {

		nchars_processed = __fsm_exec(&rep_fsm, 0, ptr, len);

		if( nchars_processed < 0 ) {
			break;
		}

		if( nchars_processed > 0) {
			len -= nchars_processed;
			ptr += (rep_fsm.sizeof_char * nchars_processed);
			nchars_total_processed += (size_t)nchars_processed;
		}
	}

	CWT_ASSERT(nchars_total_processed <= CWT_SSIZE_T_MAX);

	if( i < rep_context->from )
		return (ssize_t)-1;

	return (ssize_t)nchars_total_processed;
}

static ssize_t __fsm_exec(CwtFsm *fsm, int state_cur, const void *data, size_t len)
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

	while( TRUE ) {

		switch( trans->match_type ) {
		case Cwt_Fsm_Match_Seq:
			if( len >= trans->condition.str.len )
				nchars_processed = trans->condition.str.len;
			break;

		case Cwt_Fsm_Match_Str:
			if( fsm->exact(ptr, CWT_MIN(len, trans->condition.str.len)
					, trans->condition.str.chars, trans->condition.str.len) ) {
				nchars_processed = (ssize_t)trans->condition.str.len;
			}
			break;

		case Cwt_Fsm_Match_Char:
			if( len > 0 && fsm->belong(ptr, trans->condition.str.chars, trans->condition.str.len) ) {
				nchars_processed = 1;
			}
			break;

		case Cwt_Fsm_Match_Range:
			if( len > 0 && fsm->range(ptr, trans->condition.range.from, trans->condition.range.to) ) {
				nchars_processed = 1;
			}
			break;

		case Cwt_Fsm_Match_Fsm: {
				CwtFsm inner_fsm;

				memcpy(&inner_fsm, fsm, sizeof(inner_fsm));
				inner_fsm.trans_tab = trans->condition.trans_tab.tab;
				nchars_processed = __fsm_exec(&inner_fsm, 0, ptr, len);
			}
			break;

		case Cwt_Fsm_Match_Func:
			nchars_processed = trans->condition.trans_fn.fn(fsm, trans->condition.trans_fn.fn_context, ptr, len);
			break;

		case Cwt_Fsm_Match_Nothing:
			nchars_processed = 0;
			break;
		}

		if( nchars_processed >= 0 ) {
			if( trans->status == FSM_ACCEPT ) {
				if( trans->action )
					trans->action(ptr
					, (size_t)(nchars_processed)
					, fsm->context
					, trans->action_args);

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
			if( trans->status != FSM_ACCEPT )
				accepted = FALSE;
		}

		if( state_cur < 0 )
			break;

		trans = &fsm->trans_tab[state_cur];
		nchars_processed = (ssize_t)-1;

		if( trans->status == FSM_REJECT )
			return (ssize_t)-1;
	}

	CWT_ASSERT(nchars_total_accepted <= CWT_SSIZE_T_MAX);

	return accepted
			? (ssize_t)nchars_total_accepted
			: (ssize_t)-1;
}
