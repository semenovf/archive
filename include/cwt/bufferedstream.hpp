/**
 * @file   bufferedstream.hpp
 * @author wladt
 * @date   Feb 28, 2013 3:04:36 PM
 *
 * @brief
 */

#ifndef __CWT_BUFFEREDSTREAM_HPP__
#define __CWT_BUFFEREDSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/inputstream.hpp>
#include <cwt/outputstream.hpp>

CWT_NS_BEGIN

class BufferedInputStream : public InputStream {
	static const int MAX_SIZE = 512;
public:
	BufferedInputStream(InputStream &is) : m_buffer(), m_maxSize(MAX_SIZE), m_istream(&is) { m_buffer.reserve(m_maxSize); }
	BufferedInputStream(InputStream &is, int maxSize) : m_buffer(), m_maxSize(maxSize), m_istream(&is) { m_buffer.reserve(maxSize); }
	virtual ~BufferedInputStream() { close(); }

	virtual bool isNull() const { return m_istream ? false : true; }
	virtual ssize_t available() { return m_istream ? m_buffer.size() + m_istream->available() : ssize_t(-1); }
	virtual void close();
	virtual ssize_t read(char bytes[], size_t szMax);

protected:
	ByteArray     m_buffer;
	int           m_maxSize;
	InputStream  *m_istream;
};

class BufferedOutputStream : public OutputStream {
	static const int MAX_SIZE = 512;
public:
	BufferedOutputStream(OutputStream &os) : m_buffer(), m_maxSize(MAX_SIZE), m_ostream(&os) {}
	BufferedOutputStream(OutputStream &os, int maxSize) : m_buffer(), m_maxSize(maxSize), m_ostream(&os) {}
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


inline void BufferedInputStream::close()
{
	if(m_istream) {
		m_istream->close();
		m_istream = NULL;
	}
	m_buffer.clear();
}

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

#endif /* __CWT_BUFFEREDSTREAM_HPP__ */
