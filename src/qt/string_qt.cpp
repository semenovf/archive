/**
 * @file   string_qt.cpp
 * @author wladt
 * @date   Jan 24, 2013 2:32:45 PM
 *
 * @brief
 */


#include "string_qt.hpp"
#include <cstring>

CWT_NS_BEGIN

static const String __nullString;

String::String() : pimpl(new Impl()) {}
String::String(const Char *unicode, int size) : pimpl(new Impl(unicode, size)) {}
String::String(Char ch)              : pimpl(new Impl(ch)) {}
String::String(int size, Char ch)    : pimpl(new Impl(size, ch)) {}
String::String(const String &other)  : pimpl(new Impl(*other.pimpl)) {}

String&	String::append(const String &str) { pimpl->append(*str.pimpl); return *this; }
String&	String::append(const Char *unicode, int size) {	append(String(unicode, size)); return *this; }
String&	String::append(Char ch) { pimpl->append(QChar(ch.unicode())); return *this; }

const Char String::at(int pos) const     { const QChar ch = pimpl->at(pos); return Char(ch.unicode()); }
void String::clear()             { pimpl->clear(); }
bool String::contains(const String & str, bool cs) const { return pimpl->contains(*str.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::contains(Char ch, bool cs) const { return pimpl->contains(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
Char* String::data()             { return reinterpret_cast<Char*>(pimpl->data()); }
const Char*	String::data() const { return reinterpret_cast<const Char*>(pimpl->data()); }
bool String::isEmpty() const     { return reinterpret_cast<const Char*>(pimpl->isEmpty()); }
bool String::isNull() const      { return reinterpret_cast<const Char*>(pimpl->isNull()); }
int String::indexOf(const String &str, int from, bool cs) const { return pimpl->indexOf(*str.pimpl, from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
int String::indexOf(Char ch, int from, bool cs) const { return pimpl->indexOf(QChar(ch.unicode()), from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

int	String::length() const       { return size(); }
int	String::size() const         { return pimpl->size(); }

String&	String::prepend(const String &str) { pimpl->prepend(*str.pimpl); return *this; }
String&	String::prepend(const Char *unicode, int size) { String s(unicode, size); prepend(s); return *this; }
String&	String::prepend(Char ch) { pimpl->prepend(QChar(ch.unicode())); return *this; }

String& String::vsprintf(const char *cformat, va_list args) { pimpl->vsprintf(cformat, args); return *this; }
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

String String::substr(int pos, int n) const { String s; *s.pimpl = pimpl->mid(pos, n); return s; }

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

String&	String::operator+=(const String &other) { pimpl->operator += (*other.pimpl);	return *this; }
String&	String::operator+=(Char ch)             { pimpl->operator += (QChar(ch.unicode())); return *this; }
String&	String::operator=(const String & other) { pimpl->operator  = (*other.pimpl); return *this; }
String&	String::operator=(Char ch)              { pimpl->operator  = (ch); return *this; }
Char String::operator[](int pos)                { QCharRef qch = pimpl->operator [](pos); return Char(qch.unicode()); }
const Char String::operator[](int pos) const    { const QChar qch = pimpl->operator [](pos); return Char(qch.unicode()); }

bool operator != (const String &s1, const String &s2) {
#if QT_VERSION < 0x050000
	return *s1.pimpl != *s2.pimpl;
#else
	return operator != (*s1.pimpl, *s2.pimpl);
#endif
}

const String operator + (const String &s1, const String &s2)
{
	String s;
	operator + (*s.pimpl, *s1.pimpl);
	operator + (*s.pimpl, *s2.pimpl);
	return s;
}
const String operator+(const String &s1, Char ch)
{
	String s;
	operator + (*s.pimpl, *s1.pimpl);
	operator + (*s.pimpl, QChar(ch.unicode()));
	return s;
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
	*s.pimpl = QString::fromUtf8(str.constData());
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

const String& String::null()
{
	return __nullString;
}

CWT_NS_END
