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

	// First, read from buffer.
	if (nbytes > 0) {
		::memcpy(bytes, m_buffer.data(), size_t(nbytes));
		m_buffer.remove(0, nbytes);
		ntotalbytes += nbytes;
	}

	// Needs more bytes than was in buffer.
	if (size_t(nbytes) < n) {
		char *pbytes = bytes + nbytes;
		n -= nbytes;

		// Read remain bytes
		nbytes = readBytes(pbytes, n);

		if (nbytes > ssize_t(0)) {
			ntotalbytes += nbytes;
			m_buffer.resize(BufferMaxSize);
			nbytes = readBytes(m_buffer.data(), BufferMaxSize);

			if (nbytes > 0) {
				m_buffer.resize(nbytes);
			} else {
				m_buffer.clear();
			}
		}
	}

	return ntotalbytes;
}

void IODevice::unread(const char bytes[], size_t n)
{
	CWT_ASSERT(m_buffer.size() + n <= BufferMaxSize * 2);
	m_buffer.prepend(bytes, n);
}

ssize_t IODevice::write(const char bytes[], size_t n)
{
	return writeBytes(bytes, n);
}


ByteArray IODevice::readAll()
{
	ByteArray ba;
	char bytes[ChunkSize];
	ssize_t n;

	if (m_buffer.size() > 0)
		ba.append(m_buffer);

	while ( (n = readBytes(bytes, ChunkSize)) > 0 ) {
		ba.append(bytes, n);
	}

	if (n < 0) {
		; // error
	}

	return ba;
}

CWT_NS_END



