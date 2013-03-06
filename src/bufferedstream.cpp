/**
 * @file   bufferedstream.cpp
 * @author wladt
 * @date   Feb 28, 2013 2:34:23 PM
 *
 * @brief
 */

#include "../include/cwt/bufferedstream.hpp"
#include "../include/cwt/logger.hpp"
#include <cstring>

CWT_NS_BEGIN

ssize_t BufferedInputStream::read(char bytes[], size_t sz)
{
	ssize_t ntotalbytes = ssize_t(0);
	ssize_t nbytes;

	if (!m_istream)
		return ssize_t(-1);

	if (!sz)
		return ssize_t(0);

	nbytes = CWT_MIN(sz, m_buffer.size());

	if (nbytes > 0) {
		memcpy(bytes, m_buffer.data(), size_t(nbytes));
		m_buffer.remove(0, nbytes);
		ntotalbytes += nbytes;
	} else { // buffer is empty
		nbytes = m_istream->read(bytes, sz);

		if (nbytes < ssize_t(0))
			return ssize_t(-1);

		ntotalbytes += nbytes;
		m_buffer.resize(m_maxSize);
		nbytes = m_istream->read(m_buffer.data(), m_maxSize);

		if (nbytes > 0) {
			m_buffer.resize(nbytes);
		} else {
			m_buffer.clear();
		}
	}

	return ntotalbytes;
}

ssize_t BufferedOutputStream::write(const char bytes[], size_t sz)
{
	ssize_t ntotalbytes = ssize_t(0);

	if (!m_ostream)
		return ssize_t(-1);

	if (!sz)
		return ssize_t(0);

	if (size_t(m_buffer.size()) >= sz + m_maxSize) {
		m_buffer.append(bytes, sz);
		ntotalbytes = sz;
	} else {
		ssize_t nbytes;

		if (m_buffer.size() > 0) {
			nbytes = m_ostream->write(m_buffer.data(), m_buffer.size());

			if (nbytes < 0)
				return ssize_t(-1);
			if (size_t(nbytes) < m_buffer.size()) {
				Logger::debug("not all bytes written from buffer");
				return ssize_t(-1);
			}

			m_buffer.clear();
		}

		size_t rest = sz;
		const char *ptr = bytes;

		while (rest > size_t(m_maxSize)) {
			nbytes = m_ostream->write(ptr, m_maxSize);
			if (nbytes < 0)
				return ssize_t(-1);
			ntotalbytes += nbytes;
			ptr += nbytes;
			rest -= nbytes;
		}

		if (rest > 0) {
			m_buffer.append((const char*)ptr, rest);
			ntotalbytes += rest;
		}
	}

	CWT_ASSERT(size_t(ntotalbytes) == sz);
	return ntotalbytes;
}


void BufferedOutputStream::flush()
{
	if(m_ostream) {
		if (m_buffer.size() > 0) {
			ssize_t nbytes = m_ostream->write(m_buffer.data(), m_buffer.size());

			if (nbytes < 0 || size_t(nbytes) < m_buffer.size())
				Logger::error("failed to flush data from output stream buffer");

			m_buffer.clear();
		}

		m_ostream->flush();
	}
}

CWT_NS_END
