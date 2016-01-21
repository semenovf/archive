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
#include <pfs/zlib.hpp>
#include <pfs/io/bits/device.hpp>

// TODO Apply support of device types: RandomAccessDevice and StreamDevice

namespace pfs { namespace io {

template <typename DeviceImpl>
struct open_params;

class server;

struct device_info
{
	virtual ~device_info () {}
};

class DLL_API device
{
	friend class server;

protected:
    shared_ptr<bits::device> _d;
    shared_ptr<device_info>  _info;

public:
	typedef bits::device::native_handle_type native_handle_type;
	typedef bits::device::open_mode_flags    open_mode_flags;
	typedef bits::device::state_type         state_type;
	typedef bits::device::open_mode_type     open_mode_type;

//protected:
//	device (bits::device * pd)
//		: _d(pd)
//	{}

public:
    device () {}

//    device (const device & other)
//    	: _d(other._d)
//    {}
//
//    device & operator = (const device & other)
//    {
//    	_d = other._d;
//    	return *this;
//    }

    ~device ()
    {}

    native_handle_type native_handle () const
    {
    	PFS_ASSERT(_d);
    	return _d->native_handle();
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
		PFS_ASSERT(_d);
		return _d->open_mode() | bits::read_only;
	}

	bool is_writable () const
	{
		PFS_ASSERT(_d);
		return _d->open_mode() | bits::write_only;
	}

#if __COMMENT__
	bool is_nonblocking () const
	{
		return _d && (_oflags & bits::non_blocking);
	}
#endif

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
    	PFS_ASSERT(_d);
    	return _d->set_nonblocking(on);
    }

    /**
     * @brief Close device.
     * @return @c true if device closed successfully,
     *         @c false if error occurred while closing.
     *         In latter case error will be stored in the internal
     *         notification storage
     * @see    device::notification()
     */
	bool close (error_code * ex = 0);

	size_t available () const
	{
		PFS_ASSERT(_d);
	    return _d->bytes_available();
	}

	bool at_end () const
	{
		PFS_ASSERT(_d);
	    return _d->bytes_available() == ssize_t(0);
	}

	/**
	 * @brief Read bytes from the device.
	 */
	ssize_t read (byte_t * bytes, size_t n, error_code * ex = 0)
	{
		PFS_ASSERT(_d);
		return _d->read(bytes, n, ex);
	}

    ssize_t read (char * chars, size_t n, error_code * ex = 0)
    {
        return read(reinterpret_cast<byte_t *>(chars), n, ex);
    }

//    /**
//     * @brief Read data from device and appends them
//     */
//    error_code read (byte_string & bytes, size_t n);

    /**
     * @brief Read data from device and appends them
     */
    error_code read (byte_string & bytes);

    error_code read (byte_string & bytes, size_t n);

    /**
     * @brief Write bytes to the device.
     */
	ssize_t write (const byte_t * bytes, size_t n, error_code * ex = 0)
	{
		PFS_ASSERT(_d);
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

	state_type state () const
	{
		PFS_ASSERT(_d);
		return _d->state();
	}

	void set_info (const device_info & info)
	{
		shared_ptr<device_info> d(new device_info(info));
		_info.swap(d);
	}

	const device_info & info () const
	{
		return *_info;
	}

	device_info & info ()
	{
		return *_info;
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
	friend error_code open_device (device &, const open_params<DeviceTag> &);

    friend bool compress (device & dest, device & src, zlib::compression_level level, size_t chunkSize, error_code * ex = 0);

    friend bool uncompress (device & dest, device & src, size_t chunkSize, error_code * ex = 0);
};

ssize_t copy (device & dest, device & src, size_t chunkSize, error_code * ex = 0);

inline bool compress (device & src, device & dest, error_code * ex = 0)
{
	return compress(src, dest, zlib::DefaultCompression, 0x4000, ex);
}

inline bool uncompress (device & src, device & dest, error_code * ex = 0)
{
	return uncompress(src, dest, 0x4000, ex);
}

template <typename DeviceTag>
error_code open_device (device &, const open_params<DeviceTag> &);

}} // pfs::io

#endif /* __PFS_IO_DEVICE_HPP__ */
