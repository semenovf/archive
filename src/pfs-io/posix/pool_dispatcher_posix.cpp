/*
 * dispatcher.cpp
 *
 *  Created on: Jan 16, 2016
 *      Author: wladt
 */

#include <iostream>
#include <poll.h>
#include "pfs/io/device.hpp"
#include "pfs/io/server.hpp"
#include "pfs/io/pool.hpp"

#include <pfs/debug.hpp>

namespace pfs { namespace io {

#if __COMMENT__ // from bits/poll.h (linux)

|           |           |           |           |
 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        |  |     |  |  |  |  |  |  |  |  |  |  |
        |  |     |  |  |  |  |  |  |  |  |  |  ---- POLLIN     0x0001 - There is data to read.
        |  |     |  |  |  |  |  |  |  |  |  ------- POLLPRI    0x0002 - There is urgent data to read
        |  |     |  |  |  |  |  |  |  |  ---------- POLLOUT    0x0004 - Writing now will not block
        |  |     |  |  |  |  |  |  |  ------------- POLLERR    0x0008 - Error condition
        |  |     |  |  |  |  |  |  ---------------- POLLHUP    0x0010 - Hung up
        |  |     |  |  |  |  |  ------------------- POLLNVAL   0x0020 - Invalid polling request
        |  |     |  |  |  |  --------------------*- POLLRDNORM 0x0040 - Normal data may be read
        |  |     |  |  |  -----------------------*- POLLRDBAND 0x0080 - Priority data may be read
        |  |     |  |  --------------------------*- POLLWRNORM 0x0100 - Writing now will not block
        |  |     |  -----------------------------*- POLLWRBAND 0x0200 - Priority data may be written
        |  |     -------------------------------**- POLLMSG    0x0400
        |  |
        |  -------------------------------------**- POLLREMOVE 0x1000
        ----------------------------------------**- POLLRDHUP  0x2000

* - These values are defined in XPG4.2 (defined __USE_XOPEN || defined __USE_XOPEN2K8)
**- These are extensions for Linux (defined __USE_GNU)

#endif

#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#   define WR_EVENTS_XOR_MASK_XPG42 (POLLWRNORM | POLLWRBAND)
#else        
#   define WR_EVENTS_XOR_MASK_XPG42 0
#endif
        
#define WR_EVENTS_XOR_MASK (POLLOUT | WR_EVENTS_XOR_MASK_XPG42)

void pool::dispatch (pool::dispatcher_context2 const & context)
{
	pfs::error_code ex;
	poll_result_type result = this->poll(context._filter_events, context._millis, & ex);

	if (ex) {
		context.on_error(ex);
	} else if (result.first != result.second) {
		pfs::io::pool::iterator it = result.first;
		pfs::io::pool::iterator it_end = result.second;

		while (it != it_end) {
			pfs::io::pool::value value = *it;

            short revents = it.revents();
            
			if (value.is_server()) { // accept connection
                // Servers wait incoming data (to establish connection)
                // so ignore write events
                //
                if ((revents ^ WR_EVENTS_XOR_MASK) == 0) {
                    ; // TODO here may be need check opening/opened state
                } else {
                    pfs::io::server server = value.get_server();
                    process_server(server, context, revents);
                }
			} else {
				pfs::io::device dev = value.get_device();
                process_device(dev, context, revents);
			}

			++it;
		}
	}
}

void pool::process_server (pfs::io::server & server
        , pool::dispatcher_context2 const & context
        , short revents)
{
    pfs::io::device client;
    pfs::error_code ex = server.accept(client, true);

    if (ex) {
        // Acceptance failed
        //
        context.on_error(ex);
    } else {
        // Accepted
        //
        context.accepted(client, server);

        switch (server.type()) {
        case pfs::io::server_udp:
            // Process peer device on the spot
            // and release it automatically
            process_device(client, context, revents);
            break;

        case pfs::io::server_tcp:
            this->push_back(client, context._filter_events);
            break;

        default:
            PFS_DEBUG(fprintf(stderr, "**WARN: untested server type: %d\n", server.type()));
            this->push_back(client, context._filter_events);
            break;
        }
    }
}

void pool::process_device (pfs::io::device & dev
        , pool::dispatcher_context2 const & context
        , short revents)
{
    if (dev.available() == 0
            && (revents & poll_in)) { // TODO Check if this event enough to decide to disconnect.
        this->delete_deferred(dev);
        dev.close();
        context.disconnected(dev);
    } else {
        // Error condition (output only).
        //
        // TODO Research this feature and implement handling
        //
        if (revents & poll_err) {
            PFS_DEBUG(puts("pfs::io::pool::dispatch(): device error condition"));
        }

        // Hang up (output only).
        //
        if (revents & poll_hup) {
            this->delete_deferred(dev);
            dev.close();
            context.disconnected(dev);
        }

        // There is urgent data to read (e.g., out-of-band data on TCP socket;
        // pseudo-terminal master in packet mode has seen state change in slave).
        //
        if (revents & poll_pri) {
            context.ready_read(dev);
        }

        // There is data to read
        //
        if (revents & poll_in) {
            context.ready_read(dev);
        }

        // Writing is now possible, though a write larger than the available space
        // in a socket or pipe will still block (unless O_NONBLOCK is set).
        //
        if (revents & poll_out) {
            context.can_write(dev);
        }

        // Invalid request: fd not open (output only).
        //
        if (revents & poll_nval) {
            this->delete_deferred(dev);
            dev.close();
            context.on_error(error_code(BadFileDescriptorError));
        }
    }    
}


// XXX OBSOLETE, use dispatch(pool::dispatcher_context2)
// Remove this code after fixing tests
//
void pool::dispatch (pool::dispatcher_context & context, short filter_events, int millis)
{
	while (not context._quit.load()) {
		pfs::error_code ex;
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

					ex = server.accept(client, true);

					if (ex) {
						// Acception failed
						//
						context.on_error(ex);
					} else {
						// Accepted
						//
						context.accepted(client, server);
						this->push_back(client, filter_events);
					}
				} else {
					pfs::io::device dev = value.get_device();

					short revents = it.revents();

					if (dev.available() == 0
							&& (revents & poll_in)) { // TODO Check if this event enough to decide to disconnect.
						this->delete_deferred(dev);
						dev.close();
						context.disconnected(dev);
					} else {
						// Error condition (output only).
						//
						// TODO Research this feature and implement handling
						//
						if (revents & poll_err) {
							PFS_DEBUG(std::cout << "pfs::io::pool::dispatch(): device error condition" << std::endl);
						}

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
						// TODO Research this feature
						//
						if (revents & poll_out) {
							context.can_write(dev);
						}

						// Hang up (output only).
						//
						// TODO Research this feature and implement handling
						//
						if (revents & poll_hup) {
							PFS_DEBUG(std::cout << "pfs::io::pool::dispatch(): device hang up" << std::endl);
						}

						// Invalid request: fd not open (output only).
						//
						// TODO Implement handling
						//
						if (revents & poll_nval) {
							this->delete_deferred(dev);
							dev.close();
							context.on_error(error_code(BadFileDescriptorError));
						}
					}
				}

				++it;
			}
		}
	}
}

}}
