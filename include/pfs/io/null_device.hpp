/*
 * null_device.hpp
 *
 *  Created on: Dec 16, 2015
 *      Author: wladt
 */

#ifndef __PFS_IO_NULL_DEVICE_HPP__
#define __PFS_IO_NULL_DEVICE_HPP__

#include <pfs/io/bits/device.hpp>

namespace pfs { namespace io {

struct null_device : bits::device
{
	typedef native_handle native_handle_type;

    virtual ~null_device ()
    {}

    virtual size_t  bytes_available () const
    {
    	return 0;
    }

    virtual ssize_t read_bytes (byte_t bytes[], size_t n, error_code * ex)
    {
    	return 0;
    }

    virtual ssize_t write_bytes (const byte_t bytes[], size_t n, error_code * ex)
    {
    	return 0;
    }

    virtual bool close (error_code * ex)
    {
    	return true;
    }

    virtual bool opened () const
    {
    	return true;
    }

    virtual void flush ()
    {}

    virtual bool set_nonblocking (bool on)
    {
    	return true;
    }

    virtual native_handle_type native_handle ()
    {
    	return -1;
    }
};

}} // pfs::io

#endif /* __PFS_IO_NULL_DEVICE_HPP__ */
