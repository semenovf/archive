/**
 * @file   buffer.cpp
 * @author wladt
 * @date   Apr 12, 2013 11:20:45 AM
 *
 * @brief
 */

#include "../include/cwt/buffer.hpp"

CWT_NS_BEGIN
#ifdef __COMMENT__

void Buffer::init(const ByteArray &bytes, size_t offset, size_t count)
{
	size_t len = bytes.length();
	if (m_offset <= 0)
		m_offset = 0;
	if (m_offset >= len)
		m_offset = len;
	if (m_count >= len - m_offset)
		m_count = len - m_offset;
}


ssize_t Buffer::readBytes(char bytes[], size_t maxSize)
{
	if (!m_count)
		return 0;

	size_t count = CWT_MIN(maxSize, m_count);

	const char *data = m_data.data();
	memcpy(bytes, data + m_offset, count * sizeof(byte_t));

	m_count -= count;
	m_offset += count;

	CWT_ASSERT(count <= CWT_SSIZE_MAX);
	return ssize_t(count);
}

ssize_t Buffer::writeBytes(const char bytes[], size_t sz)
{
	CWT_ASSERT(sz <= CWT_SSIZE_MAX);
	m_data.append(bytes, sz);
	return ssize_t(sz);
}
#endif
CWT_NS_END



