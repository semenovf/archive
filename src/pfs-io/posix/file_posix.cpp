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

namespace pfs { namespace io { namespace details {

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
		r |= io::device::read_only;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::write_only;

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
        if (::close(_fd) < 0) {
        	if (pex)
        		*pex = errno;
        	r = false;
        }
    }

    _fd = -1;
    return r;
}

}}} // cwt::io::details

namespace pfs { namespace io {

static int __convert_to_native_perms (int perms)
{
	int r = 0;

	if (perms & fs::perm_user_read)   r |= S_IRUSR;
	if (perms & fs::perm_user_write)  r |= S_IWUSR;
	if (perms & fs::perm_user_exec)   r |= S_IXUSR;
	if (perms & fs::perm_group_read)  r |= S_IRGRP;
	if (perms & fs::perm_group_write) r |= S_IWGRP;
	if (perms & fs::perm_group_exec)  r |= S_IXGRP;
	if (perms & fs::perm_other_read)  r |= S_IROTH;
	if (perms & fs::perm_other_write) r |= S_IWOTH;
	if (perms & fs::perm_other_exec)  r |= S_IXOTH;

	return r;
}

template <>
error_code open_device<file> (device & d, const open_params<file> & op)
{
    if (d.opened())
        return error_code();

	int fd;
	int native_oflags = 0;
	mode_t native_mode = 0;

    if ((op.oflags & device::write_only) && (op.oflags & device::read_only)) {
    	native_oflags |= O_RDWR;
    	native_oflags |= O_CREAT;
    	native_mode   |= __convert_to_native_perms(op.permissions);
    } else if (op.oflags & device::write_only) {
    	native_oflags |= O_WRONLY;
    	native_oflags |= O_CREAT;
    	native_mode   |= __convert_to_native_perms(op.permissions);
    } else if (op.oflags & device::read_only) {
    	native_oflags |= O_RDONLY;
    }

	if (op.oflags & device::non_blocking)
		native_oflags |= O_NONBLOCK;

	fd = ::open(op.path.native().c_str(), native_oflags, native_mode);

	if (fd < 0) {
		return error_code(errno);
	}

	device dd(new details::file(fd));
	d.swap(dd);

	return error_code();
}

}} // pfs::io
