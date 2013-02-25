/**
 * @file   string.cpp
 * @author wladt
 * @date   Jan 24, 2013 2:32:45 PM
 *
 * @brief
 */


#include "../include/cwt/string.hpp"
#define QT_NO_CAST_FROM_ASCII
#include <QtCore/QString>

#define _IMPL reinterpret_cast<QString*>(__impl)
#define _CONST_IMPL reinterpret_cast<const QString*>(__impl)
#define _CAST(s) reinterpret_cast<QString*>(s)
#define _CONST_CAST(s) reinterpret_cast<const QString*>(s)

CWT_NS_BEGIN

static const String __nullString;

String::String() : __impl(new QString()) {}
String::String(const Char *unicode, int size) : __impl(NULL)
{
	QString *s = new QString;
	s->resize(size);
	QChar *data = s->data();

	for(int i = 0; i < size; i++)
		data[i] = QChar(unicode[i].unicode());

	__impl = s;
}
String::String(Char ch)              : __impl(new QString(QChar(ch.unicode()))) {}
String::String(int size, Char ch)    : __impl(new QString(size, QChar(ch.unicode()))) {}
String::String(const String &other)  : __impl(new QString(*_CONST_CAST(other.__impl))) {}
String::~String() { delete _IMPL; }

String&	String::append(const String &str) { _IMPL->append(_CONST_CAST(str.__impl)); return *this; }
String&	String::append(const Char *unicode, int size) {	append(String(unicode, size)); return *this; }
String&	String::append(Char ch) { _IMPL->append(QChar(ch.unicode())); return *this; }

const Char String::at(int pos) const     { const QChar ch = _CONST_IMPL->at(pos); return Char(ch.unicode()); }
void String::clear()             { _IMPL->clear(); }
bool String::contains(const String & str, bool cs) const { return _CONST_IMPL->contains(*_CONST_CAST(str.__impl), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::contains(Char ch, bool cs) const { return _CONST_IMPL->contains(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
Char* String::data()             { return reinterpret_cast<Char*>(_IMPL->data()); }
const Char*	String::data() const { return reinterpret_cast<const Char*>(_IMPL->data()); }
bool String::isEmpty() const     { return reinterpret_cast<const Char*>(_IMPL->isEmpty()); }
bool String::isNull() const      { return reinterpret_cast<const Char*>(_IMPL->isNull()); }
int String::indexOf(const String &str, int from, bool cs) const { return _CONST_IMPL->indexOf(*_CONST_CAST(str.__impl), from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
int String::indexOf(Char ch, int from, bool cs) const { return _CONST_IMPL->indexOf(QChar(ch.unicode()), from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

int	String::length() const       { return _CONST_IMPL->length(); }

String&	String::prepend(const String &str) { _IMPL->prepend(*_CAST(str.__impl)); return *this; }
String&	String::prepend(const Char *unicode, int size) { String s(unicode, size); prepend(s); return *this; }
String&	String::prepend(Char ch) { _IMPL->prepend(QChar(ch.unicode())); return *this; }

String& String::vsprintf(const char *cformat, va_list ap) {	_IMPL->vsprintf(cformat, ap); return *this; }
String& String::sprintf(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	String::vsprintf(cformat, args);
	va_end(args);
	return *this;
}

bool String::startsWith(const String &s, bool cs) const { return _CONST_IMPL->startsWith(*_CONST_CAST(s.__impl), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::startsWith(Char ch, bool cs) const { return _IMPL->startsWith(QChar(ch.unicode()), cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
String String::substr(int pos, int n) const { String s; *_CAST(s.__impl) = _CONST_IMPL->mid(pos, n); return s; }

double	 String::toDouble(bool *ok) const           { return _CONST_IMPL->toDouble(ok); }
float	 String::toFloat(bool *ok) const            { return _CONST_IMPL->toFloat(ok); }
int_t	 String::toInt(bool *ok, int base) const    { return _CONST_IMPL->toInt(ok, base); }
long_t   String::toLong(bool *ok, int base) const   { return _CONST_IMPL->toLongLong(ok, base); }
short_t	 String::toShort(bool *ok, int base) const  { return _CONST_IMPL->toShort(ok, base); }
uint_t	 String::toUInt(bool *ok, int base) const   { return _CONST_IMPL->toUInt(ok, base); }
ulong_t	 String::toULong(bool *ok, int base) const  { return _CONST_IMPL->toULongLong(ok, base); }
ushort_t String::toUShort(bool *ok, int base) const { return _CONST_IMPL->toUShort(ok, base); }
void     String::truncate(int count)                { _IMPL->truncate(count); }

ByteArray String::toUtf8() const
{
	ByteArray bytes;
	*reinterpret_cast<QByteArray*>(bytes.__impl) = _CONST_IMPL->toUtf8();
	return bytes;
}

const ushort_t* String::utf16() const { return _IMPL->utf16(); }

String&	String::operator+=(const String &other) { _IMPL->operator += (*_CONST_CAST(other.__impl));	return *this; }
String&	String::operator+=(Char ch)             { _IMPL->operator += (QChar(ch.unicode())); return *this; }
String&	String::operator=(const String & other) { _IMPL->operator  = (*_CONST_CAST(other.__impl)); return *this; }
String&	String::operator=(Char ch)              { _IMPL->operator  = (QChar(ch.unicode())); return *this; }
Char String::operator[](int pos)                { QCharRef qch = _IMPL->operator [](pos); return Char(qch.unicode()); }
const Char String::operator[](int pos) const    { const QChar qch = _CONST_IMPL->operator [](pos); return Char(qch.unicode()); }

bool operator != (const String &s1, const String &s2) {
#if QT_VERSION < 0x050000
	return _CONST_CAST(s1.__impl) != *_CONST_CAST(s2.__impl);
#else
	return operator != (*_CONST_CAST(s1.__impl), *_CONST_CAST(s2.__impl));
#endif
}

const String operator + (const String &s1, const String &s2)
{
	String s;
	operator + (*_CONST_CAST(s.__impl), *_CONST_CAST(s1.__impl));
	operator + (*_CONST_CAST(s.__impl), *_CONST_CAST(s2.__impl));
	return s;
}
const String operator+(const String &s1, Char ch)
{
	String s;
	operator + (*_CONST_CAST(s.__impl), *_CONST_CAST(s1.__impl));
	operator + (*_CONST_CAST(s.__impl), QChar(ch.unicode()));
	return s;
}

bool operator < (const String &s1, const  String &s2)
{
#if QT_VERSION < 0x050000
	return _CONST_CAST(s1.__impl)->operator < (*_CONST_CAST(s2.__impl));
#else
	return operator < (*_CONST_CAST(s1.__impl), *_CONST_CAST(s2.__impl));
#endif
}

bool operator <= (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return _CONST_CAST(s1.__impl)->operator <= (*_CONST_CAST(s2.__impl));
#else
	return operator <= (*_CONST_CAST(s1.__impl), *_CONST_CAST(s2.__impl));
#endif
}

bool operator == (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return _CONST_CAST(s1.__impl)->operator == (*_CONST_CAST(s2.__impl));
#else
	return operator == (*_CONST_CAST(s1.__impl), *_CONST_CAST(s2.__impl));
#endif
}

bool operator > (const String &s1, const  String &s2)
{
#if QT_VERSION < 0x050000
	return _CONST_CAST(s1.__impl)->operator > (*_CONST_CAST(s2.__impl));
#else
	return operator > (*_CONST_CAST(s1.__impl), *_CONST_CAST(s2.__impl));
#endif
}

bool operator >= (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return _CONST_CAST(s1.__impl)->operator >= (*_CONST_CAST(s2.__impl));
#else
	return operator >= (*_CONST_CAST(s1.__impl), *_CONST_CAST(s2.__impl));
#endif
}

String String::fromUtf8(const char *str, int size)
{
	String s;
	*_CAST(s.__impl) = QString::fromUtf8(str, size);
	return s;
}

String String::fromUtf8(const ByteArray &str)
{
	String s;
	*_CAST(s.__impl) = QString::fromUtf8(*reinterpret_cast<const QByteArray*>(str.__impl));
	return s;
}


String String::number(double n, char format, int precision)
{
	String s;
	*_CAST(s.__impl) = QString::number(n, format, precision);
	return s;
}

String String::number(int_t n, int base)
{
	String s;
	*_CAST(s.__impl) = QString::number(n, base);
	return s;
}

String String::number(uint_t n, int base)
{
	String s;
	*_CAST(s.__impl) = QString::number(n, base);
	return s;
}

String String::number(long_t n, int base)
{
	String s;
	*_CAST(s.__impl) = QString::number(n, base);
	return s;
}

String String::number(ulong_t n, int base)
{
	String s;
	*_CAST(s.__impl) = QString::number(n, base);
	return s;
}

const String& String::null()
{
	return __nullString;
}

CWT_NS_END
