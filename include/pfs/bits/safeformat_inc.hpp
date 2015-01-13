/*
 * safeformat_inc.hpp
 *
 *  Created on: Jan 13, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_SAFEFORMAT_INC_HPP__
#define __PFS_BITS_SAFEFORMAT_INC_HPP__

namespace pfs {

template <typename StringT>
inline safeformat<StringT>::value_type safeformat<StringT>::getc ()
{
	return _context.pos != _context.format.cend() ? *_context.pos++ : value_type(0);
}

template <typename StringT>
inline void safeformat<StringT>::ungetc ()
{
	PFS_ASSERT(_context.pos > _context.format.cbegin());
	--_context.pos;
}

/**
 * @return '\0' at the end of string, '%' otherwise
 */
template <typename StringT>
void safeformat<StringT>::read_plain_chars ()
{
	value_type v;
	while ((v = getc()) != value_type(0)) {
		if (v == value_type('%')) {
			v = getc();
			if (v == value_type('%')) {
				_context.result.append(v);
			} else {
				if (v != value_type(0)) {
					ungetc();
					return value_type('%');
				} else {
					return value_type(0);
				}
			}
		} else {
			_context.result.append(v);
		}
	}
}

template <typename StringT>
inline void safeformat<StringT>::read_tail ()
{
	_context.result.append(_context.pos, _context.format.cend());
}


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
bool safeformat<StringT>::parse_spec ()
{
	value_type v = read_plain_chars();

	if (v == value_type('%')) {

		// read flag char
		v = getc();
		if (v == value_type('0')) {

		} else if (v == value_type('-')) {

		} else if (v == value_type(' ')) {

		} else if (v == value_type('+')) {

		} else {

		}
	}
}


} // pfs

#endif /* __PFS_BITS_SAFEFORMAT_INC_HPP__ */
