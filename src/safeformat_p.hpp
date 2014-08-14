/*
 * safeformat_fsm.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __PFS_SAFEFORMAT_P_HPP__
#define __PFS_SAFEFORMAT_P_HPP__

#include "pfs/safeformat.hpp"
#include "pfs/vector.hpp"
#include "pfs/fsm.hpp"

namespace pfs {

struct conversion_spec
{
	int    flags;
	int    width;
	int    prec; // The default precision is 1
	pfs::ucchar spec_char;
};

struct safeformat_context
{
	typedef pfs::string::const_iterator const_iterator;

	pfs::string     format;
	pfs::string     result;
	conversion_spec  spec;
	pfs::vector<pfs::unitype> bind_args;
	size_t          argi; // index of current argument
};

inline void clear_conversion_spec(conversion_spec & spec)
{
	spec.flags     = safeformat::NoFlag;
	spec.width     =  0;
	spec.prec      =  1;
	spec.spec_char = pfs::ucchar::Null;
}

inline void clear_safeformat_context(safeformat_context & ctx)
{
	ctx.format.clear();
	ctx.result.clear();
	clear_conversion_spec(ctx.spec);
	ctx.bind_args.clear();
	ctx.argi = 0;
}

} // pfs

#endif /* __PFS_SAFEFORMAT_FSM_HPP__ */
