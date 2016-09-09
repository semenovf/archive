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

namespace pfs {
namespace griotte {

namespace details {
struct window;
}

DLL_API class window : public pfs::has_slots<>
{
    details::window * _d;
    
private:
    window (const window &);
    window & operator = (const window &);

public:
    window ();
    virtual ~window ();
    
    void show ();
    
public: // slots
    void set_title (pfs::string const & title);
    void resize (int width, int height);
    void move (int x, int y);
    void maximize ();
    void minimize ();
};

}}

#endif /* WINDOW_HPP */

