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
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	size_t _len;

public:
	match_seq (size_t len)
		: _len(len)
	{}

	virtual ~match_seq ()
	{}

	virtual result_type do_match (context<_Sequence> *
			, const_iterator begin
			, const_iterator end) const
	{
//		if (std::distance(begin, end) > _len)
//			return result_type(false, end);
//
//		std::advance(begin, _len);
//		return result_type(false, end);
		size_t n = _len;
		while (n > 0 && begin != end) {
			++begin;
			--n;
		}
		return n > 0
				? result_type(false, end)
				: result_type(true, begin);
	}
};

//
// Matches string of characters
//
template <typename _Sequence>
class match_str : public match_base<_Sequence>
{
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	_Sequence _p;

public:
	match_str (const _Sequence & p)
		: _p(p) {}
	virtual ~match_str ()
	{}

	virtual result_type do_match (context<_Sequence> *
			, const_iterator begin
			, const_iterator end) const
	{
		if (begin == end)
			return result_type(false, end);
		return fsm<_Sequence>::contains_chars(_p.begin(), _p.end(), begin, end);
	}
};

//
// Matches one character from characters
//
template <typename _Sequence>
class match_char : public match_base<_Sequence>
{
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	_Sequence _p;

public:
	match_char (const _Sequence & p)
		: _p(p)
	{}

	virtual ~match_char ()
	{}

	virtual result_type do_match (context<_Sequence> *
			, const_iterator begin
			, const_iterator end) const
	{
		if (begin == end)
			return result_type(false, end);
		return fsm<_Sequence>::belongs_char(*begin, _p.begin(), _p.end())
			? result_type(true, ++begin)
			: result_type(false, end);
	}
};

template <typename _Sequence>
class match_range : public match_base<_Sequence>
{
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	char_type _from;
	char_type _to;

public:
	match_range (char_type from,  char_type to)
		: _from(from)
		, _to(to)
	{}

	virtual ~match_range ()
	{}

	virtual result_type do_match (context<_Sequence> *, const_iterator begin, const_iterator end) const
	{
		return fsm<_Sequence>::range_char(*begin, _from, _to)
			? result_type(true, ++begin)
			: result_type(false, end);
	}
};

template <typename _Sequence>
class match_fsm : public match_base<_Sequence>
{
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	const transition<_Sequence> * _trans;

public:
	match_fsm (const transition<_Sequence> * trans)
		: _trans(trans)
	{}

	virtual ~match_fsm ()
	{}

	virtual result_type do_match(context<_Sequence> * ctx
			, const_iterator begin
			, const_iterator end) const
	{
		fsm<_Sequence> fsm(_trans, ctx->parse_context);
		return fsm.exec(FSM_NORMAL, begin, end);
	}
};

template <typename _Sequence>
class match_func : public match_base<_Sequence>
{
	typedef match_base<_Sequence>                base_class;
	typedef typename base_class::char_type       char_type;
	typedef typename base_class::const_iterator  const_iterator;
	typedef typename base_class::result_type     result_type;
	typedef typename match<_Sequence>::func_type func_type;

private:
	func_type _fn;
	void *    _fnContext;

public:
	match_func (func_type fn, void * fnContext)
		: _fn(fn)
		, _fnContext(fnContext)
	{}

	virtual ~match_func ()
	{}

	virtual result_type do_match (context<_Sequence> * ctx
			, const_iterator begin
			, const_iterator end) const
	{
		return _fn(ctx->parse_context, _fnContext, begin, end);
	}
};

template <typename _Sequence>
class match_rpt : public match_base<_Sequence>
{
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

private:
	match<_Sequence> _match;
	rpt_bounds       _bounds;

public:
	match_rpt (const match<_Sequence> & m, int from, int to)
		: _match(m)
	{
		_bounds.from = from;
		_bounds.to = to;
	}

	virtual ~match_rpt()
	{}

	virtual result_type do_match (context<_Sequence> * ctx
			, const_iterator begin
			, const_iterator end) const;
};

template <typename _Sequence>
typename match_rpt<_Sequence>::result_type match_rpt<_Sequence>::do_match (context<_Sequence> * ctx
		, const_iterator begin
		, const_iterator end) const
{
	int from = 0;
	int to = pfs::max_value<int>();
	const_iterator ptr(begin);
	//size_t nchars_total_processed;

	if (_bounds.from >= 0)
		from = _bounds.from;

	if (_bounds.to >= 0)
		to = _bounds.to;

	PFS_ASSERT(from <= to);

	//nchars_total_processed = 0;
	int i = 0;

	for (i = 0; i < to && ptr < end; i++) {

		//ssize_t nchars_processed = _match(ctx, ptr, end);
		result_type r = _match(ctx, ptr, end);

		//if (nchars_processed < 0) {
		if (!r.first) {
			break;
		}

//		if (nchars_processed > 0) {
//			ptr += size_t(nchars_processed);
//			nchars_total_processed += size_t(nchars_processed);
//		}
		ptr = r.second;
	}

	if (i < from) {
		//return ssize_t(-1);
		return result_type(false, end);
	}

	//return integral_cast_check<ssize_t, size_t>(nchars_total_processed);
	return result_type(true, ptr);
}

template <typename _Sequence>
class match_nothing : public match_base<_Sequence>
{
	typedef match_base<_Sequence>               base_class;
	typedef typename base_class::char_type      char_type;
	typedef typename base_class::const_iterator const_iterator;
	typedef typename base_class::result_type    result_type;

public:
	match_nothing ()
	{}

	virtual ~match_nothing ()
	{}

	virtual result_type do_match (context<_Sequence> *, const_iterator begin, const_iterator) const
	{
		return result_type(true, begin);
	}
};

}} // pfs::fsm

#endif /* __PFS_FSM_MATCH_HPP__ */
