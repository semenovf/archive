/**
 * @file   file.cpp
 * @author wladt
 * @date   Apr 11, 2013 3:21:57 PM
 *
 * @brief
 */

#include "../include/cwt/file.hpp"
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

CWT_NS_BEGIN

class File::Impl {
public:
	Impl() : m_path(NULL), m_fd(-1) { ; }
	~Impl() { close(); }

	size_t bytesAvailable(Errorable *ex);
	int close();
	bool open(Errorable *ex, const char *path, int oflags);
	ssize_t readBytes(Errorable *ex, char bytes[], size_t n);
	ssize_t writeBytes(Errorable *ex, const char bytes[], size_t n);

public:
	char *m_path;
	int   m_fd;
};

size_t File::Impl::bytesAvailable(Errorable *ex)
{
	off_t total;
	off_t cur;

	CWT_UNUSED(ex);
	CWT_ASSERT(m_fd  >= 0);

	cur = ::lseek(m_fd, 0L, SEEK_CUR);
	total = ::lseek(m_fd, 0L, SEEK_END);
	CWT_ASSERT(total >= 0L);
	CWT_ASSERT(cur >= 0L);

	::lseek(m_fd, cur, SEEK_SET);
	CWT_ASSERT(total >= cur);

	return (size_t)(total - cur);
}

bool File::Impl::open(Errorable *ex, const char *path, int oflags)
{
	int fd;
	int native_oflags = 0;

	if (oflags & IODevice::ReadWrite)
		native_oflags |= O_RDWR;
	else if (oflags & IODevice::WriteOnly)
		native_oflags |= O_WRONLY;
	else
		native_oflags |= O_RDONLY;

	if (oflags & IODevice::NonBlocking)
		native_oflags |= O_NONBLOCK;

	m_path = CWT_MALLOCA(char, strlen(path)+1);
	memcpy(m_path, path, strlen(path)+1);

	fd = ::open(path, native_oflags);

	if (fd < 0) {
		ex->addSystemError(errno, String().sprintf(_Tr("failed to open '%s'"), path));
		return false;
	}
	m_fd = fd;
	return true;
}

int File::Impl::close()
{
	int rc;

	if (m_fd > 0)
		rc = ::close(m_fd);
	if (m_path)
		::free(m_path);

	m_fd = -1;
	return rc;
}

ssize_t File::Impl::readBytes(Errorable *ex, char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::read(m_fd, bytes, n);
	if (sz < 0) {
		ex->addSystemError(errno, String().sprintf(_Tr("failed to read from file (%s)")
				, m_path != NULL ? m_path : "" ));
	}
	return sz;
}

ssize_t File::Impl::writeBytes(Errorable *ex, const char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::write(m_fd, bytes, n);
	if( sz < 0 ) {
		ex->addSystemError(errno, String().sprintf(_Tr("failed to write to file (%s)")
				, m_path != NULL ? m_path : "" ));
	}
	return sz;
}


File::File() : pimpl(new File::Impl)
{
	;
}

File::File(const File &other) : pimpl(other.pimpl)
{
	;
}


bool File::open(const char *path, int oflags)
{
	return pimpl->open(this, path, oflags);
}

size_t File::bytesAvailable()
{
	return pimpl->bytesAvailable(this);
}

int File::close()
{
	return pimpl->close();
}

bool File::opened() const
{
	return pimpl->m_fd;
}

ssize_t File::readBytes(char bytes[], size_t n)
{
	return pimpl->readBytes(this, bytes, n);
}

ssize_t File::writeBytes(const char bytes[], size_t n)
{
	return pimpl->writeBytes(this, bytes, n);
}


CWT_NS_END
