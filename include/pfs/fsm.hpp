/**
 * @file   fsm.hpp
 * @author wladt
 * @date   Oct 10, 2012 10:51:21 AM
 *
 * @brief Header file for Finite State Machine definitions.
 */

#ifndef __PFS_FSM_HPP__
#define __PFS_FSM_HPP__

//#include <cstring>

#ifndef FSM_ITERABLE_TYPE
#	include <pfs/string.hpp>
#	define FSM_ITERABLE_TYPE pfs::string
#endif

// Common predefined macros
//
#define FSM_TRANSITION                   pfs::fsm::transition<FSM_ITERABLE_TYPE>

#define FSM_MATCH_SEQ(n)                 pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_seq<FSM_ITERABLE_TYPE>(n))
#define FSM_MATCH_RANGE(min,max)         pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_range<FSM_ITERABLE_TYPE>(min,max))
#define FSM_MATCH_RPT_RANGE(min,max,f,t) pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_RANGE(min,max),(f),(t)))
#define FSM_MATCH_OPT_RANGE(min,max)     pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_RANGE(min,max),0,1))
#define FSM_MATCH_FSM(tr)                pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_fsm<FSM_ITERABLE_TYPE>(tr))
#define FSM_MATCH_RPT_FSM(tr,f,t)        pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FSM(tr),(f),(t)))
#define FSM_MATCH_OPT_FSM(tr)            pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FSM(tr),0,1))
#define FSM_MATCH_FUNC(fn,pcont)         pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_func<FSM_ITERABLE_TYPE>(fn,pcont))
#define FSM_MATCH_RPT_FUNC(fn,pcont,f,t) pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FUNC(fn,pcont),(f),(t)))
#define FSM_MATCH_OPT_FUNC(fn,pcont)     pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FUNC(fn,pcont),0,1))
#define FSM_MATCH_NOTHING                pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_nothing<FSM_ITERABLE_TYPE>)
#define FSM_MATCH_CHAR(s)                pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_char<FSM_ITERABLE_TYPE>(s))
#define FSM_MATCH_STR(s)                 pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_str<FSM_ITERABLE_TYPE>(s))
#define FSM_MATCH_RPT_STR(s,f,t)         pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_STR(s),f,t))
#define FSM_MATCH_OPT_STR(s)             pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_STR(s),0,1))
#define FSM_MATCH_RPT_CHAR(s,f,t)        pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_CHAR(s),f,t))
#define FSM_MATCH_OPT_CHAR(s)            pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_CHAR(s),0,1))

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
	typedef result_type (* func_type)(context<_Sequence> * fsm, void *fn_context, typename _Sequence::const_iterator begin, typename _Sequence::const_iterator end);

private:
	match() : _match(nullptr) {}
	void deref () {
		if (_match) {
			PFS_ASSERT(_match->_ref > 0);
			--_match->_ref;
			if (_match->_ref == 0)
				delete _match;
			_match = nullptr;
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
	void * _userContext;     /* user context */
};


template <typename _Sequence>
class fsm
{
	context<_Sequence> * _context;

public:
	typedef typename _Sequence::value_type     char_type;
	typedef typename _Sequence::const_iterator const_iterator;
	typedef typename result<_Sequence>::type   result_type;

public:
	fsm ();
	fsm (const transition<_Sequence> * initialTrans, void * context);
	~fsm ();

	void setTransitionTable (transition<_Sequence> * trans)
	{
		_context->_trans_tab = trans;
	}

	void setUserContext (void * userContext)
	{
		_context->_userContext = userContext;
	}

	//ssize_t exec (int state_cur, const_iterator begin, const_iterator end);
	result_type exec (int state_cur, const_iterator begin, const_iterator end);

public:
	static bool belongsChar (char_type ch, const_iterator begin, const_iterator end);
	static bool containsChars (const_iterator needle_begin, const_iterator needle_end
			, const_iterator haystack_begin, const_iterator haystack_end);
	static bool rangeChar (char_type ch, char_type from, char_type to);
};


/** @brief Checks if character @c ch belongs to the subset of characters
 *         specified by begin and end iterators.
 * */
template <typename _Sequence>
bool fsm<_Sequence>::belongsChar (/*typename fsm<_Sequence>::*/char_type ch, const_iterator begin, const_iterator end)
{
	const_iterator it(begin);
	while (it < end) {
		if (*it == ch)
			return true;
		++it;
	}
	return false;
}

/** @brief Checks if haystack specified by begin an end iterators
 *         (@c haystack_begin and @c haystack_begin respectively)
 *         contains the needle also specified by begin and end iterators
 *         (@c needle_begin and @c needle_end respectively).
 * */
template <typename _Sequence>
bool fsm<_Sequence>::containsChars (const_iterator needle_begin, const_iterator needle_end
		, const_iterator haystack_begin, const_iterator haystack_end)
{
	const_iterator it_needle(needle_begin);
	const_iterator it_haystack(haystack_begin);

	if (needle_begin == needle_end)
		return false;

	while(it_needle < needle_end && it_haystack < haystack_end) {
		if (*it_needle != *it_haystack)
			break;
		++it_needle;
		++it_haystack;
	}

	return it_needle == needle_end ? true : false;
}

/** @brief Checks if character 'ch' belongs to the specified range
  */
template <typename _Sequence>
inline bool fsm<_Sequence>::rangeChar(char_type ch, char_type from, char_type to)
{
	return ch >= from && ch <= to ? true : false;
}

template <typename _Sequence>
fsm<_Sequence>::fsm ()
	: _context(new context<_Sequence>)
{
	_context->_trans_tab     = nullptr;
	_context->_userContext       = nullptr;
}

template <typename _Sequence>
fsm<_Sequence>::fsm (const transition<_Sequence> * initialTrans, void * userContext)
	: _context(new context<_Sequence>)
{
	_context->_trans_tab   = initialTrans;
	_context->_userContext = userContext;
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

#include "fsm_match.hpp"
#include "fsm_exec.hpp"

#endif /* __PFS_FSM_HPP__ */
