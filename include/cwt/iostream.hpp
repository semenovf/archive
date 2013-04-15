/**
 * @file   iostream.hpp
 * @author wladt
 * @date   Feb 28, 2013 11:48:00 AM
 *
 * @brief
 */

#ifndef __CWT_INPUTSTREAM_HPP__
#define __CWT_INPUTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/iodevice.hpp>

CWT_NS_BEGIN

class InputStream {
public:
	InputStream() : m_dev(0) {;}
	InputStream(IODevice &dev) : m_dev(dev.dup()) { ; }
	~InputStream() { if (m_dev) delete m_dev; }

	bool isNull() const      { return m_dev && m_dev->opened() ? false : true; }
	ssize_t bytesAvailable() { CWT_ASSERT(m_dev); return m_dev->bytesAvailable(); }
	void close()             { CWT_ASSERT(m_dev); m_dev->close(); }
	ssize_t read(char bytes[], size_t szMax) { CWT_ASSERT(m_dev); return m_dev->read(bytes, szMax); }

private:
	IODevice *m_dev;
};


class OutputStream {
public:
	OutputStream() : m_dev(0) {;}
	OutputStream(IODevice &dev) : m_dev(dev.dup()) { ; }
	virtual ~OutputStream() { if (m_dev) delete m_dev; }

	virtual bool isNull() const { return m_dev && m_dev->opened() ? false : true; }
	virtual void close()        { CWT_ASSERT(m_dev); m_dev->close(); }
	virtual ssize_t write(const char bytes[], size_t sz) { CWT_ASSERT(m_dev); return m_dev->write(bytes, sz); }

private:
	IODevice *m_dev;
};


CWT_NS_END

#endif /* __CWT_INPUTSTREAM_HPP__ */
