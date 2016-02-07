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
	typedef device::info_type          info_type;

protected:
    shared_ptr<bits::server> _d;

protected:
    server (bits::server * p)
		: _d(p)
	{}

public:
    server ()
		: _d()
	{}

    ~server ()
    {}

    native_handle_type native_handle () const
    {
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
	error_code accept (device & peer, bool non_blocking);


	server_type type () const
	{
		return _d->type();
	}

	void set_info (info_type * info)
	{
		_d->set_info(info);
	}

	const info_type * info () const
	{
		return _d->info();
	}

	info_type * info ()
	{
		return  _d->info();
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
	friend server open_server (const open_params<ServerTag> &, error_code & ex);
};

template <typename ServerTag>
server open_server (const open_params<ServerTag> &, error_code & ex);

}} // pfs::io

#endif /* __PFS_IO_SERVER_HPP__ */
