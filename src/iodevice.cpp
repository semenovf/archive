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

static const ByteArray DEFAULT_END_LINES[] = {"\n\r", "\r\n", "\r", "\n" };
static const int __endLinesCount = sizeof(DEFAULT_END_LINES)/sizeof(DEFAULT_END_LINES[0]);


bool IODevice::cacheInput()
{
	CWT_ASSERT(BufferMaxSize >= m_buffer.size());
	size_t n = BufferMaxSize - m_buffer.size(); // free space

	if (n > size_t(BufferMaxSize/2)) {
		ssize_t nbytes;
		m_buffer.resize(BufferMaxSize);
		nbytes = readBytes(m_buffer.data() + m_buffer.size(), n);
		if (nbytes < 0) {
			this->addSystemError(errno, _Tr("read failed"));
			return false;
		} else {
			m_buffer.resize(m_buffer.size() + nbytes);
		}
	}
	return true;
}

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

		if (!cacheInput())
			return false;
	}

	if (m_head >= ssize_t(m_buffer.size())) {
		return false;
	}

	if (byte)
		*byte = m_buffer[m_head];
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
		; // error
	}

	return ba;
}


ByteArray IODevice::readLine(bool with_nl)
{
	return readLine(DEFAULT_END_LINES, __endLinesCount, with_nl);
}

ByteArray IODevice::readLine(const ByteArray & endLine, bool with_nl)
{
	ByteArray result;
	char ch;

	while (getByte(&ch)) {
		result.append(ch);
		if (result.endsWith(endLine)) {
			if (!with_nl)
				result.resize(result.length() - endLine.length());
			return result;
		}
	}

	if (this->isError())
		return ByteArray();

	return result;
}

ByteArray IODevice::readLine(const ByteArray endLines[], int count, bool with_nl)
{
	ByteArray result;
	char ch;

	while (getByte(&ch)) {
		result.append(ch);
		for (int i = 0; i < count; ++i) {
			if (result.endsWith(endLines[i])) {
				if (!with_nl)
					result.resize(result.length() - endLines[i].length());
				return result;
			}
		}
	}

	if (this->isError())
		return ByteArray();

	return result;
}


CWT_NS_END



