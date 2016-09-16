/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   internals.hpp
 * Author: wladt
 *
 * Created on September 13, 2016, 3:04 PM
 */

#ifndef __PFS_GRIOTTE_CAIRO_INTERNALS_HPP__
#define __PFS_GRIOTTE_CAIRO_INTERNALS_HPP__

#include <cairo/cairo.h>
#include "pfs/griotte/types.hpp"

namespace pfs {
namespace griotte {

namespace details {

struct scene 
{
    cairo_t * context;
    rect_type invalid_region;
};

struct painter 
{
    cairo_t * context;
};

extern rect_type empty_region;

} // details

}} // pfs::griotte

#endif /* __PFS_GRIOTTE_CAIRO_INTERNALS_HPP__ */

