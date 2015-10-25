/*
 * strtointegral.hpp
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_STRTOINTEGRAL_HPP__
#define __PFS_BITS_STRTOINTEGRAL_HPP__

#include <pfs/ctype.hpp>
#include <cerrno>

EXTERN_C_BEGIN

DLL_API int    pfs_latin1_to_digit (char latin1);

EXTERN_C_END

namespace pfs {

template <typename CharT>
inline char portable_cast_char (CharT ch)
{
	return char(uint32_t(ch));
}

/**
 * @brief Converts the initial part of the string bitween
 *        @c pos and @c end (excluding) to an @c uintegral_t value
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
template <typename CharT, typename Iter>
uintegral_t strtouintegral (Iter begin, Iter end, int radix, uintegral_t max_value, Iter * endref = nullptr)
{
	typedef CharT char_type;
	Iter pos = begin;
	uintegral_t r = uintegral_t(0);
	int sign = 1;

	errno = 0;

	if (radix < 0 || radix == 1 || radix > 36) {
		if (endref) {
			*endref = pos;
		}
		//errno = ERANGE;
		return uintegral_t(0);
	}

	if (pos < end) {

		// Skip whitespaces
		while (pos < end  && pfs::is_space(portable_cast_char<CharT>(*pos))) {
			++pos;
		}

		// Sign
		if (*pos == char_type('-')) {
			sign = -1;
			++pos;
		} else if (*pos == char_type('+')) {
			++pos;
		}

		// Autodetect base
		if (!radix) {
			radix = 10;

			if (*pos == char_type('0')) {
				radix = 8;
				++pos;

				if (pos < end
						&& (*pos == char_type('x')
								|| *pos == char_type('X'))) {
					radix = 16;
					++pos;
				}
			} else if (*pos == char_type('b')) {
				radix = 2;
				++pos;
			}
		}


		//
		// Fix for case when treats negative numbers
		//
		if (sign < 0 && max_value < pfs::max_type<uintegral_t>())
		    ++max_value;

		while (pos != end) {
			if (!pfs::is_latin1(portable_cast_char<CharT>(*pos)))
				break;

			int digit = pfs_latin1_to_digit(portable_cast_char<CharT>(*pos)); // portable for char and ucchar types

			// non-digit
			if (digit < 0)
				break;

			// non-digit in specified radix
			if (digit >= radix)
				break;

			if (r > (max_value - digit)/radix || r * radix > max_value - digit) {
				errno = ERANGE;
				r = pfs::max_type<uintegral_t>();
			} else {
				r = r * radix + digit;
			}
			++pos;
		}

		if (sign < 0)

// Disable warning C4146: unary minus operator applied to unsigned type, result still unsigned
// TODO need portable solution
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
 *        @c pos and @c end (excluding) a @c integral_t value according to the given
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
template <typename CharT, typename Iter>
integral_t strtointegral (Iter begin, Iter end, int radix, integral_t min_value, uintegral_t max_value, Iter * endref = nullptr)
{
	typedef CharT char_type;

	Iter pos = begin;
	uintegral_t r = 0;
	Iter endr(begin); // fixing MSVC 2010 error C2512: 'pfs::mbcs_string_ptr<_CodeUnitT,Holder>::mbcs_string_ptr' : no appropriate default constructor available

	// Skip whitespaces
	while (pos < end  && pfs::is_space(char(uint32_t(CharT(*pos))))) {
		++pos;
	}

	r = strtouintegral<CharT, Iter>(pos, end, radix, max_value, & endr);

	if (endref) {
		if (pos == endr) {
			*endref = begin;
		} else {
			*endref = endr;
		}
	}

	if (*pos == char_type('-')) {
		if (integral_t(r) > 0) {
			errno = ERANGE;
			return pfs::min_type<integral_t>();
		}
	} else {
		if (integral_t(r) < 0) {
			errno = ERANGE;
			return pfs::max_type<integral_t>();
		}
	}

	if (integral_t(r) < min_value) {
		errno = ERANGE;
		return pfs::min_type<integral_t>();
	}

	return integral_t(r);
}


template <typename CharT, typename IterT>
integral_t strtointegral_helper (IterT begin, IterT end, bool * ok, int base, integral_t min_value, uintegral_t max_value)
{
	IterT endptr(begin);

	integral_t r = pfs::strtointegral<CharT, IterT>(begin, end, base, min_value, max_value, & endptr);

	if (ok)
		*ok = false;

    if ((errno == ERANGE && (r == pfs::max_type<integral_t>() || r == pfs::min_type<integral_t>()))
            || (errno != 0 && r == 0)) {
    	r = 0; // error
    } else {
    	if (ok)
    		*ok = true;
    }

	return r;
}

template <typename CharT, typename IterT>
uintegral_t strtouintegral_helper (IterT begin, IterT end, bool * ok, int base, uintegral_t max_value)
{
	IterT endptr(begin);

	uintegral_t r = pfs::strtouintegral<CharT, IterT>(begin, end, base, max_value, & endptr);

	if (ok)
		*ok = false;

    if ((errno == ERANGE && (r == pfs::max_type<uintegral_t>()))
            || (errno != 0 && r == 0)) {
    	r = 0; // error
    } else {
    	if (ok)
    		*ok = true;
    }

	return r;
}


} // pfs

#endif /* __PFS_BITS_STRTOINTEGRAL_HPP__ */
