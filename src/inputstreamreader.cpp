/**
 * @file   inputstreamreader.cpp
 * @author wladt
 * @date   Mar 4, 2013 12:33:42 PM
 *
 * @brief
 */

#include "../include/cwt/inputstreamreader.hpp"

CWT_NS_BEGIN


InputStreamReader::InputStreamReader(InputStream &in, const char *charset, Char replacement)
	: m_is(&in), m_decoder(NULL)
{
	if (charset) {
		m_decoder = StreamDecoder::forName(charset, &in, replacement);
	}

	if (!m_decoder)
		m_is = NULL; // isNull is true
}

void InputStreamReader::close()
{
	if (m_is) {
		m_is->close();
		m_is = NULL;
	}

	if (m_decoder) {
		m_decoder->close();
		m_decoder = NULL;
	}
}

ssize_t InputStreamReader::read(Char chars[], size_t len)
{
	CWT_ASSERT(m_is);
	CWT_ASSERT(m_decoder);
	CWT_ASSERT(len <= CWT_SSIZE_MAX);

	return m_decoder->read(chars, len);
}

CWT_NS_END
