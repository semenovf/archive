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

namespace cwt { namespace io {

class file::impl
{
public:
	pfs::string path;
	int    fd;

	impl () : path(nullptr), fd(-1) { ; }
	~impl ()
	{
		if (fd > 0) {
			::close(fd);
			fd = -1;
		}
	}
};

static int __perms2mode (int32_t perms)
{
	int mode = 0;
	if ((perms & file::ReadOwner) || (perms & file::ReadUser))
		mode |= S_IRUSR;
	if ((perms & file::WriteOwner) || (perms & file::WriteUser))
	    mode |= S_IWUSR;
	if ((perms & file::ExeOwner) || (perms & file::ExeUser))
		mode |= S_IXUSR;
	if (perms & file::ReadGroup)
		mode |= S_IRGRP;
	if (perms & file::WriteGroup)
		mode |= S_IWGRP;
	if (perms & file::ExeGroup)
		mode |= S_IXGRP;
	if (perms & file::ReadOther)
		mode |= S_IROTH;
	if (perms & file::WriteOther)
		mode |= S_IWOTH;
	if (perms & file::ExeOther)
		mode |= S_IXOTH;

	return mode;
}

static bool __set_permissions (const pfs::string & path, int32_t perms)
{
	PFS_ASSERT(!path.isEmpty());

	if (::chmod(path.c_str(), __perms2mode(perms)) != 0) {
		return false;
	}

	return true;
}

file::file() : _d(new file::impl)
{ }

file::file (int fd) : _d(new file::impl)
{
	_d.cast<file::impl>()->fd = ::dup(fd);
}

file::file (const pfs::string & path, int32_t oflags) : _d(new file::impl)
{
	open(path, oflags);
}

ssize_t file::readBytes (char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::read(_d.cast<file::impl>()->fd, bytes, n);
	if (sz < 0) {
		pfs::string errmsg;
		errmsg << _d.cast<file::impl>()->path << ": " << _Tr("read failure");
		this->addSystemError(errno, errmsg);
	}
	return sz;
}

ssize_t file::writeBytes (const char bytes[], size_t n)
{
	ssize_t sz;
	file::impl & d = *_d.cast<file::impl>();

	sz = ::write(d.fd, bytes, n);
	if( sz < 0 ) {
		pfs::string errmsg;
		errmsg << d.path << ": " << _Tr("write failure");
		this->addSystemError(errno, errmsg);
	}
	return sz;
}

size_t file::bytesAvailable () const
{
	const file::impl & d = *_d.cast<file::impl>();

	PFS_ASSERT(d.fd  >= 0);

	off_t cur = ::lseek(d.fd, off_t(0), SEEK_CUR);
	PFS_ASSERT(cur >= off_t(0));

	off_t total = ::lseek(d.fd, off_t(0), SEEK_END);
	PFS_ASSERT(total >= off_t(0));

	PFS_ASSERT(::lseek(d.fd, cur, SEEK_SET) >= off_t(0));
	PFS_ASSERT(total >= cur);

	return (size_t)(total - cur);
}

bool file::closeDevice ()
{
	file::impl & d = *_d.cast<file::impl>();
	bool r = true;

	if (d.fd > 0) {
		if (::close(d.fd) < 0) {
			pfs::string errmsg;
			errmsg << d.path << ": " << _Tr("close failure");
			this->addSystemError(errno, errmsg);
			r = false;
		}
	}

	d.fd = -1;
	d.path.clear();
	return r;
}

void file::flushDevice ()
{
	if (_d.cast<file::impl>()->fd >= 0)
		::fsync(_d.cast<file::impl>()->fd);
}

bool file::deviceIsOpened () const
{
	return _d.cast<file::impl>()->fd >= 0;
}

bool file::open (const pfs::string & path, int32_t oflags)
{
	int fd;
	int native_oflags = 0;

	bool created = false;

	if (oflags & device::ReadWrite) {
		native_oflags |= O_RDWR;
		native_oflags |= O_CREAT;
	} else if (oflags & device::WriteOnly) {
		native_oflags |= O_WRONLY;
		native_oflags |= O_CREAT;

		struct stat st;
		if (stat(path.c_str(), &st) != 0 && errno == ENOENT)
			created = true;
	} else {
		native_oflags |= O_RDONLY;
	}

	if (oflags & device::NonBlocking)
		native_oflags |= O_NONBLOCK;

	_d.cast<file::impl>()->path = path;

	fd = ::open(path.c_str(), native_oflags);

	if (fd < 0) {
		pfs::string errmsg;
		errmsg << path << ": open failure";
		this->addSystemError(errno, errmsg);
		return false;
	}

	_d.cast<file::impl>()->fd = fd;

	if (created) {
		if (!__set_permissions(path, file::ReadOwner | file::WriteOwner)) {
			pfs::string errmsg;
			errmsg << path << ": set file permissions failure";
			this->addSystemError(errno, errmsg);
			return false;
		}
		return true;
	}

	return true;
}


size_t file::size () const
{
	const file::impl & d = *_d.cast<file::impl>();
	PFS_ASSERT(d.fd  >= 0);

	off_t cur   = ::lseek(d.fd, 0L, SEEK_CUR);
	PFS_ASSERT(cur >= off_t(0));

	off_t begin = ::lseek(d.fd, 0L, SEEK_SET);
	PFS_ASSERT(begin >= off_t(0));

	off_t end   = ::lseek(d.fd, 0L, SEEK_END);
	PFS_ASSERT(end >= off_t(0));

	PFS_ASSERT(::lseek(d.fd, cur, SEEK_SET) >= off_t(0));

	PFS_ASSERT(begin <= end);
	return size_t(end - begin);
}

bool file::setPermissions (int32_t perms)
{
	file::impl & d = *_d.cast<file::impl>();

	if (!__set_permissions(d.path, perms)) {
		pfs::string errmsg;
		errmsg << d.path << ": set file permissions failure";
		this->addSystemError(errno, errmsg);
		return false;
	}
	return true;
}

void file::rewind()
{
	::lseek(_d.cast<file::impl>()->fd, 0L, SEEK_SET);
}

}} // cwt::io


