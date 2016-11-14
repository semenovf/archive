/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "pfs/io/device_manager.hpp"

namespace pfs {
namespace io {

bool device_manager::push_reconnect (device d, time_t reconn_timeout)
{
    reconn_item item;
    item.d = d;
    item.timeout = reconn_timeout;
    item.start = time(0); // TODO may be need to use monotonic clock

    _rq.insert(item);
}

bool device_manager::reconnect_available () const
{
    if (_rq.empty())
        return false;

    // Create temporary item to compare (device field does not matter in comparison)
    reconn_item item;
    item.timeout = 0;
    item.start = time(0); // TODO may be need to use monotonic clock

    // Checking of first item will be enough.
    return (_rq.first() < item) ? true : false;
}

}} // pfs::io