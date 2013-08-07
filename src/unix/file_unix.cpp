/*
 * device.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#include "../include/cwt/io/file.hpp"
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

CWT_NS_BEGIN

namespace io {

class File::Impl {
public:
	Impl() : m_path(NULL), m_fd(-1) { ; }
	~Impl() { close(); }

	void    flush();
	size_t  bytesAvailable() const;
	int     close();
	bool    open(Errorable *ex, const char *path, int oflags);
	ssize_t readBytes(Errorable *ex, char bytes[], size_t n);
	ssize_t writeBytes(Errorable *ex, const char bytes[], size_t n);
	bool    setPermissions(Errorable *ex, int perms);
	size_t  size() const;

	static bool setPermissions(const char *path, int perms);
	static int  permsToMode(int perms);
public:
	char *m_path;
	int   m_fd;
};


void File::Impl::flush()
{
	if (m_fd > 0) {
		::fsync(m_fd);
	}
}

size_t File::Impl::bytesAvailable() const
{
	CWT_ASSERT(m_fd  >= 0);

	off_t cur = ::lseek(m_fd, off_t(0), SEEK_CUR);
	CWT_ASSERT(cur >= off_t(0));

	off_t total = ::lseek(m_fd, off_t(0), SEEK_END);
	CWT_ASSERT(total >= off_t(0));

	CWT_ASSERT(::lseek(m_fd, cur, SEEK_SET) >= off_t(0));
	CWT_ASSERT(total >= cur);

	return (size_t)(total - cur);
}

bool File::Impl::open(Errorable *ex, const char *path, int oflags)
{
	int fd;
	int native_oflags = 0;

	bool created = false;

	if (oflags & Device::ReadWrite) {
		native_oflags |= O_RDWR;
		native_oflags |= O_CREAT;
	} else if (oflags & Device::WriteOnly) {
		native_oflags |= O_WRONLY;
		native_oflags |= O_CREAT;

		struct stat st;
		if (stat(path, &st) != 0 && errno == ENOENT)
			created = true;
	} else {
		native_oflags |= O_RDONLY;
	}

	if (oflags & Device::NonBlocking)
		native_oflags |= O_NONBLOCK;

	m_path = CWT_MALLOCA(char, strlen(path)+1);
	memcpy(m_path, path, strlen(path)+1);

	fd = ::open(path, native_oflags);

	if (fd < 0) {
		ex->addSystemError(errno, _Fr("Failed to open '%s'") % path);
		return false;
	}

	m_fd = fd;

	if (created) {
		return setPermissions(path, File::ReadOwner | File::WriteOwner);
	}

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
	m_path = NULL;
	return rc;
}

ssize_t File::Impl::readBytes(Errorable *ex, char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::read(m_fd, bytes, n);
	if (sz < 0) {
		ex->addSystemError(errno
			, _Fr("Failed to read from file (%s)") % (m_path != NULL ? m_path : ""));
	}
	return sz;
}

ssize_t File::Impl::writeBytes(Errorable *ex, const char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::write(m_fd, bytes, n);
	if( sz < 0 ) {
		ex->addSystemError(errno
			, _Fr("failed to write to file (%s)") % (m_path != NULL ? m_path : ""));
	}
	return sz;
}

int File::Impl::permsToMode(int perms)
{
	int mode = 0;
	if ((perms & File::ReadOwner) || (perms & File::ReadUser))
		mode |= S_IRUSR;
	if ((perms & File::WriteOwner) || (perms & File::WriteUser))
	    mode |= S_IWUSR;
	if ((perms & File::ExeOwner) || (perms & File::ExeUser))
		mode |= S_IXUSR;
	if (perms & File::ReadGroup)
		mode |= S_IRGRP;
	if (perms & File::WriteGroup)
		mode |= S_IWGRP;
	if (perms & File::ExeGroup)
		mode |= S_IXGRP;
	if (perms & File::ReadOther)
		mode |= S_IROTH;
	if (perms & File::WriteOther)
		mode |= S_IWOTH;
	if (perms & File::ExeOther)
		mode |= S_IXOTH;

	return mode;
}

size_t File::Impl::size() const
{
	CWT_ASSERT(m_fd  >= 0);

	off_t cur   = ::lseek(m_fd, 0L, SEEK_CUR);
	CWT_ASSERT(cur >= off_t(0));

	off_t begin = ::lseek(m_fd, 0L, SEEK_SET);
	CWT_ASSERT(begin >= off_t(0));

	off_t end   = ::lseek(m_fd, 0L, SEEK_END);
	CWT_ASSERT(end >= off_t(0));

	CWT_ASSERT(::lseek(m_fd, cur, SEEK_SET) >= off_t(0));

	CWT_ASSERT(begin <= end);
	return size_t(end - begin);
}

bool File::Impl::setPermissions(Errorable *ex, int perms)
{
	CWT_ASSERT(m_path);

	if (::chmod(m_path, permsToMode(perms)) != 0) {
		ex->addSystemError(errno
			, _Fr("failed to change permissions to file (%s)") % (m_path != NULL ? m_path : ""));
		return false;
	}

	return true;
}

bool File::Impl::setPermissions(const char *path, int perms)
{
	CWT_ASSERT(path);

	if (::chmod(path, permsToMode(perms)) != 0) {
		Logger::error(_Fr("failed to change permissions to file (%s)")
				% (path != NULL ? path : ""));
		return false;
	}

	return true;
}

File::File() : pimpl(new File::Impl) { ; }
size_t  File::bytesAvailable() const { return pimpl->bytesAvailable(); }
bool    File::open(const char *path, int oflags) {	return pimpl->open(this, path, oflags); }
int     File::close()   { return pimpl->close(); }
void    File::flush()   { pimpl->flush(); }
bool    File::opened() const { return pimpl->m_fd >= 0; }
ssize_t File::readBytes(char bytes[], size_t n) { return pimpl->readBytes(this, bytes, n); }
ssize_t File::writeBytes(const char bytes[], size_t n) { return pimpl->writeBytes(this, bytes, n); }

size_t File::size() const { return pimpl->size(); }
bool File::setPermissions(int perms) { return pimpl->setPermissions(this, perms); }
bool File::setPermissions(const char *path, int perms) { return File::Impl::setPermissions(path, perms); }
void File::rewind() { ::lseek(pimpl->m_fd, 0L, SEEK_SET); }

} // namespace io

CWT_NS_END


