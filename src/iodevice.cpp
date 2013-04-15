/**
 * @file   iodevice.cpp
 * @author wladt
 * @date   Apr 12, 2013 11:50:05 AM
 *
 * @brief
 */

#include "../include/cwt/iodevice.hpp"
#include <cstring>

CWT_NS_BEGIN


ssize_t IODevice::read(char bytes[], size_t n)
{
	ssize_t ntotalbytes = ssize_t(0);
	ssize_t nbytes;

	if (!n)
		return ssize_t(0);

	nbytes = CWT_MIN(n, m_buffer.size());

	if (nbytes > 0) {
		::memcpy(bytes, m_buffer.data(), size_t(nbytes));
		m_buffer.remove(0, nbytes);
		ntotalbytes += nbytes;
	} else { // buffer is empty
		nbytes = readBytes(bytes, n);

		if (nbytes < ssize_t(0))
			return ssize_t(-1);

		ntotalbytes += nbytes;
		m_buffer.resize(BufferMaxSize);
		nbytes = readBytes(m_buffer.data(), BufferMaxSize);

		if (nbytes > 0) {
			m_buffer.resize(nbytes);
		} else {
			m_buffer.clear();
		}
	}

	return ntotalbytes;
}

ssize_t IODevice::write(const char bytes[], size_t n)
{
	return writeBytes(bytes, n);
}


CWT_NS_END



