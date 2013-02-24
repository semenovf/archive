/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   Jan 24, 2013 4:15:33 PM
 *
 * @brief
 */

#include "../include/cwt/bytearray.hpp"
#include <QtCore/QByteArray>

#define _IMPL reinterpret_cast<QByteArray*>(__impl)
#define _CONST_IMPL reinterpret_cast<const QByteArray*>(__impl)
#define _CAST(s) reinterpret_cast<QByteArray*>(s)
#define _CONST_CAST(s) reinterpret_cast<const QByteArray*>(s)


CWT_NS_BEGIN

ByteArray::ByteArray() : __impl(new QByteArray()) {}
ByteArray::ByteArray(const char *data, int size)
#if QT_VERSION < 0x050000
	: __impl(new QByteArray(data, size < 0 ? strlen(data) : size)) {}
#else
	: __impl(new QByteArray(data, size)) {}
#endif
ByteArray::ByteArray(int size, char ch) : __impl(new QByteArray(size, ch)) {}
ByteArray::ByteArray(const ByteArray &other) : __impl(new QByteArray(*_CONST_CAST(other.__impl))) {}
ByteArray::~ByteArray() { delete _IMPL; }

char* ByteArray::data() { return _IMPL->data(); }
const char* ByteArray::data() const { return _CONST_IMPL->data(); }

double	 ByteArray::toDouble(bool *ok) const           { return _CONST_IMPL->toDouble(ok); }
float	 ByteArray::toFloat(bool *ok) const            { return _CONST_IMPL->toFloat(ok); }
int_t	 ByteArray::toInt(bool *ok, int base) const    { return _CONST_IMPL->toInt(ok, base); }
long_t   ByteArray::toLong(bool *ok, int base) const   { return _CONST_IMPL->toLongLong(ok, base); }
short_t	 ByteArray::toShort(bool *ok, int base) const  { return _CONST_IMPL->toShort(ok, base); }
uint_t	 ByteArray::toUInt(bool *ok, int base) const   { return _CONST_IMPL->toUInt(ok, base); }
ulong_t	 ByteArray::toULong(bool *ok, int base) const  { return _CONST_IMPL->toULongLong(ok, base); }
ushort_t ByteArray::toUShort(bool *ok, int base) const { return _CONST_IMPL->toUShort(ok, base); }

ByteArray& ByteArray::setRawData(const char * data, uint size) { _IMPL->setRawData(data, size); return *this; }

bool operator==(const ByteArray &s1, const ByteArray &s2) {	return *_CONST_CAST(s1.__impl) == *_CONST_CAST(s2.__impl); }

CWT_NS_END




