/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   types.hpp
 * Author: wladt
 *
 * Created on September 10, 2016, 12:34 PM
 */

#ifndef __PFS_GRIOTTE_TYPES_HPP__
#define __PFS_GRIOTTE_TYPES_HPP__

#include <pfs/point.hpp>
#include <pfs/dimension.hpp>
#include <pfs/rect.hpp>
#include <pfs/position.hpp>

namespace pfs {
namespace griotte {

typedef pfs::point<int> point_type;
typedef pfs::dimension<int> dimension_type;
typedef pfs::rect<int> rect_type;
typedef pfs::position position_type;

}}

#endif /* __PFS_GRIOTTE_TYPES_HPP__ */

