/*
 * decode.hpp
 *
 *  Created on: Nov 7, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_DECODE_HPP__
#define __PFS_UTF8_DECODE_HPP__

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

#include <pfs.hpp>

namespace pfs { namespace utf8 {

/**
 * @brief Decodes utf8 trusted octet sequence into unicode code point.
 *
 * @param begin Reference to pointer to first code unit of octet sequence.
 * @param pos   Pointer to store new position after decoding.
 * @return Unicode code point. @c *pos assigned to new position after decoding.
 */
template <typename OctetIterator>
uint32_t decode (OctetIterator & p)
{
	uint8_t b = static_cast<uint8_t>(*p);
	uint32_t result;
    int nunits = 0;

    if (b < 128) {
    	result = b;
        nunits = 1;
    } else if ((b & 0xE0) == 0xC0) {
    	result = b & 0x1F;
        nunits = 2;
    } else if ((b & 0xF0) == 0xE0) {
    	result = b & 0x0F;
    	nunits = 3;
    } else if ((b & 0xF8) == 0xF0) {
    	result = b & 0x07;
    	nunits = 4;
    } else if ((b & 0xFC) == 0xF8) {
    	result = b & 0x03;
    	nunits = 5;
    } else if ((b & 0xFE) == 0xFC) {
    	result = b & 0x01;
    	nunits = 6;
    } else {
    	// Invalid
    	PFS_ASSERT_UNEXPECTED(); // It's a trusted source after all
    }

    ++p;

    while (--nunits) {
    	b = static_cast<uint8_t>(*p);

        if ((b & 0xC0) == 0x80) {
        	result = (result << 6) | (b & 0x3F);
        } else {
        	// Invalid
        	PFS_ASSERT_UNEXPECTED(); // It's a trusted source after all
        }
    	++p;
    }

    return result;
}

}} // pfs::utf8

#endif /* __PFS_UTF8_DECODE_HPP__ */
