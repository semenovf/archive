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

/* References:
 * 1. [select()—Synchronous I/O Multiplexing](http://beej.us/guide/bgnet/output/html/multipage/advanced.html)
 * 2. [7.3. Handling Partial send()s](http://beej.us/guide/bgnet/output/html/multipage/advanced.html#sendall)
 * 2. [Using poll() instead of select()](https://www-01.ibm.com/support/knowledgecenter/ssw_ibm_i_71/rzab6/poll.htm)
 */

namespace pfs { namespace io { namespace details {

struct pool_iterator;

struct pool : public bits::pool
{
	typedef struct pollfd pollfd_type;
	typedef io::device::native_handle_type native_handle_type;

	typedef vector<pollfd_type>                 pollfd_vector_type;
	typedef map<native_handle_type, io::device> device_map_type;
	typedef map<native_handle_type, io::server> server_map_type;

	device_map_type    device_map;
	server_map_type    server_map;

	pollfd_vector_type pollfds;
	pollfd_vector_type pollfds_differed;

	void push_back_pollfd (pollfd_vector_type & pfds, native_handle_type fd, int events)
	{
		details::pool::pollfd_type pfd;
		pfd.fd = fd;//d.native_handle();
		pfd.events = events;

		//details::pool * pdp = static_cast<details::pool *>(_d.get());

		pfds.push_back(pfd);
	}

	void push_back (const io::device & d, int events)
	{
		push_back_pollfd(pollfds, d.native_handle(), events);
		device_map.insert(std::pair<device::native_handle_type, device>(d.native_handle(), d));
	}

	void push_back (const io::server & s, int events)
	{
		push_back_pollfd(pollfds, s.native_handle(), events);
		server_map.insert(std::pair<server::native_handle_type, server>(s.native_handle(), s));
	}

	void push_back_differed (const io::device & d, int events)
	{
		push_back_pollfd(pollfds_differed, d.native_handle(), events);
		device_map.insert(std::pair<device::native_handle_type, device>(d.native_handle(), d));
	}

	void push_back_differed (const io::server & s, int events)
	{
		push_back_pollfd(pollfds_differed, s.native_handle(), events);
		server_map.insert(std::pair<server::native_handle_type, server>(s.native_handle(), s));
	}

	int poll (pool_iterator ** begin
			, pool_iterator ** end
			, int filter_events
			, int millis
			, error_code * ex);

};

struct pool_iterator : public bits::pool_iterator
{
public:
	typedef details::pool::pollfd_vector_type::const_iterator pointer;

	int revents;
	pointer ptr;
	pointer ptr_end;

protected:
	pool_iterator (int filter_events, pointer begin, pointer end)
		: revents(filter_events)
		, ptr(begin)
		, ptr_end(end)
	{}

public:
	virtual ~pool_iterator () {}

	virtual void next ();

	static pool_iterator * alloc_begin (int filter_events, const details::pool & p);

	static pool_iterator * alloc_end (int filter_events, const details::pool & p)
	{
		return new pool_iterator(filter_events
			, static_cast<const details::pool &>(p).pollfds.cend()
			, static_cast<const details::pool &>(p).pollfds.cend());
	}

	bool eq (pool_iterator & rhs) const
	{
		return ptr == rhs.ptr;
	}
};

int pool::poll (pool_iterator ** begin
		, pool_iterator ** end
		, int filter_events
		, int millis
		, error_code * ex)
{
	// Append differed file descriptors
	//
	if (not pollfds_differed.empty()) {
		pollfds.reserve(pollfds.size() + pollfds_differed.size());
		pollfds.insert(pollfds.end(), pollfds_differed.begin(), pollfds_differed.end());
		pollfds_differed.clear();
	}

	size_t n = pollfds.size();
	pollfd_type * pfds = pollfds.data();

	int r = ::poll(pfds, n, millis);

	*begin = 0;
	*end = 0;

	if (r > 0) {
		*begin = pool_iterator::alloc_begin(filter_events, *this);
		*end   = pool_iterator::alloc_end(filter_events, *this);
	} else if (r < 0) {
		if (ex)
			*ex = errno;
	}

	return r;
}

pool_iterator * pool_iterator::alloc_begin (int filter_events, const details::pool & p)
{
	pointer begin = static_cast<const details::pool &>(p).pollfds.cbegin();

	pointer end = static_cast<const details::pool &>(p).pollfds.cend();

	while (begin != end) {
		if (begin->revents & filter_events)
			break;
		++begin;
	}

	return new pool_iterator(filter_events, begin, end);
}

void pool_iterator::next ()
{
	while (ptr != ptr_end) {
		if (ptr->revents & revents)
			break;
		++ptr;
	}
}

}}} // pfs::io::details

namespace pfs { namespace io {

pool::pool ()
	: _d(new details::pool())
{}

void pool::push_back (const device & d, int events)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->push_back(d, events);
}

void pool::push_back (const server & s, int events)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->push_back(s, events);
}

void pool::push_back_differed (const device & d, int events)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->push_back_differed(d, events);
}

void pool::push_back_differed (const server & s, int events)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->push_back_differed(s, events);
}

pool::poll_result_type pool::poll (int filter_events
		, int millis
		, error_code * pex)
{
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	details::pool_iterator * begin = 0;
	details::pool_iterator * end = 0;

	int r = pdp->poll(& begin, & end, filter_events, millis, pex);

	if (r > 0) {
		return poll_result_type(pool::iterator(this, static_cast<bits::pool_iterator *>(begin))
			, pool::iterator(this, static_cast<bits::pool_iterator *>(end)));
	}

	return poll_result_type(pool::iterator(), pool::iterator());
}

pool::value pool::iterator::operator * () const
{
	if (_d) {
		details::pool_iterator * details_it = static_cast<details::pool_iterator *>(_d.get());
		details::pool * details_pool = static_cast<details::pool *>(_powner->_d.get());

		details::pool::native_handle_type fd = details_it->ptr->fd;
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

bool pool::iterator::operator == (const iterator & rhs) const
{
	details::pool_iterator * it1 = static_cast<details::pool_iterator *>(_d.get());
	details::pool_iterator * it2 = static_cast<details::pool_iterator *>(rhs._d.get());

	return it1->eq(*it2);
}

}} // pfs::io
