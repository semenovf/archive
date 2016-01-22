/*
 * device_impl.hpp
 *
 *  Created on: Dec 16, 2015
 *      Author: wladt
 */

#ifndef __PFS_IO_BITS_DEVICE_HPP__
#define __PFS_IO_BITS_DEVICE_HPP__

#include <pfs/error_code.hpp>

namespace pfs { namespace io { namespace bits {

// TODO Support other platform specific native file handle types.
//
typedef int native_handle_type;

enum open_mode_enum
{
      not_open     = 0                       /**< Device is not opened */
	, read_only    = 0x0001                  /**< Open device for read only */
	, write_only   = 0x0002                  /**< Open device for write only */
	, read_write   = read_only | write_only  /**< Open device for read and write */
	, write_read   = read_write              /**< Synonym for read_write */
	, non_blocking = 0x0004                  /**< Open device in non-blocking mode */
};

enum state_enum
{
	  unconnected_state = 0	// The socket is not connected.
//	, host_lookup_state	= 1	// The socket is performing a host name lookup.
	, connecting_state	= 2 // The socket has started establishing a connection.
	, connected_state   = 3 // A connection is established.
	, bound_state       = 4 // The socket is bound to an address and port (for servers).
//	, closing_state     = 6 // The socket is about to close (data may still be waiting to be written).
};

struct device
{
	typedef bits::native_handle_type native_handle_type;
	typedef uint32_t       open_mode_flags;
	typedef open_mode_enum open_mode_type;
	typedef state_enum     state_type;

	device () {}

    virtual ~device () {}

    virtual open_mode_flags open_mode () const = 0;

    virtual size_t  bytes_available () const = 0;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual error_code close () = 0;

    virtual bool opened () const = 0;

    virtual void flush () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual native_handle_type native_handle () const = 0;

    virtual state_type state () const = 0;
};

}}} // pfs::io::bits

#endif /* __PFS_IO_BITS_DEVICE_HPP__ */
