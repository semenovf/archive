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

#include "pfs/safeformat.hpp"
#include "pfs/mpl/algo/find.hpp"
#include "pfs/atomic.hpp"

namespace pfs {

atomic_int g_safeformat_compat(safeformat::compat_gcc);

template <typename T>
struct __sf_default_traits : public __sf_base_traits
{
	T _val;
	__sf_default_traits (const T & v) : __sf_base_traits(), _val(v) {}
	virtual string stringify_int (int /*base*/, bool /*uppercase*/, bool /*isUnsigned*/) const;
	virtual string stringify_float ( char /*f*/, int /*prec*/) const;
	virtual string stringify_char () const;
	virtual string stringify_string () const;
};


void safeformat::set_global_compat (compat_enum c) // [static]
{
	g_safeformat_compat.store(c);
}

int safeformat::global_compat ()
{
	return g_safeformat_compat.load();
}

/*
 * Advances until '%' or end of string.
 */
void safeformat::advance ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	while (pos < end && *pos != '%')  {
		_ctx.result.push_back(*pos);
		++pos;
	}
	_ctx.pos = pos;
}

bool safeformat::parse_percent_char ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos < end && *pos == '%')  {
		++pos;
		if (pos < end && *pos == '%') {
			_ctx.result.append(1, '%');
			++pos;
			_ctx.pos = pos;
			return true;
		}
	}
	return false;
}

bool safeformat::parse_flags ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos == end)
		return false;

	if (*pos != '%')
		return false;

	++pos;

	while (pos < end) {
		if (*pos == '0') {
			set_zero_padding();
		} else if (*pos == '-') {
			set_left_justify();
		} else if (*pos == ' ') {
			set_space_before_positive();
		} else if (*pos == '+') {
			set_need_sign();
		} else if (*pos == '#') {
			set_alternate();
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
bool safeformat::parse_field_width ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (is_digit_exclude_zero(*pos)) {
		intmax_t width = strtointmax(pos, end
				, 10
				, min_value<intmax_t>()
				, max_value<uintmax_t>()
				, & pos);

		PFS_ASSERT(!errno && width >= 0 && width <= max_value<int>()); // TODO need warning only instead of assertion

		_ctx.pos = pos;
		set_field_width(int(width));
	}
	return true;
}


/*
 * prec := '.' [ '-' ] *DIGIT
 *
 * If the precision is given as just '.', the precision is taken to be zero.
 * A negative precision is taken as if the precision were omitted.
 */
bool safeformat::parse_precision ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());
	int sign = 1;
	intmax_t prec = -1;

	if (pos < end && *pos == '.')
		++pos;

	if (pos < end && *pos == '-') {
		sign = -1;
		++pos;
	}

	if (is_digit(*pos)) {
		prec = strtointmax(pos, end
				, 10
				, min_value<intmax_t>()
				, max_value<uintmax_t>()
				, & pos);
		PFS_ASSERT(!errno && prec >= 0 && prec <= max_value<int>()); // TODO need warning only instead of assertion
	}

	if (sign > 0)
		set_precision(int(prec));

	_ctx.pos = pos;
	return true;
}


/*
 * conversion_specifier := 'd' / 'i' / 'o' / 'u' / 'x' / 'X'
 * 		 / 'e' / 'E' / 'f' / 'F' / 'g' / 'G'
 * 		 / 'c' / 's'
 * 		 / 'p'
 */
bool safeformat::parse_conv_spec ()
{
	const_iterator pos(_ctx.pos);
	const_iterator end(_ctx.format.cend());

	if (pos < end) {
		string convSpecifiers("diouxXeEfFgGcsp");

		PFS_ASSERT(convSpecifiers.contains(*pos)); // Expected conversion specifier: one of 'diouxXeEfFgGcsp';
		set_conv_specifier(char_type(*pos));
		++pos;
	}

	_ctx.pos = pos;
	return true;
}

/*
 * conversion_specification := '%' *flag [ field_width ] [ prec ] conversion_specifier
 */
bool safeformat::parse_spec ()
{
	if (!parse_percent_char()) {
		if (parse_flags()
				&& parse_field_width()
				&& parse_precision()
				&& parse_conv_spec())
		{ ; }
	}

	if (_ctx.spec.spec_char != char_type(char(0))) {
		if (_ctx.spec.flags & safeformat::ZeroPadding) {

			// If the 0 and - flags both appear, the 0 flag is ignored.
			if (_ctx.spec.flags & safeformat::LeftJustify)
				_ctx.spec.flags &= ~safeformat::ZeroPadding;

			// If a precision is given with a numeric conversion (d, i, o, u, x, and X), the 0 flag is ignored.
			if (_ctx.spec.prec > -1
					&& (   _ctx.spec.spec_char == 'd'
						|| _ctx.spec.spec_char == 'i'
						|| _ctx.spec.spec_char == 'o'
						|| _ctx.spec.spec_char == 'u'
						|| _ctx.spec.spec_char == 'x'
						|| _ctx.spec.spec_char == 'X')) {
				_ctx.spec.flags &= ~safeformat::ZeroPadding;
			}

			// '0' flag used with ‘%c’ or '%s' specifier in format string
			if (_ctx.spec.spec_char == 'c'
					|| _ctx.spec.spec_char == 's') {
				_ctx.spec.flags &= ~safeformat::ZeroPadding;
			}

		}

		// A + overrides a space if both are used
		if ((_ctx.spec.flags & safeformat::NeedSign)
				&& (_ctx.spec.flags & safeformat::SpaceBeforePositive))
			_ctx.spec.flags &= ~safeformat::SpaceBeforePositive;

		// '+' flag used with '%c' or '%s' specifier in format string
		if ((_ctx.spec.flags & safeformat::NeedSign)
				&& (_ctx.spec.spec_char == 'c'
					|| _ctx.spec.spec_char == 's')) {
			_ctx.spec.flags &= ~safeformat::NeedSign;
		}


		return true;
	}
	return false;
}

void safeformat::prepend_sign (string & r)
{
	bool isNegative = r.starts_with(_u8("-"));

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

void safeformat::do_padding (string & r)
{
	PFS_ASSERT_RANGE(_ctx.spec.width >= 0);

	if (r.length() < size_t(_ctx.spec.width)) {
		size_t count = size_t(_ctx.spec.width) - r.length();
		char paddingChar = (_ctx.spec.flags & safeformat::ZeroPadding) ? '0' : ' ';

		if (_ctx.spec.flags & safeformat::LeftJustify)
			r.append(string(count, paddingChar));
		else
			r.prepend(count, paddingChar);
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

safeformat & safeformat::operator () (string::value_type c)
{
	__sf_default_traits<string::value_type> t(c);
	return arg(& t);
}

safeformat & safeformat::operator () (const string & s)
{
	__sf_default_traits<string> t(s);
	return arg(& t);
}

safeformat & safeformat::operator () (void const * p)
{
	__sf_default_traits<void const *> t(p);
	return arg(& t);
}

//{{{ asInteger () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((unsigned int)_val, base, uppercase)
		    : to_string(_val, base, uppercase);
}

template <>
inline string __sf_default_traits<signed char>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((unsigned int)_val, base, uppercase)
		    : to_string(_val, base, uppercase);
}

template <>
inline string __sf_default_traits<unsigned char>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string(_val, base, uppercase)
		    : to_string((int)_val, base, uppercase);
}

template <>
inline string __sf_default_traits<short>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((unsigned int)_val, base, uppercase)
		    : to_string(_val, base, uppercase);
}

template <>
inline string __sf_default_traits<unsigned short>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string(_val, base, uppercase)
		    : to_string((int)_val, base, uppercase);
}

template <>
inline string __sf_default_traits<int>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((unsigned int)_val, base, uppercase)
		    : to_string(_val, base, uppercase);
}

template <>
inline string __sf_default_traits<unsigned int>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string(_val, base, uppercase)
		    : to_string((int)_val, base, uppercase);
}

template <>
inline string __sf_default_traits<long>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((unsigned long)_val, base, uppercase)
		    : to_string((long)_val, base, uppercase);
}

template <>
inline string __sf_default_traits<unsigned long>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string(_val, base, uppercase)
		    : to_string((long)_val, base, uppercase);
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((unsigned long long)_val, base, uppercase)
		    : to_string(_val, base, uppercase);
}

template <>
inline string __sf_default_traits<unsigned long long>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string(_val, base, uppercase)
		    : to_string((long long)_val, base, uppercase);
}
#endif // PFS_HAVE_LONGLONG

template <>
inline string __sf_default_traits<float>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((uintmax_t)_val, base, uppercase)
		    : to_string((intmax_t)_val, base, uppercase);
}

template <>
inline string __sf_default_traits<double>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((uintmax_t)_val, base, uppercase)
		    : to_string((intmax_t)_val, base, uppercase);
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::stringify_int (int base, bool uppercase, bool isUnsigned) const
{
	return isUnsigned
			? to_string((uintmax_t)_val, base, uppercase)
		    : to_string((intmax_t)_val, base, uppercase);
}
#endif

template <>
inline string __sf_default_traits<string::value_type>::stringify_int (int base, bool uppercase, bool /*isUnsigned*/) const
{
	return to_string(lexical_cast<int32_t>(_val), base, uppercase);
}

template <>
inline string __sf_default_traits<string>::stringify_int (int base, bool uppercase, bool is_unsigned) const
{
	return to_string(
			is_unsigned
				? lexical_cast<uintmax_t>(_val)
				: lexical_cast<intmax_t>(_val)
			, base
			, uppercase);
}

template <>
inline string __sf_default_traits<void const *>::stringify_int (int base, bool uppercase, bool /*isUnsigned*/) const
{
	return to_string(ptrdiff_t(_val), base, uppercase);
}

//}}} asInteger () -------------------------------------------------


//{{{ asChar () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::stringify_char () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<signed char>::stringify_char () const
{
	return string(1, string::value_type(char(_val)));
}

template <>
inline string __sf_default_traits<unsigned char>::stringify_char () const
{
	return string(1, string::value_type(char(_val)));
}

template <>
inline string __sf_default_traits<short>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

template <>
inline string __sf_default_traits<unsigned short>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

template <>
inline string __sf_default_traits<int>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

template <>
inline string __sf_default_traits<unsigned int>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

template <>
inline string __sf_default_traits<long>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

template <>
inline string __sf_default_traits<unsigned long>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::stringify_char () const
{
	return string(1, string::value_type(_val));
}

template <>
inline string __sf_default_traits<unsigned long long>::stringify_char () const
{
	return string(1, string::value_type(_val));
}
#endif

template <>
inline string __sf_default_traits<float>::stringify_char () const
{
	return string(1, string::value_type(uint32_t(_val)));
}

template <>
inline string __sf_default_traits<double>::stringify_char () const
{
	return string(1, string::value_type(uint32_t(_val)));
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::stringify_char () const
{
	return string(1, string::value_type(uint32_t(_val)));
}
#endif

template <>
inline string __sf_default_traits<string::value_type>::stringify_char () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<string>::stringify_char () const
{
	return string(1
		, _val.length() > 0
			? _val.at(0)
			: string::value_type('?'));
}

template <>
inline string __sf_default_traits<void const *>::stringify_char () const
{
	return string(1, string::value_type('?'));
}
//}}} asChar () -------------------------------------------------

//{{{ asFloat () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::stringify_float (char f, int prec) const
{
	return to_string(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<signed char>::stringify_float (char f, int prec) const
{
	return to_string(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned char>::stringify_float (char f, int prec) const
{
	return to_string(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<short>::stringify_float (char f, int prec) const
{
	return to_string(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned short>::stringify_float (char f, int prec) const
{
	return to_string(float(_val), f, prec);
}

template <>
inline string __sf_default_traits<int>::stringify_float (char f, int prec) const
{
	return to_string(double(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned int>::stringify_float (char f, int prec) const
{
	return to_string(double(_val), f, prec);
}

template <>
inline string __sf_default_traits<long>::stringify_float (char f, int prec) const
{
	return to_string(double(_val), f, prec);
}

template <>
inline string __sf_default_traits<unsigned long>::stringify_float (char f, int prec) const
{
	return to_string(double(_val), f, prec);
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::stringify_float (char f, int prec) const
{
#ifdef PFS_HAVE_LONG_DOUBLE
	return to_string((long double)_val, f, prec);
#else
	return to_string((double)_val, f, prec);
#endif
}

template <>
inline string __sf_default_traits<unsigned long long>::stringify_float (char f, int prec) const
{
#ifdef PFS_HAVE_LONG_DOUBLE
	return to_string((long double)_val, f, prec);
#else
	return to_string((double)_val, f, prec);
#endif
}
#endif

template <>
inline string __sf_default_traits<float>::stringify_float (char f, int prec) const
{
	return to_string(_val, f, prec);
}

template <>
inline string __sf_default_traits<double>::stringify_float (char f, int prec) const
{
	return to_string(_val, f, prec);
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::stringify_float (char f, int prec) const
{
	return to_string(_val, f, prec);
}
#endif

template <>
inline string __sf_default_traits<string::value_type>::stringify_float (char f, int prec) const
{
	return to_string(real_t(_val.value), f, prec);
}

template <>
inline string __sf_default_traits<string>::stringify_float (char f, int prec) const
{
	real_t n = lexical_cast<real_t>(_val);
	return to_string(n, f, prec);
}

template <>
inline string __sf_default_traits<void const *>::stringify_float (char f, int prec) const
{
	return to_string(real_t(ptrdiff_t(_val)), f, prec);
}
//}}} asFloat () -------------------------------------------------

//{{{ asString () -------------------------------------------------
template <>
inline string __sf_default_traits<char>::stringify_string () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<signed char>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<unsigned char>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<short>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<unsigned short>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<int>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<unsigned int>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<long>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<unsigned long>::stringify_string () const
{
	return to_string(_val);
}

#ifdef PFS_HAVE_LONGLONG
template <>
inline string __sf_default_traits<long long>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<unsigned long long>::stringify_string () const
{
	return to_string(_val);
}
#endif

template <>
inline string __sf_default_traits<float>::stringify_string () const
{
	return to_string(_val);
}

template <>
inline string __sf_default_traits<double>::stringify_string () const
{
	return to_string(_val);
}

#ifdef PFS_HAVE_LONG_DOUBLE
template <>
inline string __sf_default_traits<long double>::stringify_string () const
{
	return to_string(_val);
}
#endif

template <>
inline string __sf_default_traits<string::value_type>::stringify_string () const
{
	return string(1, _val);
}

template <>
inline string __sf_default_traits<string>::stringify_string () const
{
	return _val;
}

template <>
inline string __sf_default_traits<void const *>::stringify_string () const
{
	return to_string(ptrdiff_t(_val));
}

//}}} asString () -------------------------------------------------


safeformat & safeformat::arg (__sf_base_traits const * v)
{
	clear_spec();
	advance();

	if (!parse_spec())
		return *this;

	char_type spec_char(_ctx.spec.spec_char);
	string r;

	switch (lexical_cast<char>(spec_char)) {
	case 'd':
	case 'i':
		r = v->stringify_int(10, false, false);
		prepend_sign(r);
		do_padding(r);
		break;
	case 'o':
		r = v->stringify_int(8, false, true);
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		do_padding(r);
		break;
	case 'u':
		r = v->stringify_int(10, false, true);
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		do_padding(r);
		break;
	case 'x':
		r = v->stringify_int(16, false, true);
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		do_padding(r);
		break;
	case 'X':
		r = v->stringify_int(16, true, true); // uppercase
		// prependSign(r); // ISO specifies the '+' and ' ' only for signed conversions
		do_padding(r);
		break;
	case 'p':
		if (_ctx.compat == compat_msc) {
			set_zero_padding();
			set_field_width(sizeof(void *) * 2); // two hex chars per byte
			set_right_justify();
			r = v->stringify_int(16, true, true);  // in upper case
			do_padding(r);
		} else { // default is compat_gcc
			r = v->stringify_int(16, false, true);
			r.prepend(_u8("0x"));
			prepend_sign(r); // ISO specifies the '+' and ' ' only for signed conversions
			do_padding(r);
		}
		break;
	case 'e':
	case 'f':
	case 'g':
	case 'E':
	case 'F':
	case 'G':
		r = v->stringify_float(lexical_cast<char>(_ctx.spec.spec_char)
				, _ctx.spec.prec > 0 ? _ctx.spec.prec : 0);
		prepend_sign(r);
		do_padding(r);
		break;
	case 'c':
		r = v->stringify_char();
		do_padding(r);
		break;
	case 's':
		r = v->stringify_string();
		do_padding(r);
		break;
	default: {
            string m("invalid conversion specifier '");
            m.append(1, lexical_cast<char>(spec_char));
            m.append("' in format string \"");
            m.append(_ctx.format);
            m.append("\"");
            PFS_ASSERT_X(0, m.c_str());
            break;
        }
	}

	_ctx.result.append(r);
	advance();
	return *this;
}

} // pfs
