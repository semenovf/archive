/**
 * @file   datastream.hpp
 * @author wladt
 * @date   Feb 28, 2013 11:48:00 AM
 *
 * @brief
 */

#ifndef __CWT_DATASTREAM_HPP__
#define __CWT_DATASTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/iodevice.hpp>

CWT_NS_BEGIN

class DataStream {
public:
	DataStream() : m_dev(0) {;}
	DataStream(IODevice &dev) : m_dev(&dev) { ; }
	~DataStream() { if (m_dev) delete m_dev; }

	bool atEnd() const       { CWT_ASSERT(m_dev); return m_dev->atEnd(); }
	bool isNull() const;
	ssize_t read(char bytes[], size_t szMax);
	ssize_t write(const char bytes[], size_t sz);

private:
	IODevice *m_dev;
};


inline bool DataStream::isNull() const
{
	return m_dev && m_dev->opened() ? false : true;
}

inline ssize_t DataStream::read(char bytes[], size_t szMax)
{
	CWT_ASSERT(m_dev);
	return m_dev->read(bytes, szMax);
}

inline ssize_t DataStream::write(const char bytes[], size_t sz)
{
	CWT_ASSERT(m_dev);
	return m_dev->write(bytes, sz);
}


CWT_NS_END

#endif /* __CWT_IOSTREAM_HPP__ */
