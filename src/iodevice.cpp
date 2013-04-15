/**
 * @file   iodevice.cpp
 * @author wladt
 * @date   Apr 12, 2013 11:50:05 AM
 *
 * @brief
 */

#include "../include/cwt/iodevice.hpp"

CWT_NS_BEGIN

ByteArray IODevice::readAll()
{
	ssize_t nbytes;
	char buffer[ChunkSize];
	ByteArray bytes;

	while ((nbytes = read(buffer, ChunkSize)) > 0 ) {
		bytes.append(buffer, size_t(nbytes));
	}

	if (nbytes < 0) { // error
		bytes.clear();
	}

	return bytes;
}

CWT_NS_END



