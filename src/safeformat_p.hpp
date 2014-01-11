/*
 * safeformat_fsm.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __CWT_SAFEFORMAT_P_HPP__
#define __CWT_SAFEFORMAT_P_HPP__

#include "../include/cwt/safeformat.hpp"
#include <pfs/vector.hpp>
#include <cwt/fsm.hpp>

namespace cwt {

struct ConversionSpec
{
	int    flags;
	int    width;
	int    prec; // The default precision is 1
	pfs::ucchar spec_char;
};

struct safeformatcontext
{
	typedef pfs::string::const_iterator const_iterator;

	pfs::string     format;
	pfs::string     result;
	ConversionSpec  spec;
	pfs::vector<pfs::unitype> bind_args;
	size_t          argi; // index of current argument
};

inline void __clear_spec(ConversionSpec & spec)
{
	spec.flags     = safeformat::NoFlag;
	spec.width     =  0;
	spec.prec      =  1;
	spec.spec_char = pfs::ucchar::Null;
}

inline void __clear_context(safeformatcontext & ctx)
{
	ctx.format.clear();
	ctx.result.clear();
	__clear_spec(ctx.spec);
	ctx.bind_args.clear();
	ctx.argi = 0;
}

} // cwt

#endif /* __CWT_SAFEFORMAT_FSM_HPP__ */
