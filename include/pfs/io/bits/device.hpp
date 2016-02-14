/*
 * device_impl.hpp
 *
 *  Created on: Dec 16, 2015
 *      Author: wladt
 */

#ifndef __PFS_IO_BITS_DEVICE_HPP__
#define __PFS_IO_BITS_DEVICE_HPP__

#include <pfs/error_code.hpp>

namespace pfs { namespace io {

enum open_mode_enum
{
      not_open     = 0                       /**< Device is not opened */
	, read_only    = 0x0001                  /**< Open device for read only */
	, write_only   = 0x0002                  /**< Open device for write only */
	, read_write   = read_only | write_only  /**< Open device for read and write */
	, write_read   = read_write              /**< Synonym for read_write */
	, non_blocking = 0x0004                  /**< Open device in non-blocking mode */
};

enum device_type
{
	  device_unknown = 0
	, device_buffer
	, device_file
	, device_tcp_socket
	, device_tcp_peer
};

}}

namespace pfs { namespace io { namespace bits {

// TODO Support other platform specific native file handle types.
//
typedef int native_handle_type;


struct device_context
{
	virtual ~device_context () {}
};

class basic_device
{
public:
	typedef device_context context_type;

protected:
	context_type * _ctx;

public:
	basic_device ()
		: _ctx(0)
	{}

    virtual ~basic_device ()
    {
    	if (_ctx)
    		delete _ctx;
    }

    context_type * context ()
    {
    	return _ctx;
    }

    const context_type * context () const
    {
    	return _ctx;
    }

    void set_context (context_type * context)
    {
    	if (_ctx)
    		delete _ctx;
    	_ctx = context;
    }
};

class device : public basic_device
{
public:
	typedef bits::native_handle_type native_handle_type;
	typedef uint32_t       open_mode_flags;
	typedef open_mode_enum open_mode_type;
	typedef device_context    info_type;

public:
	device ()
		: basic_device()
	{}

    virtual ~device () {}

    virtual error_code reopen () = 0;

    virtual open_mode_flags open_mode () const = 0;

    virtual size_t  bytes_available () const = 0;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual error_code close () = 0;

    virtual bool opened () const = 0;

    virtual void flush () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual native_handle_type native_handle () const = 0;

    virtual device_type type () const = 0;
};

}}} // pfs::io::bits

#endif /* __PFS_IO_BITS_DEVICE_HPP__ */
