/*
 * device_impl.hpp
 *
 *  Created on: Dec 16, 2015
 *      Author: wladt
 */

#ifndef __PFS_IO_BITS_DEVICE_HPP__
#define __PFS_IO_BITS_DEVICE_HPP__

#include <pfs/platform/error_code.hpp>

namespace pfs { namespace io {

typedef pfs::platform::error_code error_code;

}}

namespace pfs { namespace io { namespace bits {

// TODO Support other platform specific native file handle types.
//
typedef int native_handle_type;

struct device
{
	typedef pfs::io::bits::native_handle_type native_handle_type;
	typedef uint32_t open_mode_flags;

    virtual ~device () {}

    virtual open_mode_flags open_mode () const = 0;

    virtual size_t  bytes_available () const = 0;

    virtual ssize_t read (byte_t bytes[], size_t n, error_code * ex) = 0;

    virtual ssize_t write (const byte_t bytes[], size_t n, error_code * ex) = 0;

    virtual bool close (error_code * ex) = 0;

    virtual bool opened () const = 0;

    virtual void flush () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual native_handle_type native_handle () const = 0;
};

}}} // pfs::io::bits

#endif /* __PFS_IO_BITS_DEVICE_IMPL_HPP__ */
