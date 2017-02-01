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

class device_manager : has_slots<>
{
    struct reopen_item
    {
        device d;
        time_t timeout; // reconnection timeout in seconds
        time_t start;   // start time point counting timeout from
        
        bool operator < (reopen_item const & x) const
        {
            return (start + timeout) < (x.start + x.timeout);
        }
    };
    
    typedef pfs::set<reopen_item> reopen_queue_type;
    
    class dispatcher_context1 : public pool::dispatcher_context2
    {
        friend class device_manager;
        
        device_manager * _m;
        pool * _p1;
        pool * _p2;

    private:
        dispatcher_context1 (int millis, short filter_events
                , device_manager * m, pool * p1, pool * p2)
            : pool::dispatcher_context2(millis, filter_events)
            , _m(m)
            , _p1(p1)
            , _p2(p2)
        {}
        
    public:
     	virtual void accepted (device & d, server & listener) const
        {
            _m->accepted(d, listener);
        }
        
		virtual void ready_read (device & d) const
        {
            _m->ready_read(d);
        }
        
		virtual void disconnected (device & d) const
        {
            _m->disconnected(d);
        }
        
		virtual void on_error (error_code const & ex) const
        {
            _m->error(ex);
        }
    };

    class dispatcher_context2 : public pool::dispatcher_context2
    {
        friend class device_manager;
        
        static int const default_millis = 1; // TODO Need it configurable ?!
        
        device_manager * _m;
        pool * _p1;
        pool * _p2;

    private:
        dispatcher_context2 (device_manager * m, pool * p1, pool * p2)
            : pool::dispatcher_context2(default_millis, io::poll_all) // TODO Need to reduce number of events according to specialization of this pool
            , _m(m)
            , _p1(p1)
            , _p2(p2)
        {}
        
    public:
		virtual void disconnected (device & d) const
        {
            _m->disconnected(d);
        }
        
		virtual void can_write (device & d) const
        {
            _p2->delete_deferred(d);
            _p1->push_back(d);
            
            _m->opened(d);
        }
        
		virtual void on_error (error_code const & ex) const
        {
            _m->error(ex);
        }
    };
    
    // Main device pool (for valid (operational) devices)
    pool _p1;
    
    // Device pool for partially-operational devices: usually in 'connection in progress...' state)
    pool _p2;
    
    // Reconnection queue, contains devices waiting reconnection by timeout
    reopen_queue_type _rq;
    
    dispatcher_context1 _ctx1;
    dispatcher_context2 _ctx2;
    
private:
    device_manager (device_manager const &);
    device_manager & operator = (device_manager const &);
    
private:
    void push_device (device d, pfs::error_code const & ex);
    void push_server (server s, pfs::error_code const & ex);
    
public:
    device_manager (int millis, short filter_events = io::poll_all)
        : _ctx1(millis, filter_events, this, & _p1, & _p2)
        , _ctx2(this, & _p1, & _p2)
    {}
        
    template <typename DeviceTag>
    device new_device (open_params<DeviceTag> const & op, pfs::error_code * ex = 0);

    template <typename ServerTag>
    server new_server (open_params<ServerTag> const & op, pfs::error_code * ex = 0);
    
    void push_deferred (device d, time_t reconn_timeout);

    /**
     * @brief Checks if reopen timeout is exceeded for one or more devices.
     */
    bool ready_deferred () const;

    /**
     * @brief Reopen 'ready' devices.
     */
    void reopen_deferred ();
    
    void dispatch ();
    
public: // signals
    signal2<device, server>     accepted;           // accept connection (for connection based server devices)
    signal1<device>             ready_read;
    signal1<device>             opened;             // opened (for regular files, servers) or connected (for connection based client devices)
	signal1<device>             disconnected;       // disconnection for connection based devices, including peer devices
    signal1<device>             opening;            // open (connection) in progress (for connection based client devices)
    signal2<device, error_code> open_failed;
    signal1<server>             server_opened;
    signal1<server>             server_opening;
    signal2<server, error_code> server_open_failed;
	signal1<error_code>         error;
};

template <typename DeviceTag>
device device_manager::new_device (open_params<DeviceTag> const & op, pfs::error_code * pex)
{
    pfs::error_code ex;
    device d = pfs::io::open_device(op, ex);
    push_device(d, ex);
    
    if (pex)
        *pex = ex;
    
    return d;
}

template <typename ServerTag>
server device_manager::new_server (open_params<ServerTag> const & op, pfs::error_code * pex)
{
    pfs::error_code ex;
    server s = pfs::io::open_server(op, ex);
    push_server(s, ex);
    
    if (pex)
        *pex = ex;
    
    return s;
}

}} // pfs::io

#endif /* __PFS_IO_DEVICE_MANAGER_HPP__ */

