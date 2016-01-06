/*
 * alias.hpp
 *
 *  Created on: Jan 6, 2016
 *      Author: wladt
 */

#ifndef __PFS_FSM_ALIASES_HPP__
#define __PFS_FSM_ALIASES_HPP__

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

namespace pfs { namespace fsm {

template <typename Sequence>
struct m
{
	typedef Sequence                            sequence_type;
	typedef typename sequence_type::value_type  char_type;
	typedef pfs::fsm::transition<sequence_type> transition_type;
	typedef pfs::fsm::match<sequence_type>      match_type;

	match_type seq (size_t n)
	{
		return match_type(new pfs::fsm::match_seq<sequence_type>(n));
	}

	match_type range (char_type min,  char_type max)
	{
		return match_type(new pfs::fsm::match_range<sequence_type>(min,max));
	}

//#define FSM_MATCH_RPT_RANGE(min,max,f,t) pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_RANGE(min,max),(f),(t)))
//#define FSM_MATCH_OPT_RANGE(min,max)     pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_RANGE(min,max),0,1))
//#define FSM_MATCH_FSM(tr)                pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_fsm<FSM_ITERABLE_TYPE>(tr))
//#define FSM_MATCH_RPT_FSM(tr,f,t)        pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FSM(tr),(f),(t)))
//#define FSM_MATCH_OPT_FSM(tr)            pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FSM(tr),0,1))
//#define FSM_MATCH_FUNC(fn,pcont)         pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_func<FSM_ITERABLE_TYPE>(fn,pcont))
//#define FSM_MATCH_RPT_FUNC(fn,pcont,f,t) pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FUNC(fn,pcont),(f),(t)))
//#define FSM_MATCH_OPT_FUNC(fn,pcont)     pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_FUNC(fn,pcont),0,1))
//#define FSM_MATCH_NOTHING                pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_nothing<FSM_ITERABLE_TYPE>)
//#define FSM_MATCH_CHAR(s)                pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_char<FSM_ITERABLE_TYPE>(s))
//#define FSM_MATCH_STR(s)                 pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_str<FSM_ITERABLE_TYPE>(s))
//#define FSM_MATCH_RPT_STR(s,f,t)         pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_STR(s),f,t))
//#define FSM_MATCH_OPT_STR(s)             pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_STR(s),0,1))
//#define FSM_MATCH_RPT_CHAR(s,f,t)        pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_CHAR(s),f,t))
//#define FSM_MATCH_OPT_CHAR(s)            pfs::fsm::match<FSM_ITERABLE_TYPE>(new pfs::fsm::match_rpt<FSM_ITERABLE_TYPE>(FSM_MATCH_CHAR(s),0,1))

};

}} // pfs::fsm

#endif /* __PFS_FSM_ALIASES_HPP__ */
