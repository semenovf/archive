/*
 * fsm_match.hpp
 *
 *  Created on: Jul 26, 2013
 *      Author: wladt
 */

#ifndef __CWT_FSM_MATCH_HPP__
#define __CWT_FSM_MATCH_HPP__

#ifndef __CWT_FSM_HPP__
#	error "Do not include fsm_match.hpp directly"
#endif

CWT_NS_BEGIN

/* Matches sequence of characters */
template <typename _P>
class FsmMatchSeq : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchSeq(size_t len) : m_len(len) {}
	virtual ~FsmMatchSeq() {}
	virtual ssize_t match(FsmContext<_P> *, const const_iterator & begin, const const_iterator & end) const
	{
		return begin + m_len <= end ? (ssize_t)m_len : (ssize_t)-1;
	}
private:
	size_t m_len;
};

/* Matches string of characters */
template <typename _P>
class FsmMatchStr : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchStr(const _P & p) : m_p(p) {}
	virtual ~FsmMatchStr() {}
	virtual ssize_t match(FsmContext<_P> *, const const_iterator & begin, const const_iterator & end) const
	{
		return m_p.length() == 0
				? 0
				: begin < end && Fsm<_P>::containsChars(m_p.begin(), m_p.end(), begin, end)
				  	  ? ssize_t(m_p.length(m_p.begin(), m_p.end()))
				  	  : ssize_t(-1);
	}
private:
	_P m_p;
};


/* Matches one character from characters */
template <typename _P>
class FsmMatchChar : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchChar(const _P & p) : m_p(p) {}
	virtual ~FsmMatchChar() {}
	virtual ssize_t match(FsmContext<_P> *, const const_iterator & begin, const const_iterator & end) const
	{
		return m_p.length() == 0
			? 0
			: begin < end && Fsm<_P>::belongsChar(begin.value(), m_p.begin(), m_p.end())
			    ? ssize_t(1)
				: ssize_t(-1);
	}

private:
	_P m_p;
};

template <typename _P>
class FsmMatchRange : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchRange (char_type from,  char_type to) : m_from(from), m_to(to) {}
	virtual ~FsmMatchRange() {}
	virtual ssize_t match(FsmContext<_P> *, const const_iterator & begin, const const_iterator & end) const
	{
		return begin < end && Fsm<_P>::rangeChar(begin.value(), m_from, m_to)
			? ssize_t(1)
			: ssize_t(-1);
	}
private:
	char_type m_from;
	char_type m_to;
};

template <typename _P>
class FsmMatchFsm : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchFsm(const FsmTransition<_P> *trans) : m_trans(trans) {}
	virtual ~FsmMatchFsm() {}
	virtual ssize_t match(FsmContext<_P> * ctx, const const_iterator & begin, const const_iterator & end) const
	{
		Fsm<_P> fsm(m_trans, ctx->context);
		return fsm.exec(FSM_NORMAL, begin, end);
	}

private:
	const FsmTransition<_P> *m_trans;
};

template <typename _P>
class FsmMatchFunc : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;
	typedef typename FsmMatch<_P>::func_type func_type;

	FsmMatchFunc(func_type fn, void *fnContext) : m_fn(fn), m_fnContext(fnContext) {}
	virtual ~FsmMatchFunc() {}
	virtual ssize_t match(FsmContext<_P> * ctx, const const_iterator & begin, const const_iterator & end) const
	{
		return m_fn(ctx, m_fnContext, begin, end);
	}
private:
	func_type m_fn;
	void     *m_fnContext;
};

template <typename _P>
class FsmMatchRpt : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchRpt(const FsmMatch<_P> & match, int from, int to) : m_match(match) {
		m_bounds.from = from; m_bounds.to = to;
	}
	virtual ~FsmMatchRpt() {}

	virtual ssize_t match(FsmContext<_P> * ctx, const const_iterator & begin, const const_iterator & end) const
	{
		int from = 0;
		int to = CWT_INT_MAX;
		const_iterator ptr(begin);
		size_t nchars_total_processed;

		if( m_bounds.from >= 0 )
			from = m_bounds.from;

		if( m_bounds.to >= 0 )
			to = m_bounds.to;

		CWT_ASSERT(from <= to);

		nchars_total_processed = 0;
		int i = 0;

		for (i = 0; i < to && ptr < end; i++) {

			ssize_t nchars_processed = m_match(ctx, ptr, end);

			if( nchars_processed < 0 ) {
				break;
			}

			if( nchars_processed > 0) {
				ptr += size_t(nchars_processed);
				nchars_total_processed += size_t(nchars_processed);
			}
		}

		CWT_ASSERT(nchars_total_processed <= CWT_SSIZE_MAX);

		if( i < from )
			return ssize_t(-1);

		return ssize_t(nchars_total_processed);
	}

private:
	FsmMatch<_P> m_match;
	FsmRptBounds m_bounds;
};

template <typename _P>
class FsmMatchNothing : public FsmMatchBase<_P>
{
public:
	typedef typename _P::char_type char_type;
	typedef typename _P::const_iterator const_iterator;

	FsmMatchNothing() {}
	virtual ~FsmMatchNothing() {}
	virtual ssize_t match(FsmContext<_P> *, const const_iterator & , const const_iterator & ) const
	{
		return 0;
	}
};


CWT_NS_END

#endif /* __CWT_FSM_MATCH_HPP__ */
