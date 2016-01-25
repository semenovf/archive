/*
 * server.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_SERVER_HPP__
#define __PFS_IO_SERVER_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/io/bits/server.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

template <typename ServerImpl>
struct open_params;

class server
{
public:
	typedef device::native_handle_type native_handle_type;

protected:
    shared_ptr<bits::server> _d;
    shared_ptr<device_info>  _info;

//protected:
//	server (bits::server * pd)
//		: _d(pd)
//	{}

public:
    server ()
		: _d()
	{}

    ~server ()
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

	bool opened () const
	{
	    return _d ? _d->opened() : false;
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
    error_code close ();

	/**
	 *
	 * @param
	 * @param non_blocking Sets non-blocking to the accepted device.
	 *		Using this flag saves extra calls to set_non_blocking() call.
	 *
	 * @return
	 */
	bool accept (device & peer, bool non_blocking, error_code * ex = 0);

	void set_info (device_info * info)
	{
		shared_ptr<device_info> d(info);
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

	bool operator == (const server & other)
	{
		return _d == other._d;
	}

	bool operator != (const server & other)
	{
		return ! operator == (other);
	}

	template <typename ServerTag>
	friend server open_server (const open_params<ServerTag> &, error_code * ex = 0);
};

template <typename ServerTag>
server open_server (const open_params<ServerTag> &, error_code * ex);

}} // pfs::io

#endif /* __PFS_IO_SERVER_HPP__ */
