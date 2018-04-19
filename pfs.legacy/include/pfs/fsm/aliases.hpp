/*
 * alias.hpp
 *
 *  Created on: Jan 6, 2016
 *      Author: wladt
 */

#ifndef __PFS_FSM_ALIASES_HPP__
#define __PFS_FSM_ALIASES_HPP__

#if __COMMENT__
#ifndef FSM_ITERABLE_TYPE
#	error "Undefined iterable type: #define FSM_ITERABLE_TYPE <iterable_type> before include <pfs/fsm/aliases.hpp>"
//#	define FSM_ITERABLE_TYPE pfs::string
#endif

// Common predefined aliases.
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

#endif

namespace pfs { namespace fsm {

template <typename Sequence>
struct m
{
	typedef Sequence                            sequence_type;
	typedef typename sequence_type::value_type  char_type;
	typedef pfs::fsm::transition<sequence_type> transition_type;
	typedef pfs::fsm::match<sequence_type>      match_type;
	typedef typename match_type::func_type      func_type;

	struct nothing
	{
		match_type m;
		nothing()
			: m(new pfs::fsm::match_nothing<sequence_type>())
		{}
	};

	struct seq
	{
		match_type m;
		seq (size_t n)
			: m(new pfs::fsm::match_seq<sequence_type>(n))
		{}
	};

	struct chr
	{
		match_type m;
		chr (const sequence_type & s)
			:	m(new pfs::fsm::match_char<sequence_type>(s))
		{}
	};

	struct str
	{
		match_type m;
		str(const sequence_type & s)
			: m(new pfs::fsm::match_str<sequence_type>(s))
		{}
	};

	struct func
	{
		match_type m;
		func (func_type f, void * func_context)
			: m(new pfs::fsm::match_func<sequence_type>(f, func_context))
		{}
	};

	struct tr
	{
		match_type m;
		tr (transition_type * t)
			: m(new pfs::fsm::match_fsm<sequence_type>(t))
		{}
	};

	struct range
	{
		match_type m;
		range (char_type min, char_type max)
			: m (new pfs::fsm::match_range<sequence_type>(min, max))
		{}
	};

	struct rpt_range
	{
		match_type m;
		rpt_range (char_type min, char_type max, int from, int to)
			: m(new pfs::fsm::match_rpt<sequence_type>(range(min, max).m, from, to))
		{}
	};

	struct opt_range
	{
		match_type m;
		opt_range (char_type min, char_type max)
			: m(new pfs::fsm::match_rpt<sequence_type>(range(min,max).m, 0, 1))
		{}
	};

	struct rpt_tr
	{
		match_type m;
		rpt_tr (transition_type * t, int from, int to)
			: m(new pfs::fsm::match_rpt<sequence_type>(tr(t).m, from, to))
		{}
	};

	struct opt_tr
	{
		match_type m;
		opt_tr (transition_type * t)
			: m(new pfs::fsm::match_rpt<sequence_type>(tr(t).m, 0, 1))
		{}
	};

	struct rpt_func
	{
		match_type m;
		rpt_func (func_type f, void * func_context, int from, int to)
			: m(new pfs::fsm::match_rpt<sequence_type>(func(f, func_context).m, from, to))
		{}
	};

	struct opt_func
	{
		match_type m;
		opt_func (func_type f, void * func_context)
			: m(new pfs::fsm::match_rpt<sequence_type>(func(f, func_context).m, 0, 1))
		{}
	};

	struct rpt_str
	{
		match_type m;
		rpt_str (const sequence_type & s, int from, int to)
			: m(new pfs::fsm::match_rpt<sequence_type>(str(s).m, from, to))
		{}
	};

	struct opt_str
	{
		match_type m;
		opt_str (const sequence_type & s)
			: m(new pfs::fsm::match_rpt<sequence_type>(str(s).m, 0, 1))
		{}
	};

	struct rpt_chr
	{
		match_type m;
		rpt_chr (const sequence_type & s, int from, int to)
			: m(new pfs::fsm::match_rpt<sequence_type>(chr(s).m, from, to))
		{}
	};

	struct opt_chr
	{
		match_type m;
		opt_chr (const sequence_type & s)
			: m(new pfs::fsm::match_rpt<sequence_type>(chr(s).m, 0, 1))
		{}
	};
};

}} // pfs::fsm

#endif /* __PFS_FSM_ALIASES_HPP__ */
