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

#include <pfs/griotte/global.hpp>

namespace pfs {
namespace griotte {

class scene
{
    PFS_DECLARE_NONCOPYABLE(scene)
    
public:
    scene ();
    virtual ~scene ();
};

}}

#endif /* SCENE_HPP */

