/**
 * @file   fileinputstream.cpp
 * @author wladt
 * @date   Feb 28, 2013 11:55:24 AM
 *
 * @brief
 */

#include "../include/cwt/filestream.hpp"
#include "../include/cwt/logger.hpp"
#include <unistd.h>
#include <fcntl.h>

CWT_NS_BEGIN

static int __open_file(const char *filename, int oflags, int mode = 0)
{
	#ifdef CWT_CC_GNUC
		oflags |= O_NONBLOCK;
	#endif

	#ifdef CWT_CC_MSC
	oflags |= O_BINARY;
	#endif

	int fd = ::open(filename, oflags, mode);
	if (fd < 0) {
		Logger::error(_Tr("unable to open file: %ls")
				, filename/*, cwt_str_ns()->strError(errno)*/);
	}

	return fd;
}

FileInputStream::FileInputStream(const String &filename)
	: m_fd(__open_file(filename.toUtf8().data(), O_RDONLY)) {}

FileInputStream::FileInputStream(const char *filename)
	: m_fd(__open_file(filename, O_RDONLY)) {}

FileOutputStream::FileOutputStream(const String &filename, bool append)
{
	int oflags = O_WRONLY;
	if (append)
		oflags |= O_APPEND;
	else
		oflags |= O_CREAT | O_TRUNC;
	m_fd = __open_file(filename.toUtf8().data(), oflags, S_IRUSR | S_IWUSR);
}

FileOutputStream::FileOutputStream(const char *filename, bool append)
{
	int oflags = O_WRONLY;
	if (append)
		oflags |= O_APPEND;
	else
		oflags |= O_CREAT | O_TRUNC;
	m_fd = __open_file(filename, oflags, S_IRUSR | S_IWUSR);
}

ssize_t FileInputStream::available()
{
	off_t total;
	off_t cur;

	if (m_fd < 0)
		return ssize_t(-1);

	cur = ::lseek(m_fd, 0L, SEEK_CUR);
	total = ::lseek(m_fd, 0L, SEEK_END);
	CWT_ASSERT(total >= 0L);

	::lseek(m_fd, cur, SEEK_SET);

	return (size_t)(total - cur);
}

void FileInputStream::close()
{
	if (m_fd > 0)
		::close(m_fd);
	m_fd = -1;
}

inline ssize_t FileInputStream::read(char bytes[], size_t szMax)
{
	if (m_fd < 0)
		return ssize_t(-1);
	return ::read(m_fd, bytes, szMax);
}


#define _BUF_SZ 512
ByteArray FileInputStream::readAll()
{
	ssize_t nbytes;
	char buffer[_BUF_SZ];
	ByteArray bytes;

	if (m_fd < 0)
		return bytes;

	while ((nbytes = read(buffer, _BUF_SZ)) > 0 ) {
		bytes.append(buffer, size_t(nbytes));
	}

	if (nbytes < 0) { // error
		bytes.clear();
	}

	return bytes;
}


void FileOutputStream::close()
{
	if (m_fd > 0) {
		flush();
		::close(m_fd);
	}
	m_fd = -1;
}

void FileOutputStream::flush()
{
	if(m_fd >= 0)
		::fsync(m_fd);
}

ssize_t FileOutputStream::write(const char bytes[], size_t sz)
{
	if (m_fd < 0)
		return ssize_t(-1);

	return ::write(m_fd, bytes, sz);
}

CWT_NS_END
