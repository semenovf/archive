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
	uint32_t r = (w1 & 0x03FF) << 10;
	r |= w2 & 0x03FF;
	r += 0x10000;
	return r;
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
 * 				- *pinvalid is undefined
 * 				- *pch stores valid unicode char
 *
 * 		   If need to request more units (one in this case)
 * 		   to recognized UTF16-encoded char:
 * 				- return @c begin pointer
 * 				- *pnremain stores number of requested units (one in this case)
 * 				- *pinvalid undefined
 * 				- *pch unmodified
 *
 * 		   If code unit sequence is invalid:
 * 				- return @c begin pointer incremented by one (skipped one code unit)
 * 				- *pnremain stores zero
 * 				- *pinvalid incremented by 1
 * 				- *pch unmodified
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
		// Invalid unit, skip it
		isInvalid = true;
		++p;
	}

	if (nremain) {
		if (pnremain)
			*pnremain = nremain;
	} else {
		if (pch && !isInvalid) {
			*pch = ucchar(construct_codepoint(w1, w2));
		}
	}

	if (pinvalid && isInvalid)
		++*pinvalid;

	return p;
}

bool can_backward_utf16_char (const uint16_t * begin, const uint16_t * end)
{
	const uint16_t * p = end;
	if (p > begin) {
		--p;

		if (*p < 0xD800 || *p > 0xDFFF)
			return true;
		if (*p >= 0xD800 && *p <= 0xDBFF)
			return (p + 1 < end) ? true : false;
	}
	return false;

	if (len > 0) {
		if (*p < 0xD800 || *p > 0xDFFF)
			return true;
		if (*p >= 0xD800 && *p <= 0xDBFF)
			return len > 1 ? true : false;
	}
	return false;
}


const uint16_t * backward_utf16_char (
		  const uint16_t * p
		, size_t * invalid
		, ucchar * pch)
{
	bool isInvalid = false;
	uint16_t w1 = *(p - 1);
	uint16_t w2 = 0;

	if (w1 < 0xD800 || w1 > 0xDFFF) {
		--p;
	} else {
		w1 = *(p - 2);
		uint16_t w2 = *(p - 1);

		if (w1 >= 0xD800 && w1 <= 0xDBFF
				&& w2 >= 0xDC00 && w2 <= 0xDFFF) {
			p -= 2;
		} else {
			--p;
			isInvalid = true;
		}
	}

	if (pch) {
		if (!isInvalid) {
			*pch = ucchar(construct_codepoint(w1, w2));
		} else {
			pch->invalidate();
		}
	}

	if (invalid && isInvalid)
		++ *invalid;

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
size_t ucchar::encodeUtf16 (uint16_t * s, size_t sz)
{
	if (_value < 0x10000) {
		PFS_ASSERT(sz >= 1);
		*s = _value;
		return 1;
	}

	PFS_ASSERT(sz >= 2);

	uint32_t u1 = _value - 0x10000;
	uint16_t w1 = 0xD800 | (u1 >> 10);
	uint16_t w2 = 0xDC00 | (u1 & 0x03FF);
	*s = w1;
	++*s = w2;

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
int ucchar::decodeUtf16 (const uint16_t * units, size_t len)
{
	const uint16_t * p = advance_utf16_char(units, nullptr, this);
	int r = p - units;
	PFS_ASSERT(r < 0);
	return size_t(r) <= len && this->isValid() ? r : -1;
}


} // pfs
