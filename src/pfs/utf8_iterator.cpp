/*
 * utf8_iterator.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: wladt
 */

#include "pfs/utf8_iterator.hpp"

namespace pfs {

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

//inline uint32_t construct_codepoint (const uint8_t * p
//        , size_t n
//        , uint32_t initial
//        , bool & isInvalid)
//{
//    uint32_t r = initial;
//
//    isInvalid = false;
//
//    for (size_t i = 1; i < n; ++i) {
//        if ((p[i] & 0xC0) == 0x80) {
//            r = (r << 6) | (p[i] & 0x3F);
//        } else {
//            isInvalid = true;
//            break;
//        }
//    }
//
//    return r;
//}

static void advance_forward (const char *  & p
		, utf8_iterator::difference_type n)
{
    while (n--) {
    	uint8_t b = *reinterpret_cast<const uint8_t * &>(p);

        if (b < 128) {
            ++p;
        } else if ((b & 0xE0) == 0xC0) {
            p += 2;
        } else if ((b & 0xF0) == 0xE0) {
        	p += 3;
        } else if ((b & 0xF8) == 0xF0) {
        	p += 4;
        } else if ((b & 0xFC) == 0xF8) {
        	p += 5;
        } else if ((b & 0xFE) == 0xFC) {
        	p += 6;
        } else { // Invalid char
            ++p;
        }
    }
}

inline void advance_backward (const char *  & p
		, utf8_iterator::difference_type n)
{

}


/**
 * @note For internal use only.
 *       Used for forward movement through valid (trusted)
 *       UTF8-encoded sequence.
 *
 * @param begin
 * @param pch
 * @return
 */
void advance (const char *  & p
		, utf8_iterator::difference_type n)
{
	if (n > 0) {
		advance_forward(p, n);
	} else if (n < 0) {
		advance_backward(p, -n);
	}
}

} // pfs

