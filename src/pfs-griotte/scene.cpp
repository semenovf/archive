/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   scene.cpp
 * Author: wladt
 * 
 * Created on August 2, 2016, 4:36 PM
 */

#include "pfs/griotte/scene.hpp"

#include <iostream>

namespace pfs {
namespace griotte {

//scene::scene ()
//{
//}
//
//scene::scene (const scene& orig)
//{
//}
//
//scene::~scene ()
//{
//}

void scene::repaint ()
{
    // TODO repaint
    std::cout << "*** REPAINT ***" << std::endl;

    _invalid_region = s_empty_region;
}

}}