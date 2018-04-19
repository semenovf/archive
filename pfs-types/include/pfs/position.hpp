/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   position.hpp
 * Author: wladt
 *
 * Created on September 10, 2016, 12:36 PM
 */

#ifndef __PFS_POSITION_HPP__
#define __PFS_POSITION_HPP__

namespace pfs {

struct position
{
    enum {
          center_x = 1 << 0
        , center_y = 1 << 1
        , center_xy = center_x | center_y
        , center    = center_xy
        , top       = 1 << 2
        , bottom    = 1 << 3
        , left      = 1 << 4
        , right     = 1 << 5
    };

private:
    int _value;
    
public:
    explicit position (int value) : _value(value) {}
    
    int value () const
    {
        return _value;
    }
};

}

#endif /* __PFS_POSITION_HPP__ */

