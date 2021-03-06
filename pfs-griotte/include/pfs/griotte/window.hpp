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
#include <pfs/griotte/types.hpp>

namespace pfs {
namespace griotte {

class scene;

namespace details {
struct window;
}

DLL_API class window : public has_slots<>
{
    details::window * _d;
    
private:
    window (const window &);
    window & operator = (const window &);

public:
    window ();
    ~window ();
    
    void show (scene &);
    
public: // slots
    void set_title (pfs::string const & title);
    void resize (int width, int height);
    
    void move (int x, int y);
    void move (position_type const & pos);

    void move (point_type const & p)
    {
        move(p.x(), p.y());
    }
    
    void move (int pos)
    {
        move(position(pos));
    }
    
    void maximize ();
    void minimize ();
};

}}

#endif /* WINDOW_HPP */

