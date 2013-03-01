/**
 * @file   bufferedoutputstream.hpp
 * @author wladt
 * @date   Feb 28, 2013 2:25:50 PM
 *
 * @brief
 */

#ifndef __CWT_BUFFEREDOUTPUTSTREAM_HPP__
#define __CWT_BUFFEREDOUTPUTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/outputstream.hpp>

CWT_NS_BEGIN

class BufferedOutputStream : public OutputStream {
	static const int MAX_SIZE = 512;
public:
	BufferedOutputStream(OutputStream *os) : m_buffer(), m_maxSize(MAX_SIZE), m_ostream(os) {}
	BufferedOutputStream(OutputStream *os, int maxSize) : m_buffer(), m_maxSize(maxSize), m_ostream(os) {}
	virtual ~BufferedOutputStream() { close(); }

	virtual bool isNull() const { return m_ostream ? false : true; }
	virtual void close();
	virtual void flush();
	virtual ssize_t write(const char bytes[], size_t sz);
protected:
	ByteArray     m_buffer;
	int           m_maxSize;
	OutputStream *m_ostream;
};


inline void BufferedOutputStream::close()
{
	if(m_ostream) {
		flush();
		m_ostream->close();
		m_ostream = NULL;
	}
	m_buffer.clear();
}

CWT_NS_END

#endif /* __CWT_BUFFEREDOUTPUTSTREAM_HPP__ */
