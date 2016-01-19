/*
 * dispatcher.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: wladt
 */

#include "pfs/io/device.hpp"
#include "pfs/io/server.hpp"
#include "pfs/io/pool.hpp"

namespace pfs { namespace io {

void pool::dispatch (pool::dispatcher_context & context, short filter_events, int millis)
{
	pfs::error_code ex;

	while (not context.finish()) {
		poll_result_type result = this->poll(filter_events, millis, & ex);

		if (ex) {
			context.on_error(ex);
		} else if (result.first != result.second) {
			pfs::io::pool::iterator it = result.first;
			pfs::io::pool::iterator it_end = result.second;

			while (it != it_end) {
				pfs::io::pool::value value = *it;

				if (value.is_server()) { // accept connection
					pfs::io::device client;
					pfs::io::server server = value.get_server();

					if (not server.accept(client, true, & ex)) {
						// Acception failed
						//
						context.on_error(ex);
					} else {
						// Accepted
						//
						context.connected(client);
						this->push_back(client, filter_events);
					}
				} else {
					pfs::io::device dev = value.get_device();

					short revents = it.revents();

					if (dev.available() == 0
							&& (revents & poll_in)) { // TODO Check if this event enough to decide to disconnect.
						context.disconnected(dev);
						this->delete_differed(dev);
					} else {
						// There is urgent data to read (e.g., out-of-band data on TCP socket;
						// pseudoterminal master in packet mode has seen state change in slave).
						//
						if (revents & poll_pri) {
							context.ready_read(dev);
						}

						// There is data to read
						//
						if (revents & poll_in) {
							context.ready_read(dev);
						}

						// Writing is now possible, though a write larger that the available space
						// in a socket or pipe will still block (unless O_NONBLOCK is set).
						//
						// TODO Implement handling
						//
						if (revents & poll_out) {
							;//context.can_write(dev);
						}

						// Error condition (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_err) {
							PFS_WARN("pfs::io::pool::dispatch(): device error condition");
						}

						// Hang up (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_hup) {
							PFS_WARN("pfs::io::pool::dispatch(): device hang up");
						}

						// Invalid request: fd not open (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_nval) {
							context.on_error(error_code(BadFileDescriptorError));
						}
					}
				}

				++it;
			}
		}
	};
}

}}
