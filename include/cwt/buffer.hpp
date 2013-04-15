/**
 * @file   buffer.hpp
 * @author wladt
 * @date   Apr 12, 2013 11:12:54 AM
 *
 * @brief
 */

#ifndef __CWT_BUFFER_HPP__
#define __CWT_BUFFER_HPP__

#include <cwt/cwt.h>
#include <cwt/iodevice.hpp>

CWT_NS_BEGIN

#ifdef __COMMENT__

class DLL_API Buffer : public IODevice {
public:
	static const size_t ChunkSize = 512;
private:
	void init(const ByteArray &bytes, size_t offset, size_t count);

public:
	Buffer();
	Buffer(const Buffer &other);
	Buffer(const ByteArray &bytes, size_t offset = 0, size_t count = CWT_SIZE_MAX);
	virtual ~Buffer() { ; }
	bool open(int oflags) { CWT_UNUSED(oflags); return true; }

	Buffer& operator=(const Buffer &other)
	{
		m_data = other.m_data;
		return *this;
	}

	virtual size_t bytesAvailable();
	virtual int close() { return 0; }
	virtual bool opened() const { return true; }
	virtual ssize_t read(char bytes[], size_t n);
	virtual ssize_t write(const char bytes[], size_t n);
	virtual IODevice* dup() { return dynamic_cast<IODevice*>(new Buffer(*this)); }

private:
    ByteArray m_data;
	size_t m_offset;
	size_t m_count;
};

inline Buffer::Buffer()
	: m_data(), m_offset(0), m_count(0)
{
	;
}

inline Buffer::Buffer(const Buffer &other)
	: m_data(other.m_data), m_offset(0), m_count(0)
{
	init(other.m_data, other.m_offset, other.m_count);
}

inline Buffer::Buffer(const ByteArray &bytes, size_t offset, size_t count)
	: m_data(bytes), m_offset(0), m_count(0)
{
	init(bytes, offset, count);
}

inline size_t Buffer::bytesAvailable()
{
	return m_data.length() - m_offset;
}

#endif

CWT_NS_END

#endif /* __CWT_BUFFER_HPP__ */
