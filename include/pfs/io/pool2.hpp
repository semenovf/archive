/*
 * pool2.hpp
 *
 *  Created on: Mar 16, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_POOL2_HPP__
#define __PFS_IO_POOL2_HPP__

// FIXME
#if __FIXME__

#include <pfs/io/pool.hpp>
#include <pfs/io/server.hpp>

namespace pfs { namespace io {

class pool2 : public pool
{
private:
	template <typename Context
		, typename ConnectedHandler     // (device &, server & listener)
		, typename ReadyReadHandler     // (device &)
		, typename DisconnectedHandler  // (device &)
		, typename CanWriteHandler      // (device &)
		, typename ErrorHandler>        // (const error_code & )
	class dispatcher_context : public pool::dispatcher_context2
	{
		Context & _ctx;

	public:
		dispatcher_context (Context & ctx)
			: pool::dispatcher_context2()
			, _ctx(ctx)
		{}

		dispatcher_context (Context & ctx, int millis)
			: pool::dispatcher_context2(millis)
			, _ctx(ctx)
		{}

		dispatcher_context (Context & ctx, int millis, short filter_events)
			: pool::dispatcher_context2(millis, filter_events)
			, _ctx(ctx)
		{}

		virtual ~dispatcher_context () {}

	public:
		virtual void connected (device & d, server & listener)
		{
			ConnectedHandler(_ctx, d, listener);
		}

		virtual void ready_read (device & d)
		{
			ReadyReadHandler(_ctx, d);
		}

		virtual void disconnected (device & d)
		{
			DisconnectedHandler(_ctx, d);
		}

		virtual void can_write (device & d)
		{
			CanWriteHandler(_ctx, d);
		}

		virtual void on_error (const error_code & ex)
		{
			ErrorHandler(_ctx, ex);
		}
	};

public:
	pool2 () : pool () {}

	template <typename Context
		, typename ConnectedHandler // (device &, server & listener)
		, typename ReadyReadHandler     // (device &)
		, typename DisconnectedHandler  // (device &)
		, typename CanWriteHandler      // (device &)
		, typename ErrorHandler>        // (const error_code & )
	void dispatch (int millis = 0, short filter_events = poll_all);
};

template <typename Context
	, typename ConnectedHandler // (device &, server & listener)
	, typename ReadyReadHandler     // (device &)
	, typename DisconnectedHandler  // (device &)
	, typename CanWriteHandler      // (device &)
	, typename ErrorHandler>        // (const error_code & )
void pool2::dispatch (int millis, short filter_events)
{
	dispatcher_context
}


}}

#endif

#endif /* __PFS_IO_POOL2_HPP__ */
