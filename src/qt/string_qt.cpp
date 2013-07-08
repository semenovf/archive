/**
 * @file   string_qt.cpp
 * @author wladt
 * @date   Jan 24, 2013 2:32:45 PM
 *
 * @brief
 */


#include "../../include/cwt/string.hpp"
#include <cstring>
#define QT_NO_CAST_FROM_ASCII
#include <QString>

CWT_NS_BEGIN

static const String __nullString;

class String::Impl : public QString {
public:
	Impl() : QString() {}
	Impl(const Char *unicode);
	Impl(const Char *unicode, size_t size);
	Impl(Char ch)                 : QString(QChar(ch.unicode())) {}
	Impl(size_t size, Char ch);
	Impl(const Impl &other)       : QString(other) {}
	Impl(const QString &other)    : QString(other) {}
	Impl& operator=(Char ch)      { QString::operator  = (QChar(ch.unicode())); return *this; }
//	~Impl() { QChar *d = data(); memset(d, 0, sizeof(QChar) * length()); }
};


// TODO need an optimized version
String::Impl::Impl(const Char *unicode) : QString()
{
	while(*unicode != Char(0)) {
		this->append(QChar(unicode->unicode()));
		++unicode;
	}
}

// TODO need an optimized version
String::Impl::Impl(const Char *unicode, size_t size) : QString()
{
	CWT_ASSERT(size <= CWT_INT_MAX);
	resize(int(size));
	QChar *chars = QString::data();

	for(size_t i = 0; i < size; i++)
		chars[i] = QChar(unicode[i].unicode());
}

// TODO need an optimized version
String::Impl::Impl(size_t size, Char ch) : QString()
{
	CWT_ASSERT(size <= CWT_INT_MAX);
	resize(int(size));
	QChar *chars = QString::data();

	for(size_t i = 0; i < size; i++)
		chars[i] = QChar(ch.unicode());
}

CWT_PIMPL_IMPL_COPYABLE(String)

String::String()                     : pimpl(new Impl()) {}
String::String(const Char *unicode)  : pimpl(new Impl(unicode)) {}
String::String(const Char *unicode, size_t size)  : pimpl(new Impl(unicode, size)) {}
String::String(Char ch)              : pimpl(new Impl(ch)) {}
String::String(size_t size, Char ch) : pimpl(new Impl(size, ch)) {}

String&	String::append(const String &str)             { detach(); pimpl->append(*str.pimpl); return *this; }
String&	String::append(const Char *unicode, int size) {	detach(); append(String(unicode, size)); return *this; }
String&	String::append(Char ch)                       { detach(); pimpl->append(QChar(ch.unicode())); return *this; }

const Char String::at(int pos) const  { const QChar ch = pimpl->at(pos); return Char(ch.unicode()); }
void String::clear()                  { detach(); pimpl->clear(); }
bool String::contains(const String & str, bool cs) const { return pimpl->contains(*str.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::contains(Char ch, bool cs) const { return pimpl->contains(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
int String::compare(const String &other, bool cs) const { return pimpl->compare(*other.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
Char* String::data()                  { detach(); return reinterpret_cast<Char*>(pimpl->data()); }
const Char*	String::data() const      { return reinterpret_cast<const Char*>(pimpl->data()); }
bool String::isEmpty() const          { return reinterpret_cast<const Char*>(pimpl->isEmpty()); }
bool String::isNull() const           { return reinterpret_cast<const Char*>(pimpl->isNull()); }
ssize_t String::indexOf(const String &str, int from, bool cs) const { return (ssize_t)pimpl->indexOf(*str.pimpl, from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
ssize_t String::indexOf(Char ch, int from, bool cs) const { return (ssize_t)pimpl->indexOf(QChar(ch.unicode()), from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

size_t String::size() const      { return size_t(pimpl->size()); }

String&	String::prepend(const String &str)             { detach(); pimpl->prepend(*str.pimpl); return *this; }
String&	String::prepend(const Char *unicode, int size) { detach(); String s(unicode, size); prepend(s); return *this; }
String&	String::prepend(Char ch)                       { detach(); pimpl->prepend(QChar(ch.unicode())); return *this; }
String& String::remove(size_t pos, size_t n)
{
	CWT_ASSERT(pos <= CWT_INT_MAX);
	CWT_ASSERT(n <= CWT_INT_MAX);
	detach();
	remove(int(pos), int(n));
	return *this;
}

String& String::replace(const String &before, const String &after, bool cs)
{
	detach();
	pimpl->replace(*before.pimpl, *after.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive);
	return *this;
}

void String::reserve(size_t size) { detach(); CWT_ASSERT(size <= CWT_INT_MAX); pimpl->reserve(int(size)); }
void String::resize(size_t size)  { detach(); CWT_ASSERT(size <= CWT_INT_MAX); pimpl->resize(int(size)); }

String& String::vsprintf(const char *cformat, va_list args) { detach(); pimpl->vsprintf(cformat, args); return *this; }
String& String::sprintf(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	detach();
	String::vsprintf(cformat, args);
	va_end(args);
	return *this;
}

bool String::endsWith(const String &s, bool cs) const { return pimpl->endsWith(*s.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::endsWith(Char ch, bool cs) const { return pimpl->endsWith(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::startsWith(const String &s, bool cs) const { return pimpl->startsWith(*s.pimpl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::startsWith(Char ch, bool cs) const { return pimpl->startsWith(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

String String::substr(size_t pos, size_t n) const
{
	CWT_ASSERT(pos <= CWT_INT_MAX);
	CWT_ASSERT(n <= CWT_INT_MAX);
	String s;
	*s.pimpl = pimpl->mid(int(pos), int(n));
	return s;
}

String String::substr(size_t pos) const
{
	CWT_ASSERT(pos <= CWT_INT_MAX);
	String s;
	*s.pimpl = pimpl->mid(int(pos));
	return s;
}


String&	String::setNumber(long_t n, int base)         { detach(); pimpl->setNum(n, base); return *this; }
String&	String::setNumber(ulong_t n, int base)        { detach(); pimpl->setNum(n, base); return *this; }
String&	String::setNumber(int_t n, int base)          { detach(); pimpl->setNum(n, base); return *this; }
String&	String::setNumber(uint_t n, int base)         { detach(); pimpl->setNum(n, base); return *this; }
String&	String::setNumber(short_t n, int base)        { detach(); pimpl->setNum(n, base); return *this; }
String&	String::setNumber(ushort_t n, int base)       { detach(); pimpl->setNum(n, base); return *this; }
String&	String::setNumber(float n, char f, int prec)  { detach(); pimpl->setNum(n, f, prec); return *this; }
String&	String::setNumber(double n, char f, int prec) { detach(); pimpl->setNum(n, f, prec); return *this; }

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

String&	String::operator+=(const String &other) { detach(); pimpl->operator += (*other.pimpl);	return *this; }
String&	String::operator+=(Char ch)             { detach(); pimpl->operator += (QChar(ch.unicode())); return *this; }
String&	String::operator=(Char ch)              { detach(); pimpl->operator  = (ch); return *this; }
Char String::operator[](int pos)                { detach(); QCharRef qch = pimpl->operator [](pos); return Char(qch.unicode()); }
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

String String::fromUtf8(const char *utf8)
{
	String s;
	*s.pimpl = QString::fromUtf8(utf8);
	return s;
}

String String::fromUtf8(const char *utf8, size_t size)
{
	String s;
	CWT_ASSERT(size <= CWT_INT_MAX);
	*s.pimpl = QString::fromUtf8(utf8, int(size));
	return s;
}


String String::fromUtf8(const ByteArray &bytes)
{
	String s;
	*s.pimpl = QString::fromUtf8(bytes.constData());
	return s;
}

String String::fromLatin1 (const char * str, size_t size)
{
	String s;
	CWT_ASSERT(size <= CWT_INT_MAX);
	*s.pimpl = QString::fromLatin1(str, int(size));
	return s;
}

String String::fromLatin1 (const char * str)
{
	String s;
	size_t size = ::strlen(str);
	CWT_ASSERT(size > CWT_INT_MAX);
	*s.pimpl = QString::fromLatin1(str, size);
	return s;
}

String String::fromUtf16 (const ushort_t * unicode, size_t size)
{
	String s;
	CWT_ASSERT(size <= CWT_INT_MAX);
	*s.pimpl = QString::fromUtf16(unicode, int(size));
	return s;
}

String String::fromUtf16 (const ushort_t * unicode)
{
	String s;
	*s.pimpl = QString::fromUtf16(unicode);
	return s;
}


String String::number(double n, char format, int precision)
{
	String s;
	*s.pimpl = QString::number(n, format, precision);
	return s;
}

String String::number(float n, char format, int precision)
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

const String& String::constNull()
{
	return __nullString;
}

CWT_NS_END
