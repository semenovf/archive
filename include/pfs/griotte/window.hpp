/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   window.hpp
 * Author: wladt
 *
 * Created on August 2, 2016, 1:51 PM
 */

#ifndef __PFS_WINDOW_HPP__
#define __PFS_WINDOW_HPP__

#include <pfs/string.hpp>
#include <pfs/sigslot.hpp>
#include <pfs/griotte/global.hpp>

namespace pfs {
namespace griotte {

namespace details {
struct window;
}

DLL_API class window : pfs::has_slots<>
{
    details::window * _d;
    
    PFS_DECLARE_NONCOPYABLE(window)
    
public:
    window (int width, int height);
    window (int width, int height, pfs::string const & title);
    //TODO window (int width, int height, pfs::string const & title, GLFWmonitor* monitor, GLFWwindow* share);
    
    virtual ~window ();
    
    bool should_close ();
    
public: // slots
    void set_title (pfs::string const & title);
    void repaint ();
    void maximize ();
    void minimize ();
    
    void iconify ()
    {
        minimize();
    }
};

}}

#endif /* WINDOW_HPP */

