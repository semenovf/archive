/*
 * utf16.cpp
 *
 *  Created on: 18 апр. 2015 г.
 *      Author: wladt
 */

#include "pfs/ucchar.hpp"

namespace pfs {

inline uint32_t construct_codepoint (uint32_t w1, uint32_t w2)
{
    if (w2) {
        uint32_t r = (w1 & 0x03FF) << 10;
        r |= w2 & 0x03FF;
        r += 0x10000;
        return r;
    }

    return w1;
}

//2.2 Decoding UTF-16
// 1) If W1 < 0xD800 or W1 > 0xDFFF, the character value U is the value
// 	  of W1. Terminate.
//
// 2) Determine if W1 is between 0xD800 and 0xDBFF. If not, the sequence
//    is in error and no valid character can be obtained using W1.
//    Terminate.
//
// 3) If there is no W2 (that is, the sequence ends with W1), or if W2
//    is not between 0xDC00 and 0xDFFF, the sequence is in error.
//    Terminate.
//
// 4) Construct a 20-bit unsigned integer U', taking the 10 low-order
//    bits of W1 as its 10 high-order bits and the 10 low-order bits of
//    W2 as its 10 low-order bits.
//
// 5) Add 0x10000 to U' to obtain the character value U. Terminate.
//

/**
 * @note For internal use only. Used for parse untrusted source
 *       as UTF16-encoded sequence.
 *
 * @param begin Pointer to first code unit.
 * @param end Pointer to after last code unit.
 * @param pnremain Pointer to store number of requested code units.
 * @param pinvalid Pointer to store incremented number of invalid chars.
 * @param pch Pointer store resulting unicode char.
 * 		Recommended to pass replacement char in *pch.
 * @return If successfully recognized UTF16-encoded char:
 * 				- return @c begin pointer incremented by valid code units
 * 				- *pnremain stores zero
 * 				- *pinvalid unmodified
 * 				- *pch stores valid unicode char
 *
 * 		   If need to request more units (one in this case)
 * 		   to recognized UTF16-encoded char:
 * 				- return @c begin pointer
 * 				- *pnremain stores number of requested units (one in this case)
 * 				- *pinvalid unmodified
 * 				- *pch stores invalid unicode char
 *
 * 		   If code unit sequence is invalid:
 * 				- return @c begin pointer incremented by one (skipped one code unit)
 * 				- *pnremain stores zero
 * 				- *pinvalid incremented by 1
 * 				- *pch stores invalid unicode char
 */
const uint16_t * advance_utf16_char (
		  const uint16_t * begin
		, const uint16_t * end
		, size_t * pnremain
		, size_t * pinvalid
		, ucchar * pch)
{
	PFS_ASSERT(begin < end);
	const uint16_t * p = begin;
	bool isInvalid = false;
	size_t nremain = 0; // number of requested units

	uint16_t w1 = *p;
	uint16_t w2 = 0;

	if (w1 < 0xD800 || w1 > 0xDFFF) {
		// Valid unit
		++p;
	} else if (w1 >= 0xD800 && w1 <= 0xDBFF) {
		if (p + 1 < end) {
			w2 = *(p + 1);
			if (w2 >= 0xDC00 && w2 <= 0xDFFF) {
				// Valid unit sequence
				p += 2;
			} else {
				// Invalid unit, skip it
				isInvalid = true;
				++p;
			}
		} else {
			// need more units
			nremain = 1;
			p = begin;
		}
	} else {
		isInvalid = true;
	}

	if (!isInvalid) {
        if (nremain) {
            if (pnremain)
                *pnremain = nremain;
            if (pch)
                pch->invalidate();
        } else {
            if (pch && !isInvalid) {
                *pch = ucchar(construct_codepoint(w1, w2));
            }
        }
	} else {
        ++p;

	    if (pinvalid)
	        ++*pinvalid;

        if (pch)
            pch->invalidate();
	}

	return p;
}


#define ASSERT_INVALID_UTF16 PFS_ASSERT_X(false, "invalid (untrusted) UTF16-encoded sequence")

/**
 * @note For internal use only.
 *       Used for forward movement through valid (trusted)
 *       UTF16-encoded sequence.
 *
 * @param begin
 * @param pch
 * @return
 */
const uint16_t * advance_utf16_char (
          const uint16_t * begin
        , ucchar * pch)
{
    uint16_t w1 = 0;
    uint16_t w2 = 0;
    const uint16_t * p = begin;

    if (*p < 0xD800 || *p > 0xDFFF) {
        w1 = *p;
        ++p;
    } else if (*p >= 0xD800 && *p <= 0xDBFF
            && *(p + 1) >= 0xDC00 && *(p + 1) <= 0xDFFF) {
        w1 = *p;
        w2 = *(p + 1);
        p += 2;
    } else {
        ASSERT_INVALID_UTF16;
    }

    if (pch)
        *pch = ucchar(construct_codepoint(w1, w2));

    return p;
}


/**
 * @note For internal use only.
 *       Used for backward movement through valid (trusted)
 *       UTF16-encoded sequence.
 *
 * @param rbegin
 * @param pch
 * @return
 */
const uint16_t * backward_utf16_char (
          const uint16_t * rbegin
        , ucchar * pch)
{
    uint16_t w1 = 0;
    uint16_t w2 = 0;
    const uint16_t * p = rbegin - 1;

    if (*p < 0xD800 || *p > 0xDFFF) {
        w1 = *p;
    } else if (*(p - 1) >= 0xD800 && *(p - 1) <= 0xDBFF
            && *p >= 0xDC00 && *p <= 0xDFFF) {
        w1 = *(p - 1);
        w2 = *p;
        --p;
    } else {
        ASSERT_INVALID_UTF16;
    }

    if (pch)
        *pch = ucchar(construct_codepoint(w1, w2));

    return p;
}

//
// Source: https://www.ietf.org/rfc/rfc2781.txt
//
//2.1 Encoding UTF-16
//
//   Encoding of a single character from an ISO 10646 character value to
//   UTF-16 proceeds as follows. Let U be the character number, no greater
//   than 0x10FFFF.
//
//   1) If U < 0x10000, encode U as a 16-bit unsigned integer and
//      terminate.
//
//   2) Let U' = U - 0x10000. Because U is less than or equal to 0x10FFFF,
//      U' must be less than or equal to 0xFFFFF. That is, U' can be
//      represented in 20 bits.
//
//   3) Initialize two 16-bit unsigned integers, W1 and W2, to 0xD800 and
//      0xDC00, respectively. These integers each have 10 bits free to
//      encode the character value, for a total of 20 bits.
//
//   4) Assign the 10 high-order bits of the 20-bit U' to the 10 low-order
//      bits of W1 and the 10 low-order bits of U' to the 10 low-order
//      bits of W2. Terminate.
//
//   Graphically, steps 2 through 4 look like:
//   U' = yyyyyyyyyyxxxxxxxxxx
//   W1 = 110110yyyyyyyyyy
//   W2 = 110111xxxxxxxxxx
//
size_t ucchar::encodeUtf16 (uint16_t * buffer, size_t sz)
{
    uint16_t * p = buffer;

	if (_value < 0x10000) {
		PFS_ASSERT_X(sz >= 1, "buffer must be at least 1 unit size");
		*p = _value;
		return 1;
	}

	PFS_ASSERT_X(sz >= 2, "buffer must be at least 2 units size");

	uint32_t u1 = _value - 0x10000;
	uint16_t w1 = 0xD800 | (u1 >> 10);
	uint16_t w2 = 0xDC00 | (u1 & 0x03FF);
	*p++ = w1;
	*p = w2;

	return 2;
}

//
// Source: https://www.ietf.org/rfc/rfc2781.txt
//
//2.2 Decoding UTF-16
//
//   Decoding of a single character from UTF-16 to an ISO 10646 character
//   value proceeds as follows. Let W1 be the next 16-bit integer in the
//   sequence of integers representing the text. Let W2 be the (eventual)
//   next integer following W1.
//
//   1) If W1 < 0xD800 or W1 > 0xDFFF, the character value U is the value
//      of W1. Terminate.
//
//   2) Determine if W1 is between 0xD800 and 0xDBFF. If not, the sequence
//      is in error and no valid character can be obtained using W1.
//      Terminate.
//
//   3) If there is no W2 (that is, the sequence ends with W1), or if W2
//      is not between 0xDC00 and 0xDFFF, the sequence is in error.
//      Terminate.
//
//   4) Construct a 20-bit unsigned integer U', taking the 10 low-order
//      bits of W1 as its 10 high-order bits and the 10 low-order bits of
//      W2 as its 10 low-order bits.
//
//   5) Add 0x10000 to U' to obtain the character value U. Terminate.
//
size_t ucchar::decodeUtf16 (const uint16_t * begin, size_t sz)
{
    const uint16_t * end = begin + sz;
    const uint16_t * p = advance_utf16_char(begin, end
            , nullptr // pnremain
            , nullptr // pinvalid
            , this);

    PFS_ASSERT(p >= begin && p <= end);
    return this->isValid() ? p - begin : 0;
}

} // pfs
