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

struct file : public bits::device
{
	bits::device::native_handle_type _fd;

	file () : _fd(-1) {}

	file (bits::device::native_handle_type fd) : _fd(fd) {}

	~file ()
	{
		if (_fd > 0) {
			::close(_fd);
			_fd = -1;
		}
	}

    virtual open_mode_flags open_mode () const;

    virtual size_t bytes_available () const;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);

    virtual bool close (error_code * ex);

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual void flush ()
    {
#ifdef PFS_HAVE_FSYNC
    	::fsync(_fd);
#else
#   error "Don't know how to sync file"
#endif
    }

    virtual bool set_nonblocking (bool on)
    {
        int flags = fcntl(_fd, F_GETFL, 0);
        if (on)
        	flags |= O_NONBLOCK;
        else
        	flags &= ~O_NONBLOCK;
        return fcntl(_fd, F_SETFL, flags) >= 0;
    }

    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }
};

bits::device::open_mode_flags file::open_mode () const
{
	bits::device::open_mode_flags r = 0;
	char buf[1] = { 0 };

	if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::ReadOnly;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::WriteOnly;

	return r;
}

size_t file::bytes_available () const
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

ssize_t file::read (byte_t * bytes, size_t n, error_code * pex)
{
    ssize_t sz;

    sz = ::read(_fd, bytes, n);
    if (sz < 0 && pex) {
        *pex = errno;
    }
    return sz;
}

ssize_t file::write (const byte_t * bytes, size_t n, error_code * pex)
{
    ssize_t sz;

    sz = ::write(_fd, bytes, n);
    if( sz < 0 && pex) {
        *pex = errno;
    }
    return sz;
}

bool file::close (error_code * pex)
{
    bool r = true;

    if (_fd > 0) {
        if (::close(_fd) < 0 && pex) {
            *pex = errno;
            r = false;
        }
    }

    _fd = -1;
    return r;
}

template <>
bool open_device<file> (device & d, const open_params<file> & op, error_code * pex)
{
    if (d.opened())
        return false;

	int fd;
	int native_oflags = 0;

    if ((op.oflags & device::WriteOnly) && (op.oflags & device::ReadOnly)) {
    	native_oflags |= O_RDWR;
    	native_oflags |= O_CREAT;
    } else if (op.oflags & device::WriteOnly) {
    	native_oflags |= O_WRONLY;
    	native_oflags |= O_CREAT;
    } else if (op.oflags & device::ReadOnly) {
    	native_oflags |= O_RDONLY;
    }

	if (op.oflags & device::NonBlocking)
		native_oflags |= O_NONBLOCK;

	fd = ::open(op.path.native().c_str(), native_oflags);

	if (fd < 0) {
		if (pex)
			*pex = errno;
		return false;
	}

	d._d = new file(fd);

	return true;
}


#if __COMMENT__

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

file::file (int fd)
	: device(new file_impl)
{
    file_impl * d = dynamic_cast<file_impl *>(_d);
	d->_fd = ::dup(fd);
}

file::file (const pfs::string & path, int32_t oflags)
	: device(new file_impl)
{
	open(path, oflags);
}

file::file (const pfs::fs::path & p, int32_t oflags)
	: device(new file_impl)
{
	this->open(p.native(), oflags);
}

bool file::open (const pfs::fs::path & p, int32_t oflags)
{
	return this->open(p.native(), oflags);
}
size_t file::size () const
{
	PFS_ASSERT(_d);

    const file_impl * d = dynamic_cast<const file_impl * >(_d);

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
	PFS_ASSERT(_d);

    const file_impl * d = dynamic_cast<const file_impl * >(_d);

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
	PFS_ASSERT(_d);
	file_impl * d = dynamic_cast<file_impl * >(_d);
	::lseek(d->_fd, 0L, SEEK_SET);
}

size_t file::offset () const
{
	PFS_ASSERT(_d);
	file_impl * d = dynamic_cast<file_impl * >(_d);
	return ::lseek(d->_fd, 0L, SEEK_CUR);
}

void file::setOffset (size_t off)
{
	PFS_ASSERT(_d);
	file_impl * d = dynamic_cast<file_impl * >(_d);
	::lseek(d->_fd, off, SEEK_SET);
}

#endif

}} // cwt::io
