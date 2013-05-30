/**
 * @file   fsm.c
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Implementation of
 */

#include <cwt/fsm.hpp>
#include <cwt/char.hpp>
#include <cstdlib>
#include <cstring>

CWT_NS_BEGIN

static void reproduce (FsmContext *copy, const FsmContext *orig, const FsmTransition *initialTab);

ssize_t FsmMatchSeq::match(FsmContext *fsm, const void *data, size_t len) const
{
	CWT_UNUSED2(fsm, data);
	return len >= m_len ? (ssize_t)m_len : (ssize_t)-1;
}

ssize_t FsmMatchStr::match(FsmContext *fsm, const void *data, size_t len) const
{
	return m_len == 0
			? 0
			: len >= m_len && fsm->exactCharFn(data, CWT_MIN(len, m_len), m_str, m_len)
			  	  ? (ssize_t)m_len
			  	  : (ssize_t)-1;
}

ssize_t FsmMatchStringStr::match(FsmContext *fsm, const void *data, size_t len) const
{
	size_t length = (size_t)m_str.length();
	return length == 0
			? 0
			: len >= length && fsm->exactCharFn(data, CWT_MIN(len, length), m_str.data(), length)
			  	  ? (ssize_t)length
			  	  : (ssize_t)-1;
}


ssize_t FsmMatchChar::match(FsmContext *fsm, const void *data, size_t len) const
{
	return m_count == 0
			? 0
			: len > 0 && fsm->belongCharFn(data, m_chars, m_count)
			  	  ? (ssize_t)1
			  	  : (ssize_t)-1;
}


ssize_t FsmMatchStringChar::match(FsmContext *fsm, const void *data, size_t len) const
{
	size_t count = (size_t)m_str.length();
	return count == 0
			? 0
			: len > 0 && fsm->belongCharFn(data, m_str.data(), count)
			  	  ? (ssize_t)1
			  	  : (ssize_t)-1;
}

ssize_t FsmMatchRange::match(FsmContext *fsm, const void *data, size_t len) const
{
	return len > 0 && fsm->rangeCharFn(data, m_from, m_to)
		? (ssize_t)1
		: (ssize_t)-1;
}

ssize_t FsmMatchFsm::match(FsmContext *fsm, const void *data, size_t len) const
{
	FsmContext sub;
	reproduce(&sub, fsm, m_trans);
	return execFsmContext(&sub, 0, data, len);
}

ssize_t FsmMatchFunc::match(FsmContext *fsm, const void *data, size_t len) const
{
	return m_fn(fsm, m_fnContext, data, len);
}

ssize_t FsmMatchRpt::match(FsmContext *fsm, const void *data, size_t len) const
{
	int from = 0;
	int to = CWT_INT_MAX;
	const char *ptr;
	int i;
	ssize_t nchars_processed;
	size_t nchars_total_processed;

	ptr = (const char*)data;

	if( !ptr )
		return 0;

	if( m_bounds.from >= 0 )
		from = m_bounds.from;

	if( m_bounds.to >= 0 )
		to = m_bounds.to;

	CWT_ASSERT(from <= to);

	nchars_processed = (ssize_t)-1;
	nchars_total_processed = 0;

	for( i = 0; i < to && len > 0; i++ ) {

		nchars_processed = m_match(fsm, ptr, len);

		if( nchars_processed < 0 ) {
			break;
		}

		if( nchars_processed > 0) {
			len -= nchars_processed;
			ptr += (fsm->sizeof_char * nchars_processed);
			nchars_total_processed += (size_t)nchars_processed;
		}
	}

	CWT_ASSERT(nchars_total_processed <= CWT_SSIZE_MAX);

	if( i < from )
		return (ssize_t)-1;

	return (ssize_t)nchars_total_processed;
}

static void reproduce (FsmContext *copy, const FsmContext *orig, const FsmTransition *initialTab)
{
	CWT_ASSERT(orig);
	CWT_ASSERT(copy);

	memcpy(copy, orig, sizeof(*orig));
	copy->trans_tab = initialTab;
}

ssize_t execFsmContext (FsmContext *fsmContext, int state_cur, const void *data, size_t datalen)
{
	const char *ptr;
	ssize_t nchars_processed;
	size_t nchars_total_processed;
	size_t nchars_total_accepted;
	const FsmTransition *trans;
	bool accepted;
	size_t len;

	ptr = (const char*)data;
	nchars_processed = (ssize_t)-1;
	nchars_total_processed = (size_t)0;
	nchars_total_accepted  = (size_t)0;
	accepted = false;
	len = datalen;

	trans = &fsmContext->trans_tab[state_cur];

	CWT_ASSERT(trans);

	do {
		nchars_processed = trans->match(fsmContext, ptr, len);

		if (nchars_processed >= 0) {
			if( trans->action ) {

				if (!trans->action(ptr
						, (size_t)(nchars_processed)
						, fsmContext->context
						, trans->action_args))
					return (ssize_t)-1;
			}

			if( trans->status == FSM_ACCEPT ) {
				accepted = true;
			}

			ptr += (fsmContext->sizeof_char * nchars_processed);
			len -= nchars_processed;
			nchars_total_processed += nchars_processed;

			if (trans->status == FSM_ACCEPT)
				nchars_total_accepted = nchars_total_processed;

			if (trans->status == FSM_REJECT) { /* finish, use case see at cwt-csv:csv_rfc4180.hpp in 'nonquoted_char_fsm' for ex. */
				state_cur = -1;
				accepted = false;
			} else {
				state_cur = trans->state_next;
			}
		} else {
			state_cur = trans->state_fail;

			if( trans->status != FSM_ACCEPT ) {
				accepted = false;
			}

			ptr = (const char*)data + (fsmContext->sizeof_char * nchars_total_accepted);
			len = datalen + nchars_total_accepted;

			nchars_total_processed = nchars_total_accepted;
		}

		if( state_cur < 0 )
			break;

		trans = &fsmContext->trans_tab[state_cur];
		nchars_processed = (ssize_t)-1;

/*
		if (trans->status == FSM_REJECT) {
			if( trans->action )
				trans->action(ptr
				, len
				, fsmContext->context
				, trans->action_args);

			return (ssize_t)-1;
		}
*/

	} while( true );

	CWT_ASSERT(nchars_total_accepted <= CWT_SSIZE_MAX);

	return accepted
			? (ssize_t)nchars_total_accepted
			: (ssize_t)-1;
}

CWT_NS_END
