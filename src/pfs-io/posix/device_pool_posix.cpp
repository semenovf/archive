/*
 * device_pool_posix.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include <poll.h>
#include <cerrno>
#include <pfs/map.hpp>
#include <pfs/vector.hpp>
#include "pfs/io/device.hpp"
#include "pfs/io/device_pool.hpp"

namespace pfs { namespace io { namespace details {

struct device_pool : public bits::device_pool
{
	typedef struct pollfd pollfd_type;
	typedef io::device::native_handle_type native_handle_type;

	typedef vector<pollfd_type> vector_type;
	typedef map<native_handle_type, io::device> map_type;

	map_type    m;
	vector_type v;
};

}}} // pfs::io::details

namespace pfs { namespace io {

device_pool::device_pool ()
	: _d(new details::device_pool())
{}

void device_pool::push_back (const device & d, int events)
{
	if (_d.is_null())
		return;

	details::device_pool::pollfd_type pfd;
	pfd.fd = d.native_handle();
	pfd.events = events;

	details::device_pool * pdp = static_cast<details::device_pool *>(_d.get());

	pdp->m.insert(std::pair<device::native_handle_type, device>(pfd.fd, d));
	pdp->v.push_back(pfd);
}

int device_pool::poll (vector<device> & devices
		, int filter_events
		, int millis
		, error_code * pex)
{
	details::device_pool * pdp = static_cast<details::device_pool *>(_d.get());
	size_t n = pdp->v.size();
	details::device_pool::pollfd_type * pfds = pdp->v.data();

	int r = ::poll(pfds, n, millis);

	if (r > 0) {
		devices.clear();
		devices.reserve(r);

		for (size_t i = 0; i < n; ++i) {
			if (pfds[i].revents & filter_events) {
				details::device_pool::map_type::iterator it = pdp->m.find(pfds[i].fd);
				details::device_pool::map_type::iterator it_end = pdp->m.end();

				PFS_ASSERT(it != it_end);
				PFS_ASSERT(it->second.opened());

				devices.push_back(it->second);
			}
		}

		return static_cast<int>(devices.size());

	} else if (r < 0) {
		if (pex)
			*pex = errno;
	}

	return r;
}

}} // pfs::io
