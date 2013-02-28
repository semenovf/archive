/**
 * @file   fileinputstream.hpp
 * @author wladt
 * @date   Feb 28, 2013 11:53:25 AM
 *
 * @brief
 */

#ifndef __CWT_FILEINPUTSTREAM_HPP__
#define __CWT_FILEINPUTSTREAM_HPP__

#include <cwt/cwt.h>
#include <cwt/inputstream.hpp>
#include <cwt/string.hpp>

CWT_NS_BEGIN

class FileInputStream : public InputStream {
public:
	FileInputStream(const String &filename);
	FileInputStream(int fd) : m_fd(fd) { close(); }
	~FileInputStream() { close(); }

	virtual bool isNull() const { return m_fd < 0 ? true: false; }
	virtual ssize_t available();
	virtual void close();
	virtual ssize_t read(char bytes[], size_t szMax);

private:
	int    m_fd;
};

CWT_NS_END

#endif /* __CWT_FILEINPUTSTREAM_HPP__ */
