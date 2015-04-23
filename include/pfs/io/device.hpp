/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_DEVICE_HPP__
#define __PFS_IO_DEVICE_HPP__

//#include <pfs/array.hpp>
#include <pfs/pimpl.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/errorable_ext.hpp>
#include <pfs/zlib.hpp>

namespace pfs { namespace io {

struct device_impl
{
    virtual ~device_impl () {}

    virtual ssize_t readBytes (byte_t [] /*bytes*/, size_t /*n*/, errorable_ext & /*ex*/)
    {
        PFS_ASSERT_UNEXPECTED();
        return -1;
    }

    virtual ssize_t writeBytes (const byte_t [] /*bytes*/, size_t /*n*/, errorable_ext & /*ex*/)
    {
        PFS_ASSERT_UNEXPECTED();
        return -1;
    }

    virtual size_t  bytesAvailable () const
    {
        PFS_ASSERT_UNEXPECTED();
        return -1;
    }

    virtual bool closeDevice (errorable_ext & /*ex*/)
    {
        PFS_ASSERT_UNEXPECTED();
        return false;
    }

    virtual bool deviceIsOpened () const
    {
        PFS_ASSERT_UNEXPECTED();
        return false;
    }

    virtual void flushDevice ()
    {
        PFS_ASSERT_UNEXPECTED();
    }
};

class DLL_API device : public errorable_ext
{
protected:
    pimpl _d;

public:
	typedef char char_type;

private:
	device (const device & other);
	device & operator = (const device & other);

protected:
    device () {}
	device (device_impl * d) : _d(d) {}

public:
	enum OpenMode {
		  ReadOnly    = 0
		, WriteOnly   = 0x0001
		, ReadWrite   = 0x0002
		, WriteRead   = ReadWrite
		, NonBlocking = 0x0004
		, Unbuffered  = 0x0008
	};

public:
	bool isNull() const
	{
	    return _d.isNull();
	}

	bool opened () const
	{
	    return _d.isNull()
	            ? false
	            : _d.cast<device_impl>()->deviceIsOpened();
	}

	void flush ()
	{
	    if (!_d.isNull())
	        _d.cast<device_impl>()->flushDevice();
	}

	bool close ()
	{
	    return _d.isNull()
	            ? true
	            : _d.cast<device_impl>()->closeDevice(*this);
	}

	size_t available () const
	{
	    return _d.isNull()
	            ? 0
	            : _d.cast<device_impl>()->bytesAvailable();
	}

	bool atEnd () const
	{
	    return _d.isNull()
	            ? true
	            : _d.cast<device_impl>()->bytesAvailable() == ssize_t(0);
	}

	ssize_t read (byte_t bytes[], size_t n)
	{
	    return _d.isNull()
	            ? 0
	            : _d.cast<device_impl>()->readBytes(bytes, n, *this);
	}

	byte_string read (size_t n);

	ssize_t write (const byte_t bytes[], size_t n)
	{
	    return _d.isNull()
	            ? 0
	            : _d.cast<device_impl>()->writeBytes(bytes, n, *this);
	}

	ssize_t write (const byte_string & bytes, size_t n)
	{
	    return _d.isNull()
	            ? 0
	            : _d.cast<device_impl>()->writeBytes(bytes.data(), pfs::min(n, bytes.size()), *this);
	}

	ssize_t write (const byte_string & bytes)
	{
	    return _d.isNull()
	            ? 0
	            : _d.cast<device_impl>()->writeBytes(bytes.data(), bytes.size(), *this);
	}

    bool compress (device & dest, zlib::compression_level level = zlib::DefaultCompression, size_t chunkSize = 0x4000);
    bool uncompress (device & dest, size_t chunkSize = 0x4000);
};

}} // pfs::io

#endif /* __PFS_IO_DEVICE_HPP__ */
