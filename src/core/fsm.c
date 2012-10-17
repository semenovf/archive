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
/*#include <cwt/logger.h>*/

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

DLL_API_EXPORT BOOL cwtBelongChar(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(char, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactChar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(char, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtBelongCwtChar(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(CWT_CHAR, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactCwtChar(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(CWT_CHAR, s, n1, seq, n2);
}

DLL_API_EXPORT BOOL cwtBelongInt(const void *ch, const void *subset, size_t n)
{
	_BELONG_CHAR(int, ch, subset, n);
}

DLL_API_EXPORT BOOL cwtExactInt(const void *s, size_t n1, const void *seq, size_t n2)
{
	_EXACT_STR(int, s, n1, seq, n2);
}


static ssize_t __fsm_exec(CwtFsm *fsm, int state_cur, const void *data, size_t len_orig)
{
	const char *ptr;
	BOOL accepted = FALSE;
	ssize_t nchars_processed;
	size_t nchars_total_processed;
	CwtFsmTransition *trans;
	size_t len;

	if( len_orig == 0 )
		return (ssize_t)0;

	ptr = (const char*)data;
	len = len_orig;
	nchars_processed = (ssize_t)-1;
	nchars_total_processed = (size_t)0;

	trans = &fsm->trans_tab[state_cur];

	while( len > 0 ) {

		switch( trans->match_type ) {

		case Cwt_Fsm_Match_Str:
			if( fsm->exact(ptr, CWT_MIN(len, trans->condition.str.len)
					, trans->condition.str.chars, trans->condition.str.len) ) {
				nchars_processed = (ssize_t)trans->condition.str.len;
			}
			break;

		case Cwt_Fsm_Match_Char:
			if( fsm->belong(ptr, trans->condition.str.chars, trans->condition.str.len) ) {
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
			nchars_processed = trans->condition.trans_fn.fn(fsm->context, ptr, len);
			break;

		case Cwt_Fsm_Match_Nothing:
			nchars_processed = 0;
			break;
		}

		if( nchars_processed >= 0 ) {
			ptr += (fsm->sizeof_char * nchars_processed);
			len -= nchars_processed;
			nchars_total_processed += nchars_processed;

			if( trans->status == FSM_ACCEPT ) {
				accepted = TRUE;

				if( trans->action )
					trans->action(data, (size_t)nchars_processed, fsm->context, trans->action_args);
			}

			state_cur = trans->state_next;
		} else {
			state_cur = trans->state_fail;
		}

		if( state_cur < 0 )
			break;

		trans = &fsm->trans_tab[state_cur];
		nchars_processed = (ssize_t)-1;

		if( trans->status == FSM_REJECT )
			return (ssize_t)-1;
	}

	CWT_ASSERT(nchars_total_processed <= CWT_SSIZE_T_MAX);

	return accepted
			? (ssize_t)nchars_total_processed
			: (ssize_t)-1;
}
