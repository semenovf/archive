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
#include "pfs/io/server.hpp"
#include "pfs/io/pool.hpp"

namespace pfs { namespace io { namespace details {

struct pool : public bits::pool
{
	typedef struct pollfd pollfd_type;
	typedef io::device::native_handle_type native_handle_type;

	typedef vector<pollfd_type>                 fd_vector_type;
	typedef map<native_handle_type, io::device> device_map_type;
	typedef map<native_handle_type, io::server> server_map_type;

	device_map_type    device_map;
	server_map_type    server_map;
	fd_vector_type     fd_vector;
};

struct pool_iterator : public bits::pool_iterator
{
public:
	typedef details::pool::fd_vector_type::const_iterator pointer;

	int revents;
	pointer it;
	pointer it_end;

protected:
	pool_iterator (int filter_events, pointer begin, pointer end)
		: revents(filter_events)
		, it(begin)
		, it_end(end)

	{}

public:
	virtual ~pool_iterator () {}

	virtual void next ();

	static pool_iterator * alloc_begin (int filter_events, const details::pool & p);

	static pool_iterator * alloc_end (int filter_events, const details::pool & p)
	{
		return new pool_iterator(filter_events
			, static_cast<const details::pool &>(p).fd_vector.cend()
			, static_cast<const details::pool &>(p).fd_vector.cend());
	}
};

pool_iterator * pool_iterator::alloc_begin (int filter_events, const details::pool & p)
{
	pointer begin = static_cast<const details::pool &>(p).fd_vector.cbegin();

	pointer end = static_cast<const details::pool &>(p).fd_vector.cend();

	while (begin != end) {
		if (begin->revents & filter_events)
			break;
		++begin;
	}

	return new pool_iterator(filter_events, begin, end);
}

void pool_iterator::next ()
{
	while (it != it_end) {
		if (it->revents & revents)
			break;
		++it;
	}
}

}}} // pfs::io::details

namespace pfs { namespace io {

pool::pool ()
	: _d(new details::pool())
{}

void pool::push_back (const device & d, int events)
{
	if (_d.is_null())
		return;

	details::pool::pollfd_type pfd;
	pfd.fd = d.native_handle();
	pfd.events = events;

	details::pool * pdp = static_cast<details::pool *>(_d.get());

	pdp->device_map.insert(std::pair<device::native_handle_type, device>(pfd.fd, d));
	pdp->fd_vector.push_back(pfd);
}

void pool::push_back (const server & d, int events)
{
	if (_d.is_null())
		return;

	details::pool::pollfd_type pfd;
	pfd.fd = d.native_handle();
	pfd.events = events;

	details::pool * pdp = static_cast<details::pool *>(_d.get());

	pdp->server_map.insert(std::pair<server::native_handle_type, server>(pfd.fd, d));
	pdp->fd_vector.push_back(pfd);
}

pool::poll_result_type pool::poll (int filter_events
		, int millis
		, error_code * pex)
{
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	size_t n = pdp->fd_vector.size();
	details::pool::pollfd_type * pfds = pdp->fd_vector.data();

	int r = ::poll(pfds, n, millis);

	if (r > 0) {
		return poll_result_type(pool::iterator(this, details::pool_iterator::alloc_begin(filter_events, *pdp))
			, pool::iterator(this, details::pool_iterator::alloc_end(filter_events, *pdp)));
	} else if (r < 0) {
		if (pex)
			*pex = errno;
	}

	return poll_result_type(pool::iterator(), pool::iterator());
}

pool::value pool::iterator::operator * () const
{
	if (_d) {
		details::pool_iterator * details_it = static_cast<details::pool_iterator *>(_d.get());
		details::pool * details_pool = static_cast<details::pool *>(_powner->_d.get());

		details::pool::native_handle_type fd = details_it->it->fd;
		PFS_ASSERT(fd >= 0);

		// Search through servers
		//
		{
			details::pool::server_map_type::const_iterator it = details_pool->server_map.find(fd);

			if (it != details_pool->server_map.cend()) {
				return pool::value(it->second);
			}
		}

		// Search through devices
		//
		{
			details::pool::device_map_type::const_iterator it = details_pool->device_map.find(fd);

			if (it != details_pool->device_map.cend()) {
				return pool::value(it->second);
			}
		}

		PFS_ASSERT_UNEXPECTED_X("Expected server or device found at pool");
	}

	return pool::value();
}

}} // pfs::io
