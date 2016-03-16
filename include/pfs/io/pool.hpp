/*
 * device_pool.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_POOL_HPP__
#define __PFS_IO_POOL_HPP__

#include <pfs/vector.hpp>
#include <pfs/atomic.hpp>
#include <pfs/shared_ptr.hpp>
#include <pfs/io/device.hpp>
#include <pfs/io/server.hpp>
#include <pfs/io/bits/pool.hpp>

namespace pfs { namespace io {

enum poll_enum {
	  poll_in   = 0x0001  // POLLIN There is data to read.
	, poll_pri  = 0x0002  // POLLPRI There is urgent data to read (e.g., out-of-band data on TCP socket; pseudoterminal master in packet mode has seen state change in slave).
	, poll_out  = 0x0004  // POLLOUT Writing is now possible, though a write larger that the available space in a socket or pipe will still block (unless O_NONBLOCK is set).
	//, POLLRDHUP (since Linux 2.6.17) Stream socket peer closed connection, or shut down writing half of connection.  The _GNU_SOURCE feature test macro must be defined (before including any header files) in order to obtain this definition.
	, poll_err  = 0x0008  // POLLERR Error condition (output only).
	, poll_hup  = 0x0010  // POLLHUP Hang up (output only).
	, poll_nval = 0x0020  // POLLNVAL Invalid request: fd not open (output only).

	, poll_all  = 0xFFFF
};


class pool
{
public:

	enum value_enum
	{
		  Null
		, Listener
		, Device
	};

	class value
	{
		friend class pool;

		value_enum type;
		server s;
		device d;

	public:
		value ()
			: type(Null)
		{}

		value (const value & other)
			: type(other.type)
			, s(other.s)
			, d(other.d)
		{}

		value & operator = (const value & other)
		{
			type = other.type;
			s = other.s;
			d = other.d;
			return *this;
		}

		value (const server & other)
			: type(Listener)
			, s(other)
		{}

		value (const device & other)
			: type(Device)
			, d(other)
		{}

		server & get_server ()
		{
			return s;
		}

		server & get_listener ()
		{
			return s;
		}

		device & get_device ()
		{
			return d;
		}

		const server & get_server () const
		{
			return s;
		}

		const server & get_listener () const
		{
			return s;
		}

		const device & get_device () const
		{
			return d;
		}

		bool is_server () const
		{
			return type == Listener;
		}

		bool is_device () const
		{
			return type == Device;
		}

		bool operator == (const server & rhs) const
		{
			return type == Listener && s == rhs;
		}

		bool operator == (const device & rhs) const
		{
			return type == Device && d == rhs;
		}

		bool operator != (const server & rhs) const
		{
			return ! operator == (rhs);
		}

		bool operator != (const device & rhs) const
		{
			return ! operator == (rhs);
		}
	};

	class iterator
	{
		friend class pool;

		pool * _powner;
		shared_ptr<bits::pool_iterator> _d;

	protected:
		iterator (pool * owner, bits::pool_iterator * pd)
			: _powner(owner)
			, _d(pd)

		{}

	public:
		iterator ()
			: _powner(0)
		{}

		iterator & operator ++ ()
		{
			_d->next();
			return *this;
		}

		// TODO Implement
//		iterator operator ++ (int)
//		{
//		}

		value operator * () const;

		bool operator == (const iterator & rhs) const;

		bool operator != (const iterator & rhs) const
		{
			return ! operator == (rhs);
		}

		short revents () const;
};

private:
	shared_ptr<bits::pool> _d;

protected:
	pool (bits::pool * pd)
		: _d(pd)
	{}

    void swap (pool & other)
    {
    	_d.swap(other._d);
    }

public:
	pool ();

	size_t device_count () const;
	size_t server_count () const;

	void push_back (device d, short events = poll_all);
	void push_back (server s, short events = poll_all);

	void delete_deferred (device d);
	void delete_deferred (server s);

	pfs::vector<device> fetch_devices (bool (* filter) (const device & d, void * context), void * context);
	pfs::vector<server> fetch_servers (bool (* filter) (const server & s, void * context), void * context);

	typedef std::pair<pool::iterator, pool::iterator> poll_result_type;

	/**
	 * @brief Wait for some event on a file descriptor.
	 *
	 * @details The call will block until either
	 * 		@li a device becomes ready;
	 * 		@li the call is interrupted by a signal handler; or
	 * 		@li the timeout expires.
	 *
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
	poll_result_type poll (short filter_events = poll_all
			 , int millis = 0
			 , error_code * ex = 0);

	class dispatcher_context2
	{
		friend class pool;

		int   _millis;
		short _filter_events;

	public:
		dispatcher_context2 ()
			: _millis(0)
			, _filter_events(poll_all)
		{}

		dispatcher_context2 (int millis)
			: _millis(millis)
			, _filter_events(poll_all)
		{}

		dispatcher_context2 (int millis, short filter_events)
			: _millis(millis)
			, _filter_events(filter_events)
		{}

		virtual ~dispatcher_context2 () {}

	public:
		virtual void connected (device &, server & listener) {}
		virtual void ready_read (device &) {}
		virtual void disconnected (device &) {}
		virtual void can_write (device &) {}
		virtual void on_error (const error_code & ) {}
	};

	void dispatch (dispatcher_context2 & context);

	// XXX OBSOLETE, use dispatcher_context2;
	//
	class dispatcher_context
	{
		friend class pool;

		atomic_integer<int> _quit;

	public:
		dispatcher_context ()
			: _quit(0)
		{}

		virtual ~dispatcher_context () {}

		void quit ()
		{
			_quit.store(1);
		}

	public:
		virtual void connected (device &, server & listener) {}
		virtual void ready_read (device &) {}
		virtual void disconnected (device &) {}
		virtual void can_write (device &) {} // unused yet
		virtual void on_error (const error_code & ) {}
	};

	// XXX OBSOLETE, use dispatch(dispatcher_context2 context);
	void dispatch (dispatcher_context & context, short filter_events = poll_all, int millis = 0);
};

}} // pfs::io

#endif /* __PFS_IO_POOL_HPP__ */
