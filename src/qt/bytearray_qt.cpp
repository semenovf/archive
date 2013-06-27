/**
 * @file   bytearray_qt.cpp
 * @author wladt
 * @date   Jan 24, 2013 4:15:33 PM
 *
 * @brief
 */

#include "bytearray_qt.hpp"
#include <QByteArray>

CWT_NS_BEGIN

class ByteArray::Impl : public QByteArray
{
public:
	Impl() : QByteArray() {  }
	Impl(const char *data) : QByteArray(data) {}
	Impl(const char *data, size_t size) : QByteArray(data, int(size)) { CWT_ASSERT(size <= CWT_INT_MAX); }
	Impl(size_t size, char ch) : QByteArray(int(size), ch) { CWT_ASSERT(size <= CWT_INT_MAX); }
};

CWT_PIMPL_IMPL_COPYABLE(ByteArray)

ByteArray::ByteArray()                              : pimpl(new ByteArray::Impl()) {}
ByteArray::ByteArray(const char *data)              : pimpl(new Impl(data)) {}
ByteArray::ByteArray(const char *data, size_t size) : pimpl(new Impl(data, size)) {}
ByteArray::ByteArray(size_t size, char ch)          : pimpl(new Impl(size, ch)) {}

ByteArray& ByteArray::append(const ByteArray &bytes)     { detach(); pimpl->append(*bytes.pimpl); return *this; }
ByteArray& ByteArray::append(const char *data, int size) { detach(); pimpl->append(data, size); return *this; }
ByteArray& ByteArray::append(char ch)                    { detach(); pimpl->append(ch); return *this; }
void ByteArray::clear()                                  { detach(); pimpl->clear(); }
char* ByteArray::data()                                  { detach(); return pimpl->data(); }
const char* ByteArray::data() const                      { return pimpl->data(); }
const char* ByteArray::constData() const                 { return pimpl->data(); }
size_t ByteArray::length() const                         { return size(); }
ByteArray &ByteArray::prepend(const ByteArray &bytes)    { detach(); pimpl->prepend(*bytes.pimpl); return *this; }
ByteArray &ByteArray::prepend(const char *str, int len)  { detach(); if(len > 0) pimpl->prepend(str, len); else pimpl->prepend(str); return *this; }
ByteArray &ByteArray::prepend(char ch)                   { detach(); pimpl->prepend(ch); return *this; }
ByteArray& ByteArray::remove(size_t pos, size_t len)     { detach(); CWT_ASSERT(pos <= CWT_INT_MAX || len > CWT_INT_MAX); pimpl->remove((int)pos, (int)len); return *this; }
void ByteArray::reserve(size_t size)                     { detach(); CWT_ASSERT(size <= CWT_INT_MAX); pimpl->reserve((int)size); }
void ByteArray::resize(size_t size)                      { detach(); CWT_ASSERT(size <= CWT_INT_MAX); pimpl->resize((int)size); }

ByteArray& ByteArray::setNumber(long_t n, int base)      { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(ulong_t n, int base)     { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(int_t n, int base)       { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(uint_t n, int base)      { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(short_t n, int base)     { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(ushort_t n, int base)    { detach(); pimpl->setNum(n, base); return *this; }
ByteArray& ByteArray::setNumber(double n, char f, int prec) { detach(); pimpl->setNum(n, f, prec); return *this; }
ByteArray& ByteArray::setNumber(float n, char f, int prec)  { detach(); pimpl->setNum(n, f, prec); return *this; }

double	 ByteArray::toDouble(bool *ok) const             { return pimpl->toDouble(ok); }
float	 ByteArray::toFloat(bool *ok) const              { return pimpl->toFloat(ok); }
int_t	 ByteArray::toInt(bool *ok, int base) const      { return pimpl->toInt(ok, base); }
long_t   ByteArray::toLong(bool *ok, int base) const     { return pimpl->toLongLong(ok, base); }
short_t	 ByteArray::toShort(bool *ok, int base) const    { return pimpl->toShort(ok, base); }
uint_t	 ByteArray::toUInt(bool *ok, int base) const     { return pimpl->toUInt(ok, base); }
ulong_t	 ByteArray::toULong(bool *ok, int base) const    { return pimpl->toULongLong(ok, base); }
ushort_t ByteArray::toUShort(bool *ok, int base) const   { return pimpl->toUShort(ok, base); }

ByteArray& ByteArray::setRawData(const char * data, uint size) { detach(); pimpl->setRawData(data, size); return *this; }
size_t ByteArray::size() const { return size_t(pimpl->size()); }

bool ByteArray::startsWith(const ByteArray &ba) const { return pimpl->startsWith(*ba.pimpl); }
bool ByteArray::startsWith(const char *s) const { return pimpl->startsWith(s); }
bool ByteArray::startsWith(char c) const { return pimpl->startsWith(c); }

//ByteArray&	ByteArray::operator=(const ByteArray &other) { pimpl->operator  = (*other.pimpl); return *this;}
ByteArray&	ByteArray::operator = (const char *str) { detach(); pimpl->operator  = (str); return *this;}

bool operator == (const ByteArray &s1, const ByteArray &s2) { return *s1.pimpl == *s2.pimpl; }


ByteArray ByteArray::number(double n, char format, int precision)
{
	ByteArray ba;
	*ba.pimpl = QByteArray::number(n, format, precision);
	return ba;
}

ByteArray ByteArray::number(float n, char format, int precision)
{
	ByteArray ba;
	*ba.pimpl = QByteArray::number(n, format, precision);
	return ba;
}

ByteArray ByteArray::number(int_t n, int base)
{
	ByteArray ba;
	*ba.pimpl = QByteArray::number(n, base);
	return ba;
}

ByteArray ByteArray::number(uint_t n, int base)
{
	ByteArray ba;
	*ba.pimpl = QByteArray::number(n, base);
	return ba;
}

ByteArray ByteArray::number(long_t n, int base)
{
	ByteArray ba;
	*ba.pimpl = QByteArray::number(n, base);
	return ba;
}

ByteArray ByteArray::number(ulong_t n, int base)
{
	ByteArray ba;
	*ba.pimpl = QByteArray::number(n, base);
	return ba;
}

CWT_NS_END




