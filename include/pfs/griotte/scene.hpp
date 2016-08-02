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

namespace pfs {
namespace griotte {

class scene
{
private:
    scene (scene const & other);
    scene & operator = (scene const & other);
    
public:
    scene ();
    
    virtual ~scene ();
private:

};

}}

#endif /* SCENE_HPP */

