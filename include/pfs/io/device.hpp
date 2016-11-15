/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_DEVICE_HPP__
#define __PFS_IO_DEVICE_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/utility.hpp>
//#include <pfs/zlib.hpp>
#include <pfs/io/bits/device.hpp>

// TODO Apply support of device types: RandomAccessDevice and StreamDevice

namespace pfs { namespace io {

template <typename DeviceImpl>
struct open_params;

class server;
class device;

class DLL_API device
{
	friend class server;

public:
	typedef bits::device::native_handle_type native_handle_type;
	typedef bits::device::open_mode_flags    open_mode_flags;
	typedef bits::device::open_mode_type     open_mode_type;
	typedef bits::device::context_type       context_type;

protected:
    shared_ptr<bits::device> _d;

protected:
    device (bits::device * p)
		: _d(p)
	{}

public:
    device () {}

    ~device ()
    {}

    native_handle_type native_handle () const
    {
    	return _d->native_handle();
    }

    error_code reopen ()
    {
    	return _d->reopen();
    }

    operator bool () const
	{
    	return opened();
	}

    bool is_null () const
    {
    	return _d.is_null();
    }

	bool is_readable () const
	{
		return _d->open_mode() | read_only;
	}

	bool is_writable () const
	{
		return _d->open_mode() | write_only;
	}

	bool is_nonblocking () const
	{
		return _d->is_nonblocking();
	}

	bool opened () const
	{
	    return _d ? _d->opened() : false;
	}

	void flush ()
	{
	    if (_d) _d->flush();
	}

    bool set_nonblocking (bool on)
    {
    	return _d->set_nonblocking(on);
    }

    /**
     * @brief Close device.
     *
     * @return error code value.
     *
     * @see    device::notification()
     */
    error_code close ();

	size_t available () const
	{
	    return _d->bytes_available();
	}

	bool at_end () const
	{
	    return _d->bytes_available() == ssize_t(0);
	}

	/**
	 * @brief Read bytes from the device.
	 */
	ssize_t read (byte_t * bytes, size_t n, error_code * ex = 0)
	{
		return _d->read(bytes, n, ex);
	}

    ssize_t read (char * chars, size_t n, error_code * ex = 0)
    {
        return read(reinterpret_cast<byte_t *>(chars), n, ex);
    }

    /**
     * @brief Read data from device and appends them
     */
    error_code read (byte_string & bytes)
    {
    	return read(bytes, available());
    }

    error_code read (byte_string & bytes, size_t n);

    /**
     * @brief Write bytes to the device.
     */
	ssize_t write (const byte_t * bytes, size_t n, error_code * ex = 0)
	{
		return _d->write(bytes, n, ex);
	}

    ssize_t write (const char * chars, size_t n, error_code * ex = 0)
    {
        return write(reinterpret_cast<const byte_t *>(chars), n, ex);
    }

    error_code write (const byte_string & bytes, size_t n)
	{
    	error_code ex;
    	this->write(bytes.data(), pfs::min(n, bytes.size()), & ex);
    	return ex;
	}

	error_code write (const byte_string & bytes)
	{
    	error_code ex;
    	this->write(bytes.data(), bytes.size(), & ex);
    	return ex;
	}

	device_type type () const
	{
		return _d->type();
	}

	void set_context (context_type * ctx)
	{
		_d->set_context(ctx);
	}

	const context_type * context () const
	{
		return _d->context();
	}

	context_type * context ()
	{
		return _d->context();
	}

    string url () const
    {
        return _d->url();
    }
    
	void swap (device & other)
	{
		_d.swap(other._d);
	}

	bool operator == (const device & other)
	{
		return _d == other._d;
	}

	bool operator != (const device & other)
	{
		return ! operator == (other);
	}

	template <typename DeviceTag>
	friend device open_device (const open_params<DeviceTag> &, error_code & ex);

//    friend bool compress (device & dest, device & src, zlib::compression_level level, size_t chunkSize, error_code * ex = 0);
//
//    friend bool uncompress (device & dest, device & src, size_t chunkSize, error_code * ex = 0);
};

ssize_t copy (device & dest, device & src, size_t chunkSize, error_code * ex = 0);

//inline bool compress (device & src, device & dest, error_code * ex = 0)
//{
//	return compress(src, dest, zlib::DefaultCompression, 0x4000, ex);
//}
//
//inline bool uncompress (device & src, device & dest, error_code * ex = 0)
//{
//	return uncompress(src, dest, 0x4000, ex);
//}

template <typename DeviceTag>
device open_device (open_params<DeviceTag> const &, error_code & ex);

template <typename DeviceTag>
inline device open_device (open_params<DeviceTag> const & op)
{
	error_code ex;
	return open_device<DeviceTag>(op, ex);
}

template <typename DeviceTag>
inline device open_device ()
{
	error_code ex;
    open_params<DeviceTag> op;
	return open_device<DeviceTag>(op, ex);
}

}} // pfs::io

#endif /* __PFS_IO_DEVICE_HPP__ */
