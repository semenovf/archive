/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   griotte.hpp
 * Author: wladt
 *
 * Created on August 2, 2016, 11:57 AM
 */

#ifndef __PFS_GRIOTTE_HPP__
#define __PFS_GRIOTTE_HPP__

#include <pfs.hpp>
#include <pfs/stringlist.hpp>
#include <pfs/griotte/global.hpp>
#include <pfs/griotte/window.hpp>

namespace pfs {
namespace griotte {

namespace details {
struct context;
}

DLL_API class context : public has_slots<>, private pfs::noncopyable
{
    static context * _s_self;

    details::context * _d;
    
public: // static
    static context & instance ()
    {
        PFS_ASSERT(_s_self);
        return *_s_self;
    }
        
public:
    context ();
    ~context ();

    bool init ();
    void poll_events ();
    
    /**
     * @brief Fill @a info with font engine specification information.
     * @details @a info[i] contains specification parameter name and 
     *             @a info[i+1] contains paramater value.
     * @param info Data to store font engine specification information.
     */
    void font_engine_info (stringlist & info);
    
public: // signals
    signal1<string const &> emit_error;
    
private: // slots
    void on_error_default (string const & errstr);
};

}}

#endif /* __PFS_GRIOTTE_HPP__ */

