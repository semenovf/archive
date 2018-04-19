/*
 * fsm_exec.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: wladt
 */

#ifndef __CWT_FSM_EXEC_HPP__
#define __CWT_FSM_EXEC_HPP__

#ifndef __CWT_FSM_HPP__
#	error "Do not include fsm_exec.hpp directly"
#endif

CWT_NS_BEGIN

template <typename _P>
ssize_t Fsm<_P>::exec(int state_cur, const Fsm<_P>::const_iterator & begin, const Fsm<_P>::const_iterator & end)
{
	const_iterator ptr = begin;
	size_t nchars_total_processed = 0;
	size_t nchars_total_accepted  = 0;
	const FsmTransition<_P> * trans;
	bool accepted = false;

	CWT_ASSERT(m_fsmContext);
	CWT_ASSERT(m_fsmContext->trans_tab);

	trans = & m_fsmContext->trans_tab[state_cur];

	CWT_ASSERT(trans);

	do {
		ssize_t nchars_processed = trans->match(m_fsmContext, ptr, end);

		if (nchars_processed >= 0) {
			if( trans->action ) {
				if (!trans->action(ptr, ptr + size_t(nchars_processed), m_fsmContext->context, trans->action_args))
					return ssize_t(-1);
			}

			if( trans->status == FSM_ACCEPT ) {
				accepted = true;
			}

			ptr += size_t(nchars_processed);
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

			ptr = begin + nchars_total_accepted;
			nchars_total_processed = nchars_total_accepted;
		}

		if( state_cur < 0 )
			break;

		trans = & m_fsmContext->trans_tab[state_cur];
		//nchars_processed = ssize_t(-1);

	} while( true );

	CWT_ASSERT(nchars_total_accepted <= CWT_SSIZE_MAX);

	return accepted
			? (ssize_t)nchars_total_accepted
			: (ssize_t)-1;
}

CWT_NS_END


#endif /* __CWT_FSM_EXEC_HPP__ */
