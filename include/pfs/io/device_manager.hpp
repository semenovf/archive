/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   device_manager.hpp
 * Author: wladt
 *
 * Created on November 14, 2016, 4:29 PM
 */

#ifndef __PFS_IO_DEVICE_MANAGER_HPP__
#define __PFS_IO_DEVICE_MANAGER_HPP__

#include <ctime>
#include <pfs/sigslot.hpp>
#include <pfs/set.hpp>
#include <pfs/io/pool.hpp>

namespace pfs {
namespace io {

// All devices must be in non-blocking mode.

class device_manager
{
public:
    static time_t const default_reconn_timeout = 5; // seconds
    
private:
    struct reconn_item
    {
        device d;
        time_t timeout; // reconnection timeout in seconds
        time_t start;   // start time point counting timeout from
        
        bool operator < (reconn_item const & x)
        {
            return (start + timeout) < (x.start + x.timeout);
        }
    };
    
    time_t _reconn_timeout;
    
    // Main device pool (for valid (operational) devices)
    pool _pool;
    
    // Device pool for partially-operational devices: usually in 'connection in progress...' state)
    pool _poolr;
    
    // Reconnection queue, contains devices waiting reconnection by timeout
    pfs::set<reconn_item> _rq;
    
private:
    device_manager (device_manager const &);
    device_manager & operator = (device_manager const &);
    
public:
    device_manager ()
        : _reconn_timeout(default_reconn_timeout)
    {}
        
    device_manager (time_t reconn_timeout)
        : _reconn_timeout(reconn_timeout)
    {}

    bool push_good (device d)
    {
        _pool.push_back(d);
    }

    bool push_deferred (device d) // 
    {
        _pool.push_back(d);
    }
    
    bool push_reconnect (device d, time_t reconn_timeout);

    bool push_reconnect (device d)
    {
        return push_reconnect(d, _reconn_timeout);
    }

    /**
     * @brief Checks reconnection timeout is exceeded
     */
    bool reconnect_available () const;
};

}} // pfs::io

#endif /* __PFS_IO_DEVICE_MANAGER_HPP__ */

