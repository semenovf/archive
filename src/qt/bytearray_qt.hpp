/**
 * @file   bytearray_impl.hpp
 * @author wladt
 * @date   Feb 27, 2013 11:48:29 AM
 *
 * @brief
 */


#include "../include/cwt/bytearray.hpp"
#include <QByteArray>

CWT_NS_BEGIN

class ByteArray::Impl : public QByteArray {
public:
	Impl() : QByteArray() {}
	Impl(const char *data, int size)
	#if QT_VERSION < 0x050000
		: QByteArray(data, size < 0 ? strlen(data) : size) {}
	#else
		: QByteArray(data, size) {}
	#endif
	Impl(int size, char ch) : QByteArray(size, ch) {}
	Impl(const Impl &other) : QByteArray(other) {}

	Impl&	operator=(const Impl& other) { QByteArray::operator  = (other); return *this; }
	Impl&	operator=(const char * str) { QByteArray::operator  = (str); return *this; }
};

CWT_NS_END

