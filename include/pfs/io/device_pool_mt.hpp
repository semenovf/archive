/*
 * device_pool_mt.hpp
 *
 *  Created on: Jan 14, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_DEVICE_POOL_MT_HPP__
#define __PFS_IO_DEVICE_POOL_MT_HPP__

#include <pfs/mutex.hpp>
#include <pfs/io/device_pool.hpp>

namespace pfs { namespace io {

template <typename Mutex = pfs::fake_mutex>
class device_pool_mt : protected device_pool
{
	Mutex  _mutex;

public:
	device_pool_mt ()
		: device_pool ()
	{}

	void push_back (const device & d, int events)
	{
		lock_guard<Mutex> locker(_mutex);
		device_pool::push_back(d, events);
	}

	int poll (vector<device> & devices, int filter_events = poll_all, int millis = 0, error_code * ex = 0)
	{
		lock_guard<Mutex> locker(_mutex);
		return poll(devices, filter_events, millis, ex);
	}
};

}}

#endif /* __PFS_IO_DEVICE_POOL_MT_HPP__ */
