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

void pool::dispatch (pool::dispatcher_context & context, int filter_events, int millis)
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
					this->push_back_differed(server, filter_events);

					if (not server.accept(client, true, & ex)) {
						// Acception failed
						context.on_error(ex);
					} else {
						// Accepted
						context.on_connected(client);
						this->push_back_differed(client, filter_events);
					}
				} else {
					pfs::io::device client = value.get_device();

					int revents = it.revents();

					if (client.available() == 0
							&& revents & poll_in) { // TODO Check if this event enough to decide to disconnect.
						context.on_disconnected(client);
						this->delete_differed(client);
					} else {
						// There is data to read
						//
						if (revents & poll_in) {
							context.on_ready_read(client);
						}

						// There is urgent data to read (e.g., out-of-band data on TCP socket;
						// pseudoterminal master in packet mode has seen state change in slave).
						//
						// TODO Implement handling
						//
						if (revents & poll_pri) {
							;
						}

						// Writing is now possible, though a write larger that the available space
						// in a socket or pipe will still block (unless O_NONBLOCK is set).
						//
						// TODO Implement handling
						//
						if (revents & poll_out) {
							;
						}

						// Error condition (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_err) {
							;
						}

						// Hang up (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_hup) {
							;
						}

						// Invalid request: fd not open (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_nval) {
							;
						}

						this->push_back_differed(client, filter_events);
					}
				}

				++it;
			}
		}

		this->update();
	};
}

}}
