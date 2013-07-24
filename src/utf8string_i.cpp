/*
 * utf8string_i.cpp
 *
 *  Created on: Jul 22, 2013
 *      Author: wladt
 */

#include "utf8string_p.hpp"
#include "../../include/cwt/unicode.hpp"
#include "../../include/cwt/logger.hpp"

CWT_NS_BEGIN

UChar Utf8Entry::value() const
{
	uint32_t uc = Unicode::Null;
	uint32_t min_uc = 0; // for 'Overlong' encodings recognition
	int n = Utf8String::decodeBytes(cursor, 6, uc, min_uc);

	CWT_ASSERT(n >= 0);
	CWT_ASSERT(Unicode::isValid(uc, min_uc));
	return uc;
}

Utf8Entry & Utf8Entry::next ()
{
	byte_t ch = byte_t(cursor[0]);

	if (ch < 128) {
		cursor++;
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
		cursor--;
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

Utf8String::iterator Utf8String::find(const Utf8String & s, Utf8String::iterator from)
{
	CWT_ASSERT(from <= end());
	ssize_t fromPos = from.distance(begin());
	CWT_ASSERT(fromPos >= 0);
	size_t pos = pimpl->find(*s.pimpl, size_t(fromPos));

	if (pos == Utf8String::Impl::npos)
		return end();

	return Utf8String::iterator(data() + pos);
}

Utf8String::const_iterator Utf8String::find(const Utf8String & s, Utf8String::const_iterator from) const
{
	CWT_ASSERT(from <= end());
	ssize_t fromPos = from.distance(begin());
	CWT_ASSERT(fromPos >= 0);
	size_t pos = pimpl->find(*s.pimpl, size_t(fromPos));

	if (pos == Utf8String::Impl::npos)
		return end();

	return Utf8String::const_iterator(data() + pos);
}

Utf8String::iterator Utf8String::find(const char * s)
{
	return find(s, strlen(s));
}

Utf8String::iterator Utf8String::find(const char * s, size_t n)
{
	detach();
	size_t pos = pimpl->find(s, 0, n);

	if (pos == Utf8String::Impl::npos)
		return end();

	return Utf8String::iterator(data() + pos);
}

Utf8String::const_iterator Utf8String::find(const char * s) const
{
	return find(s, strlen(s));
}

Utf8String::const_iterator Utf8String::find(const char * s, size_t n) const
{
	size_t pos = pimpl->find(s, 0, n);

	if (pos == Utf8String::Impl::npos)
		return end();

	return Utf8String::const_iterator(data() + pos);
}


CWT_NS_END


