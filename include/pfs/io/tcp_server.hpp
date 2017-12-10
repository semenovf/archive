/*
 * tcp_server.hpp
 *
 *  Created on: Mar 16, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_TCP_SERVER_HPP__
#define __PFS_IO_TCP_SERVER_HPP__

#include <pfs/io/pool.hpp>
#include <pfs/io/listener.hpp>
#include <pfs/sigslot.hpp>

namespace pfs { namespace io {

class tcp_server
{
private:
	class dispatcher_context : public pool::dispatcher_context2
	{
		friend class tcp_sercer;

		tcp_server & _server;

	public:
		dispatcher_context (tcp_server & s, int millis = 0, short filter_events = poll_all)
			: pool::dispatcher_context2(millis, filter_events)
			, _server(s)
		{}

		virtual ~dispatcher_context () {}

	public:
		virtual void connected (device & d, server & listener)
		{
			_server._ctx.connected(d);
		}

		virtual void ready_read (device & d)
		{
			_server._ctx.ready_read(d);
		}

		virtual void disconnected (device & d)
		{
			_server._ctx.disconnected(d);
		}

		virtual void can_write (device & d)
		{
			_server._ctx.can_write(d);
		}

		virtual void on_error (const error_code & ex)
		{
			_server._ctx.error(ex);
		}
	};

private:
	pool               _pool;
	dispatcher_context _ctx;

public:
	tcp_server (listener l, int millis = 0, short filter_events = poll_all)
		: _ctx(millis, poll_all)
	{
		_pool.push_back(l);
	}

	void dispatch (int millis = 0, short filter_events = poll_all)
	{
		_pool.dispatch(_ctx);
	}

public: // signals
	signal1<device &>           connected;
	signal1<device &>           ready_read;
	signal1<device &>           disconnected;
	signal1<device &>           can_write;
	signal1<const error_code &> error;
};

}}


#endif /* __PFS_IO_TCP_SERVER_HPP__ */
