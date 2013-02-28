/**
 * @file   fileoutputstream.hpp
 * @author wladt
 * @date   Feb 28, 2013 12:22:59 PM
 *
 * @brief
 */

#ifndef __CWT_FILEOUTPUTSTREAM_HPP__
#define __CWT_FILEOUTPUTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/outputstream.hpp>

CWT_NS_BEGIN

class FileOutputStream : public OutputStream {
public:
	FileOutputStream(const String &filename, bool append = true);
	virtual ~FileOutputStream() {}

	virtual bool isNull() const { return m_fd < 0 ? true: false;  }
	virtual void close();
	virtual void flush();
	virtual ssize_t write(const byte_t bytes[], size_t sz);
private:
	int    m_fd;
};

CWT_NS_END

#endif /* __CWT_FILEOUTPUTSTREAM_HPP__ */
