/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   Jan 24, 2013 4:15:33 PM
 *
 * @brief
 */

#include <cwt/bytearray.hpp>
#include <QtCore/QByteArray>

#define _IMPL reinterpret_cast<QByteArray*>(__impl)
#define _CONST_IMPL reinterpret_cast<const QByteArray*>(__impl)
#define _CAST(s) reinterpret_cast<QByteArray*>(s)
#define _CONST_CAST(s) reinterpret_cast<const QByteArray*>(s)


CWT_NS_BEGIN

ByteArray::ByteArray() : __impl(new QByteArray()) {}
ByteArray::ByteArray(const char *data, int size) : __impl(new QByteArray(data, size)) {}
ByteArray::ByteArray(int size, char ch) : __impl(new QByteArray(size, ch)) {}
ByteArray::ByteArray(const ByteArray &other) : __impl(new QByteArray(*_CONST_CAST(other.__impl))) {}
ByteArray::~ByteArray() { delete _IMPL; }

char* ByteArray::data() { return _IMPL->data(); }
const char* ByteArray::data() const { return _CONST_IMPL->data(); }
ByteArray& ByteArray::setRawData(const char * data, uint size) { _IMPL->setRawData(data, size); return *this; }

bool operator==(const ByteArray &s1, const ByteArray &s2) {	return *_CONST_CAST(s1.__impl) == *_CONST_CAST(s2.__impl); }

CWT_NS_END




