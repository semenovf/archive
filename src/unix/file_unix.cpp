/*
 * device.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#include "pfs/io/file.hpp"
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

namespace pfs { namespace io {

struct file_impl : public device_impl
{
	string _path;
	int    _fd;

	file_impl () : _path(nullptr), _fd(-1) { ; }
	~file_impl ()
	{
		if (_fd > 0) {
			::close(_fd);
			_fd = -1;
		}
	}
    virtual ssize_t readBytes      (byte_t bytes[], size_t n, errorable_ext & ex);
    virtual ssize_t writeBytes     (const byte_t bytes[], size_t n, errorable_ext & ex);
    virtual size_t  bytesAvailable () const;
    virtual bool    closeDevice    (errorable_ext & ex);
    virtual bool    deviceIsOpened () const;
    virtual void    flushDevice    ();
};

static int perms2mode (int32_t perms)
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

static bool set_permissions (const pfs::string & path, int32_t perms)
{
	PFS_ASSERT(!path.isEmpty());

	if (::chmod(path.c_str(), perms2mode(perms)) != 0) {
		return false;
	}

	return true;
}

ssize_t file_impl::readBytes (byte_t bytes[], size_t n, errorable_ext & ex)
{
    ssize_t sz;

    sz = ::read(_fd, bytes, n);
    if (sz < 0) {
        pfs::string errmsg;
        errmsg << _path << ": " << _Tr("read failure");
        ex.addSystemError(errno, errmsg);
    }
    return sz;
}

ssize_t file_impl::writeBytes (const byte_t bytes[], size_t n, errorable_ext & ex)
{
    ssize_t sz;

    sz = ::write(_fd, bytes, n);
    if( sz < 0 ) {
        pfs::string errmsg;
        errmsg << _path << ": " << _Tr("write failure");
        ex.addSystemError(errno, errmsg);
    }
    return sz;
}

size_t file_impl::bytesAvailable () const
{
    PFS_ASSERT(_fd  >= 0);

    off_t cur = ::lseek(_fd, off_t(0), SEEK_CUR);
    PFS_ASSERT(cur >= off_t(0));

    off_t total = ::lseek(_fd, off_t(0), SEEK_END);
    PFS_ASSERT(total >= off_t(0));

    PFS_ASSERT(::lseek(_fd, cur, SEEK_SET) >= off_t(0));
    PFS_ASSERT(total >= cur);

    return (size_t)(total - cur);
}

bool file_impl::closeDevice (errorable_ext & ex)
{
    bool r = true;

    if (_fd > 0) {
        if (::close(_fd) < 0) {
            pfs::string errmsg;
            errmsg << _path << ": " << _Tr("close failure");
            ex.addSystemError(errno, errmsg);
            r = false;
        }
    }

    _fd = -1;
    _path.clear();
    return r;
}

bool file_impl::deviceIsOpened () const
{
    return _fd >= 0;
}

void file_impl::flushDevice ()
{
    if (_fd >= 0)
        ::fsync(_fd);
}

file::file (int fd) : device(new file_impl)
{
    file_impl * d = _d.cast<file_impl>();
	d->_fd = ::dup(fd);
}

file::file (const pfs::string & path, int32_t oflags) : device(new file_impl)
{
	open(path, oflags);
}

bool file::open (const pfs::string & path, int32_t oflags)
{
	int fd;
	int native_oflags = 0;

	bool created = false;

	if (!_d.isNull()) {
	    close();
	} else {
	    pimpl d(new file_impl);
	    _d.swap<file_impl>(d);
	}

	file_impl * d = _d.cast<file_impl>();

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

	d->_path = path;

	fd = ::open(path.c_str(), native_oflags);

	if (fd < 0) {
		pfs::string errmsg;
		errmsg << path << ": open failure";
		this->addSystemError(errno, errmsg);
		return false;
	}

	d->_fd = fd;

	if (created) {
		if (!set_permissions(path, file::ReadOwner | file::WriteOwner)) {
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
    if (isNull())
        return 0;

	const file_impl * d = _d.cast<file_impl>();

	PFS_ASSERT(d->_fd  >= 0);

	off_t cur   = ::lseek(d->_fd, 0L, SEEK_CUR);
	PFS_ASSERT(cur >= off_t(0));

	off_t begin = ::lseek(d->_fd, 0L, SEEK_SET);
	PFS_ASSERT(begin >= off_t(0));

	off_t end   = ::lseek(d->_fd, 0L, SEEK_END);
	PFS_ASSERT(end >= off_t(0));

	PFS_ASSERT(::lseek(d->_fd, cur, SEEK_SET) >= off_t(0));

	PFS_ASSERT(begin <= end);
	return size_t(end - begin);
}

bool file::setPermissions (int32_t perms)
{
    if (isNull())
        return false;

    const file_impl * d = _d.cast<file_impl>();

	if (!set_permissions(d->_path, perms)) {
		pfs::string errmsg;
		errmsg << d->_path << ": set file permissions failure";
		this->addSystemError(errno, errmsg);
		return false;
	}
	return true;
}

void file::rewind ()
{
    if (!isNull()) {
        file_impl * d = _d.cast<file_impl>();
        ::lseek(d->_fd, 0L, SEEK_SET);
    }
}

}} // cwt::io
