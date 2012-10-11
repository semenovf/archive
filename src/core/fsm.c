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

static size_t __fsm_exec(CwtFsm *fsm, int state_cur, const void *data, size_t len);

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

/*
#define _EXACT_STR(char_type,s1,n1,s2,n2)                       \
	size_t i = 0;                                               \
	if( (n1) != (n2) )                                          \
		return FALSE;                                           \
	while( i < (n1)                                             \
		&& (*((char_type*)(s1)++) == *((char_type*)(s2)++)) ) { \
		i++;                                                    \
	}                                                           \
	return i == (n1) ? TRUE : FALSE;
*/
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


static size_t __fsm_exec(CwtFsm *fsm, int state_cur, const void *data, size_t len)
{
	const void *ptr;
	size_t nchars_processed;
	size_t nchars_total_processed;
	CwtFsmTransition *trans;

	ptr = data;
	nchars_processed = 0;
	nchars_total_processed = 0;

	trans = &fsm->trans_tab[state_cur];

	while( len > 0 ) {

		switch( trans->match_type ) {

		case Cwt_Fsm_Match_Str:
			if( fsm->exact(ptr, CWT_MIN(len, trans->condition.str.len)
					, trans->condition.str.chars, trans->condition.str.len) ) {
				nchars_processed = trans->condition.str.len;
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
				inner_fsm.trans_tab = trans->condition.trans_tab;
				nchars_processed = __fsm_exec(&inner_fsm, trans->state_next, ptr, len);
			}
			break;

		case Cwt_Fsm_Match_Func:
			nchars_processed = trans->condition.trans_fn(fsm->context, ptr, len);
			break;

		case Cwt_Fsm_Match_Nil:
			break;
		}

		/* accepted */
		if( nchars_processed > 0 ) {
			ptr += nchars_processed;
			len -= nchars_processed;
			nchars_total_processed += nchars_processed;
			state_cur = trans->state_next;

			if( state_cur < 0 )
				break;

			nchars_processed = 0;
		} else {
			if( trans->is_accept == FSM_REJECT )
				break;
		}
		trans = &fsm->trans_tab[state_cur];
	}

	return (nchars_processed > 0)
			? nchars_total_processed
			: 0;
}
