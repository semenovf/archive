/*
 * strtolong.hpp
 *
 *  Created on: Jan 14, 2015
 *      Author: wladt
 */

#ifndef __PFS_BITS_STRTOLONG_HPP__
#define __PFS_BITS_STRTOLONG_HPP__

#include <pfs/string.h>
#include <pfs/ctype.hpp>
#include <cerrno>

namespace pfs {

/**
 * @brief Converts the initial part of the string bitween
 *        @c pos and @c end (excluding) to an @c ulong_t value
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
ulong_t strtoulong (Iter begin, Iter end, int radix, Iter * endref = nullptr)
{
	typedef CharT char_type;
	Iter pos = begin;
	ulong_t r = ulong_t(0);
	int sign = 1;

	errno = 0;

	if (radix < 0 || radix == 1 || radix > 36) {
		if (endref) {
			*endref = pos;
		}
		//errno = ERANGE;
		return ulong_t(0);
	}

	if (pos < end) {

		// Skip whitespaces
		while (pos < end  && pfs::is_space(*pos)) {
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

		while (pos != end) {
			int digit = pfs_latin1_to_digit(*pos);

			// non-digit
			if (digit < 0)
				break;

			// non-digit in specified radix
			if (digit >= radix)
				break;

			if (r > (PFS_ULONG_MAX - digit)/radix || r * radix > PFS_ULONG_MAX - digit) {
				errno = ERANGE;
				r = PFS_ULONG_MAX;
			} else {
				r = r * radix + digit;
			}
			++pos;
		}

		if (sign < 0)
			r = - r;
	}

	if (endref)
		*endref = pos;
	return r;
}


/**
 * @brief The strtolong() function converts the initial part of the string bitween
 *        @c pos and @c end (excluding) a @c long_t value according to the given
 *        @c base, which must be between 2 and 36 inclusive, or be the special value 0.
 *
 * @details The strtolong() function returns the result of the conversion,
 *          unless the value would underflow or overflow.
 *          If an underflow occurs, strtolong() returns @c min.
 *          If an overflow occurs, strtolong() returns @c max.
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
long_t strtolong (Iter begin, Iter end, int radix, Iter * endref = nullptr)
{
	typedef CharT char_type;

	Iter pos = begin;
	ulong_t r = ulong_t(0);
	Iter endr;

	// Skip whitespaces
	while (pos < end  && pfs::is_space(*pos)) {
		++pos;
	}

	r = strtoulong<CharT, Iter>(pos, end, radix, & endr);

	if (endref) {
		if (pos == endr) {
			*endref = begin;
		} else {
			*endref = endr;
		}
	}

	if (*pos == char_type('-')) {
		if (long_t(r) > 0) {
			errno = ERANGE;
			return PFS_LONG_MIN;
		}
	} else {
		if (long_t(r) < 0) {
			errno = ERANGE;
			return PFS_LONG_MAX;
		}
	}

	return r;
}

} // pfs

#endif /* __PFS_BITS_STRTOLONG_HPP__ */
