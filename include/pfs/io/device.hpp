/*
 * device.hpp
 *
 *  Created on: Jul 12, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_DEVICE_HPP__
#define __PFS_IO_DEVICE_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/byte_string.hpp>
#include <pfs/string.hpp>
#include <pfs/errorable_ext.hpp>
#include <pfs/zlib.hpp>

namespace pfs { namespace io {

struct device_impl
{
    virtual ~device_impl () {}

    virtual size_t  bytesAvailable () const = 0;
    virtual ssize_t readBytes      (byte_t [] /*bytes*/, size_t /*n*/, errorable_ext &) = 0;
    virtual ssize_t writeBytes     (const byte_t [] /*bytes*/, size_t /*n*/, errorable_ext &) = 0;
    virtual bool    closeDevice    (errorable_ext &) = 0;
    virtual bool    deviceIsOpened () const = 0;
    virtual void    flushDevice    () = 0;
    virtual void    setNonBlocking () = 0;
};

class DLL_API device : public errorable_ext
{
protected:
    device_impl * _d;
    uint32_t      _oflags;

public:
	typedef char char_type;

private:
	device (const device & other);
	device & operator = (const device & other);

protected:
	device (device_impl * d) : _d(d), _oflags(0) {}

	bool checkNotOpened ()
	{
	    if (opened()) {
            this->addError(_u8("device is already opened"));
            return false;
	    }
	    return true;
	}

	bool checkReadable ()
	{
        if (!isReadable()) {
            this->addError(_u8("device is not readable"));
            return false;
        }
        return true;
	}

    bool checkWritable ()
    {
        if (!isWritable()) {
            this->addError(_u8("device is not writable"));
            return false;
        }
        return true;
    }

public:
	enum OpenMode {
	      NotOpen     = 0
		, ReadOnly    = 0x0001
		, WriteOnly   = 0x0002
		, ReadWrite   = ReadOnly | WriteOnly
		, WriteRead   = ReadWrite
		, NonBlocking = 0x0004
//		, Unbuffered  = 0x0008
	};

public:
    device () : _d(nullptr), _oflags(NotOpen) {}
    ~device () {
        if (_d) {
            close();
            delete _d;
            _d = nullptr;
        }
    }

    bool isNull() const { return _d == nullptr; }

	bool isReadable () const { return _d && (_oflags & ReadOnly); }
	bool isWritable () const { return _d && (_oflags & WriteOnly); }
	bool isNonBlocking () const { return _d && (_oflags & NonBlocking); }

	bool opened () const
	{
	    return _d ? _d->deviceIsOpened() : false;
	}

	void flush ()
	{
	    if (_d) _d->flushDevice();
	}

    virtual void setNonBlocking ()
    {
        if (_d) _d->setNonBlocking();
    }

	bool close ()
	{
	    return _d ? _d->closeDevice(*this) : true;
	}

	size_t available () const
	{
	    return _d ? _d->bytesAvailable() : 0;
	}

	bool atEnd () const
	{
	    return _d ? _d->bytesAvailable() == ssize_t(0) : true;
	}

	ssize_t read (byte_t bytes[], size_t n)
	{
	    return checkReadable() ? _d->readBytes(bytes, n, *this) : -1;
	}

    ssize_t read (char chars[], size_t n)
    {
        return read(reinterpret_cast<byte_t *>(chars), n);
    }

	byte_string read (size_t n);

	ssize_t write (const byte_t bytes[], size_t n)
	{
	    return checkWritable() ? _d->writeBytes(bytes, n, *this) : -1;
	}

    ssize_t write (const char * chars, size_t n)
    {
        return write(reinterpret_cast<const byte_t *>(chars), n);
    }

	ssize_t write (const byte_string & bytes, size_t n)
	{
	    return checkWritable() ? _d->writeBytes(bytes.data(), pfs::min(n, bytes.size()), *this) : -1;
	}

	ssize_t write (const byte_string & bytes)
	{
	    return checkWritable() ? _d->writeBytes(bytes.data(), bytes.size(), *this) : -1;
	}

    bool compress (device & dest, zlib::compression_level level = zlib::DefaultCompression, size_t chunkSize = 0x4000);
    bool uncompress (device & dest, size_t chunkSize = 0x4000);
};

}} // pfs::io

#endif /* __PFS_IO_DEVICE_HPP__ */
