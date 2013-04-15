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
	CWT_UNUSED(len);
	return String();
}


ssize_t TextStream::write(const String &s)
{
	CWT_ASSERT(m_encoder);
	ByteArray ba = m_encoder->fromUnicode(s.data(), s.length());
	return m_ds->write(ba.data(), ba.length());
}



String TextStream::readAll()
{
	String chunk;
	String result;

	while (!(chunk = read(ChunkSize)).isEmpty()) // TODO reimplement
		result.append(chunk);

//	if (nchars != 0) {
//		result.clear();
//		return String(); // empty;
//	}
	return result;
}

CWT_NS_END
