/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_DEVICE_HPP__
#define __PFS_IO_DEVICE_HPP__

#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/string.hpp>
#include <pfs/zlib.hpp>
#include <pfs/io/bits/device.hpp>

// TODO Apply support of device types: RandomAccessDevice and StreamDevice

namespace pfs { namespace io {

template <typename DeviceImpl>
struct open_params;

class DLL_API device
{
protected:
    bits::device * _d;

public:
	typedef bits::device::native_handle_type native_handle_type;

	enum OpenMode {
	      NotOpen     = 0                     /**< Device is not opened */
		, ReadOnly    = 0x0001                /**< Open device for read only */
		, WriteOnly   = 0x0002                /**< Open device for write only */
		, ReadWrite   = ReadOnly | WriteOnly  /**< Open device for read and write */
		, WriteRead   = ReadWrite             /**< Synonym for ReadWrite */
		, NonBlocking = 0x0004                /**< Open device in non-blocking mode */
	};

private:
	device (const device & other);
	device & operator = (const device & other);

public:
    device () : _d(0) {}
    ~device () {
        if (_d) {
            close();
        }
    }

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
    	return _d == 0;
    }

	bool is_readable () const
	{
		PFS_ASSERT(_d);
		return _d->open_mode() | ReadOnly;
	}

	bool is_writable () const
	{
		PFS_ASSERT(_d);
		return _d->open_mode() | WriteOnly;
	}

#if __COMMENT__
	bool is_nonblocking () const
	{
		return _d && (_oflags & NonBlocking);
	}
#endif

	bool opened () const
	{
	    return _d ? _d->opened() : false;
	}

	void flush ()
	{
	    if (_d)
	    	_d->flush();
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

    /**
     * @brief Read data from device and appends them
     */
    ssize_t read (byte_string & bytes, size_t n, error_code * ex = 0);

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

	ssize_t write (const byte_string & bytes, size_t n, error_code * ex = 0)
	{
	    return write(bytes.data(), pfs::min(n, bytes.size()), ex);
	}

	ssize_t write (const byte_string & bytes, error_code * ex = 0)
	{
	    return write(bytes.data(), bytes.size(), ex);
	}

    bool compress (device & dest, zlib::compression_level level, size_t chunkSize);

    bool compress (device & dest)
    {
    	return compress(dest, zlib::DefaultCompression, 0x4000);
    }

    bool uncompress (device & dest, size_t chunkSize);

    bool uncompress (device & dest)
    {
    	return uncompress(dest, 0x4000);
    }

    template <typename DeviceImpl>
    friend bool open_device (device &, const open_params<DeviceImpl> &);
};

}} // pfs::ios

#endif /* __PFS_IO_DEVICE_HPP__ */
