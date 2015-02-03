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

#include <pfs/safeformat.hpp>
#include <pfs/bits/strtolong.hpp>

#include <iostream>

namespace pfs {

/*
 * Advances until '%' or end of string.
 */
void safeformat::advance ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	while (pos < end && !eq_latin1(char_type(*pos), '%'))  {
		_ctx.result.append(1, *pos);
		++pos;
	}
	_ctx.pos = pos;
}

bool safeformat::parsePercentChar ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos < end && eq_latin1(char_type(*pos), '%'))  {
		++pos;
		if (pos < end && eq_latin1(char_type(*pos), '%')) {
			_ctx.result.append(1, '%');
			++pos;
			_ctx.pos = pos;
			return true;
		}
	}
	return false;
}

bool safeformat::parseFlags ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos == end)
		return false;

	if (! eq_latin1(char_type(*pos), '%'))
		return false;

	++pos;

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
bool safeformat::parseFieldWidth ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (isDigitExcludeZero(char_type(*pos))) {
		long_t width = strtolong<ucchar, const_iterator>(pos, end, 10, & pos);

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
bool safeformat::parsePrecision ()
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
		long_t prec = strtolong<ucchar, const_iterator>(pos, end, 10, & pos);
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
bool safeformat::parseConvSpec ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos < end) {
		string convSpecifiers("diouxXeEfFgGcsp");

		std::cout << char(char_type(*pos)) << std::endl;
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
bool safeformat::parseSpec ()
{
	clearSpec();
	advance();
	if (!parsePercentChar()) {
		if (parseFlags()
				&& parseFieldWidth()
				&& parsePrecision()
				&& parseConvSpec())
		{ ; }
	}
	advance();

	if (_ctx.spec.spec_char != char_type(0)) {
		if (_ctx.spec.flags & safeformat::ZeroPadding) {

			// If the 0 and - flags both appear, the 0 flag is ignored.
			if (_ctx.spec.flags & safeformat::LeftJustify)
				_ctx.spec.flags &= ~safeformat::ZeroPadding;

			// If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
			if (_ctx.spec.prec > -1
					&& (   _ctx.spec.spec_char == pfs::ucchar('d')
						|| _ctx.spec.spec_char == pfs::ucchar('i')
						|| _ctx.spec.spec_char == pfs::ucchar('o')
						|| _ctx.spec.spec_char == pfs::ucchar('u')
						|| _ctx.spec.spec_char == pfs::ucchar('x')
						|| _ctx.spec.spec_char == pfs::ucchar('X'))) {
				_ctx.spec.flags &= ~safeformat::ZeroPadding;
			}

			// '0' flag used with ‘%c’ or '%s' specifier in format string
			if (_ctx.spec.spec_char == pfs::ucchar('c')
					|| _ctx.spec.spec_char == pfs::ucchar('s')) {
				_ctx.spec.flags &= ~safeformat::ZeroPadding;
			}

		}

		// A + overrides a space if both are used
		if ((_ctx.spec.flags & safeformat::NeedSign)
				&& (_ctx.spec.flags & safeformat::SpaceBeforePositive))
			_ctx.spec.flags &= ~safeformat::SpaceBeforePositive;

		// '+' flag used with '%c' or '%s' specifier in format string
		if ((_ctx.spec.flags & safeformat::NeedSign)
				&& (_ctx.spec.spec_char == pfs::ucchar('c')
					|| _ctx.spec.spec_char == pfs::ucchar('s'))) {
			_ctx.spec.flags &= ~safeformat::NeedSign;
		}


		return true;
	}
	return false;
}

void safeformat::prependSign (string & r)
{
	bool isNegative = r.startsWith("-");

	// When 0 is printed with an explicit precision 0, the output is empty.
	if (_ctx.spec.prec == 0 && r == string("0"))
		r.clear();

	// The precision, if any, gives the minimum number of digits that must appear;
	// if the converted value requires fewer digits, it is padded on the left with zeros.
	if (_ctx.spec.prec > 0 && r.length() < size_t(_ctx.spec.prec))
		r.prepend(string(_ctx.spec.prec - r.length(), '0'));

	if (! isNegative) {
		// A sign (+ or -) should always be placed before a number produced by a signed conversion
		if (_ctx.spec.flags & safeformat::NeedSign) {
			r.prepend(1, '+');
		}
		// A blank should be left before a positive number
		else if (_ctx.spec.flags & safeformat::SpaceBeforePositive) {
			r.prepend(1, ' ');
		}
	}
}

void safeformat::doPadding (string & r)
{
	PFS_ASSERT_RANGE(_ctx.spec.width >= 0);
	if (r.length() < size_t(_ctx.spec.width)) {
		size_t count = size_t(_ctx.spec.width) - r.length();
		char paddingChar = (_ctx.spec.flags & safeformat::ZeroPadding) ? '0' : ' ';

		if (_ctx.spec.flags & safeformat::LeftJustify)
			r.append(pfs::string(count, paddingChar));
		else
			r.prepend(pfs::string(count, paddingChar));
	}
}


//
// isUnsigned is applicable to string only
//
static string asInteger (const safeformat::variant_type & v, int base, bool uppercase, bool isUnsigned)
{
	string r;
	if (v.is<long_t>()) {
		long_t n = v.get<long_t>();
		r = string::toString(n, base, uppercase);
	} else if (v.is<ulong_t>()) {
		r = string::toString(v.get<ulong_t>(), base, uppercase);
	} else if (v.is<real_t>()) {
		r = string::toString(long_t(v.get<real_t>()), base, uppercase);
	} else if (v.is<ucchar>()) {
		r = string::toString(ulong_t(v.get<ucchar>().value()), base, uppercase);
	} else if (v.is<pfs::string>()) {
		if (isUnsigned) {
			ulong_t n = v.get<pfs::string>()
#ifdef PFS_HAVE_LONGLONG
					.toULongLong(nullptr, 10);
#else
					.toULong(nullptr, 10);
#endif
			r = string::toString(n, base, uppercase);
		} else {
			long_t n = v.get<pfs::string>()
#ifdef PFS_HAVE_LONGLONG
					.toLongLong(nullptr, 10);
#else
					.toLong(nullptr, 10);
#endif
			r = string::toString(n, base, uppercase);
		}
	}

	return r;
}

static string asFloat (const safeformat::variant_type & v, char f, int prec)
{
	string r;

	if (v.is<long_t>()) {
		r = string::toString(real_t(v.get<long_t>()), f, prec);
	} else if (v.is<ulong_t>()) {
		r = string::toString(real_t(v.get<ulong_t>()), f, prec);
	} else if (v.is<real_t>()) {
		r = string::toString(v.get<real_t>(), f, prec);
	} else if (v.is<ucchar>()) {
		r = string::toString(real_t(v.get<ucchar>().value()), f, prec);
	} else if (v.is<pfs::string>()) {
		real_t n = v.get<pfs::string>().toReal();
		r = string::toString(n, f, prec);
	}

	return r;
}

static string asChar (const safeformat::variant_type & v)
{
	string r;

	if (v.is<long_t>()) {
		r = string(1, ucchar(int(v.get<long_t>())));
	} else if (v.is<ulong_t>()) {
		r = string(1, ucchar(int(v.get<ulong_t>())));
	} else if (v.is<real_t>()) {
		r = string(1, ucchar(int(v.get<real_t>())));
	} else if (v.is<ucchar>()) {
		r = string(1, v.get<ucchar>());
	} else if (v.is<pfs::string>()) {
		string s = v.get<pfs::string>();
		r = string(1
			, s.length() > 0
				? s.charAt(0)
				: ucchar::ReplacementChar);
	}
	return r;
}

static string asString (const safeformat::variant_type & v)
{
	string r;

	if (v.is<long_t>()) {
		r = string::toString(v.get<long_t>());
	} else if (v.is<ulong_t>()) {
		r = string::toString(v.get<ulong_t>());
	} else if (v.is<real_t>()) {
		r = string::toString(v.get<real_t>());
	} else if (v.is<ucchar>()) {
		r = string(1, v.get<ucchar>());
	} else if (v.is<pfs::string>()) {
		r = v.get<pfs::string>();
	}

	return r;
}


safeformat & safeformat::operator () (const variant_type & v)
{
	if (!parseSpec())
		return *this;

	char_type spec_char(_ctx.spec.spec_char);
	PFS_ASSERT(is_latin1(spec_char));
	string r;

	switch (char(spec_char)) {
	case 'd':
	case 'i':
		r = asInteger(v, 10, false, false);
		prependSign(r);
		doPadding(r);
		break;
	case 'o':
		r = asInteger(v, 8, false, true);
		prependSign(r);
		doPadding(r);
		break;
	case 'u':
		r = asInteger(v, 10, false, true);
		prependSign(r);
		doPadding(r);
		break;
	case 'x':
	case 'p':
		r = asInteger(v, 16, false, true);
		prependSign(r);
		doPadding(r);
		break;
	case 'X':
		r = asInteger(v, 16, true, true); // uppercase
		prependSign(r);
		doPadding(r);
		break;
	case 'e':
	case 'f':
	case 'g':
	case 'E':
	case 'F':
	case 'G':
		r = asFloat(v, char(_ctx.spec.spec_char)
				, _ctx.spec.prec > 0 ? _ctx.spec.prec : 0);
		prependSign(r);
		doPadding(r);
		break;
	case 'c':
		r = asChar(v);
		doPadding(r);
		break;
	case 's':
		r = asString(v);
		doPadding(r);
		break;
	default:
		PFS_ASSERT(spec_char != spec_char); // TODO need exception: invalid conversion specifier
		break;
	}

	_ctx.result.append(r);
	return *this;
}

} // pfs
