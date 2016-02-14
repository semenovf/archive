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
#include <pfs/mutex.hpp>
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

	pfs::mutex      mtx;
	device_map_type device_map;
	server_map_type server_map;

	pollfd_vector_type pollfds;
	bool update;  // need updated 'pollfds' before poll() system call.

	pool ()
		: update(true)
	{}

	void update_pollfd (short events);

	void push_back (io::device d, short events)
	{
		pfs::lock_guard<pfs::mutex> locker(mtx);
		device_map.insert(std::pair<device::native_handle_type, device>(d.native_handle(), d));
		update = true;
	}

	void push_back (io::server s, short events)
	{
		pfs::lock_guard<pfs::mutex> locker(mtx);
		server_map.insert(std::pair<server::native_handle_type, server>(s.native_handle(), s));
		update = true;
	}

	void delete_deferred (io::device d)
	{
		pfs::lock_guard<pfs::mutex> locker(mtx);
		PFS_ASSERT(device_map.erase(d.native_handle()) == 1);
		update = true;
	}

	void delete_deferred (io::server s)
	{
		pfs::lock_guard<pfs::mutex> locker(mtx);
		PFS_ASSERT(server_map.erase(s.native_handle()) == 1);
		update = true;
	}

	int poll (pool_iterator ** begin
			, pool_iterator ** end
			, short filter_events
			, int millis
			, error_code * ex);

	pfs::vector<device> fetch_devices (bool (* filter) (const device & d));

	pfs::vector<server> fetch_servers (bool (* filter) (const server & s));
};

struct pool_iterator : public bits::pool_iterator
{
public:
	typedef details::pool::pollfd_vector_type::const_iterator pointer;

	short filter_events;
	pointer ptr;
	pointer ptr_end;

protected:
	pool_iterator (short events, pointer begin, pointer end)
		: filter_events(events)
		, ptr(begin)
		, ptr_end(end)
	{}

public:
	virtual ~pool_iterator () {}

	virtual void next ();

	static pool_iterator * alloc_begin (short filter_events, const details::pool & p);

	static pool_iterator * alloc_end (short filter_events, const details::pool & p)
	{
		return new pool_iterator(filter_events
			, p.pollfds.cend()
			, p.pollfds.cend());
	}

	bool eq (pool_iterator & rhs) const
	{
		return ptr == rhs.ptr;
	}

	short revents () const
	{
		return ptr->revents;
	}
};

int pool::poll (pool_iterator ** begin
		, pool_iterator ** end
		, short filter_events
		, int millis
		, error_code * ex)
{
	if (update) {
		update_pollfd(filter_events);
		update = false;
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

void pool::update_pollfd (short events)
{
	pfs::lock_guard<pfs::mutex> locker(mtx);

	pollfds.clear();
	pollfds.reserve(server_map.size() + device_map.size());

	if (server_map.size() > 0) {
		server_map_type::const_iterator it = server_map.cbegin();
		server_map_type::const_iterator it_end = server_map.cend();

		while (it != it_end) {
			pollfd_type pfd;
			pfd.fd = it->second.native_handle();
			pfd.events = events;

			PFS_ASSERT(pfd.fd >= 0);

			pollfds.push_back(pfd);

			++it;
		}
	}

	if (device_map.size() > 0) {
		device_map_type::const_iterator it = device_map.cbegin();
		device_map_type::const_iterator it_end = device_map.cend();

		while (it != it_end) {
			pollfd_type pfd;
			pfd.fd = it->second.native_handle();
			pfd.events = events;

			PFS_ASSERT(pfd.fd >= 0);

			pollfds.push_back(pfd);

			++it;
		}
	}
}

pfs::vector<device> pool::fetch_devices (bool (* filter) (const device & d))
{
	pfs::lock_guard<pfs::mutex> locker(mtx);

	pfs::vector<device> r;

	if (device_map.size() > 0) {
		device_map_type::const_iterator it = device_map.cbegin();
		device_map_type::const_iterator it_end = device_map.cend();

		while (it != it_end) {
			if (filter) {
				if (filter(it->second))
					r.push_back(it->second);
			} else {
				r.push_back(it->second);
			}
			++it;
		}
	}

	return r;
}

pfs::vector<server> pool::fetch_servers (bool (* filter) (const server & s))
{
	pfs::lock_guard<pfs::mutex> locker(mtx);

	pfs::vector<server> r;

	if (server_map.size() > 0) {
		server_map_type::const_iterator it = server_map.cbegin();
		server_map_type::const_iterator it_end = server_map.cend();

		while (it != it_end) {
			if (filter) {
				if (filter(it->second))
					r.push_back(it->second);
			} else {
				r.push_back(it->second);
			}
			++it;
		}
	}

	return r;
}


pool_iterator * pool_iterator::alloc_begin (short filter_events, const details::pool & p)
{
	pointer begin = p.pollfds.cbegin();
	pointer end   = p.pollfds.cend();

	while (begin != end) {
		if (begin->revents & filter_events)
			break;
		++begin;
	}

	return new pool_iterator(filter_events, begin, end);
}

void pool_iterator::next ()
{
	while (++ptr != ptr_end) {
		if (ptr->revents & filter_events)
			break;
	}
}

}}} // pfs::io::details

namespace pfs { namespace io {

pool::pool ()
	: _d(new details::pool())
{}

size_t pool::device_count () const
{
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	return pdp->device_map.size();
}

size_t pool::server_count () const
{
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	return pdp->server_map.size();
}

void pool::push_back (device d, short events)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->push_back(d, events);
}

void pool::push_back (server s, short events)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->push_back(s, events);
}

void pool::delete_deferred (device d)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->delete_deferred(d);
}

void pool::delete_deferred (server s)
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	pdp->delete_deferred(s);
}


pfs::vector<device> pool::fetch_devices (bool (* filter) (const device & d))
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	return  pdp->fetch_devices(filter);
}

pfs::vector<server> pool::fetch_servers (bool (* filter) (const server & s))
{
	PFS_ASSERT(_d);
	details::pool * pdp = static_cast<details::pool *>(_d.get());
	return  pdp->fetch_servers(filter);
}

//vector<device> pool::get_devices () const
//{
//	vector<device> r;
//
//	if (_d) {
//		details::pool * pdp = static_cast<details::pool *>(_d.get());
//		details::pool::device_map_type::iterator it = pdp->device_map.begin();
//		details::pool::device_map_type::iterator it_end = pdp->device_map.end();
//
//		while (it != it_end) {
//			r.push_back(it->second);
//		}
//	}
//
//	return r;
//}
//
///**
// * @brief Returns list of all listeners in the pool.
// */
//vector<server> pool::get_listeners () const
//{
//	vector<server> r;
//
//	if (_d) {
//		details::pool * pdp = static_cast<details::pool *>(_d.get());
//		details::pool::server_map_type::iterator it = pdp->server_map.begin();
//		details::pool::server_map_type::iterator it_end = pdp->server_map.end();
//
//		while (it != it_end) {
//			r.push_back(it->second);
//		}
//	}
//
//	return r;
//}


pool::poll_result_type pool::poll (short filter_events
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
			pfs::lock_guard<pfs::mutex> locker(details_pool->mtx);

			details::pool::server_map_type::const_iterator it = details_pool->server_map.find(fd);

			if (it != details_pool->server_map.cend()) {
				return pool::value(it->second);
			}
		}

		// Search through devices
		//
		{
			pfs::lock_guard<pfs::mutex> locker(details_pool->mtx);

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

	if (it1 == 0 && it2 == 0)
		return true;

	PFS_ASSERT(it1);
	PFS_ASSERT(it2);
	return it1->eq(*it2);
}

short pool::iterator::revents () const
{
	details::pool_iterator * it = static_cast<details::pool_iterator *>(_d.get());
	return it->revents();
}

}} // pfs::io
