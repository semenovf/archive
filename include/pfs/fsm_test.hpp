/**
 * @file   fsm_test.h
 * @author wladt
 * @date   Oct 31, 2012 4:48:26 PM
 *
 * @brief Used for testing FSM rules. Simply include this file
 *        in test source file.
 */

#ifndef __PFS_FSM_TEST_HPP__
#define __PFS_FSM_TEST_HPP__

#include <pfs/test/test.hpp>
#include <pfs/fsm.hpp>

namespace pfs { namespace fsm {

template <typename _Sequence>
bool test_valid_entry (transition<_Sequence> * trans_tab, const _Sequence & s)
{
	fsm<_Sequence> fsm;
	fsm.set_transition_table(trans_tab);

	typename fsm<_Sequence>::result_type result = fsm.exec(s.cbegin(), s.cend());

	return result.first && result.second == s.cend();
}

template <typename _Sequence>
bool test_invalid_entry (transition<_Sequence> * trans_tab, const _Sequence & s, ssize_t offset)
{
	fsm<_Sequence> fsm;
	fsm.set_transition_table(trans_tab);

	typename fsm<_Sequence>::result_type result = fsm.exec(s.cbegin(), s.cend());

	if (offset >= 0) {
		typename _Sequence::const_iterator it(s.cbegin());
		std::advance(it, offset);
		return result.first && result.second == it;
	}

	return result.first == false && result.second == s.cend();
}

}} // pfs::fsm

#endif /* __PFS_FSM_TEST_HPP__ */
