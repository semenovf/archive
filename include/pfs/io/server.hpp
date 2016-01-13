/*
 * server.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_SERVER_HPP__
#define __PFS_IO_SERVER_HPP__

#include <pfs/io/bits/server.hpp>
#include <pfs/io/device.hpp>

namespace pfs { namespace io {

template <typename ServerImpl>
struct open_params;

class server
{
protected:
    bits::server * _d;

private:
	server (const server & other);
	server & operator = (const server & other);

public:
    server () : _d(0) {}

    ~server () {
        if (_d) {
            close();
        }
    }

    operator bool () const
	{
    	return opened();
	}

    bool is_null () const
    {
    	return _d == 0;
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
	bool close (error_code * ex = 0);

	/**
	 *
	 * @param
	 * @param non_blocking Sets non-blocking to the accepted device.
	 *		Using this flag saves extra calls to set_non_blocking() call.
	 *
	 * @return
	 */
	bool accept (device & peer, bool non_blocking, error_code * ex = 0);

	template <typename ServerTag>
	friend bool open_server (server &, const open_params<ServerTag> &, error_code * ex = 0);
};

template <typename ServerTag>
bool open_server (server &, const open_params<ServerTag> &, error_code * ex);

}} // pfs::io

#endif /* __PFS_IO_SERVER_HPP__ */
