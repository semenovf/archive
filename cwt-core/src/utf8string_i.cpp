/*
 * utf8string_i.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: wladt
 */

#include "utf8string_p.hpp"

CWT_NS_BEGIN

UChar Utf8Entry::value() const
{
	uint32_t uc = UChar::Null;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition
	int n = Utf8String::decodeBytes(cursor, 6, uc, min_uc);

	CWT_ASSERT(n >= 0);
	CWT_ASSERT(UChar::isValid(uc, min_uc));
	return uc;
}

Utf8Entry & Utf8Entry::next ()
{
	byte_t ch = byte_t(cursor[0]);

	if (ch < 128) {
		++cursor;
	} else if ((ch & 0xE0) == 0xC0) {
		cursor += 2;
	} else if ((ch & 0xF0) == 0xE0) {
		cursor += 3;
	} else if ((ch & 0xF8) == 0xF0) {
		cursor += 4;
	} else if ((ch & 0xFC) == 0xF8) {
		cursor += 5;
	} else if ((ch & 0xFE) == 0xFC) {
		cursor += 6;
	} else {
		//Logger::fatal(_Tr("Unexpected UTF-8 sequence. May be used illegal operations to initialize Utf8String instance."));
		; // end() reached
	}

	return *this;
}

Utf8Entry & Utf8Entry::prev ()
{
	if ((*(cursor - 1) & 0x80) == 0x00) {
		--cursor;
	} else if ((*(cursor - 2) & 0xE0) == 0xC0) {
		cursor -= 2;
	} else if ((*(cursor - 3) & 0xF0) == 0xE0) {
		cursor -= 3;
	} else if ((*(cursor - 4) & 0xF8) == 0xF0) {
		cursor -= 4;
	} else if ((*(cursor - 5) & 0xFC) == 0xF8) {
		cursor -= 5;
	} else if ((*(cursor - 6) & 0xFE) == 0xFC) {
		cursor -= 6;
	} else {
		//Logger::fatal(_Tr("Unexpected UTF-8 sequence. May be used illegal operations to initialize Utf8String instance."));
		; // begin() reached
	}
	return *this;
}

UChar Utf8Entry::r_value() const
{
	int need = -1;

	if ((*(cursor - 1) & 0x80) == 0x00) {
		need = 1;
	} else if ((*(cursor - 2) & 0xE0) == 0xC0) {
		need = 2;
	} else if ((*(cursor - 3) & 0xF0) == 0xE0) {
		need = 3;
	} else if ((*(cursor - 4) & 0xF8) == 0xF0) {
		need = 4;
	} else if ((*(cursor - 5) & 0xFC) == 0xF8) {
		need = 5;
	} else if ((*(cursor - 6) & 0xFE) == 0xFC) {
		need = 6;
	} else {
		;
	}

	uint32_t uc = UChar::Null;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition
	int n = Utf8String::decodeBytes(cursor - need, need, uc, min_uc);

	CWT_ASSERT(n >= 0);
	CWT_ASSERT(UChar::isValid(uc, min_uc));
	return uc;
}

Utf8String::iterator Utf8String::begin()
{
	detach();
	return iterator(pimpl->data());
}

Utf8String::iterator Utf8String::end()
{
	detach();
	return iterator(pimpl->data() + pimpl->size());
}


Utf8String::const_iterator Utf8String::begin() const
{
	return iterator(pimpl->data());
}

Utf8String::const_iterator Utf8String::end() const
{
	return iterator(pimpl->data() + pimpl->size());
}

Utf8String::reverse_iterator Utf8String::rbegin()
{
	detach();
	return reverse_iterator(pimpl->data() + pimpl->size());
}

Utf8String::reverse_iterator Utf8String::rend()
{
	detach();
	return reverse_iterator(pimpl->data());
}

Utf8String::const_reverse_iterator Utf8String::rbegin() const
{
	return reverse_iterator(pimpl->data() + pimpl->size());
}

Utf8String::const_reverse_iterator Utf8String::rend() const
{
	return reverse_iterator(pimpl->data());
}


Utf8String::const_iterator Utf8String::find(const char * s, size_t pos, size_t n) const
{
	if (pos >= length())
		return end();

	size_t real_pos = (begin() + pos).distance(begin());
	size_t find_pos = pimpl->find(s, real_pos, n);

	if (find_pos == Utf8String::Impl::npos)
		return end();

	return Utf8String::const_iterator(data() + find_pos);
}

Utf8String::const_iterator Utf8String::find(const Utf8String & s, Utf8String::const_iterator pos) const
{
	return find(s.constData(), length(begin(), pos), s.size());
}

Utf8String::const_iterator Utf8String::find(const char * s, size_t from) const
{
	return find(s, from, strlen(s));
}


CWT_NS_END


