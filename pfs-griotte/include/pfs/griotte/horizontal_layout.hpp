/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   horizontal_layout.hpp
 * Author: wladt
 *
 * Created on September 14, 2016, 12:32 PM
 */

#ifndef __PFS_GRIOTTE_HORIZONTAL_LAYOUT_HPP__
#define __PFS_GRIOTTE_HORIZONTAL_LAYOUT_HPP__

#include <pfs/griotte/linear_layout.hpp>

namespace pfs {
namespace griotte {

class widget;

class horizontal_layout : public linear_layout
{
public:
    horizontal_layout () : linear_layout() {}
    virtual ~horizontal_layout () {}
};

}}

#endif /* __PFS_GRIOTTE_HORIZONTAL_LAYOUT_HPP__ */

