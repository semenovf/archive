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
#include <pfs/sigslot.hpp>
#include <pfs/griotte/window.hpp>

namespace pfs {
namespace griotte {

class window;

namespace details {
struct application;
}

DLL_API class application : public has_slots<>
{
    friend class window;
    friend void __activate_helper (application &, window &);
    
public:
    typedef void (* activated_callback_type)(application &, window &);
    
private:
    static application * _s_self;

    details::application *  _d;
    activated_callback_type _activated;
    
public: // static
    static application & instance ()
    {
        PFS_ASSERT(_s_self);
        return *_s_self;
    }
        
private:
    application (const application &);
    application & operator = (const application &);

public:
    application (activated_callback_type activated, string const & application_id = string());
    ~application ();

    int run ();
    
public:
    signal1<string const &> emit_error;
    
private:    
    void default_print_error (string const & errstr);
};

}}

#endif /* __PFS_GRIOTTE_HPP__ */

