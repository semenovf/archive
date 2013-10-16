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
	Impl() : path(nullptr), fd(-1) { ; }
	String path;
	int    fd;
};

static int __perms2mode(int32_t perms)
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

static bool __set_permissions(const String & path, int32_t perms)
{
	CWT_ASSERT(!path.isEmpty());
	if (::chmod(path.c_str(), __perms2mode(perms)) != 0) {
		Logger::error(errno
			, _Fr("Failed to change permissions to file (%s)") % path);
		return false;
	}

	return true;
}

File::File() : pimpl(new File::Impl)
{ }

File::File(int fd) : pimpl(new File::Impl)
{
	pimpl->fd = ::dup(fd);
}

File::File(const String & path, int32_t oflags) : pimpl(new File::Impl)
{
	open(path, oflags);
}

ssize_t File::readBytes(char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::read(pimpl->fd, bytes, n);
	if (sz < 0) {
		Logger::error(errno
			, _Fr("Failed to read from file (%s)") % (pimpl->path != NULL ? pimpl->path : ""));
	}
	return sz;
}

ssize_t File::writeBytes(const char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::write(pimpl->fd, bytes, n);
	if( sz < 0 ) {
		Logger::error(errno
			, _Fr("failed to write to file (%s)") % (pimpl->path != NULL ? pimpl->path : ""));
	}
	return sz;
}

size_t  File::bytesAvailable() const
{
	CWT_ASSERT(pimpl->fd  >= 0);

	off_t cur = ::lseek(pimpl->fd, off_t(0), SEEK_CUR);
	CWT_ASSERT(cur >= off_t(0));

	off_t total = ::lseek(pimpl->fd, off_t(0), SEEK_END);
	CWT_ASSERT(total >= off_t(0));

	CWT_ASSERT(::lseek(pimpl->fd, cur, SEEK_SET) >= off_t(0));
	CWT_ASSERT(total >= cur);

	return (size_t)(total - cur);
}

bool File::closeDevice()
{
	bool r = true;

	if (pimpl->fd > 0) {
		if (::close(pimpl->fd) < 0) {
			Logger::error(errno, _Tr("Failed to close file"));
			r = false;
		}
	}

	pimpl->fd = -1;
	pimpl->path.clear();
	return r;
}

void File::flushDevice()
{
	if (pimpl->fd >= 0)
		::fsync(pimpl->fd);
}


bool File::deviceIsOpened() const
{
	return pimpl->fd >= 0;
}

bool File::open(const String & path, int32_t oflags)
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

	pimpl->path = path;

	fd = ::open(path.c_str(), native_oflags);

	if (fd < 0) {
		Logger::error(errno, _Fr("Failed to open '%s'") % path);
		return false;
	}

	pimpl->fd = fd;

	if (created) {
		return __set_permissions(path.c_str(), File::ReadOwner | File::WriteOwner);
	}

	return true;
}


size_t File::size() const
{
	CWT_ASSERT(pimpl->fd  >= 0);

	off_t cur   = ::lseek(pimpl->fd, 0L, SEEK_CUR);
	CWT_ASSERT(cur >= off_t(0));

	off_t begin = ::lseek(pimpl->fd, 0L, SEEK_SET);
	CWT_ASSERT(begin >= off_t(0));

	off_t end   = ::lseek(pimpl->fd, 0L, SEEK_END);
	CWT_ASSERT(end >= off_t(0));

	CWT_ASSERT(::lseek(pimpl->fd, cur, SEEK_SET) >= off_t(0));

	CWT_ASSERT(begin <= end);
	return size_t(end - begin);
}

bool File::setPermissions(int32_t perms)
{
	return __set_permissions(pimpl->path, perms);
}

void File::rewind()
{
	::lseek(pimpl->fd, 0L, SEEK_SET);
}

} // namespace io

CWT_NS_END


