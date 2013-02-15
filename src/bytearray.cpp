/**
 * @file   bytearray.cpp
 * @author wladt
 * @date   Jan 24, 2013 4:15:33 PM
 *
 * @brief
 */

#include <cwt/bytearray.hpp>

CWT_NS_BEGIN

ByteArray::ByteArray() : __impl() {}
ByteArray::ByteArray(const char *data, int size) : __impl(data, size) {}
ByteArray::ByteArray(int size, char ch) : __impl(size, ch) {}
ByteArray::ByteArray(const ByteArray &other) : __impl(other.__impl) {}
ByteArray::~ByteArray() {}

char* ByteArray::data() { return __impl.data(); }
const char* ByteArray::data() const { return __impl.data(); }
ByteArray& ByteArray::setRawData(const char * data, uint size) { __impl.setRawData(data, size); return *this; }

bool operator==(const ByteArray &s1, const ByteArray &s2) {	return s1.__impl == s2.__impl; }

CWT_NS_END




