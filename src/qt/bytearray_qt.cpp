/**
 * @file   bytearray_qt.cpp
 * @author wladt
 * @date   Jan 24, 2013 4:15:33 PM
 *
 * @brief
 */

#include "bytearray_qt.hpp"

CWT_NS_BEGIN

ByteArray::ByteArray() : pimpl(new Impl()) {}
ByteArray::ByteArray(const char *data, int size) : pimpl(new Impl(data, size)) {}
ByteArray::ByteArray(int size, char ch) : pimpl(new Impl(size, ch)) {}
ByteArray::ByteArray(const ByteArray &other) : pimpl(new Impl(*other.pimpl)) {}

ByteArray& ByteArray::append(const ByteArray &bytes)     { pimpl->append(*bytes.pimpl); return *this; }
ByteArray& ByteArray::append(const char *data, int size) { pimpl->append(data, size); return *this; }
ByteArray& ByteArray::append(char ch)                    { pimpl->append(ch); return *this; }
void ByteArray::clear()                                  { pimpl->clear(); }
char* ByteArray::data()                                  { return pimpl->data(); }
const char* ByteArray::data() const                      { return pimpl->data(); }
size_t ByteArray::length() const                         { return size(); }
ByteArray& ByteArray::remove(int pos, int len)           { pimpl->remove(pos, len); return *this; }
void ByteArray::reserve(int size)                        { pimpl->reserve(size); }
void ByteArray::resize(int size)                         { pimpl->resize(size); }
double	 ByteArray::toDouble(bool *ok) const             { return pimpl->toDouble(ok); }
float	 ByteArray::toFloat(bool *ok) const              { return pimpl->toFloat(ok); }
int_t	 ByteArray::toInt(bool *ok, int base) const      { return pimpl->toInt(ok, base); }
long_t   ByteArray::toLong(bool *ok, int base) const     { return pimpl->toLongLong(ok, base); }
short_t	 ByteArray::toShort(bool *ok, int base) const    { return pimpl->toShort(ok, base); }
uint_t	 ByteArray::toUInt(bool *ok, int base) const     { return pimpl->toUInt(ok, base); }
ulong_t	 ByteArray::toULong(bool *ok, int base) const    { return pimpl->toULongLong(ok, base); }
ushort_t ByteArray::toUShort(bool *ok, int base) const   { return pimpl->toUShort(ok, base); }

ByteArray& ByteArray::setRawData(const char * data, uint size) { pimpl->setRawData(data, size); return *this; }
size_t ByteArray::size() const { return size_t(pimpl->size()); }

ByteArray&	ByteArray::operator=(const ByteArray &other) { pimpl->operator  = (*other.pimpl); return *this;}
ByteArray&	ByteArray::operator=(const char *str) { pimpl->operator  = (str); return *this;}

bool operator==(const ByteArray &s1, const ByteArray &s2) {	return *s1.pimpl == *s2.pimpl; }

CWT_NS_END




