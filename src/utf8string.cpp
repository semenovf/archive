/*
 * utf8string.cpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#include "../../include/cwt/utf8string.hpp"
#include <string>
#include <cstring>

CWT_NS_BEGIN

struct Utf8String::Impl : public std::string
{
	Impl() : std::string() {}
	Impl(const Impl & other) : std::string(other) { }
};

CWT_PIMPL_IMPL_COPYABLE(Utf8String);

Utf8String::Utf8String() : pimpl(new Utf8String::Impl())
{
}

Utf8String::Utf8String(const char *latin1) : pimpl(new Utf8String::Impl())
{
	append(latin1, strlen(latin1));
}

Utf8String::Utf8String(const char *latin1, size_t length) : pimpl(new Utf8String::Impl())
{
	append(latin1, length);
}

Utf8String::Utf8String(size_t count, char latin1) : pimpl(new Utf8String::Impl())
{
	for (size_t i = 0; i < count; ++i) {
		if (latin1 < 127)
			pimpl->append(1, latin1);
		else
			pimpl->append(1, ReplacementChar);
	}
}

bool Utf8String::isEmpty() const
{
	return pimpl->empty();
}

Utf8String& Utf8String::append(const Utf8String & s)
{
	detach();
	pimpl->append(*s.pimpl);
	return *this;
}

Utf8String&	Utf8String::append(const char *latin1, int length)
{
	detach();
	const char *end = latin1 + length;

	while (latin1 < end) {
		if (*latin1 < 127)
			pimpl->append(1, *latin1);
		else
			pimpl->append(1, ReplacementChar);
		++latin1;
	}

	return *this;
}

Utf8String& Utf8String::append(const char *latin1)
{
	return append(latin1, strlen(latin1));
}

void Utf8String::clear()
{
	detach();
	pimpl->clear();
}

const char*	Utf8String::data() const
{
	return pimpl->data();
}

Utf8String operator + (const Utf8String &s1, const Utf8String &s2)
{
	Utf8String s(s1);
	s.append(s2);
	return s;
}

bool operator != (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl != *s2.pimpl;
}

bool operator < (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl < *s2.pimpl;
}

bool operator <= (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl <= *s2.pimpl;
}

bool operator == (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl == *s2.pimpl;
}

bool operator > (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl > *s2.pimpl;
}

bool operator >= (const Utf8String &s1, const Utf8String &s2)
{
	return *s1.pimpl >= *s2.pimpl;
}


CWT_NS_END
