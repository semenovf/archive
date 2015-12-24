/*
 * integral.hpp
 *
 *  Created on: Nov 20, 2015
 *      Author: wladt
 */

#include "pfs/string.hpp"

namespace pfs {

//template <>
//inline bool __is_space<char> (char v)
//{
//	return std::isspace(v) ? true : false;
//}
//
//template <>
//inline bool __is_space<unsigned char> (unsigned char v)
//{
//	return std::isspace(static_cast<int>(v)) ? true : false;
//}
//
//template <>
//inline bool __eq_latin1<char> (char c1, char c2)
//{
//	return c1 == c2;
//}
//
//template <>
//inline bool __eq_latin1<unsigned char> (unsigned char c1, char c2)
//{
//	return c1 == static_cast<unsigned char>(c2);
//}
//
//template <>
//inline int __to_digit<char> (char c, int radix)
//{
//	if (c >= '0' && c <= '9')
//		return c - '0';
//	else if (c >= 'A' && c <= 'Z')
//		return c - 'A' + 10;
//	else if (c >= 'a' && c <= 'z')
//		return c - 'a' + 10;
//
//	return -1;
//}
//
//template <>
//inline int __to_digit<unsigned char> (unsigned char c, int radix)
//{
//	return __to_digit(static_cast<char>(c), radix);
//}

/**
 * @brief Converts the initial part of the string bitween
 *        @c pos and @c end (excluding) to an @c uintmax_t value
 *        according to the given base, which must be
 *        between 2 and 36 inclusive, or be the special value 0.
 *
 * @details The strtoulong() function returns either the result of the conversion or,
 *          if there was a leading minus sign, the negation of the result
 *          of the conversion represented as an unsigned value,
 *          unless the original (nonnegated) value would overflow;
 *          in the latter case, strtoulong()  returns @c max and
 *          sets errno to ERANGE.
 *
 * @param pos
 * @param end
 * @param base
 * @param endref
 * @param max
 * @return
 */
uintmax_t strtouintmax (string::const_iterator begin
		, string::const_iterator end
		, int radix
		, uintmax_t max_value
		, string::const_iterator * endref)
{
	string::const_iterator pos = begin;
	uintmax_t r = uintmax_t(0);
	int sign = 1;

	errno = 0;

	if (radix < 0 || radix == 1 || radix > 36) {
		if (endref) {
			*endref = pos;
		}
		return 0;
	}

	if (pos < end) {

		// Skip whitespaces
		//
		while (pos < end  && __is_space(*pos)) {
			++pos;
		}

		// Sign
		//
		if (__eq_latin1(*pos, '-')) {
			sign = -1;
			++pos;
		} else if (__eq_latin1(*pos, '+')) {
			++pos;
		}

		// Autodetect base
		if (!radix) {
			radix = 10;

			if (__eq_latin1(*pos, '0')) {
				radix = 8;
				++pos;

				if (pos < end
						&& (__eq_latin1(*pos, 'x')
								|| __eq_latin1(*pos, 'X'))) {
					radix = 16;
					++pos;
				}
			} else if (__eq_latin1(*pos, 'b')) {
				radix = 2;
				++pos;
			}
		}

		// Fix for case when treats negative numbers
		//
		if (sign < 0 && max_value < pfs::max_type<uintmax_t>())
		    ++max_value;

		while (pos != end) {
			int digit;

			if (!__to_digit(*pos, radix, digit))
				break;

			if (r > (max_value - digit)/radix || r * radix > max_value - digit) {
				errno = ERANGE;
				r = pfs::max_type<uintmax_t>();
			} else {
				r = r * radix + digit;
			}

			++pos;
		}

		if (sign < 0)

// Disable warning C4146: unary minus operator applied to unsigned type, result still unsigned
// TODO Need portable solution
#ifdef PFS_CC_MSVC
#	pragma warning(push)
#	pragma warning(disable:4146)
#endif
			r = - r;
#ifdef PFS_CC_MSVC
#	pragma warning(pop)
#endif
	}

	if (endref)
		*endref = pos;
	return r;
}

/**
 * @brief The strtointegral() function converts the initial part of the string bitween
 *        @c pos and @c end (excluding) a @c intmax_t value according to the given
 *        @c base, which must be between 2 and 36 inclusive, or be the special value 0.
 *
 * @details The strtointegral() function returns the result of the conversion,
 *          unless the value would underflow or overflow.
 *          If an underflow occurs, strtointegral() returns @c min.
 *          If an overflow occurs, strtointegral() returns @c max.
 *          In both cases, @c errno is set to ERANGE.
 *
 * @param pos
 * @param end
 * @param base
 * @param endref
 * @param min
 * @param max
 * @return
 */
intmax_t strtointmax (string::const_iterator begin
		, string::const_iterator end
		, int radix
		, intmax_t min_value
		, uintmax_t max_value
		, string::const_iterator * endref)
{
	uintmax_t r = 0;
	string::const_iterator pos = begin;
	string::const_iterator endr(begin); // fixing MSVC 2010 error C2512: 'pfs::mbcs_string_ptr<_CodeUnitT,Holder>::mbcs_string_ptr' : no appropriate default constructor available

	// Skip whitespaces
	//
	while (pos < end  && __is_space(*pos))
		++pos;

	r = strtouintmax(pos, end, radix, max_value, & endr);

	if (endref) {
		if (pos == endr) {
			*endref = begin;
		} else {
			*endref = endr;
		}
	}

	if (__eq_latin1(*pos, '-')) {
		if (static_cast<intmax_t>(r) > 0) {
			errno = ERANGE;
			return pfs::min_type<intmax_t>();
		}
	} else {
		if (static_cast<intmax_t>(r) < 0) {
			errno = ERANGE;
			return pfs::max_type<intmax_t>();
		}
	}

	if (static_cast<intmax_t>(r) < min_value) {
		errno = ERANGE;
		return pfs::min_type<intmax_t>();
	}

	return static_cast<intmax_t>(r);
}

} // pfs
