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
#include <pfs/notification.hpp>
#include <pfs/string.hpp>
#include <pfs/zlib.hpp>
#include <pfs/io/bits/device.hpp>

// TODO Apply support of device types: RandomAccessDevice and StreamDevice

namespace pfs { namespace io {

class DLL_API device
{
protected:
    bits::device * _d;
    uint32_t       _oflags;
    notification   _nx;

public:
//	typedef char char_type;
	typedef bits::device::native_handle_type native_handle_type;

	enum OpenMode {
	      NotOpen     = 0
		, ReadOnly    = 0x0001
		, WriteOnly   = 0x0002
		, ReadWrite   = ReadOnly | WriteOnly
		, WriteRead   = ReadWrite
		, NonBlocking = 0x0004
	};

private:
	device (const device & other);
	device & operator = (const device & other);

protected:
//	device (device_impl * d) : _d(d), _oflags(0) {}

	bool check_not_opened ()
	{
	    if (opened()) {
            _nx.append(_u8("device is already opened"));
            return false;
	    }
	    return true;
	}

	bool check_readable ()
	{
        if (!is_readable()) {
            _nx.append(_u8("device is not readable"));
            return false;
        }
        return true;
	}

    bool check_writable ()
    {
        if (!is_writable()) {
            _nx.append(_u8("device is not writable"));
            return false;
        }
        return true;
    }

    void set_flags (uint32_t oflags)
    {
    	_oflags = oflags;
    }

public:
    device () : _d(nullptr), _oflags(NotOpen) {}
    ~device () {
        if (_d) {
            close();
            delete _d;
            _d = nullptr;
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

//    bool is_null () const
//    {
//    	return _d == nullptr;
//    }

	bool is_readable () const
	{
		return _d && (_oflags & ReadOnly);
	}

	bool is_writable () const
	{
		return _d && (_oflags & WriteOnly);
	}

	bool is_nonblocking () const
	{
		return _d && (_oflags & NonBlocking);
	}

	bool is_error () const
	{
		return _nx.count_error_type() > 0;
	}

	bool opened () const
	{
	    return _d ? _d->opened() : false;
	}

	void flush ()
	{
	    if (_d) _d->flush();
	}

    bool set_nonblocking (bool on);

	bool close ()
	{
		// TODO update _nx if error
		error_code ex;
	    return _d ? _d->close(& ex) : true;
	}

	size_t available () const
	{
	    return _d ? _d->bytes_available() : 0;
	}

	bool at_end () const
	{
	    return _d ? _d->bytes_available() == ssize_t(0) : true;
	}

	ssize_t read (byte_t bytes[], size_t n)
	{
		// TODO update _nx if error
		error_code ex;
	    return check_readable() ? _d->read(bytes, n, & ex) : -1;
	}

    ssize_t read (char chars[], size_t n)
    {
        return read(reinterpret_cast<byte_t *>(chars), n);
    }

    /**
     * @brief Read data from device and appends them
     */
    ssize_t read (byte_string & bytes, size_t n);

	ssize_t write (const byte_t bytes[], size_t n)
	{
		// TODO update _nx if error
		error_code ex;
	    return check_writable() ? _d->write(bytes, n, & ex) : -1;
	}

    ssize_t write (const char * chars, size_t n)
    {
        return write(reinterpret_cast<const byte_t *>(chars), n);
    }

	ssize_t write (const byte_string & bytes, size_t n)
	{
		// TODO update _nx if error
		error_code ex;
	    return check_writable() ? _d->write(bytes.data(), pfs::min(n, bytes.size()), & ex) : -1;
	}

	ssize_t write (const byte_string & bytes)
	{
		// TODO update _nx if error
		error_code ex;
	    return check_writable() ? _d->write(bytes.data(), bytes.size(), & ex) : -1;
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

#if __cplusplus < 201103L
//    template <typename DeviceImpl>
//    friend bool open_device (device &);
//
//    template <typename DeviceImpl, typename Arg1>
//    friend bool open_device (device &, const Arg1 &);
//
//    template <typename DeviceImpl, typename Arg1, typename Arg2>
//    friend bool open_device (device &, const Arg1 &, const Arg2 &);
//
//    template <typename DeviceImpl, typename Arg1, typename Arg2, typename Arg3>
//    friend bool open_device (device &, const Arg1 &, const Arg2 &, const Arg3 &);
//
//    template <typename DeviceImpl, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
//    friend bool open_device (device &, const Arg1 &, const Arg2 &, const Arg3 &, const Arg4 &);
//
//    template <typename DeviceImpl, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
//    friend bool open_device (device &, const Arg1 &, const Arg2 &, const Arg3 &, const Arg4 &, const Arg5 &);

    template <typename DeviceImpl>
    friend bool open_device (device &);

    template <typename DeviceImpl, typename Arg1>
    friend bool open_device (device &, Arg1);

    template <typename DeviceImpl, typename Arg1, typename Arg2>
    friend bool open_device (device &, Arg1, Arg2);

    template <typename DeviceImpl, typename Arg1, typename Arg2, typename Arg3>
    friend bool open_device (device &, Arg1, Arg2, Arg3);

    template <typename DeviceImpl, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    friend bool open_device (device &, Arg1, Arg2, Arg3, Arg4);

    template <typename DeviceImpl, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    friend bool open_device (device &, Arg1, Arg2, Arg3, Arg4, Arg5);

#else
#	error "Implement using variadic template"
#endif
};

}} // pfs::io

#endif /* __PFS_IO_DEVICE_HPP__ */
