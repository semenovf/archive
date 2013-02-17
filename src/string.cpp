/**
 * @file   string.cpp
 * @author wladt
 * @date   Jan 24, 2013 2:32:45 PM
 *
 * @brief
 */


#include <cwt/string.hpp>

CWT_NS_BEGIN

static const String __nullString;

String::String()                     : __impl() {}
String::String(const Char *unicode, int size) : __impl(reinterpret_cast<const QChar*>(unicode), size) {}
String::String(Char ch)              : __impl(ch.__impl) {}
String::String(int size, Char ch)    : __impl(size, ch.__impl) {}
String::String(const String &other)  : __impl(other.__impl) {}
String::~String() {}

String&	String::append(const String &str) { __impl.append(str.__impl); return *this; }
String&	String::append(const Char *unicode, int size)
{
#if QT_VERSION < 0x050000
	append(String(unicode, size));
#else
	__impl.append(reinterpret_cast<const QChar*>(unicode), size);
#endif
	return *this;
}

String&	String::append(Char ch) { __impl.append(ch.__impl); return *this; }

const Char String::at(int pos) const     { const QChar ch = __impl.at(pos); return Char(ch.unicode()); }
void String::clear()             { __impl.clear(); }
Char* String::data()             { return reinterpret_cast<Char*>(__impl.data()); }
const Char*	String::data() const { return reinterpret_cast<const Char*>(__impl.data()); }
bool String::isEmpty() const     { return reinterpret_cast<const Char*>(__impl.isEmpty()); }
bool String::isNull() const      { return reinterpret_cast<const Char*>(__impl.isNull()); }
int String::indexOf(const String &str, int from, bool cs) const { return __impl.indexOf(str.__impl, from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
int String::indexOf(Char ch, int from, bool cs) const { return __impl.indexOf(ch.__impl, from, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

int	String::length() const       { return __impl.length(); }

String&	String::prepend(const String &str) { __impl.prepend(str.__impl); return *this; }
String&	String::prepend(const Char *unicode, int size) { String s(unicode, size); prepend(s); return *this; }
String&	String::prepend(Char ch) { __impl.prepend(ch.__impl); return *this; }

String& String::vsprintf(const char *cformat, va_list ap) {	__impl.vsprintf(cformat, ap); return *this; }
String& String::sprintf(const char * cformat, ...)
{
	va_list args;
	va_start(args, cformat);
	String::vsprintf(cformat, args);
	va_end(args);
	return *this;
}

bool String::startsWith(const String &s, bool cs) const { return __impl.startsWith(s.__impl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }
bool String::startsWith(Char c, bool cs) const { return __impl.startsWith(c.__impl, cs ? Qt::CaseSensitive : Qt::CaseInsensitive); }

double	 String::toDouble(bool *ok) const           { return __impl.toDouble(ok); }
float	 String::toFloat(bool *ok) const            { return __impl.toFloat(ok); }
int_t	 String::toInt(bool *ok, int base) const    { return __impl.toInt(ok, base); }
long_t   String::toLong(bool *ok, int base) const   { return __impl.toLongLong(ok, base); }
short_t	 String::toShort(bool *ok, int base) const  { return __impl.toShort(ok, base); }
uint_t	 String::toUInt(bool *ok, int base) const   { return __impl.toUInt(ok, base); }
ulong_t	 String::toULong(bool *ok, int base) const  { return __impl.toULongLong(ok, base); }
ushort_t String::toUShort(bool *ok, int base) const { return __impl.toUShort(ok, base); }
void     String::truncate(int count)                  { __impl.truncate(count); }

ByteArray String::toUtf8() const
{
	ByteArray bytes;
	bytes.__impl = __impl.toUtf8();
	return bytes;
}

const ushort_t* String::utf16() const { return __impl.utf16(); }

String&	String::operator+=(const String &other) { __impl.operator +=(other.__impl);	return *this; }
String&	String::operator+=(Char ch) { __impl.operator +=(ch.__impl); return *this; }
String&	String::operator=(const String & other) { __impl.operator =(other.__impl); return *this; }
String&	String::operator=(Char ch) { __impl.operator =(ch.__impl); return *this; }
Char String::operator[](int pos) { QCharRef qch = __impl.operator [](pos); return Char(qch.unicode()); }
const Char String::operator[](int pos) const { const QChar qch = __impl.operator [](pos); return Char(qch.unicode()); }

bool operator != (const String &s1, const String &s2) {
#if QT_VERSION < 0x050000
	return s1.__impl.operator != (s2.__impl);
#else
	return operator != (s1.__impl, s2.__impl);
#endif
}

const String operator + (const String &s1, const String &s2)
{
	String s;
	operator + (s.__impl, s1.__impl);
	operator + (s.__impl, s2.__impl);
	return s;
}
const String operator+(const String &s1, Char ch)
{
	String s;
	operator + (s.__impl, s1.__impl);
	operator + (s.__impl, ch.__impl);
	return s;
}

bool operator < (const String &s1, const  String &s2)
{
#if QT_VERSION < 0x050000
	return s1.__impl.operator < (s2.__impl);
#else
	return operator < (s1.__impl, s2.__impl);
#endif
}

bool operator <= (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return s1.__impl.operator <= (s2.__impl);
#else
	return operator <= (s1.__impl, s2.__impl);
#endif
}

bool operator == (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return s1.__impl.operator == (s2.__impl);
#else
	return operator == (s1.__impl, s2.__impl);
#endif
}

bool operator > (const String &s1, const  String &s2)
{
#if QT_VERSION < 0x050000
	return s1.__impl.operator > (s2.__impl);
#else
	return operator > (s1.__impl, s2.__impl);
#endif
}

bool operator >= (const String &s1, const String &s2)
{
#if QT_VERSION < 0x050000
	return s1.__impl.operator >= (s2.__impl);
#else
	return operator >= (s1.__impl, s2.__impl);
#endif
}

String String::fromUtf8(const char *str, int size)
{
	String s;
	s.__impl = QString::fromUtf8(str, size);
	return s;
}

String String::fromUtf8(const ByteArray &str)
{
	String s;
	s.__impl = QString::fromUtf8(str.__impl);
	return s;
}


String String::number(double n, char format, int precision)
{
	String s;
	s.__impl = QString::number(n, format, precision);
	return s;
}

String String::number(int_t n, int base)
{
	String s;
	s.__impl = QString::number(n, base);
	return s;
}

String String::number(uint_t n, int base)
{
	String s;
	s.__impl = QString::number(n, base);
	return s;
}

String String::number(long_t n, int base)
{
	String s;
	s.__impl = QString::number(n, base);
	return s;
}

String String::number(ulong_t n, int base)
{
	String s;
	s.__impl = QString::number(n, base);
	return s;
}

const String& String::null()
{
	return __nullString;
}

CWT_NS_END
