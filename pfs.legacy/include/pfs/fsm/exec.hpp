/*
 * fsm_exec.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: wladt
 */

#ifndef __PFS_FSM_EXEC_HPP__
#define __PFS_FSM_EXEC_HPP__

#ifndef __PFS_FSM_HPP__
#	error "Do not include fsm_exec.hpp directly"
#endif

namespace pfs { namespace fsm {

template <typename _Sequence>
//ssize_t fsm<_Sequence>::exec (int state_cur, const_iterator begin, const_iterator end)
typename fsm<_Sequence>::result_type fsm<_Sequence>::exec (int state_cur, const_iterator begin, const_iterator end)
{
	const_iterator ptr = begin;
	const_iterator ptr_accepted = begin;
//	size_t nchars_total_processed = 0;
//	size_t nchars_total_accepted  = 0;
	const transition<_Sequence> * trans;
	bool accepted = false;

	// FIXME no need after _contex
	PFS_ASSERT(_context);

	PFS_ASSERT(_context->_trans_tab);

	trans = & _context->_trans_tab[state_cur];

	PFS_ASSERT(trans);

	do {
		//ssize_t nchars_processed = trans->_match(_context, ptr, end);
		result_type result = trans->_match(_context, ptr, end);

		//if (nchars_processed >= 0) {
		if (result.first) {
			if (trans->_action) {
				//if (!trans->_action(ptr, ptr + size_t(nchars_processed), _context->_userContext, trans->_action_args)) {
				if (!trans->_action(ptr, result.second, _context->parse_context, trans->_action_args)) {

					// Let's support this situation
					//
					//static FSM_TRANSITION record_fsm[] = {
					//	  {-1,  1, FSM_MATCH_SEQ(10) , FSM_ACCEPT, on_record1, 0 }
					//	, {-1,  2, FSM_MATCH_SEQ(11) , FSM_ACCEPT, on_record2, 0 }
					//	, {-1, -1, FSM_MATCH_SEQ(12) , FSM_ACCEPT, on_record3, 0 }
					//};
					// ===
					if (trans->_status == FSM_ACCEPT) {
						if (trans->_state_fail >= 0) {
							trans = & _context->_trans_tab[trans->_state_fail];
							continue;
						}
					}
					// ===

					//return ssize_t(-1);
					return result_type(false, end);
				}
			}

			if( trans->_status == FSM_ACCEPT ) {
				accepted = true;
			}

			//ptr += size_t(nchars_processed);
			//nchars_total_processed += size_t(nchars_processed);
			ptr = result.second;

			if (trans->_status == FSM_ACCEPT) {
				//nchars_total_accepted = nchars_total_processed;
				ptr_accepted = ptr;
			}

			if (trans->_status == FSM_REJECT) {
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

			//ptr = begin + nchars_total_accepted;
			//nchars_total_processed = nchars_total_accepted;
			ptr = ptr_accepted;
		}

		if (state_cur < 0)
			break;

		trans = & _context->_trans_tab[state_cur];
		//nchars_processed = ssize_t(-1);

	} while (true);

	return accepted
			//? integral_cast_check<ssize_t, size_t>(nchars_total_accepted)
			? result_type(true, ptr_accepted)
			//: (ssize_t)-1;
			: result_type(false, end);
}

}} // pfs::fsm

#endif /* __PFS_FSM_EXEC_HPP__ */
