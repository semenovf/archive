/*
 * encode.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_ENCODE_HPP__
#define __PFS_UTF8_ENCODE_HPP__

/* UTF-8
 *
 * Bits Last code point/    Byte 1      Byte 2      Byte 3      Byte 4      Byte 5      Byte 6
 *  7   U+007F              0xxxxxxx
 *  11  U+07FF              110xxxxx    10xxxxxx
 *  16  U+FFFF              1110xxxx    10xxxxxx    10xxxxxx
 *  21  U+1FFFFF            11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  26  U+3FFFFFF           111110xx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 *  31  U+7FFFFFFF          1111110x    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
 */

#include <iterator>
#include <pfs.hpp>

namespace pfs { namespace utf8 {

template <class OutputOctetIterator>
OutputOctetIterator encode (uint32_t uc, OutputOctetIterator result)
{
    if (uc < 0x80) {
        *result++ = uint8_t(uc);
    } else if (uc < 0x0800) {
    	*result++ = 0xC0 | uint8_t(uc >> 6);
    	*result++ = 0x80 | uint8_t(uc & 0x3f);
    } else if (uc < 0x10000) {
    	*result++ = 0xE0 | uint8_t(uc >> 12);
    	*result++ = 0x80 | (uint8_t(uc >> 6)  & 0x3F);
    	*result++ = 0x80 | uint8_t(uc & 0x3F);
    } else if (uc < 0x200000) {
    	*result++ = 0xF0 | uint8_t(uc >> 18);
    	*result++ = 0x80 | (uint8_t(uc >> 12) & 0x3F);
    	*result++ = 0x80 | (uint8_t(uc >> 6)  & 0x3F);
    	*result++ = 0x80 | uint8_t(uc & 0x3F);
    } else if (uc < 0x4000000) {
    	*result++ = 0xF8 | uint8_t(uc >> 24);
    	*result++ = 0x80 | (uint8_t(uc >> 18) & 0x3F);
    	*result++ = 0x80 | (uint8_t(uc >> 12) & 0x3F);
    	*result++ = 0x80 | (uint8_t(uc >> 6)  & 0x3F);
    	*result++ = 0x80 | uint8_t(uc & 0x3F);
    } else if (uc < 0x80000000) {
    	*result++ = 0xFC | uint8_t(uc >> 30);
    	*result++ = 0x80 | (uint8_t(uc >> 24) & 0x3F);
    	*result++ = 0x80 | (uint8_t(uc >> 18) & 0x3F);
    	*result++ = 0x80 | (uint8_t(uc >> 12) & 0x3F);
    	*result++ = 0x80 | (uint8_t(uc >> 6)  & 0x3F);
    	*result++ = 0x80 | uint8_t(uc & 0x3F);
    }

    return result;
}

}} // pfs::utf8

#endif /* __PFS_UTF8_ENCODE_HPP__ */
