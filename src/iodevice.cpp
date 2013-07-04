/**
 * @file   iodevice.cpp
 * @author wladt
 * @date   Apr 12, 2013 11:50:05 AM
 *
 * @brief
 */

#include "../include/cwt/iodevice.hpp"
#include <cstring>
#include <cerrno>

CWT_NS_BEGIN

static const ByteArray __defaultEndLines[] = {"\n\r", "\r\n", "\r", "\n" };

ssize_t IODevice::read(char bytes[], size_t n)
{
	ssize_t ntotalbytes = ssize_t(0);
	ssize_t nbytes;

	if (!n)
		return ssize_t(0);

	if (m_head > 0) {
		m_buffer.remove(0, m_head);
		m_head = 0;
	}

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

bool IODevice::getByte(char * byte)
{
	if (m_head >= ssize_t(m_buffer.size())) {
		m_buffer.clear();
		m_head = 0;

		m_buffer.resize(BufferMaxSize);
		ssize_t nbytes = readBytes(m_buffer.data(), m_buffer.size());

		if (nbytes == 0) {
			m_buffer.clear();
			return false; // end of data
		} else if (nbytes < 0) {
			this->addSystemError(errno, _Tr("read failed"));
			return false;
		} else {
			if (size_t(nbytes) != m_buffer.size())
				m_buffer.resize(nbytes);
		}
	}

	CWT_ASSERT(m_head < ssize_t(m_buffer.size()));

	if (byte)
		*byte = m_buffer.at(m_head);
	++m_head;

	return true;
}

void IODevice::ungetByte(char byte)
{
	if (m_head > 0 && m_head <= ssize_t(m_buffer.size()))
		m_buffer[--m_head] = byte;
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
		this->addSystemError(errno, _Tr("read failed"));
	}

	return ba;
}


/*
 * Returns byte array contains line of data.
 * @c ok stores @c true if one of endLines[] has been reached.
 */
ByteArray IODevice::readLineData(const ByteArray endLines[], int count, bool * ok, size_t maxSize)
{
	char ch;
	size_t n = 0;
	ByteArray r;

	if (ok)
		*ok = false;

	while (getByte(&ch)) {

		if (n == maxSize) {
			ungetByte(ch);
			this->addError(_Tr("maximum limit exceeded"));
			return r;
		}

		r.append(ch);
		for (int i = 0; i < count; ++i) {
			if (r.endsWith(endLines[i])) {
				r.resize(r.length() - endLines[i].length());
				if (ok)
					*ok = true;
				return r;
			}
		}
	}

	return r;
}


inline ByteArray IODevice::readLine(bool * ok, size_t maxSize)
{
	return readLineData(__defaultEndLines, sizeof(__defaultEndLines)/sizeof(__defaultEndLines[0]), ok, maxSize);
}


CWT_NS_END



