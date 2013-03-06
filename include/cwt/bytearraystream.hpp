/**
 * @file   bytearraystream.hpp
 * @author wladt
 * @date   Mar 6, 2013 10:12:44 AM
 *
 * @brief
 */

#ifndef __CWT_BYTEARRAYSTREAM_HPP__
#define __CWT_BYTEARRAYSTREAM_HPP__

#include <cwt/cwt.h>

#include <cwt/cwt.h>
#include <cwt/bytearray.hpp>
#include <cwt/inputstream.hpp>
#include <cwt/outputstream.hpp>

CWT_NS_BEGIN

class ByteArrayInputStream : public InputStream {
public:
	ByteArrayInputStream(const ByteArray &bytes, size_t offset = 0, size_t count = CWT_SIZE_MAX);
	virtual ~ByteArrayInputStream() { close(); }

	virtual bool isNull() const { return m_bytes ? false : true; }
	virtual ssize_t available() { return m_bytes ? m_bytes->length() - m_offset : ssize_t(-1); }
	virtual void close() { m_bytes = NULL; }
	virtual ssize_t read(char bytes[], size_t maxSize);

private:
	const ByteArray *m_bytes;
	size_t m_offset;
	size_t m_count;
};


class ByteArrayOutputStream : public OutputStream {
public:
	ByteArrayOutputStream(ByteArray &bytes) : m_bytes(&bytes) {}
	virtual ~ByteArrayOutputStream() { close(); }

	virtual bool isNull() const { return m_bytes ? false : true; }
	virtual void close() { m_bytes = NULL; }
	virtual void flush() {}
	virtual ssize_t write(const char bytes[], size_t sz);
protected:
	ByteArray *m_bytes;
};
CWT_NS_END

#endif /* __CWT_BYTEARRAYSTREAM_HPP__ */
