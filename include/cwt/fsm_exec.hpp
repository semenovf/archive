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

namespace cwt { namespace fsm {

template <typename _P>
ssize_t fsm<_P>::exec(int state_cur, const /*fsm<_P>::*/const_iterator & begin, const /*fsm<_P>::*/const_iterator & end)
{
	const_iterator ptr = begin;
	size_t nchars_total_processed = 0;
	size_t nchars_total_accepted  = 0;
	const transition<_P> * trans;
	bool accepted = false;

	PFS_ASSERT(_context);
	PFS_ASSERT(_context->_trans_tab);

	trans = & _context->_trans_tab[state_cur];

	PFS_ASSERT(trans);

	do {
		ssize_t nchars_processed = trans->_match(_context, ptr, end);

		if (nchars_processed >= 0) {
			if( trans->_action ) {
				if (!trans->_action(ptr, ptr + size_t(nchars_processed), _context->_userContext, trans->_action_args))
					return ssize_t(-1);
			}

			if( trans->_status == FSM_ACCEPT ) {
				accepted = true;
			}

			ptr += size_t(nchars_processed);
			nchars_total_processed += nchars_processed;

			if (trans->_status == FSM_ACCEPT)
				nchars_total_accepted = nchars_total_processed;

			if (trans->_status == FSM_REJECT) { /* finish, use case see at cwt-csv:csv_rfc4180.hpp in 'nonquoted_char_fsm' for ex. */
				state_cur = -1;
				accepted = false;
			} else {
				state_cur = trans->_state_next;
			}
		} else {
			state_cur = trans->_state_fail;

			if( trans->_status != FSM_ACCEPT ) {
				accepted = false;
			}

			ptr = begin + nchars_total_accepted;
			nchars_total_processed = nchars_total_accepted;
		}

		if( state_cur < 0 )
			break;

		trans = & _context->_trans_tab[state_cur];
		//nchars_processed = ssize_t(-1);

	} while( true );

	PFS_ASSERT(nchars_total_accepted <= PFS_SSIZE_MAX);

	return accepted
			? (ssize_t)nchars_total_accepted
			: (ssize_t)-1;
}

}} // cwt::fsm

#endif /* __CWT_FSM_EXEC_HPP__ */
