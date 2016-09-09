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
    
    static application * _s_self;

    details::application * _d;
    
    
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
    application (string const & application_id = string());
    ~application ();

    int run ();
    
public: // signals
    signal1<string const &> emit_error;
    signal2<application &, window &> activated;
    
private: // slots
    void on_error_default (string const & errstr);
};

}}

#endif /* __PFS_GRIOTTE_HPP__ */

