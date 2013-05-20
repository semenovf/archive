/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Mar 4, 2013 12:33:42 PM
 *
 * @brief
 */

#include "../include/cwt/textstream.hpp"

CWT_NS_BEGIN


String TextStream::read(size_t len)
{
	CWT_ASSERT(m_dev);

	// TODO need to support default decoder
	CWT_ASSERT(m_decoder);

	size_t nremainBytes = 0;
	char bytes[IODevice::ChunkSize];
	String result;

	if (m_remainChars.length() > 0) {
		result.append(m_remainChars.substr(0, CWT_MIN(len, m_remainChars.length())));
		m_remainChars.remove(0, CWT_MIN(len, m_remainChars.length()));
	}

	while (result.length() < len) {
		ssize_t n = m_dev->read(bytes, len);

		if (n <= 0)
			break;

		String s = m_decoder->toUnicode(bytes, len, &nremainBytes);
		if (nremainBytes > 0)
			m_dev->unread(&bytes[0] + len - nremainBytes, nremainBytes);

		result += s;
	}

	if (result.length() > len) {
		m_remainChars.append(result.substr(len));
		result.remove(len, result.length()-len);
	}

	return result;
}


ssize_t TextStream::write(const String &s)
{
	CWT_ASSERT(m_encoder);
	CWT_ASSERT(m_dev);
	ByteArray ba = m_encoder->fromUnicode(s.data(), s.length());
	return m_dev->write(ba.data(), ba.length());
}



String TextStream::readAll()
{
	CWT_ASSERT(m_decoder);
	CWT_ASSERT(m_dev);

	String result;
	char bytes[ChunkSize];
	ssize_t n;
	size_t nremainBytes = 0;

	while ((n = m_dev->read(bytes, ChunkSize)) > 0) {
		String s = m_decoder->toUnicode(bytes, n, &nremainBytes);
		if (nremainBytes > 0)
			m_dev->unread(&bytes[0] + n - nremainBytes, nremainBytes);

		result += s;
	}

	return result;
}

CWT_NS_END
