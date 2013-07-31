/*
 * safeformat_fsm.hpp
 *
 *  Created on: Jul 29, 2013
 *      Author: wladt
 */

#ifndef __CWT_SAFEFORMAT_P_HPP__
#define __CWT_SAFEFORMAT_P_HPP__

#include "../include/cwt/safeformat.hpp"
#include "../include/cwt/unicode.hpp"
#include "../include/cwt/vector.hpp"

CWT_NS_BEGIN

struct ConversionSpec
{
	int    flags;
	int    width;
	int    prec; // The default precision is 1
	UChar  spec_char;
};

struct SafeFormatContext
{
	typedef typename String::const_iterator const_iterator;

	String          format;
	String          result;
	ConversionSpec  spec;
	Vector<UniType> bind_args;
	size_t          argi; // index of current argument
};

inline void __clear_spec(ConversionSpec & spec)
{
	spec.flags     = SafeFormat::NoFlag;
	spec.width     =  0;
	spec.prec      =  1;
	spec.spec_char = Unicode::Null;
}

inline void __clear_context(SafeFormatContext & ctx)
{
	ctx.format.clear();
	ctx.result.clear();
	__clear_spec(ctx.spec);
	ctx.bind_args.clear();
	ctx.argi = 0;
}

CWT_NS_END

#endif /* __CWT_SAFEFORMAT_FSM_HPP__ */
