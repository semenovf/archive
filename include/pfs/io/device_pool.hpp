/*
 * device_pool.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_DEVICE_POOL_HPP__
#define __PFS_IO_DEVICE_POOL_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/bits/device_pool.hpp>

namespace pfs { namespace io {

enum poll_enum {
	  poll_in   = 0x0001  // POLLIN There is data to read.
	, poll_pri  = 0x0002  // POLLPRI There is urgent data to read (e.g., out-of-band data on TCP socket; pseudoterminal master in packet mode has seen state change in slave).
	, poll_out  = 0x0004  // POLLOUT Writing is now possible, though a write larger that the available space in a socket or pipe will still block (unless O_NONBLOCK is set).
	//, POLLRDHUP (since Linux 2.6.17) Stream socket peer closed connection, or shut down writing half of connection.  The _GNU_SOURCE feature test macro must be defined (before including any header files) in order to obtain this definition.
	, poll_err  = 0x0008  // POLLERR Error condition (output only).
	, poll_hup  = 0x0010  // POLLHUP Hang up (output only).
	, poll_nval = 0x0020 // POLLNVAL Invalid request: fd not open (output only).

	, poll_all  = 0xFFFF
};


class device_pool
{
private:
	shared_ptr<bits::device_pool> _d;

protected:
	device_pool (bits::device_pool * pd)
		: _d(pd)
	{}

    void swap (device_pool & other)
    {
    	_d.swap(other._d);
    }

public:
	device_pool ();

	void push_back (const device & d, int events);

	/**
	 * @brief Wait for some event on a file descriptor.
	 *
	 * @details The call will block until either
	 * 		@li a device becomes ready;
	 * 		@li the call is interrupted by a signal handler; or
	 * 		@li the timeout expires.
	 *
	 * @param devices
	 * @param filter_events
	 * @param millis The timeout argument specifies the number of milliseconds
	 * 		that poll() should block waiting for a device to become ready.
	 * 		Zero value Causes poll() to return immediately, even if no devices
	 * 		are ready.
	 * @param devices List of devices become ready.
	 * @param ex Pointer to store error code on error.
	 *
	 * @return On success, a positive number is returned;
	 * 		this is the number of descriptors with events or errors reported.
	 * 		A value of 0 indicates that the call timed out and no file
	 * 		descriptors were ready.
	 * 		On error, -1 is returned, and @a *ex is set appropriately.
	 */
	int poll (vector<device> & devices, int filter_events = poll_all, int millis = 0, error_code * ex = 0);
};

}} // pfs::io


#endif /* __PFS_IO_DEVICE_POOL_HPP__ */
