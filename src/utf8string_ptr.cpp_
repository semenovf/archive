/**
 * @file utf8string_ptr.cpp
 * @author wladt
 * @date Nov 22, 2013
 */


#include "../include/pfs/bits/utf8string_ptr.hpp"

namespace pfs {

utf8string_ptr::value_type utf8string_ptr::value () const
{
	uint32_t uc = ucchar::Null;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition
	int n = ucchar::decodeUtf8(_p, 6, uc, min_uc);

	if (n > 0) {
		if (ucchar::isValid(uc, min_uc)) {
			return uc;
		}
	}

	return ucchar::MaxCodePoint;
}

void utf8string_ptr::jump_forward (int n)
{
	while(n-- > 0) {
		byte_t ch = byte_t(_p[0]);
		if (ch < 128) {
			++_p;
		} else if ((ch & 0xE0) == 0xC0) {
			_p += 2;
		} else if ((ch & 0xF0) == 0xE0) {
			_p += 3;
		} else if ((ch & 0xF8) == 0xF0) {
			_p += 4;
		} else if ((ch & 0xFC) == 0xF8) {
			_p += 5;
		} else if ((ch & 0xFE) == 0xFC) {
			_p += 6;
		} else {
			++_p; // invalid;
		}
	}
}

void utf8string_ptr::jump_backward (int n)
{
	while(n-- > 0) {
		if ((*(_p - 1) & 0x80) == 0x00) {
			--_p;
		} else if ((*(_p - 2) & 0xE0) == 0xC0) {
			_p -= 2;
		} else if ((*(_p - 3) & 0xF0) == 0xE0) {
			_p -= 3;
		} else if ((*(_p - 4) & 0xF8) == 0xF0) {
			_p -= 4;
		} else if ((*(_p - 5) & 0xFC) == 0xF8) {
			_p -= 5;
		} else if ((*(_p - 6) & 0xFE) == 0xFC) {
			_p -= 6;
		} else {
			--_p; // invalid
		}
	}
}

utf8string_ptr::difference_type operator - (const utf8string_ptr & p1, const utf8string_ptr & p2)
{
	utf8string_ptr::difference_type n = 0;
	int sign = 1;

	if (p1 < p2)
		sign = -1;

	const utf8string_ptr limit(PFS_MAX(p1, p2));
	utf8string_ptr p(PFS_MIN(p1, p2));

	while (p < limit) {
		p.jump_forward(1);
		++n;
	}

	return n * sign;
}


} // pfs

