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

#include <cairo/cairo.h>
#include "pfs/griotte/scene.hpp"

#include <iostream>

namespace pfs {
namespace griotte {

namespace details {

struct scene 
{
    cairo_t * context;    
    rect_type invalid_region;
};

rect_type empty_region;

}

scene::scene ()
    : _d(new details::scene)
{
    _d->context = 0;
    _d->invalid_region = details::empty_region;
}

scene::~scene ()
{
    delete _d;
}

bool scene::is_invalid () const
{
    if (_d->invalid_region != details::empty_region)
        return true;
    return false;
}

void scene::invalidate (unit_type x, unit_type y, unit_type w, unit_type h)
{
    _d->invalid_region = rect_type(x, y, w, h);
}

void scene::repaint ()
{
    // TODO repaint
    std::cout << "*** REPAINT ***" << std::endl;

    _d->invalid_region = details::empty_region;
}

}}