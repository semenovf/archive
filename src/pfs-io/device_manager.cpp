/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/io/device_manager.hpp"
#include <pfs/debug.hpp>

namespace pfs {
namespace io {

void device_manager::push_device (device d, pfs::error_code const & ex)
{
    if (!ex) {
        _p1.push_back(d);
        opened(d);
    } else {
        if (ex == pfs::OperationInProgressError) {
            _p2.push_back(d);
            opening(d);
        } else {
            open_failed(d, ex);
        }
    }
}

void device_manager::push_server (server s, pfs::error_code const & ex)
{
    if (!ex) {
        _p1.push_back(s);
        server_opened(s);
    } else {
        if (ex == pfs::OperationInProgressError) {
            _p2.push_back(s);
            server_opening(s);
        } else {
            server_open_failed(s, ex);
        }
    }
}

void device_manager::push_deferred (device d, time_t reconn_timeout)
{
    PFS_ASSERT(d.set_nonblocking(true));
    reopen_item item;
    item.d = d;
    item.timeout = reconn_timeout;
    item.start = time(0); // TODO may be need to use monotonic clock

    _rq.insert(item);
}

bool device_manager::ready_deferred () const
{
    if (_rq.empty())
        return false;

    // Create temporary item to compare (device field does not matter in comparison)
    reopen_item item;
    item.timeout = 0;
    item.start = time(0); // TODO may be need to use monotonic clock

    // Checking first item will be enough.
    return (*_rq.begin() < item) ? true : false;
}

void device_manager::reopen_deferred ()
{
    if (_rq.empty())
        return;

    // Create temporary item to compare (device field does not matter in comparison)
    reopen_item item;
    item.timeout = 0;
    item.start = time(0); // TODO may be need to use monotonic clock

    reopen_queue_type::iterator it = _rq.begin();
    reopen_queue_type::iterator it_end = _rq.end();
    
    while (*it < item && it != it_end) {
        device d = it->d;
        pfs::error_code ex = d.reopen();
        push_device(d, ex);
        
        ++it;
    }
    
    _rq.erase(_rq.begin(), it);
}

void device_manager::dispatch ()
{
    //if (_p1.server_count() > 0 || _p1.device_count())
	_p1.dispatch(_ctx1);

	if (_p2.device_count() > 0)
		_p2.dispatch(_ctx2);

    if (ready_deferred())
        reopen_deferred();
}

}} // pfs::io