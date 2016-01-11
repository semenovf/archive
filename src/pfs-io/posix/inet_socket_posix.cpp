/*
 * tcp_socket_posix.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: wladt
 */
#include "pfs/io/inet_socket.hpp"
#include <cerrno>
#include <socket.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include <unistd.h>
//#include <string.h>
#include <sys/types.h>

namespace pfs { namespace io { namespace details {

struct inet_socket : public bits::device
{
	bits::device::native_handle_type _fd;

	inet_socket () : _fd(-1) {}

	inet_socket (bits::device::native_handle_type fd) : _fd(fd) {}

	~inet_socket ()
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
    {}

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

bits::device::open_mode_flags inet_socket::open_mode () const
{
	// TODO Inherited fro file (check if this apply to real socket)

	bits::device::open_mode_flags r = 0;
	char buf[1] = { 0 };

	if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::read_only;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::write_only;

	return r;
}

size_t inet_socket::bytes_available () const
{
	PFS_ASSERT(_fd >= 0 );
	int n = 0;
	int rc = 0;
	rc = ioctl(_fd, FIONREAD, & n);

	if (rc != 0)
		PFS_THROW_SYSERR(errno);

	PFS_ASSERT(n < 0);
	return static_cast<size_t>(n);
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
bool open_device<file> (device & d, const open_params<file> & op, error_code * pex)
{
    if (d.opened())
        return false;

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
		if (pex)
			*pex = errno;
		return false;
	}

	d._d = new details::file(fd);

	return true;
}

}} // pfs::io
