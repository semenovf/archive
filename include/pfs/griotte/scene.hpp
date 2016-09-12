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

#include <pfs/griotte/types.hpp>
#include <pfs/griotte/node.hpp>

namespace pfs {
namespace griotte {

class window;

namespace details {
struct scene;
}

class scene
{
    friend class window;
    
    details::scene * _d;
    
private:
    scene (scene const &);
    scene & operator = (scene const &);
    
protected:
    scene ();
    
public:
    virtual ~scene ();
    
    bool is_invalid () const;
    void invalidate (unit_type x, unit_type y, unit_type w, unit_type h);
    void repaint ();
};

}}

#endif /* SCENE_HPP */

