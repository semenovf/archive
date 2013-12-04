/**
 * @file uccharptr.cpp
 * @author wladt
 * @date Dec 3, 2013
 */

#include "../include/pfs/bits/uccharptr.hpp"
#include "utf8string_p.hpp"

namespace pfs {

template <typename _utf8string>
ucchar_ref_basic<_utf8string>::ucchar_ref_basic (difference_type offset, _utf8string & owner)
	: _pos(0), _owner(& owner)
{
	if (offset < 0)
		offset = owner.length() + offset;
	PFS_ASSERT(offset >= 0);

	typename _utf8string::impl::const_pointer first = _owner->_pimpl->data();
	typename _utf8string::impl::const_pointer p = first;

	while(offset-- > 0) {
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
			PFS_ASSERT(0); // invalid;
		}
	}

	_pos = p - first;
}

template <typename _utf8string>
ucchar_ref_basic<_utf8string>::operator ucchar () const
{
	PFS_ASSERT(_pos >= 0 && size_t(_pos) < _owner->_pimpl->size());
	ucchar r;
	utf8string::impl::const_pointer p = _owner->_pimpl->data();
	size_t nremain = _owner->_pimpl->size() - _pos;
	PFS_ASSERT(r.decodeUtf8(p + _pos, nremain) > 0);
	return r;
}

template ucchar_ref_basic<utf8string>::operator ucchar () const;
template ucchar_ref_basic<const utf8string>::operator ucchar () const;

ucchar_ref::ucchar_ref (difference_type offset, utf8string & owner)
	: ucchar_ref_basic<utf8string>(offset, owner)
{
}

ucchar_const_ref::ucchar_const_ref (difference_type offset, const utf8string & owner)
	: ucchar_ref_basic<const utf8string>(offset, owner)
{
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
	size_t n2 = c.encodeUtf8 (p2);

	_owner->_pimpl->replace(_pos, n1, p2, 0, n2);

	// length not changed

	return *this;
}

#ifdef __COMMENT__
ucchar_ref & ucchar_ref::operator = (const ucchar_ref & other)
{
	size_t sz1 = _owner->_pimpl->size();
	size_t sz2 = other._owner->_pimpl->size();

	PFS_ASSERT(_pos >= 0 && size_t(_pos) < sz1);
	PFS_ASSERT(other._pos >= 0 && size_t(other._pos) < sz2);

	_owner->detach();

	ucchar c1;
	ucchar c2;
	utf8string::impl::const_pointer p1 = _owner->_pimpl->data();
	utf8string::impl::const_pointer p2 = other._owner->_pimpl->data();

	size_t nremain1 = sz1 - _pos;
	size_t nremain2 = sz2 - other._pos;

	int n1 = c1.decodeUtf8(p1 + _pos, nremain1);
	int n2 = c2.decodeUtf8(p2 + other._pos, nremain2);

	PFS_ASSERT(n1 > 0 && n2 > 0);

	_owner->_pimpl->replace(_pos, n1, p2, other._pos, n2);

	return *this;
}
#endif

template <typename _utf8string, typename _Ref>
bool ucchar_ptr_basic<_utf8string, _Ref>::isOutOfBounds() const
{
	return _ref._pos < 0 || size_t(_ref._pos) >= _ref._owner->_pimpl->size();
}

template bool ucchar_ptr_basic<utf8string, ucchar_ref>::isOutOfBounds() const;
template bool ucchar_ptr_basic<const utf8string, ucchar_const_ref>::isOutOfBounds() const;


static ucchar_ptr::difference_type __increment (
		  ucchar_ptr::difference_type n
		, size_t codeUnitsCount
		, std::string::const_pointer begin
		, ucchar_ptr::difference_type codeUnitsOffset)
{
	size_t sz = codeUnitsCount;
	std::string::const_pointer end    = begin + sz;
	std::string::const_pointer first  = begin + codeUnitsOffset;
	std::string::const_pointer p      = first;

	// n is positive, see first line
	while (n > 0 && p < begin) {
		++p;
		--n;
	}

	while (n > 0 && p < end) {
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

		--n;
	}

	while (n > 0) {
		++p;
		--n;
	}

	return p - first;
}

template <typename _utf8string, typename _Ref>
ucchar_ptr_basic<_utf8string, _Ref> & ucchar_ptr_basic<_utf8string, _Ref>::operator += (difference_type n)
{
	if (n < 0)
		return this->operator -= (0 - n);
	_ref._pos += __increment(n
			, _ref._owner->_pimpl->size() // code units count
			, _ref._owner->_pimpl->data() // begin
			, _ref._pos                   // offset in bytes from begin
	);
	return *this;
}

template ucchar_ptr_basic<utf8string, ucchar_ref> &
	ucchar_ptr_basic<utf8string, ucchar_ref>::operator += (difference_type n);

template ucchar_ptr_basic<const utf8string, ucchar_const_ref> &
	ucchar_ptr_basic<const utf8string, ucchar_const_ref>::operator += (difference_type n);


static ucchar_ptr::difference_type __decrement (
		  ucchar_ptr::difference_type n
		, size_t codeUnitsCount
		, std::string::const_pointer begin
		, ucchar_ptr::difference_type codeUnitsOffset)
{
	std::string::const_pointer end    = begin + codeUnitsCount;
	std::string::const_pointer first  = begin + codeUnitsOffset;
	std::string::const_pointer p      = first;

	// n is positive, see first line
	while (n > 0 && p >= end) {
		--p;
		--n;
	}

	while(n > 0 && p > begin) {
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
	}

	while (n > 0) {
		--p;
		--n;
	}

	return p - first;
}


template <typename _utf8string, typename _Ref>
ucchar_ptr_basic<_utf8string, _Ref> & ucchar_ptr_basic<_utf8string, _Ref>::operator -= (difference_type n)
{
	if (n < 0)
		return this->operator += (0 - n);

	_ref._pos += __decrement(n
			, _ref._owner->_pimpl->size() // code units count
			, _ref._owner->_pimpl->data() // begin
			, _ref._pos                   // offset in bytes from begin
		);

	return *this;
}

template ucchar_ptr_basic<utf8string, ucchar_ref> &
	ucchar_ptr_basic<utf8string, ucchar_ref>::operator -= (difference_type n);

template ucchar_ptr_basic<const utf8string, ucchar_const_ref> &
	ucchar_ptr_basic<const utf8string, ucchar_const_ref>::operator -= (difference_type n);

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
