/*
 * unicode.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_UNICODE_HPP__
#define __PFS_UNICODE_HPP__

#include <cctype>
#include <pfs.hpp>

namespace pfs { namespace unicode {

typedef uint32_t char_type;

enum {
	  max_code_point           = 0x0010ffff
	, null_code_point          = 0x00000000
	, default_replacement_char = 0x0000FFFD
	, max_bmp                  = 0x0000FFFF
	, bom_charstatic           = 0x0000FEFF
	, hi_surrogate_start       = 0x0000D800
	, hi_surrogate_end         = 0x0000DBFF
	, low_surrogate_start      = 0x0000DC00
	, low_surrogate_end        = 0x0000DFFF
};

/**
 * @brief Checks if unicode code point belongs
 *        to Low surrogate range [DC00-DFFF] (used for utf-16 character encoding).
 * @param c Unicode code point.
 * @return @c true if @c c belongs to Hi surrogate range.
 */
inline bool is_low_surrogate (char_type c)
{
	return (c & 0xfffffc00) == 0xdc00;
}

/**
 * @brief Checks if unicode code point belongs
 *        to Hi surrogate range [D800-DBFF] (used for utf-16 character encoding).
 * @param c Unicode code point.
 * @return @c true if @c c belongs to Hi surrogate range.
 */
inline bool is_hi_surrogate (char_type c)
{
	return (c & 0xfffffc00) == 0xd800;
}

/**
 * @brief Checks if unicode code point belongs
 *        to Hi or Low surrogate ranges (used for utf-16 character encoding).
 * @param c Unicode code point.
 * @return @c true if @c c belongs to Hi or Low surrogate ranges.
 */
inline bool is_surrogate (char_type c)
{
	return (c - hi_surrogate_start < 2048u);
}

} // unicode

typedef uint32_t unicode_char_t;

} // pfs

#endif /* __PFS_UNICODE_HPP__ */
