/**
 * @file uccharptr.cpp
 * @author wladt
 * @date Dec 3, 2013
 */

#include "../include/pfs/bits/uccharptr.hpp"
#include "utf8string_p.hpp"

namespace pfs {

ptrdiff_t incrementCodePoints (
		  ptrdiff_t codePointsCount
		, size_t codeUnitsSize
		, const char * begin
		, ptrdiff_t codeUnitsOffset)
{
	std::string::const_pointer end    = begin + codeUnitsSize;
	std::string::const_pointer first  = begin + codeUnitsOffset;
	std::string::const_pointer p      = first;

	// n is positive, see first line
	while (codePointsCount > 0 && p < begin) {
		++p;
		--codePointsCount;
	}

	while (codePointsCount > 0 && p < end) {
		byte_t ch = byte_t(p[0]);

		if (ch < 128) {
			++p;
		} else if ((ch & 0xE0) == 0xC0) {
			p += 2;
		} else if ((ch & 0xF0) == 0xE0) {
			p += 3;
		} else if ((ch & 0xF8) == 0xF0) {
			p += 4;
		} else if ((ch & 0xFC) == 0xF8) {
			p += 5;
		} else if ((ch & 0xFE) == 0xFC) {
			p += 6;
		} else {
			PFS_ASSERT(0); // invalid data inside utf8 string
		}

		--codePointsCount;
	}

	while (codePointsCount > 0) {
		++p;
		--codePointsCount;
	}

	return p - first;
}

ptrdiff_t decrementCodePoints (
		  ptrdiff_t codePointsCount
		, size_t codeUnitsSize
		, const char * begin
		, ptrdiff_t codeUnitsOffset)
{
	std::string::const_pointer end    = begin + codeUnitsSize;
	std::string::const_pointer first  = begin + codeUnitsOffset;
	std::string::const_pointer p      = first;

	// n is positive, see first line
	while (codePointsCount > 0 && p > end) {
		--p;
		--codePointsCount;
	}

	while(codePointsCount > 0 && p > begin) {
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
			PFS_ASSERT(0); // invalid data inside utf8 string
		}
		--codePointsCount;
	}

	while (codePointsCount > 0) {
		--p;
		--codePointsCount;
	}

	return p - first;
}


ucchar_ref & ucchar_ref::operator = (ucchar c)
{
	size_t sz1 = _owner->_pimpl->size();
	PFS_ASSERT(_pos >= 0 && size_t(_pos) < sz1);

	_owner->detach();

	utf8string::impl::const_pointer p1 = _owner->_pimpl->data();
	size_t nremain1 = sz1 - _pos;
	ucchar c1;
	int n1 = c1.decodeUtf8(p1 + _pos, nremain1);

	PFS_ASSERT(n1 > 0);

	char p2[6];
	size_t n2 = c.encodeUtf8(p2);

	_owner->_pimpl->replace(_pos, n1, p2, 0, n2);

	// length not changed

	return *this;
}

template class ucchar_ref_basic<const utf8string>;
template class ucchar_ref_basic<utf8string>;
template class ucchar_ptr_basic<utf8string, ucchar_ref_basic<utf8string> >;
template class ucchar_ptr_basic<const utf8string, ucchar_ref_basic<const utf8string> >;

/*
template ucchar_ptr_basic<utf8string, ucchar_ptr>::difference_type
ucchar_ptr_basic<utf8string, ucchar_ptr>::difference (const ucchar_ptr_basic<utf8string, ucchar_ptr> & it2) const;

template ucchar_ptr_basic<utf8string, ucchar_ptr>::difference_type
ucchar_ptr_basic<utf8string, ucchar_ptr>::difference (const ucchar_ptr_basic<const utf8string, ucchar_const_ptr> & it2) const;

template ucchar_ptr_basic<const utf8string, ucchar_const_ptr>::difference_type
ucchar_ptr_basic<const utf8string, ucchar_const_ptr>::difference (const ucchar_ptr_basic<utf8string, ucchar_ptr> & it2) const;

template ucchar_ptr_basic<const utf8string, ucchar_const_ptr>::difference_type
ucchar_ptr_basic<const utf8string, ucchar_const_ptr>::difference (const ucchar_ptr_basic<const utf8string, ucchar_const_ptr> & it2) const;
*/

} // pfs
