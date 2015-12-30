/*
 * fsm_match.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: wladt
 */

#ifndef __PFS_FSM_MATCH_HPP__
#define __PFS_FSM_MATCH_HPP__

#ifndef __PFS_FSM_HPP__
#	error "Do not include fsm_match.hpp directly"
#endif

#include <pfs/limits.hpp>

namespace pfs { namespace fsm {

//
// Matches sequence of characters
//
template <typename _Sequence>
class match_seq : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	match_seq (size_t len) : m_len(len) {}
	virtual ~match_seq () {}
	virtual ssize_t do_match (context<_Sequence> *, const_iterator begin, const_iterator end) const
	{
		return begin + m_len <= end ? (ssize_t)m_len : (ssize_t)-1;
	}
private:
	size_t m_len;
};

//
// Matches string of characters
//
template <typename _Sequence>
class match_str : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	match_str (const _Sequence & p) : m_p(p) {}
	virtual ~match_str () {}
	virtual ssize_t do_match (context<_Sequence> *, const_iterator begin, const_iterator end) const
	{
		return m_p.length() == 0
				? 0
				: begin < end && fsm<_Sequence>::containsChars(m_p.begin(), m_p.end(), begin, end)
				  	  ? ssize_t(m_p.end() - m_p.begin())
				  	  : ssize_t(-1);
	}
private:
	_Sequence m_p;
};

//
// Matches one character from characters
//
template <typename _Sequence>
class match_char : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	match_char (const _Sequence & p) : m_p(p) {}
	virtual ~match_char () {}
	virtual ssize_t do_match (context<_Sequence> *, const_iterator begin, const_iterator end) const
	{
		return m_p.length() == 0
			? 0
			: begin < end && fsm<_Sequence>::belongsChar(*begin, m_p.begin(), m_p.end())
			    ? ssize_t(1)
				: ssize_t(-1);
	}

private:
	_Sequence m_p;
};

template <typename _Sequence>
class match_range : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

private:
	char_type _from;
	char_type _to;

public:
	match_range (char_type from,  char_type to) : _from(from), _to(to) {}
	virtual ~match_range () {}
	virtual ssize_t do_match (context<_Sequence> *, const_iterator begin, const_iterator end) const
	{
		return begin < end && fsm<_Sequence>::rangeChar(*begin, _from, _to)
			? ssize_t(1)
			: ssize_t(-1);
	}
};

template <typename _Sequence>
class match_fsm : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	match_fsm (const transition<_Sequence> * trans) : _trans(trans) {}
	virtual ~match_fsm () {}
	virtual ssize_t do_match(context<_Sequence> * ctx, const_iterator begin, const_iterator end) const
	{
		fsm<_Sequence> fsm(_trans, ctx->_userContext);
		return fsm.exec(FSM_NORMAL, begin, end);
	}

private:
	const transition<_Sequence> * _trans;
};

template <typename _Sequence>
class match_func : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;
	typedef typename match<_Sequence>::func_type func_type;

	match_func (func_type fn, void * fnContext) : _fn(fn), _fnContext(fnContext) {}
	virtual ~match_func () {}
	virtual ssize_t do_match (context<_Sequence> * ctx, const_iterator begin, const_iterator end) const
	{
		return _fn(ctx, _fnContext, begin, end);
	}
private:
	func_type _fn;
	void *    _fnContext;
};

template <typename _Sequence>
class match_rpt : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	match_rpt (const match<_Sequence> & m, int from, int to) : _match(m)
	{
		_bounds.from = from;
		_bounds.to = to;
	}
	virtual ~match_rpt() {}
	virtual ssize_t do_match (context<_Sequence> * ctx, const_iterator begin, const_iterator end) const override;

private:
	match<_Sequence> _match;
	rpt_bounds _bounds;
};

template <typename _Sequence>
ssize_t match_rpt<_Sequence>::do_match (context<_Sequence> * ctx, const_iterator begin, const_iterator end) const
{
	int from = 0;
	int to = pfs::max_value<int>();
	const_iterator ptr(begin);
	size_t nchars_total_processed;

	if (_bounds.from >= 0)
		from = _bounds.from;

	if (_bounds.to >= 0)
		to = _bounds.to;

	PFS_ASSERT(from <= to);

	nchars_total_processed = 0;
	int i = 0;

	for (i = 0; i < to && ptr < end; i++) {

		ssize_t nchars_processed = _match(ctx, ptr, end);

		if (nchars_processed < 0) {
			break;
		}

		if (nchars_processed > 0) {
			ptr += size_t(nchars_processed);
			nchars_total_processed += size_t(nchars_processed);
		}
	}

	if (i < from)
		return ssize_t(-1);

	return integral_cast_check<ssize_t, size_t>(nchars_total_processed);
}

template <typename _Sequence>
class match_nothing : public match_base<_Sequence>
{
public:
	typedef typename _Sequence::value_type     char_type;
	typedef typename _Sequence::const_iterator const_iterator;

	match_nothing () {}
	virtual ~match_nothing () {}
	virtual ssize_t do_match (context<_Sequence> *, const_iterator, const_iterator) const
	{
		return 0;
	}
};

}} // pfs::fsm

#endif /* __PFS_FSM_MATCH_HPP__ */
