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
struct app;
}

DLL_API class app : public has_slots<>
{
    static app * _s_self;

    details::app * _d;
    
public: // static
    static app & instance ()
    {
        PFS_ASSERT(_s_self);
        return *_s_self;
    }
        
private:
    app (const app &);
    app & operator = (const app &);

public:
    app ();
    ~app ();

    bool ready ();
    
public: // signals
    signal1<string const &> emit_error;
    signal0<> activated;
    
private: // slots
    void on_error_default (string const & errstr);
};

}}

#endif /* __PFS_GRIOTTE_HPP__ */

