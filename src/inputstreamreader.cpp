/**
 * @file   inputstreamreader.cpp
 * @author wladt
 * @date   Mar 4, 2013 12:33:42 PM
 *
 * @brief
 */

#include "../include/cwt/inputstreamreader.hpp"

CWT_NS_BEGIN

ssize_t InputStreamReader::read(Char chars[], size_t sz)
{
	CWT_ASSERT(m_is);
	CWT_ASSERT(m_codec);
	CWT_ASSERT(sz <= CWT_SSIZE_MAX);

	if (!sz)
		return ssize_t(0);

	Char *pchars = chars;
	size_t ncharsTotal = 0, ncharsRemain = sz;

	while (true) {
		size_t buflen = ssize_t(m_buffer.length()); // Previous buffer length

		m_buffer.resize(BufSize);
		const char *bytes = m_buffer.data();

		// fill in internal buffer
		if (buflen < BufSize) {
			ssize_t nbytes = m_is->read(bytes + buflen, BufSize-buflen);
			if (nbytes < 0)
				return ssize_t(-1);

			if (!buflen && !nbytes) // No more bytes (at end of stream)
				return ssize_t(0);

			m_buffer.resize(buflen + size_t(nbytes));
		}

		// decode at least CWT_MIN(sz, buflen) bytes
		String str = m_codec->decode(bytes, m_buffer.length(), &m_state);

		size_t ncharsDecode = str.length();

		if (ncharsDecode > 0) {
			memcpy(pchars, str.data(), ncharsDecode * sizeof(Char));
			pchars += ncharsDecode;
			ncharsTotal += ncharsDecode;

			/* move remain chars to head of buffer */
			if (m_state.remain_chars > 0) {
				m_buffer.remove(m_buffer.length() - m_state.remain_chars, m_state.remain_chars);
				m_buffer.resize(m_state.remain_chars);
				m_state.remain_chars = 0;
			} else {
				m_buffer.clear();
			}
		}
	}

	return nchars;
}

CWT_NS_END
