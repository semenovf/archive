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
#include <pfs/bits/strtointegral.hpp>
#include <pfs/atomic.hpp>

//#include <iostream>

namespace pfs {

atomic_integer<int> g_safeformatCompat(safeformat::CompatGCC);

template <typename T>
struct __sf_default_traits : public __sf_base_traits
{
	T _val;
	__sf_default_traits (const T & v) : __sf_base_traits(), _val(v) {}
	virtual string asInteger (int /*base*/, bool /*uppercase*/, bool /*isUnsigned*/) const;
	virtual string asFloat ( char /*f*/, int /*prec*/) const;
	virtual string asChar () const;
	virtual string asString () const;
};


void safeformat::setGlobalCompat (Compat c) // [static]
{
	g_safeformatCompat.store(c);
}

int safeformat::globalCompat ()
{
	return g_safeformatCompat.load();
}

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

	_ctx.pos = pos;
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
		integral_t width = strtointegral<ucchar, const_iterator>(pos, end, 10, PFS_INTEGRAL_MIN, PFS_UINTEGRAL_MAX, & pos);

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
	integral_t prec = -1;

	if (pos < end && eq_latin1(char_type(*pos), '.'))
		++pos;

	if (pos < end && eq_latin1(char_type(*pos), '-')) {
		sign = -1;
		++pos;
	}

	if (isDigit(char_type(*pos))) {
		prec = strtointegral<ucchar, const_iterator>(pos, end, 10, PFS_INTEGRAL_MIN, PFS_UINTEGRAL_MAX, & pos);
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
	if (!parsePercentChar()) {
		if (parseFlags()
				&& parseFieldWidth()
				&& parsePrecision()
				&& parseConvSpec())
		{ ; }
	}

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
			r.append(string(count, paddingChar));
		else
			r.prepend(string(count, paddingChar));
	}
}

safeformat & safeformat::operator () (char c)
{
	__sf_default_traits<char> t(c);
	return arg(& t);
}

safeformat & safeformat::operator () (signed char n)
{
	__sf_default_traits<signed char> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (unsigned char n)
{
	__sf_default_traits<unsigned char> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (short n)
{
	__sf_default_traits<short> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (unsigned short n)
{
	__sf_default_traits<unsigned short> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (int n)
{
	__sf_default_traits<int> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (unsigned int n)
{
	__sf_default_traits<unsigned int> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (long n)
{
	__sf_default_traits<long> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (unsigned long n)
{
	__sf_default_traits<unsigned long> t(n);
	return arg(& t);
}

#ifdef PFS_HAVE_LONGLONG
safeformat & safeformat::operator () (long long n)
{
	__sf_default_traits<long long> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (unsigned long long n)
{
	__sf_default_traits<unsigned long long> t(n);
	return arg(& t);
}
#endif

safeformat & safeformat::operator () (float n)
{
	__sf_default_traits<float> t(n);
	return arg(& t);
}

safeformat & safeformat::operator () (double n)
{
	__sf_default_traits<double> t(n);
	return arg(& t);
}

#ifdef PFS_HAVE_LONG_DOUBLE
safeformat & safeformat::operator () (long double n)
{
	__sf_default_traits<long double> t(n);
	return arg(& t);
}
#endif

safeformat & safeformat::operator () (ucchar c)
{
	__sf_default_traits<ucchar> t(c);
	return arg(& t);
}

safeformat & safeformat::operator () (const string & s)
{
	__sf_default_traits<string> t(s);
	return arg(& t);
}

safeformat & safeformat::operator () (void * p)
{
	__sf_default_traits<void *> t(p);
	return arg(& t);
}

//{{{ asInteger () -------------------------------------------------
template <>
string __sf_default_traits<char>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((unsigned int)_val, base, uppercase)
		    : string::toString(_val, base, uppercase);
}

template <>
string __sf_default_traits<signed char>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((unsigned int)_val, base, uppercase)
		    : string::toString(_val, base, uppercase);
}

template <>
string __sf_default_traits<unsigned char>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString(_val, base, uppercase)
		    : string::toString((int)_val, base, uppercase);
}

template <>
string __sf_default_traits<short>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((unsigned int)_val, base, uppercase)
		    : string::toString(_val, base, uppercase);
}

template <>
string __sf_default_traits<unsigned short>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString(_val, base, uppercase)
		    : string::toString((int)_val, base, uppercase);
}

template <>
string __sf_default_traits<int>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((unsigned int)_val, base, uppercase)
		    : string::toString(_val, base, uppercase);
}

template <>
string __sf_default_traits<unsigned int>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString(_val, base, uppercase)
		    : string::toString((int)_val, base, uppercase);
}

template <>
string __sf_default_traits<long>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((unsigned long)_val, base, uppercase)
		    : string::toString((long)_val, base, uppercase);
}

template <>
string __sf_default_traits<unsigned long>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString(_val, base, uppercase)
		    : string::toString((long)_val, base, uppercase);
}

#ifdef PFS_HAVE_LONGLONG
template <>
string __sf_default_traits<long long>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((unsigned long long)_val, base, uppercase)
		    : string::toString(_val, base, uppercase);
}

template <>
string __sf_default_traits<unsigned long long>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString(_val, base, uppercase)
		    : string::toString((long long)_val, base, uppercase);
}
#endif // PFS_HAVE_LONGLONG

template <>
string __sf_default_traits<float>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((uintegral_t)_val, base, uppercase)
		    : string::toString((integral_t)_val, base, uppercase);
}

template <>
string __sf_default_traits<double>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((uintegral_t)_val, base, uppercase)
		    : string::toString((integral_t)_val, base, uppercase);
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
string __sf_default_traits<long double>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? string::toString((uintegral_t)_val, base, uppercase)
		    : string::toString((integral_t)_val, base, uppercase);
}
#endif

template <>
string __sf_default_traits<ucchar>::asInteger (int base, bool uppercase, bool /*isUnsigned*/) const
{
	return string::toString(int32_t(_val), base, uppercase);
}

template <>
string __sf_default_traits<string>::asInteger (int base, bool uppercase, bool isUnsigned) const
{
	string r;

	if (isUnsigned) {
		uintegral_t n = _val.toUnsignedIntegral(nullptr, 10);
		r = string::toString(n, base, uppercase);
	} else {
		integral_t n = _val.toIntegral(nullptr, 10);
		r = string::toString(n, base, uppercase);
	}

	return r;
}

template <>
string __sf_default_traits<void *>::asInteger (int base, bool uppercase, bool /*isUnsigned*/) const
{
	return string::toString(ptrdiff_t(_val), base, uppercase);
}

//}}} asInteger () -------------------------------------------------


//{{{ asChar () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::asChar () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<signed char>::asChar () const
{
	return string(1, ucchar(char(_val)));
}

template <>
inline string __sf_default_traits<unsigned char>::asChar () const
{
	return string(1, ucchar(char(_val)));
}

template <>
inline string __sf_default_traits<short>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<unsigned short>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<int>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<unsigned int>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<long>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<unsigned long>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<unsigned long long>::asChar () const
{
	return string(1, ucchar(uint32_t(_val)));
}
#endif

template <>
inline string __sf_default_traits<float>::asChar () const
{
	return string(1, ucchar(int(_val)));
}

template <>
inline string __sf_default_traits<double>::asChar () const
{
	return string(1, ucchar(int(_val)));
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::asChar () const
{
	return string(1, ucchar(int(_val)));
}
#endif

template <>
inline string __sf_default_traits<ucchar>::asChar () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<string>::asChar () const
{
	return string(1
		, _val.length() > 0
			? _val.charAt(0)
			: ucchar::ReplacementChar);
}

template <>
inline string __sf_default_traits<void *>::asChar () const
{
	return string(1, ucchar(ucchar::ReplacementChar));
}
//}}} asChar () -------------------------------------------------

//{{{ asFloat () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::asFloat (char f, int prec) const
{
	return string::toString(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<signed char>::asFloat (char f, int prec) const
{
	return string::toString(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned char>::asFloat (char f, int prec) const
{
	return string::toString(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<short>::asFloat (char f, int prec) const
{
	return string::toString(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned short>::asFloat (char f, int prec) const
{
	return string::toString(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<int>::asFloat (char f, int prec) const
{
	return string::toString(double(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned int>::asFloat (char f, int prec) const
{
	return string::toString(double(_val), f, prec);
}

template <>
inline string __sf_default_traits<long>::asFloat (char f, int prec) const
{
	return string::toString(double(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned long>::asFloat (char f, int prec) const
{
	return string::toString(double(_val), f, prec);
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::asFloat (char f, int prec) const
{
#ifdef PFS_HAVE_LONG_DOUBLE
	return string::toString((long double)_val, f, prec);
#else
	return string::toString((double)_val, f, prec);
#endif
}

template <>
inline string __sf_default_traits<unsigned long long>::asFloat (char f, int prec) const
{
#ifdef PFS_HAVE_LONG_DOUBLE
	return string::toString((long double)_val, f, prec);
#else
	return string::toString((double)_val, f, prec);
#endif
}
#endif

template <>
inline string __sf_default_traits<float>::asFloat (char f, int prec) const
{
	return string::toString(_val, f, prec);
}

template <>
inline string __sf_default_traits<double>::asFloat (char f, int prec) const
{
	return string::toString(_val, f, prec);
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::asFloat (char f, int prec) const
{
	return string::toString(_val, f, prec);
}
#endif

template <>
inline string __sf_default_traits<ucchar>::asFloat (char f, int prec) const
{
	return string::toString(real_t(_val.value()), f, prec);
}

template <>
inline string __sf_default_traits<string>::asFloat (char f, int prec) const
{
	real_t n = _val.toReal();
	return string::toString(n, f, prec);
}

template <>
inline string __sf_default_traits<void *>::asFloat (char f, int prec) const
{
	return string::toString(real_t(ptrdiff_t(_val)), f, prec);
}
//}}} asFloat () -------------------------------------------------

//{{{ asString () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::asString () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<signed char>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<unsigned char>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<short>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<unsigned short>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<int>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<unsigned int>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<long>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<unsigned long>::asString () const
{
	return string::toString(_val);
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<unsigned long long>::asString () const
{
	return string::toString(_val);
}
#endif

template <>
inline string __sf_default_traits<float>::asString () const
{
	return string::toString(_val);
}

template <>
inline string __sf_default_traits<double>::asString () const
{
	return string::toString(_val);
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::asString () const
{
	return string::toString(_val);
}
#endif

template <>
inline string __sf_default_traits<ucchar>::asString () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<string>::asString () const
{
	return _val;
}

template <>
inline string __sf_default_traits<void *>::asString () const
{
	return string::toString(ptrdiff_t(_val));
}

//}}} asString () -------------------------------------------------


safeformat & safeformat::arg (const __sf_base_traits * v)
{
	clearSpec();
	advance();
	if (!parseSpec())
		return *this;

	char_type spec_char(_ctx.spec.spec_char);
	PFS_ASSERT(is_latin1(spec_char));
	string r;

	switch (char(spec_char)) {
	case 'd':
	case 'i':
		r = v->asInteger(10, false, false);
		prependSign(r);
		doPadding(r);
		break;
	case 'o':
		r = v->asInteger(8, false, true);
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		doPadding(r);
		break;
	case 'u':
		r = v->asInteger(10, false, true);
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		doPadding(r);
		break;
	case 'x':
		r = v->asInteger(16, false, true);
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		doPadding(r);
		break;
	case 'X':
		r = v->asInteger(16, true, true); // uppercase
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		doPadding(r);
		break;
	case 'p':
		if (_ctx.compat == CompatMSC) {
			setZeroPadding();
			setFieldWidth(sizeof(void *) * 2); // two hex chars per byte
			setRightJustify();
			r = v->asInteger(16, true, true);  // in upper case
			doPadding(r);
		} else { // default is CompatGCC
			r = v->asInteger(16, false, true);
			r.prepend("0x");
			prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
			doPadding(r);
		}
		break;
	case 'e':
	case 'f':
	case 'g':
	case 'E':
	case 'F':
	case 'G':
		r = v->asFloat(char(_ctx.spec.spec_char)
				, _ctx.spec.prec > 0 ? _ctx.spec.prec : 0);
		prependSign(r);
		doPadding(r);
		break;
	case 'c':
		r = v->asChar();
		doPadding(r);
		break;
	case 's':
		r = v->asString();
		doPadding(r);
		break;
	default:
		PFS_ASSERT(spec_char != spec_char); // TODO need exception: invalid conversion specifier
		break;
	}

	_ctx.result.append(r);
	advance();
	return *this;
}

} // pfs
