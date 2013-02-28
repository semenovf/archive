/**
 * @file   bufferedinputstream.hpp
 * @author wladt
 * @date   Feb 28, 2013 3:04:36 PM
 *
 * @brief
 */

#ifndef __CWT_BUFFEREDINPUTSTREAM_HPP__
#define __CWT_BUFFEREDINPUTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/inputstream.hpp>

CWT_NS_BEGIN

class BufferedInputStream : public InputStream {
	static const int MAX_SIZE = 512;
public:
	BufferedInputStream(InputStream *is) : m_buffer(), m_maxSize(MAX_SIZE), m_istream(is) { m_buffer.reserve(m_maxSize); }
	BufferedInputStream(InputStream *is, int maxSize) : m_buffer(), m_maxSize(maxSize), m_istream(is) {}
	virtual ~BufferedInputStream() { close(); }

	virtual bool isNull() const { return m_istream ? false : true; }
	virtual ssize_t available() { return m_istream ? m_buffer.size() + m_istream->available() : ssize_t(-1); }
	virtual void close();
	virtual ssize_t read(byte_t bytes[], size_t szMax);

protected:
	ByteArray     m_buffer;
	int           m_maxSize;
	InputStream  *m_istream;
};

inline void BufferedInputStream::close()
{
	if(m_istream) {
		m_istream->close();
		m_istream = NULL;
	}
	m_buffer.clear();
}

CWT_NS_END

#endif /* __CWT_BUFFEREDINPUTSTREAM_HPP__ */
