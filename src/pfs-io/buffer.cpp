/*
 * buffer.cpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#include "pfs/io/buffer.hpp"
#include <pfs/array.hpp>

namespace pfs { namespace io { namespace details {

struct buffer : public bits::device
{
	static const size_t default_buffer_size = 256;

    typedef array<byte_t> buffer_type;

    buffer_type _buffer;
    size_t _pos;

    buffer (byte_t a[], size_t n)
        : _buffer(a, n)
        , _pos(0)
    {}

    buffer (size_t sz)
        : _buffer(sz, true)
        , _pos(0)
    {}

    virtual open_mode_flags open_mode () const
    {
    	return io::device::read_write | io::device::non_blocking;
    }

    virtual size_t  bytes_available () const
    {
    	 return _buffer.size() - _pos;
    }

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);

    virtual bool close (error_code * ex)
    {
    	PFS_UNUSED(ex);
    	return true;
    }

    virtual bool opened () const
    {
    	return true;
    }

    virtual void flush ()
    { }

    virtual bool set_nonblocking (bool on)
    {
    	PFS_UNUSED(on);
    	return true;
    }

    virtual native_handle_type native_handle () const
    {
    	return -1;
    }
};

ssize_t buffer::read (byte_t * bytes, size_t n, error_code * ex)
{
	PFS_UNUSED(ex);

    if (_pos >= _buffer.size())
        return 0;

    n = pfs::min(n, _buffer.size() - _pos);
    buffer_type::copy(bytes, _buffer,  _pos, n);
    _pos += n;

    return integral_cast_check<ssize_t>(n);
}

ssize_t buffer::write (const byte_t * bytes, size_t n, error_code * ex)
{
	PFS_UNUSED(ex);
    PFS_ASSERT(max_value<size_t>() - _pos >= n);

    size_t size = _buffer.size() - _pos;

    if (size < n)
        _buffer.realloc(_buffer.size() + n - size);

    buffer_type::copy(_buffer, bytes, _pos, n);

    return integral_cast_check<ssize_t>(n);
}

}}} // pfs::io::details

namespace pfs { namespace io {

template <>
error_code open_device<buffer> (device & d, const open_params<buffer> & op)
{
    if (d.opened())
    	return error_code();

    bits::device * p = 0;

    if (op.pbytes) {
    	p = new details::buffer(op.pbytes, op.size);
    } else if (op.size > 0) {
    	p = new details::buffer(op.size);
    } else {
    	p = new details::buffer(details::buffer::default_buffer_size);
    }

	device dd(p);
	d.swap(dd);

    return error_code();
}

}} // pfs::io
