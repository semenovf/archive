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

typedef int unit_type;
typedef double unitf_type;

typedef pfs::point<unit_type>      point_type;
typedef pfs::dimension<unit_type>  dimension_type;
typedef pfs::rect<unit_type>       rect_type;

typedef pfs::point<unitf_type>     pointf_type;
typedef pfs::dimension<unitf_type> dimensionf_type;
typedef pfs::rect<unitf_type>      rectf_type;

typedef pfs::position              position_type;

enum font_slant_enum
{
      font_slant_normal
    , font_slant_italic
    , font_slant_oblique
};

enum font_weight_enum
{
      font_weight_normal
    , font_weight_bold
};


}}

#endif /* __PFS_GRIOTTE_TYPES_HPP__ */

