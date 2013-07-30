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

CWT_NS_BEGIN

struct ConversionSpec
{
	int    flags;
	int    field_width;
	int    prec;
	UChar  spec_char;
};

struct SafeFormatContext
{
	typedef typename String::const_iterator const_iterator;

	String format;
	String result;
	const_iterator cursor;
	const_iterator end;
	ConversionSpec spec;
	Fsm<String>    fsm;
};

inline void __clear_spec(ConversionSpec & spec)
{
	spec.flags       = SafeFormat::NoFlag;
	spec.field_width = -1;
	spec.prec        = -1;
	spec.spec_char   = Unicode::Null;
}

inline void __clear_context(SafeFormatContext & ctx)
{
	ctx.format.clear();
	ctx.result.clear();
	__clear_spec(ctx.spec);
}

CWT_NS_END

#endif /* __CWT_SAFEFORMAT_FSM_HPP__ */
