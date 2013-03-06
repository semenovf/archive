/**
 * @file   bytearraystream.cpp
 * @author wladt
 * @date   Mar 6, 2013 10:23:40 AM
 *
 * @brief
 */

#include "../include/cwt/bytearraystream.hpp"
#include <cstring>

CWT_NS_BEGIN

ByteArrayInputStream::ByteArrayInputStream(const ByteArray &bytes, size_t offset, size_t count)
	: m_bytes(&bytes)
	, m_offset(offset)
	, m_count(count)
{
	size_t len = m_bytes->length();
	if (m_offset <= 0)
		m_offset = 0;
	if (m_offset >= len)
		m_offset = len;
	if (m_count >= len - m_offset)
		m_count = len - m_offset;
}

ssize_t ByteArrayInputStream::read(char bytes[], size_t maxSize)
{
	if (!m_count)
		return 0;

	size_t count = CWT_MIN(maxSize, m_count);

	const char *data = m_bytes->data();
	memcpy(bytes, data + m_offset, count * sizeof(byte_t));

	m_count -= count;
	m_offset += count;

	CWT_ASSERT(count <= CWT_SSIZE_MAX);
	return ssize_t(count);
}

ssize_t ByteArrayOutputStream::write(const char bytes[], size_t sz)
{
	CWT_ASSERT(sz <= CWT_SSIZE_MAX);
	m_bytes->append(bytes, sz);
	return ssize_t(sz);
}

CWT_NS_END
