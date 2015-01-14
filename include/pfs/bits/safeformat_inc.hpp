/*
 * safeformat_inc.hpp
 *
 *  Created on: Jan 13, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_SAFEFORMAT_INC_HPP__
#define __PFS_BITS_SAFEFORMAT_INC_HPP__

namespace pfs {

//template <typename StringT>
//inline void safeformat<StringT>::ungetc ()
//{
//	PFS_ASSERT(_ctx.pos > _ctx.format.cbegin());
//	--_ctx.pos;
//}

/**
 * @return '\0' at the end of string, '%' otherwise
 */
template <typename StringT>
void safeformat<StringT>::advance ()
{
	char_type v;
	while ((v = getc()) != char_type(0)) {
		if (getc() == char_type('%')) {
			nextc();
			if (getc() == char_type('%')) {
				_ctx.result.append(char_type('%'));
			} else {
			}
		} else {
			_ctx.result.append(v);
		}
	}
}

//template <typename StringT>
//inline void safeformat<StringT>::read_tail ()
//{
//	_context.result.append(_context.pos, _context.format.cend());
//}


/*
 * conversion_specification := '%' *flag [ field_width ] [ prec ] conversion_specifier
 * flag := '0' ; the value should be zero padded (ignored when 'prec' is specified)
 * 		 / '-' ; the value is left-justified
 * 		 / ' ' ; the value should be padded whith spaces (ignored when 'prec' is specified)
 * 		 / '+'
 *
 * field_width := 1*DIGIT
 * prec := '.' [ '-' ] *DIGIT
 * conversion_specifier := 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
 * 		 / 'e' / 'E' / 'f' / 'F' / 'g' / 'G'
 * 		 / 'c' / 's'
 * 		 / 'p'
 * */
template <typename StringT>
void safeformat<StringT>::parseFlags ()
{
	char_type v;
	while ((v = getc()) != char_type(0)) {
		if (v == char_type('0')) {
			setZeroPadding();
		} else if (v == char_type('-')) {
			setLeftJustify();
		} else if (v == char_type(' ')) {
			setSpaceBeforePositive();
		} else if (v == char_type('+')) {
			setNeedSign();
		} else {
			break;
		}
		nextc();
	}
}

template <typename StringT>
void safeformat<StringT>::parseFieldWidth ()
{
	PFS_ASSERT_FORMAT(hasMore());
	char_type v;
	const_iterator p0 = _ctx.pos;

	if (isDigitExcludeZero(getc())) {
		while (isDigit(getc())) nextc();
	}

	if (_ctx.pos > p0) {
		StringT s(p0, _ctx.pos);
	}
}

template <typename StringT>
void safeformat<StringT>::parseSpec ()
{
	char_type v;

	advance();
	PFS_ASSERT_FORMAT(hasMore());
	parseFlags();
}


} // pfs

#endif /* __PFS_BITS_SAFEFORMAT_INC_HPP__ */
