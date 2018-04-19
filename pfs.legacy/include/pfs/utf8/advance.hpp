/*
 * utf8_iterator.hpp
 *
 *  Created on: Oct 28, 2015
 *      Author: wladt
 */

#ifndef __PFS_UTF8_ADVANCE_HPP__
#define __PFS_UTF8_ADVANCE_HPP__

#include <pfs.hpp>

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

namespace pfs { namespace utf8 {

template <typename CodeUnitIterator>
void advance_forward (CodeUnitIterator & p, ptrdiff_t n)
{
    while (n--) {
        if ((*p & 0x80) == 0x00) {
            ++p;
        } else if ((*p & 0xE0) == 0xC0) {
            p += 2;
        } else if ((*p & 0xF0) == 0xE0) {
        	p += 3;
        } else if ((*p & 0xF8) == 0xF0) {
        	p += 4;
        } else if ((*p & 0xFC) == 0xF8) {
        	p += 5;
        } else if ((*p & 0xFE) == 0xFC) {
        	p += 6;
        } else { // Invalid char
            ++p;
        }
    }
}

template <typename CodeUnitIterator>
void advance_backward (CodeUnitIterator & p, ptrdiff_t n)
{
    while (n--) {
        if ((*(p - 1) & 0x80) == 0x00) {
            --p;
        } else if ((*(p - 2) & 0xE0) == 0xC0) {
            p -= 2;
        } else if ((*(p - 3) & 0xF0) == 0xE0) {
            p -= 3;
        } else if ((*(p - 4) & 0xF8) == 0xF0) {
            p -= 4;
        } else if ((*(p - 5) & 0xFC) == 0xF8) {
            p -= 5;
        } else if ((*(p - 6) & 0xFE) == 0xFC) {
            p -= 6;
        } else {
            --p;
        }
    }
}

}} // pfs::utf8


#endif /* __PFS_UTF8_ADVANCE_HPP__ */
