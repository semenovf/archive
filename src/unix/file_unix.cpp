/*
 * device.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#include "../include/cwt/io/file.hpp"
#include <cwt/safeformat.hpp>
#include <cwt/logger.hpp>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

CWT_NS_BEGIN

namespace io {

class File::Impl {
public:
	Impl() : m_path(nullptr), m_fd(-1) { ; }
	~Impl() { close(); }

	void    flush();
	size_t  bytesAvailable() const;
	bool    close();
	bool    open(const String & path, int32_t oflags);
	ssize_t readBytes(char bytes[], size_t n);
	ssize_t writeBytes(const char bytes[], size_t n);
	size_t  size() const;

	static bool setPermissions(const String & path, int32_t perms);
	static int  permsToMode(int32_t perms);
public:
	String m_path;
	int    m_fd;
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

bool File::Impl::open(const String & path, int32_t oflags)
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
		if (stat(path.c_str(), &st) != 0 && errno == ENOENT)
			created = true;
	} else {
		native_oflags |= O_RDONLY;
	}

	if (oflags & Device::NonBlocking)
		native_oflags |= O_NONBLOCK;

	m_path = path;

	fd = ::open(path.c_str(), native_oflags);

	if (fd < 0) {
		Logger::error(errno, _Fr("Failed to open '%s'") % path);
		return false;
	}

	m_fd = fd;

	if (created) {
		return setPermissions(path.c_str(), File::ReadOwner | File::WriteOwner);
	}

	return true;
}

bool File::Impl::close()
{
	bool r = true;

	if (m_fd > 0) {
		if (::close(m_fd) < 0) {
			Logger::error(errno, _Tr("Failed to close file"));
			r = false;
		}
	}

	m_fd = -1;
	m_path.clear();
	return r;
}

ssize_t File::Impl::readBytes(char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::read(m_fd, bytes, n);
	if (sz < 0) {
		Logger::error(errno
			, _Fr("Failed to read from file (%s)") % (m_path != NULL ? m_path : ""));
	}
	return sz;
}

ssize_t File::Impl::writeBytes(const char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::write(m_fd, bytes, n);
	if( sz < 0 ) {
		Logger::error(errno
			, _Fr("failed to write to file (%s)") % (m_path != NULL ? m_path : ""));
	}
	return sz;
}

int File::Impl::permsToMode(int32_t perms)
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

bool File::Impl::setPermissions(const String & path, int32_t perms)
{
	CWT_ASSERT(!path.isEmpty());
	if (::chmod(path.c_str(), permsToMode(perms)) != 0) {
		Logger::error(errno
			, _Fr("Failed to change permissions to file (%s)") % path);
		return false;
	}

	return true;
}

File::File() : pimpl(new File::Impl) { ; }
File::File(int fd) : pimpl(new File::Impl) { pimpl->m_fd = ::dup(fd); }
File::File(const String & path, int32_t oflags) : pimpl(new File::Impl) { pimpl->open(path, oflags); }
size_t  File::bytesAvailable() const { return pimpl->bytesAvailable(); }
bool    File::open(const String & path, int32_t oflags) {	return pimpl->open(path, oflags); }
bool    File::close()   { return pimpl->close(); }
void    File::flush()   { pimpl->flush(); }
bool    File::opened() const { return pimpl->m_fd >= 0; }
ssize_t File::readBytes(char bytes[], size_t n) { return pimpl->readBytes(bytes, n); }
ssize_t File::writeBytes(const char bytes[], size_t n) { return pimpl->writeBytes(bytes, n); }

size_t File::size() const { return pimpl->size(); }
bool File::setPermissions(int32_t perms) { return pimpl->setPermissions(pimpl->m_path, perms); }
void File::rewind() { ::lseek(pimpl->m_fd, 0L, SEEK_SET); }

} // namespace io

CWT_NS_END


