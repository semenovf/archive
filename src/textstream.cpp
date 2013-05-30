/**
 * @file   textstream.cpp
 * @author wladt
 * @date   Mar 4, 2013 12:33:42 PM
 *
 * @brief
 */

#include "../include/cwt/textstream.hpp"
#include <cstdio>

CWT_NS_BEGIN

const String TextInputStream::DEFAULT_END_LINE = _U("\n");

String TextInputStream::read(size_t len)
{
	if (m_isString) {
		CWT_ASSERT(m_string);
		if (m_pos < m_string->length()) {
			String result(m_string->substr(m_pos, len));
			m_pos += result.length();
			return result;
		}
		return String();
	}

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


String TextInputStream::readAll()
{
	if (m_isString) {
		CWT_ASSERT(m_string);
		if (m_pos < m_string->length()) {
			String result(m_string->substr(m_pos, m_string->length()));
			m_pos += result.length();
			return result;
		}
		return String();
	}

	CWT_ASSERT(m_decoder);
	CWT_ASSERT(m_dev);

	String result;
	char bytes[ChunkSize];
	ssize_t n;
	size_t nremainBytes = 0;

	while ((n = m_dev->read(bytes, ChunkSize)) > 0) {
		String s = m_decoder->toUnicode(bytes, n, &nremainBytes);
		result += s;

		if (nremainBytes > 0) {
			m_dev->unread(&bytes[0] + n - nremainBytes, nremainBytes);

			// Prevent infinite loop
			// Remain bytes stored at internal buffer of the I/O device.
			// Can read this bytes after.
			if (m_dev->atEnd()) { // no data at device
				Logger::warn(_Tr("TextStream::readAll(): remains %d bytes after decoding"), nremainBytes);
				break;
			}
		}
	}

	return result;
}

Char TextInputStream::getChar()
{
	String ch = read(1);
	return ch.length() > 0 ? ch[0] : Char();
}

String TextInputStream::readLine()
{
	const String endLines[1] = { DEFAULT_END_LINE };
	return readLine(endLines, 1);
}

String TextInputStream::readLine(const String &endLine)
{
	String result;
	Char ch;

	while ((ch = getChar()).unicode()) {
		result.append(ch);
		if (result.endsWith(endLine)) {
			result.resize(result.length() - endLine.length());
			return result;
		}
	}

	return result;
}


String TextInputStream::readLine(const String& endLines[], int count)
{
	String result;
	Char ch;

	while ((ch = getChar()).unicode()) {
		result.append(ch);
		for (int i = 0; i < count; ++i) {
			if (result.endsWith(endLines[i])) {
				result.resize(result.length() - endLines[i].length());
				return result;
			}
		}
	}

	return result;
}


ssize_t TextOutputStream::write(const String &s)
{
	if (m_isString) {
		CWT_ASSERT(m_string);
		CWT_ASSERT(m_string->length() >= CWT_SSIZE_MAX);
		m_string->append(s);
		return ssize_t(s.length());
	}

	CWT_ASSERT(m_encoder);
	CWT_ASSERT(m_dev);
	ByteArray ba = m_encoder->fromUnicode(s.data(), s.length());
	return m_dev->write(ba.data(), ba.length());
}

CWT_NS_END
