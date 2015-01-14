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

namespace pfs {

template <typename CharT, typename Iter>
long_t strtolong (Iter pos, Iter end, int base, Iter * endref = nullptr, long_t min = PFS_LONG_MIN, long_t max = PFS_LONG_MAX)
{
	typedef CharT char_type;
	long_t r = long_t(0);
	int sign = 1;

	PFS_ASSERT_RANGE(base == 0 || (base >= 2 && base <= 36));

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
		if (!base) {
			base = 10;

			if (*pos == char_type('0')) {
				base = 8;
				++pos;

				if (pos < end
						&& (*pos == char_type('x')
								|| *pos == char_type('X'))) {
					base = 16;
					++pos;
				}
			} else if (*pos == char_type('b')) {
				base = 2;
				++pos;
			}
		}

		while (pos != end) {
			int digit = pfs_latin1_to_digit(*pos);

			// non-digit
			if (digit < 0)
				break;

			// non-digit in specified radix
			if (digit >= base)
				break;

			r = r * base + digit;
			++pos;
		}

		if (sign < 0)
			r = - r;
	}

	if (endref)
		endref = pos;
	return r;
}

}

#endif /* __PFS_BITS_STRTOLONG_HPP__ */
