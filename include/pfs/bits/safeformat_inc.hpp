/*
 * safeformat_inc.hpp
 *
 *  Created on: Jan 13, 2015
 *      Author: wladt
 */

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

/*
 * flag := '0' ; the value should be zero padded (ignored when 'prec' is specified)
 * 	 / '-' ; the value is left-justified
 * 	 / ' ' ; the value should be padded whith spaces (ignored when 'prec' is specified)
 * 	 / '+'
 */

#ifndef __PFS_BITS_SAFEFORMAT_INC_HPP__
#define __PFS_BITS_SAFEFORMAT_INC_HPP__

#include <pfs/bits/strtolong.hpp>

namespace pfs {

/*
 * Advances until '%' or end of string.
 */
template <typename StringT>
void safeformat<StringT>::advance ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	while (pos < end && !eq_latin1(char_type(*pos), '%'))  {
		_ctx.result.append(char_type(*pos));
		++pos;
	}
	_ctx.pos = pos;
}

template <typename StringT>
inline bool safeformat<StringT>::parsePercentChar ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos < end && eq_latin1(char_type(*pos), '%'))  {
		++pos;
		if (pos < end && eq_latin1(char_type(*pos), '%')) {
			_ctx.result.append(char_type('%'));
			_ctx.pos = ++pos;
			return true;
		}
	}
	return false;
}

template <typename StringT>
bool safeformat<StringT>::parseFlags ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos == end)
		return false;

	if (! eq_latin1(char_type(*pos), '%'))
		return false;

	while (pos < end) {
		if (eq_latin1(char_type(*pos), '0')) {
			setZeroPadding();
		} else if (eq_latin1(char_type(*pos), '-')) {
			setLeftJustify();
		} else if (eq_latin1(char_type(*pos), ' ')) {
			setSpaceBeforePositive();
		} else if (eq_latin1(char_type(*pos), '+')) {
			setNeedSign();
		} else if (eq_latin1(char_type(*pos), '#')) {
			setAlternate();
		} else {
			break;
		}
		++pos;
	}

	return true;
}

/*
 *  field_width := 1*DIGIT
 */
template <typename StringT>
bool safeformat<StringT>::parseFieldWidth ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (isDigitExcludeZero(char_type(*pos))) {
		long_t width = strtolong (pos, end, 10, & pos);

		PFS_ASSERT(!errno && width >= 0 && width <= PFS_INT_MAX); // TODO need warning only instead of assertion

		_ctx.pos = pos;
		setFieldWidth(int(width));
	}
	return true;
}


/*
 * prec := '.' [ '-' ] *DIGIT
 *
 * If the precision is given as just '.', the precision is taken to be zero.
 * A negative precision is taken as if the precision were omitted.
 */
template <typename StringT>
bool safeformat<StringT>::parsePrecision ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());
	int sign = 1;
	long_t prec = 0;

	if (pos < end && eq_latin1(char_type(*pos), '.'))
		++pos;

	if (pos < end && eq_latin1(char_type(*pos), '-')) {
		sign = -1;
		++pos;
	}

	if (isDigit(char_type(*pos))) {
		long_t prec = strtolong (pos, end, 10, & pos);
		PFS_ASSERT(!errno && prec >= 0 && prec <= PFS_INT_MAX); // TODO need warning only instead of assertion
	}

	if (sign > 0)
		setPrecision(int(prec));

	_ctx.pos = pos;
	return true;
}


/*
 * conversion_specifier := 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
 * 		 / 'e' / 'E' / 'f' / 'F' / 'g' / 'G'
 * 		 / 'c' / 's'
 * 		 / 'p'
 */
template <typename StringT>
bool safeformat<StringT>::parseConvSpec ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos < end) {
		StringT convSpecifiers("diouxXeEfFgGcsp");

		PFS_ASSERT_X(convSpecifiers.contains(char_type(*pos))
				, _Tr("Expected conversion specifier: one of 'diouxXeEfFgGcsp'"));
		setConvSpecifier(char_type(*pos));
		++pos;
	}

	_ctx.pos = pos;
	return true;
}

/*
 * conversion_specification := '%' *flag [ field_width ] [ prec ] conversion_specifier
 */
template <typename StringT>
bool safeformat<StringT>::parseSpec ()
{
	clearSpec();
	advance();
	if (!parsePercentChar()) {
		if (parseFlags()
				&& parseFieldWidth()
				&& parsePrecision()
				&& parseConvSpec())
			;
	}
	advance();
	return _ctx.spec.spec_char == char_type(0);
}


template <typename StringT>
safeformat<StringT> & safeformat<StringT>::operator () (char c)
{
	if (!parseSpec())
		return *this;

	char_type spec_char(_ctx.spec.spec_char);
	PFS_ASSERT(is_latin1(spec_char));
	StringT r;

	//pfs::unitype ut = ctx->bind_args[ctx->argi++];

	switch (char(spec_char)) {
	case 'd':
	case 'i':
		break;
	case 'o':
		break;
	case 'u':
		break;
	case 'x':
	case 'p':
		break;
	case 'X':
		break;
	case 'e':
	case 'f':
	case 'g':
	case 'E':
	case 'F':
	case 'G':
		break;
	case 'c':
		break;
	case 's':
		break;
	default:
		PFS_ASSERT(spec_char != spec_char); // TODO need exception: invalid conversion specifier
		break;
	}
	return *this;
}

#ifdef __COMMENT__
template <typename StringT>
safeformat<StringT> & safeformat<StringT>::operator () (char c)
{
	parseSpec();

	char_type spec_char(_ctx.spec.spec_char);

	// Error, conversion specifiers too few.
	if (spec_char == char_type(0)) {
		return *this;
	}

	PFS_ASSERT(is_latin1(spec_char));
	StringT r;

	//pfs::unitype ut = ctx->bind_args[ctx->argi++];

	switch (char(spec_char)) {
	case 'd':
	case 'i':
		r.setNumber(cast_to<long_int>(c), 10);
		break;

	case 'o':
			r.setNumber(ulong_t(ut.toInteger()), 8);
			break;
		case 'u':
			r.setNumber(ulong_t(ut.toInteger()), 10);
			expect = __EXPECT_DECIMAL;
			break;
		case 'x':
		case 'p':
			r.setNumber(ulong_t(ut.toInteger()), 16, false);
			expect = __EXPECT_DECIMAL;
			break;
		case 'X':
			r.setNumber(ulong_t(ut.toInteger()), 16, true); // uppercase
			expect = __EXPECT_DECIMAL;
			break;
		case 'e':
		case 'f':
		case 'g':
		case 'E':
		case 'F':
		case 'G':
			r.setNumber(ut.toFloat(), char(ctx->spec.spec_char), ctx->spec.prec > 0 ? ctx->spec.prec : 0);
			expect = __EXPECT_FLOAT;
			break;
		case 'c':
			r = pfs::string(1, char(ut.toInteger()));
			expect = __EXPECT_CHAR;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::ZeroPadding)
					, _Tr("'0' flag used with ‘%c’ specifier in format string:"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::ZeroPadding;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::NeedSign)
					, _Tr("'+' flag used with ‘%c’ specifier in format string:"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::NeedSign;
			break;

		case 's':
			r = ut.toString();
			expect = __EXPECT_STRING;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::ZeroPadding)
					, _Tr("'0' flag used with ‘%s’ specifier in format string"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::ZeroPadding;

			ok = PFS_VERIFY_X(
				PFS_VERIFY_X(!(ctx->spec.flags & safeformat::NeedSign)
					, _Tr("'0' flag used with ‘%s’ specifier in format string"))
				, ctx->format.c_str());

			if (!ok)
				ctx->spec.flags &= ~safeformat::NeedSign;
			break;
		default:
			break;
		}

		PFS_ASSERT(expect != __EXPECT_UNKNOWN);

//		if (false) {
//			PFS_WARN(_Tr("Incompatible value for conversion specification in format string:"));
//			PFS_WARN(ctx->format.c_str());
//		} else {

			if (ctx->spec.flags & safeformat::ZeroPadding) {

				// If the 0 and - flags both appear, the 0 flag is ignored.
				if (ctx->spec.flags & safeformat::LeftJustify)
					ctx->spec.flags &= ~safeformat::ZeroPadding;

				// If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
				if (ctx->spec.prec > -1
						&& (  ctx->spec.spec_char == pfs::ucchar('d')
							|| ctx->spec.spec_char == pfs::ucchar('i')
							|| ctx->spec.spec_char == pfs::ucchar('o')
							|| ctx->spec.spec_char == pfs::ucchar('u')
							|| ctx->spec.spec_char == pfs::ucchar('x')
							|| ctx->spec.spec_char == pfs::ucchar('X')
							))
					ctx->spec.flags &= ~safeformat::ZeroPadding;
			}

			// A + overrides a space if both are used
			if ((ctx->spec.flags & safeformat::NeedSign)
					&& (ctx->spec.flags & safeformat::SpaceBeforePositive))
				ctx->spec.flags &= ~safeformat::SpaceBeforePositive;

			if (expect == __EXPECT_DECIMAL || expect == __EXPECT_FLOAT) {
				bool isNegative = r.startsWith("-");

				// When 0 is printed with an explicit precision 0, the output is empty.
				if (ctx->spec.prec == 0 && r == pfs::string("0"))
					r.clear();

				// The precision, if any, gives the minimum number of digits that must appear;
				// if the converted value requires fewer digits, it is padded on the left with zeros.
				if (ctx->spec.prec > 0 && r.length() < size_t(ctx->spec.prec))
					r.prepend(pfs::string(ctx->spec.prec - r.length(), '0'));

				if (! isNegative) {
					// A sign (+ or -) should always be placed before a number produced by a signed conversion
					if (ctx->spec.flags & safeformat::NeedSign) {
						r.prepend(pfs::string("+"));
					}
					// A blank should be left before a positive number
					else if (ctx->spec.flags & safeformat::SpaceBeforePositive) {
						r.prepend(pfs::string(" "));
					}
				}
			}

			// If the converted value has fewer characters than the field width, it will be padded with spaces
			// on the left (or right, if the left-adjustment flag has been given).
			PFS_ASSERT(ctx->spec.width >= 0);
			if (r.length() < size_t(ctx->spec.width)) {
				size_t count = size_t(ctx->spec.width) - r.length();
				char paddingChar = (ctx->spec.flags & safeformat::ZeroPadding) ? '0' : ' ';

				if (ctx->spec.flags & safeformat::LeftJustify)
					r.append(pfs::string(count, paddingChar));
				else
					r.prepend(pfs::string(count, paddingChar));
			}
//		}
		ctx->result.append(r);

	return true;
}
#endif

} // pfs

#endif /* __PFS_BITS_SAFEFORMAT_INC_HPP__ */
