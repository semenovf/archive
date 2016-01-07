/**
 * @file   fsm.hpp
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Header file for Finite State Machine definitions.
 */

#ifndef __PFS_FSM_HPP__
#define __PFS_FSM_HPP__

#include <utility> // std::pair
#include <pfs/bits/assert.h>

#define FSM_NORMAL  0
#define FSM_REJECT  1
#define FSM_ACCEPT  2

namespace pfs { namespace fsm {

template <typename _Sequence>
struct result
{
	typedef typename std::pair<bool, typename _Sequence::const_iterator> type;
};

template <typename _Sequence>
struct context;

template <typename _Sequence>
struct transition;

struct rpt_bounds
{
	int from, to;
};

template <typename _Sequence>
class match_base
{
public:
	typedef typename _Sequence::value_type     char_type;
	typedef typename _Sequence::const_iterator const_iterator;
	typedef typename result<_Sequence>::type   result_type;

	match_base() : _ref(1) {}
	virtual ~match_base() {}

	//virtual ssize_t do_match (context<_Sequence> * fsm, const_iterator begin, const_iterator end) const = 0;
	virtual result_type do_match (context<_Sequence> * fsm, const_iterator begin, const_iterator end) const = 0;

	int _ref;
};

template <typename _Sequence>
class match
{
	match_base<_Sequence> * _match;

public:
	typedef typename _Sequence::value_type     char_type;
	typedef typename _Sequence::const_iterator const_iterator;
	typedef typename result<_Sequence>::type   result_type;

	//typedef ssize_t (* func_type)(context<_Sequence> * fsm, void *fn_context, typename _Sequence::const_iterator begin, typename _Sequence::const_iterator end);
	typedef result_type (* func_type)(void * parse_context
			, void * fn_context
			, const_iterator begin
			, const_iterator end);

private:
	match() : _match(0) {}
	void deref () {
		if (_match) {
			PFS_ASSERT(_match->_ref > 0);
			--_match->_ref;
			if (_match->_ref == 0)
				delete _match;
			_match = 0;
		}
	}

public:
	match (match_base<_Sequence> * match) : _match(match) { PFS_ASSERT(_match); }
	match (const match & other)
	{
		PFS_ASSERT(other._match);
		_match = other._match;
		++_match->_ref;
	}

	~match () { deref(); }

	//ssize_t operator () (context<_Sequence> * fsm, const_iterator begin, const_iterator end) const
	result_type operator () (context<_Sequence> * fsm, const_iterator begin, const_iterator end) const
	{
		return _match->do_match(fsm, begin, end);
	}
};

template <typename _Sequence>
struct transition
{
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	int _state_next;
	int _state_fail;
	match<_Sequence> _match;
	int _status; /* last entry in the chain of ...*/
	bool (* _action)(const_iterator begin, const_iterator end, void * context, void * action_args);
	void * _action_args;
};

template <typename _Sequence>
struct context
{
	const transition<_Sequence> * _trans_tab;
	void * parse_context;     /* user context */
};


template <typename _Sequence>
class fsm
{
	context<_Sequence> * _context;

public:
	typedef _Sequence                              sequence_type;
	typedef typename sequence_type::value_type     char_type;
	typedef typename sequence_type::const_iterator const_iterator;
	typedef typename result<sequence_type>::type   result_type;

public:
	fsm ();
	fsm (const transition<_Sequence> * initialTrans, void * context);
	~fsm ();

	void set_transition_table (transition<_Sequence> * tb)
	{
		_context->_trans_tab = tb;
	}

	void set_user_context (void * context)
	{
		_context->parse_context = context;
	}

	//ssize_t exec (int state_cur, const_iterator begin, const_iterator end);
	result_type exec (int state_cur, const_iterator begin, const_iterator end);

	result_type exec (const_iterator begin, const_iterator end)
	{
		return exec(0, begin, end);
	}

public:
	/** @brief Checks if character @c ch belongs to the subset of characters
	 *         specified by begin and end iterators.
	 */
	static bool belongs_char (char_type ch, const_iterator begin, const_iterator end);

	/** @brief Checks if haystack specified by begin and end iterators
	 *         (@a haystack_begin and @a haystack_begin respectively)
	 *         contains the needle also specified by begin and end iterators
	 *         (@a needle_begin and @a needle_end respectively).
	 *
	 *  @return pair
	 */
	static result_type contains_chars (const_iterator needle_begin, const_iterator needle_end
			, const_iterator haystack_begin, const_iterator haystack_end);

	/** @brief Checks if character 'ch' belongs to the specified range
	  */
	static bool range_char (char_type ch, char_type from, char_type to);
};


template <typename _Sequence>
bool fsm<_Sequence>::belongs_char (char_type ch
		, const_iterator begin
		, const_iterator end)
{
	while (begin < end) {
		if (*begin == ch)
			return true;
		++begin;
	}
	return false;
}

template <typename _Sequence>
typename fsm<_Sequence>::result_type fsm<_Sequence>::contains_chars (const_iterator needle_begin
		, const_iterator needle_end
		, const_iterator haystack_begin
		, const_iterator haystack_end)
{
	const_iterator itn(needle_begin);
	const_iterator ith(haystack_begin);

	if (needle_begin == needle_end)
		return result_type(false, haystack_end);

	while(itn < needle_end && ith < haystack_end) {
		if (*itn != *ith)
			break;
		++itn;
		++ith;
	}

	return itn == needle_end
			? result_type(true, ith)
			: result_type(false, haystack_end);
}

template <typename _Sequence>
inline bool fsm<_Sequence>::range_char (char_type ch, char_type from, char_type to)
{
	return ch >= from && ch <= to ? true : false;
}

template <typename _Sequence>
fsm<_Sequence>::fsm ()
	: _context(new context<_Sequence>)
{
	_context->_trans_tab   = 0;
	_context->parse_context = 0;
}

template <typename _Sequence>
fsm<_Sequence>::fsm (const transition<_Sequence> * initialTrans, void * userContext)
	: _context(new context<_Sequence>)
{
	_context->_trans_tab   = initialTrans;
	_context->parse_context = userContext;
}

// FIXME make _context automatically allocated
//
template <typename _Sequence>
fsm<_Sequence>::~fsm()
{
	if (_context) {
		delete _context;
	}
}

}} // pfs::fsm

#include <pfs/fsm/match.hpp>
#include <pfs/fsm/exec.hpp>

#endif /* __PFS_FSM_HPP__ */
