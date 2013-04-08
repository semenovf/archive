/**
 * @file   string.cpp
 * @author wladt
 * @date   Jan 24, 2013 2:32:45 PM
 *
 * @brief
 */

#include "../../include/cwt/string.hpp"
#include <cstring>

CWT_NS_BEGIN

static const String __nullString;

/**
 * Constructs a null string. Null strings are also empty.
 */
String::String() : BaseClass() {}

/**
 * Constructs a string of size 1 containing the character ch.
 */
String::String(Char ch) : BaseClass() { append(ch.unicode()); }


/**
 * Constructs a string initialized with the first size characters of the Char array unicode.
 * If unicode is 0, a null string is constructed.
 * If size is negative, unicode is assumed to point to a nul-terminated array and its
 * length is determined dynamically. The terminating nul-character is not considered part of the string.
 *
 * @param unicode
 * @param size
 */
String::String(const Char *unicode, int size) : BaseClass()
{
	append(unicode, size);
}

/**
 * Constructs a string of the given size with every character set to ch.
 */
String::String(int size, Char ch) : BaseClass()
{
	resize(size);
	data_type *d = data();
	for(int i = 0; i < size; i++) {
		d[i] = ch.unicode();
	}
}


/**
 *
 * @param other
 */
String::String(const String &other) : BaseClass(other)
{
}


/**
 *
 * @param str
 * @return
 */
String&	String::append(const String &str)
{
	append(str.data(), str.length());
}

/**
 *
 * @param unicode
 * @param size
 * @return
 */
String&	String::append(const Char *unicode, int size)
{
	// Calculate size if it is negative.
	if (size < 0) {
		const Char *ptr = unicode;
		size = 0;
		while (*ptr != 0) {
			ptr++;
			size++;
		}
	}

	resize(size);
	memcpy(data(), reinterpret_cast<data_type*>(unicode), size * sizeof(data_type));

	return *this;
}


/**
 *
 * @param ch
 * @return
 */
String&	String::append(Char ch)
{
	BaseClass::append(ch.unicode());
	return *this;
}


int	String::length() const
{
	return size();
}


const Char String::at(int pos) const
{
	return Char(BaseClass::at(pos));
}


Char* String::data()
{
	return reinterpret_cast<Char*>(BaseClass::data());
}

const Char*	String::data() const
{
	return reinterpret_cast<const Char*>(BaseClass::data());
}

bool String::contains(const String & str, bool cs) const
{
	return indexOf(str, 0, cs);
}

bool String::contains(Char ch, bool cs) const {
	return indexOf(ch, 0, cs);
}

String&	String::prepend(const String &str)
{
	return prepend(str.data());
}

String&	String::prepend(const Char *unicode, int size)
{
	// Calculate size if it is negative.
	if (size < 0) {
		const Char *ptr = unicode;
		size = 0;
		while (*ptr != 0) {
			ptr++;
			size++;
		}
	}

	data_type *d = data();
	reserve(length() + size);
	memcpy(d + length(), d, length() * sizeof(data_type));
	memcpy(d, reinterpret_cast<data_type*>(unicode), size * sizeof(data_type));
	resize(length() + size);
	return *this;
}

String&	String::prepend(Char ch)
{
	return prepend(&ch, 1);
}


String String::substr(int pos, int n) const
{
	return String(data(), length());
}


const String& String::null()
{
	return __nullString;
}


String&	String::operator += (const String &other)
{
	return append(other);
}

String&	String::operator += (Char ch)
{
	return append(ch);
}

String&	String::operator = (const String & other)
{
	clear();
	append(other);
	return *this;
}

String&	String::operator = (Char ch)
{
	clear();
	append(ch);
	return *this;
}

const String operator + (const String &s1, const String &s2)
{
	String s;
	s.append(s1);
	s.append(s2);
	return s;
}
const String operator+(const String &s1, Char ch)
{
	String s;
	s.append(s1);
	s.append(ch);
	return s;
}

Char String::operator[](int pos)
{
	return Char(BaseClass::at(pos));
}

const Char String::operator[](int pos) const
{
	return Char(BaseClass::at(pos));
}


#ifdef CWT_STRING_SELF_IMPL


int String::indexOf(const String &str, int from, bool cs) const { return pimpl->indexOf(*str.pimpl, from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
int String::indexOf(Char ch, int from, bool cs) const { return pimpl->indexOf(QChar(ch.unicode()), from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

String& String::vsprintf(const char *cformat, va_list ap) {	pimpl->vsprintf(cformat, ap); return *this; }
String& String::sprintf(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	String::vsprintf(cformat, args);
	va_end(args);
	return *this;
}

bool String::endsWith(const String &s, bool cs) const { return pimpl->endsWith(*s.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::endsWith(Char ch, bool cs) const { return pimpl->endsWith(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::startsWith(const String &s, bool cs) const { return pimpl->startsWith(*s.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::startsWith(Char ch, bool cs) const { return pimpl->startsWith(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

double	 String::toDouble(bool *ok) const           { return pimpl->toDouble(ok); }
float	 String::toFloat(bool *ok) const            { return pimpl->toFloat(ok); }
int_t	 String::toInt(bool *ok, int base) const    { return pimpl->toInt(ok, base); }
long_t   String::toLong(bool *ok, int base) const   { return pimpl->toLongLong(ok, base); }
short_t	 String::toShort(bool *ok, int base) const  { return pimpl->toShort(ok, base); }
uint_t	 String::toUInt(bool *ok, int base) const   { return pimpl->toUInt(ok, base); }
ulong_t	 String::toULong(bool *ok, int base) const  { return pimpl->toULongLong(ok, base); }
ushort_t String::toUShort(bool *ok, int base) const { return pimpl->toUShort(ok, base); }
void     String::truncate(int count)                { pimpl->truncate(count); }
const Char*	String::unicode() const                 { return reinterpret_cast<const Char*>(pimpl->unicode()); }

ByteArray String::toUtf8() const
{
	const char *chars = pimpl->toUtf8().data();
	ByteArray bytes(chars, strlen(chars));
	return bytes;
}

const ushort_t* String::utf16() const { return pimpl->utf16(); }


bool operator != (const String &s1, const String &s2) {
#if QT_VERSION < 0x050000
	return *s1.pimpl != *s2.pimpl;
#else
	return operator != (*s1.pimpl, *s2.pimpl);
#endif
}



bool operator < (const String &s1, const  String &s2)
{
#if QT_VERSION < 0x050000
	return s1.pimpl->operator < (*s2.pimpl);
#else
	return operator < (*s1.pimpl, *s2.pimpl);
#endif
}

bool operator <= (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return s1.pimpl->operator <= (*s2.pimpl);
#else
	return operator <= (*s1.pimpl, *s2.pimpl);
#endif
}

bool operator == (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return s1.pimpl->operator == (*s2.pimpl);
#else
	return operator == (*s1.pimpl, *s2.pimpl);
#endif
}

bool operator > (const String &s1, const  String &s2)
{
#if QT_VERSION < 0x050000
	return s1.pimpl->operator > (*s2.pimpl);
#else
	return operator > (*s1.pimpl, *s2.pimpl);
#endif
}

bool operator >= (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return s1.pimpl->operator >= (*s2.pimpl);
#else
	return operator >= (*s1.pimpl, *s2.pimpl);
#endif
}

String String::fromUtf8(const char *str, int size)
{
	String s;
	*s.pimpl = QString::fromUtf8(str, size);
	return s;
}

String String::fromUtf8(const ByteArray &str)
{
	String s;
	*s.pimpl = QString::fromUtf8(str.data());
	return s;
}


String String::number(double n, char format, int precision)
{
	String s;
	*s.pimpl = QString::number(n, format, precision);
	return s;
}

String String::number(int_t n, int base)
{
	String s;
	*s.pimpl = QString::number(n, base);
	return s;
}

String String::number(uint_t n, int base)
{
	String s;
	*s.pimpl = QString::number(n, base);
	return s;
}

String String::number(long_t n, int base)
{
	String s;
	*s.pimpl = QString::number(n, base);
	return s;
}

String String::number(ulong_t n, int base)
{
	String s;
	*s.pimpl = QString::number(n, base);
	return s;
}

#endif // CWT_STRING_SELF_IMPL

CWT_NS_END
