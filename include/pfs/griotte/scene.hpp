/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   scene.hpp
 * Author: wladt
 *
 * Created on August 2, 2016, 4:36 PM
 */

#ifndef __PFS_GRIOTTE_SCENE_HPP__
#define __PFS_GRIOTTE_SCENE_HPP__

#include <pfs/griotte/node.hpp>

namespace pfs {
namespace griotte {

class scene
{
    static rect_type s_empty_region;
    
    rect_type _invalid_region;
    
private:
    scene (scene const &);
    scene & operator = (scene const &);
    
public:
    scene () {}
    virtual ~scene () {}
    
    bool is_invalid () const
    {
        if (_invalid_region != s_empty_region)
            return true;
        return false;
    }
    
    void invalidate (unit_type x, unit_type y, unit_type w, unit_type h)
    {
        _invalid_region = rect(x, y, w, h);
    }
    
    void repaint ();
};

}}

#endif /* SCENE_HPP */

